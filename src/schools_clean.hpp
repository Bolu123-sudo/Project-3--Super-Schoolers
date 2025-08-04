/*


#include <algorithm>
#include <cmath>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

// file paths (can change)
const string GRADES_FILE = "data/SchoolGrades-High,Middle,Elematary School.csv";
const string MATH_FILE = "data/EodOfYear Math Scores- Grade 3 up.csv";
const string GRAD_FILE = "data/High School Graduations.csv";
const string SAFETY_AH_FILE = "data/School Safety A-H.csv";
const string SAFETY_IZ_FILE = "data/School Safety I-Z.csv";
const string OUTPUT_FILE = "data/schools_clean.csv";

// row skip because firsst lines arent needed
int GRADES_SKIP = 5;
int MATH_SKIP = 4;
int GRAD_SKIP = 6;
int SAFETY_SKIP = 7;

static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static string toLower(const string& s) {
    string out = s;
    transform(out.begin(), out.end(), out.begin(), [](unsigned char c){ return static_cast<char>(tolower(c)); });
    return out;
}

static vector<string> splitLine(const string& line, char delim = ',') {
    vector<string> res;
    string cur;
    istringstream ss(line);
    while (getline(ss, cur, delim)) {
        res.push_back(trim(cur));
    }
    return res;
}

static double parseNum(const string& s) {
    string cleaned;
    for (char c : s) {
        if (c == '%' || c == ',') continue;
        cleaned.push_back(c);
    }
    try {
        if (cleaned.empty()) return 0.0;
        return stod(cleaned);
    } catch (...) {
        return 0.0;
    }
}

static string csvEscape(const string& s) {
    if (s.find(',') != string::npos) return "\"" + s + "\"";
    return s;
}

static string makeKey(const string& a, const string& b) {
    return a + "|" + b;
}

struct SchoolRecord {
    string SchoolName;
    string Level;
    string County;
    string DistrictNumber;
    double TestScore = 0.0;
    double GraduationRate = 0.0;
    double Incidents = 0.0;
    double SafetyScore = 1.0;
};

static vector<pair<string,string>> LEVEL_OVERRIDES = {
    {"St. Petersburg Prep", "High"},
    {"Central Elementary", "Elementary"},
    {"Westside Middle", "Middle"},
};

static string inferLevel(const string& schoolname) {
    string low = toLower(schoolname);
    for (auto &p : LEVEL_OVERRIDES) {
        if (!p.first.empty() && low.find(toLower(p.first)) != string::npos)
            return p.second;
    }
    if (low.find("elementary") != string::npos || low.find("elem") != string::npos) return "Elementary";
    if (low.find("middle") != string::npos || low.find("ms ") != string::npos) return "Middle";
    if (low.find("high") != string::npos || low.find("senior") != string::npos || low.find("hs ") != string::npos) return "High";
    return "Unknown";
}

vector<SchoolRecord> parseGrades() {
    vector<SchoolRecord> list;
    ifstream in(GRADES_FILE);
    if (!in) {
        cerr << "cant open grades file: " << GRADES_FILE << "\n";
        return list;
    }
    string line;
    for (int i = 0; i < GRADES_SKIP && getline(in, line); ++i) {}
    if (!getline(in, line)) {
        cerr << "grades file truncated or header missing\n";
        return list;
    }
    while (getline(in, line)) {
        auto cols = splitLine(line);
        if (cols.size() < 5) continue;
        string distNum = cols[0];
        string distName = cols[1];
        string schoolName;
        if (cols.size() > 4 && !cols[4].empty()) schoolName = cols[4];
        else {
            for (const auto& c : cols) {
                if (!c.empty()) { schoolName = c; break; }
            }
        }
        if (schoolName.empty()) continue;
        SchoolRecord rec;
        rec.SchoolName = schoolName;
        rec.Level = inferLevel(schoolName);
        rec.County = distName;
        rec.DistrictNumber = distNum;
        list.push_back(rec);
    }
    unordered_set<string> seen;
    vector<SchoolRecord> uniq;
    for (auto &r : list) {
        string key = r.SchoolName + "|" + r.Level + "|" + r.County + "|" + r.DistrictNumber;
        if (seen.insert(key).second) {
            uniq.push_back(r);
        }
    }
    return uniq;
}

struct DistrictInfo { string County; double TestScore; };

unordered_map<string, DistrictInfo> parseMath() {
    unordered_map<string, DistrictInfo> map;
    ifstream in(MATH_FILE);
    if (!in) {
        cerr << "cant open math file: " << MATH_FILE << "\n";
        return map;
    }
    string line;
    for (int i = 0; i < MATH_SKIP && getline(in, line); ++i) {}
    //data start
    if (!getline(in, line)) {}
    while (getline(in, line)) {
        auto cols = splitLine(line);
        if (cols.size() < 2) continue;
        string distNum = cols[0];
        string distName = cols[1];
        double score = 0.0;
        for (int i = (int)cols.size() - 1; i >= 0; --i) {
            if (cols[i].find('%') != string::npos) {
                score = parseNum(cols[i]);
                break;
            }
        }
        if (score == 0.0) {
            for (int i = (int)cols.size() - 1; i >= 0; --i) {
                bool hasDigit = any_of(cols[i].begin(), cols[i].end(), ::isdigit);
                if (hasDigit) {
                    score = parseNum(cols[i]);
                    break;
                }
            }
        }
        string key = makeKey(distNum, distName);
        map[key] = DistrictInfo{distName, score};
    }
    return map;
}

unordered_map<string, double> parseGraduation() {
    unordered_map<string,double> out;
    ifstream in(GRAD_FILE);
    if (!in) {
        cerr << " cant open graduation file: " << GRAD_FILE << "\n";
        return out;
    }
    string line;
    for (int i = 0; i < GRAD_SKIP && getline(in, line); ++i) {}
    vector<vector<string>> rows;
    while (getline(in, line)) rows.emplace_back(splitLine(line));
    size_t i = 0;
    while (i < rows.size()) {
        if (rows[i].size() < 4) { ++i; continue; }
        string distNum = rows[i][0];
        string distName = rows[i][1];
        double totalDiplomas = 0.0;
        double standardDiplomas = 0.0;
        size_t j = i;
        while (j < rows.size() && rows[j].size() >= 3 && rows[j][0] == distNum && rows[j][1] == distName) {
            string label = toLower(rows[j][2]);
            if (label.find("total diplomas") != string::npos) {
                if (rows[j].size() > 3) totalDiplomas = parseNum(rows[j][3]);
            } else if (label.find("standard diploma") != string::npos) {
                if (rows[j].size() > 3) standardDiplomas = parseNum(rows[j][3]);
            }
            ++j;
        }
        double rate = 0.0;
        if (totalDiplomas > 0.0) rate = (standardDiplomas / totalDiplomas) * 100.0;
        string key = makeKey(distNum, distName);
        out[key] = rate;
        i = j;
    }
    return out;
}

vector<SchoolRecord> parseSafety() {
    unordered_map<string,double> incident_acc;
    auto process = [&](const string& path) {
        ifstream in(path);
        if (!in) {
            cerr << "cant open safety file: " << path << "\n";
            return;
        }
        string line;
        for (int i = 0; i < SAFETY_SKIP && getline(in, line); ++i) {}
        while (getline(in, line)) {
            auto cols = splitLine(line);
            if (cols.size() < 6) continue;
            string distNum = cols[0];
            string distName = cols[1];
            string schoolName = cols[3];
            double tot = 0.0;
            for (size_t k = 5; k < cols.size(); ++k) tot += parseNum(cols[k]);
            string key = distNum + "|" + distName + "|" + schoolName;
            incident_acc[key] += tot;
        }
    };
    process(SAFETY_AH_FILE);
    process(SAFETY_IZ_FILE);
    vector<SchoolRecord> out;
    for (auto& kv : incident_acc) {
        vector<string> parts;
        istringstream ss(kv.first);
        string tok;
        while (getline(ss, tok, '|')) parts.push_back(tok);
        if (parts.size() != 3) continue;
        SchoolRecord r;
        r.DistrictNumber = parts[0];
        r.County = parts[1];
        r.SchoolName = parts[2];
        r.Incidents = kv.second;
        out.push_back(r);
    }
    return out;
}

int main() {
    try {
        auto gradeList = parseGrades();
        auto mathMap = parseMath();
        auto gradMap = parseGraduation();
        auto safetyList = parseSafety();

        unordered_map<string,double> safetyLookup;
        for (auto& s : safetyList) {
            string key = s.SchoolName + "|" + s.DistrictNumber + "|" + s.County;
            safetyLookup[key] = s.Incidents;
        }

        vector<SchoolRecord> merged;
        merged.reserve(gradeList.size());
        for (auto& base : gradeList) {
            SchoolRecord m = base;
            string districtKey = base.DistrictNumber + "|" + base.County;
            if (mathMap.count(districtKey)) {
                m.TestScore = mathMap[districtKey].TestScore;
            }
            if (gradMap.count(districtKey)) {
                m.GraduationRate = gradMap[districtKey];
            }
            string safetyKey = base.SchoolName + "|" + base.DistrictNumber + "|" + base.County;
            if (safetyLookup.count(safetyKey)) {
                m.Incidents = safetyLookup[safetyKey];
            }
            merged.push_back(m);
        }

        double minInc = numeric_limits<double>::infinity();
        double maxInc = -numeric_limits<double>::infinity();
        for (auto& r : merged) {
            minInc = min(minInc, r.Incidents);
            maxInc = max(maxInc, r.Incidents);
        }
        for (auto& r : merged) {
            if (maxInc > minInc) {
                double norm = (r.Incidents - minInc) / (maxInc - minInc);
                r.SafetyScore = 1.0 - norm;
            } else {
                r.SafetyScore = 1.0;
            }
        }

        ofstream out(OUTPUT_FILE);
        if (!out) {
            cerr << "cant write to: " << OUTPUT_FILE << "\n";
            return 1;
        }
        out << "SchoolName,Level,County,TestScore,GraduationRate,SafetyScore\n";
        for (auto& r : merged) {
            out << csvEscape(r.SchoolName) << ","
                << r.Level << ","
                << csvEscape(r.County) << ","
                << fixed << setprecision(1) << r.TestScore << ","
                << fixed << setprecision(1) << r.GraduationRate << ","
                << fixed << setprecision(3) << r.SafetyScore << "\n";
        }
        out.close();
        cout << "[schools_clean] wrote " << merged.size() << " rows\n";
    } catch (const exception& e) {
        cerr << "ex: " << e.what() << "\n";
        return 2;
    }
    return 0;
}
*/