#include "SchoolSystem.h"
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <vector>
#include "MaxHeap.h"

static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

static vector<string> splitLine(const string& line, char delim = ',') {
    vector<string> result;
    string cell;
    istringstream ss(line);
    while (getline(ss, cell, delim)) {
        result.push_back(trim(cell));
    }
    return result;
}

static double parseNum(const string& s) {
    string cleaned;
    for (char c : s) {
        if (c == '%' || c == ',') continue;
        cleaned += c;
    }
    try { return stod(cleaned); } catch (...) { return 0.0; }
}

static string makeKey(const string& a, const string& b) {
    return a + "|" + b;
}

static string toLower(const string& s) {
    string out = s;
    transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

static string inferLevel(const string& name) {
    string low = toLower(name);
    if (low.find("elementary") != string::npos || low.find("elem") != string::npos) return "Elementary";
    if (low.find("middle") != string::npos || low.find("ms ") != string::npos) return "Middle";
    if (low.find("high") != string::npos || low.find("senior") != string::npos || low.find("hs ") != string::npos) return "High";
    return "Unknown";
}

void SchoolSystem::loadData() {
    vector<School> loaded;

    // --- Parse: Grades (base school list) ---
    ifstream gradeFile("data/SchoolGrades-High,Middle,Elementary School.csv");
    if (!gradeFile) {
        gradeFile.open("../data/SchoolGrades-High,Middle,Elematary School.csv");

    }
    string line;
    for (int i = 0; i < 5; ++i) getline(gradeFile, line); // skip
    getline(gradeFile, line); // column header

    vector<School> base;
    while (getline(gradeFile, line)) {
        auto cols = splitLine(line);
        if (cols.size() < 5) continue;

        string county = cols[1];
        string name = cols[4];
        if (name.empty()) continue;

        School s;
        s.name = name;
        s.county = county;
        s.level = inferLevel(name);
        s.testScore = 0.0f;
        s.graduationRate = 0.0f;
        s.safetyScore = 1.0f;
        s.stabilityScore = 0.0f;

        base.push_back(s);
    }

    // --- Parse: Math Scores ---
    unordered_map<string, double> testScores;
    ifstream mathFile("data/EodOfYear Math Scores- Grade 3 up.csv");


    for (int i = 0; i < 4; ++i) getline(mathFile, line);
    getline(mathFile, line);
    while (getline(mathFile, line)) {
        auto cols = splitLine(line);
        if (cols.size() < 2) continue;
        string county = toLower(cols[1]);
        for (int i = static_cast<int>(cols.size()) - 1; i >= 0; --i) {
            if (cols[i].find('%') != string::npos) {
                testScores[county] = parseNum(cols[i]);
                break;
            }
        }
    }


    // --- Parse: Graduation Rates ---
    unordered_map<string, double> gradRates;
    ifstream gradFile("data/High School Graduations.csv");
    for (int i = 0; i < 6; ++i) getline(gradFile, line); // skip header/notes
    vector<vector<string>> rows;
    while (getline(gradFile, line)) rows.push_back(splitLine(line));

    for (size_t i = 0; i < rows.size();) {
        if (rows[i].size() < 5) { ++i; continue; }
        string county = toLower(rows[i][1]);
        string school = toLower(trim(rows[i][2]));
        double total = 0.0, standard = 0.0;
        size_t j = i;
        while (j < rows.size()) {
            if (rows[j].size() < 5) { ++j; continue; }
            string county_j = toLower(rows[j][1]);
            string school_j = toLower(trim(rows[j][2]));
            if (county_j != county || school_j != school) break;
            string label = toLower(rows[j][3]);
            if (label.find("total") != string::npos && rows[j].size() > 4)
                total = parseNum(rows[j][4]);
            if (label.find("standard") != string::npos && rows[j].size() > 4)
                standard = parseNum(rows[j][4]);
            ++j;
        }
        double rate = (total > 0) ? (standard / total) * 100.0 : 0.0;
        string key = makeKey(county, school);
        gradRates[key] = rate;
        i = j;
    }


    // --- Parse: Safety Reports ---
    unordered_map<string, double> incidentMap;
    auto parseSafety = [&](const string& path) {
        ifstream file(path);

        for (int i = 0; i < 7; ++i) getline(file, line);
        while (getline(file, line)) {
            auto cols = splitLine(line);
            if (cols.size() < 6) continue;
            string county = toLower(cols[1]);
            string schoolName = cols[3];
            string key = makeKey(county, schoolName);
            double total = 0.0;
            for (size_t i = 5; i < cols.size(); ++i)
                total += parseNum(cols[i]);
            incidentMap[key] += total;
        }
    };
    parseSafety("data/School Safety A-H.csv");
    parseSafety("data/School Safety I-Z.csv");

    // --- Parse: Stability Scores ---
    unordered_map<string, double> stabilityMap;
    ifstream stabFile("data/Stability Rates.csv");

    getline(stabFile, line); getline(stabFile, line); // skip notes
    getline(stabFile, line);
    while (getline(stabFile, line)) {
        auto cols = splitLine(line);
        if (cols.size() < 7) continue;
        string county = toLower(cols[1]);
        string school = cols[3];
        string key = makeKey(county, school);
        stabilityMap[key] = parseNum(cols[6]);
    }


    // --- Merge everything into base schools ---
    double minInc = 1e9, maxInc = -1e9;
    for (auto& s : base) {
        string countyKey = toLower(s.county);
        string schoolKey = makeKey(countyKey, s.name);
        string compositeKey = makeKey(countyKey, schoolKey);

        if (testScores.count(countyKey)) s.testScore = static_cast<float>(testScores[countyKey]);
        if (gradRates.count(countyKey)) s.graduationRate = static_cast<float>(gradRates[compositeKey]);
        if (incidentMap.count(schoolKey)) {
            double inc = incidentMap[schoolKey];
            minInc = min(minInc, inc);
            maxInc = max(maxInc, inc);
            s.safetyScore = static_cast<float>(inc);
        }
        if (stabilityMap.count(schoolKey)) s.stabilityScore = static_cast<float>(stabilityMap[schoolKey]);
    }

    for (auto& s : base) {
        if (maxInc > minInc && s.safetyScore != 1.0f) {
            double norm = (s.safetyScore - minInc) / (maxInc - minInc);
            s.safetyScore = 1.0f - norm;
        } else {
            s.safetyScore = 1.0f;
        }
    }

    // --- Load into BTree and index ---
    for (auto& s : base) {
        tree.insert(s);
        string key = toLower(s.county);
        countyIndex[key].push_back(s);
    }

    cout << "[SchoolSystem] Loaded " << base.size() << " schools\n";
}

vector<School> SchoolSystem::filterSchools(const string& county, const string& level) {
    vector<School> result;
    string key = toLower(county);
    for (const auto& s : countyIndex[key]) {
        if (toLower(s.level) == toLower(level))
            result.push_back(s);
    }
    return result;
}

vector<School> SchoolSystem::recommendTopSchools(const string& county, const string& level,
                                                 float wTest, float wGrad, float wSafe, float wStable, int topN) {
    auto filtered = filterSchools(county, level);

    return MaxHeap::getTopSchools(filtered, wTest, wGrad, wSafe, wStable, topN);
}
