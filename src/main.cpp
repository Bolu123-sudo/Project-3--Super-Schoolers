#include <iostream>
#include "SchoolSystem.h"
using namespace std;

int getPreferenceWeight(const string& metric) {
    int weight;
    while (true) {
        cout << "Enter your preference weight for " << metric << " (1 to 10): ";
        cin >> weight;
        if (!cin.fail() && weight >= 1 && weight <= 10) return weight;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please enter a number from 1 to 10.\n";
    }
}

int main() {
    SchoolSystem sys;
    sys.loadData();

    cout << "\n==== Florida School Recommendation System ====\n";

    string county, level;
    cout << "Enter a Florida county: ";
    cin.ignore();
    getline(cin, county);

    cout << "Enter school level (Elementary / Middle / High): ";
    getline(cin, level);

    int wTest = getPreferenceWeight("Test Scores");
    int wGrad = getPreferenceWeight("Graduation Rate");
    int wSafe = getPreferenceWeight("Safety");
    int wStab = getPreferenceWeight("Stability");

    int topN;
    cout << "How many top schools would you like to see? ";
    cin >> topN;

    auto results = sys.recommendTopSchools(county, level,
                                           wTest, wGrad, wSafe, wStab, topN);

    cout << "\nTop " << results.size() << " schools in " << county << " (" << level << "):\n";
    for (int i = 0; i < results.size(); ++i) {
        const auto& s = results[i];
        float score = s.computeScore(wTest, wGrad, wSafe, wStab);
        cout << i + 1 << ". " << s.name << " | Score: " << fixed << setprecision(2) << score << "\n";
    }

    if (results.empty()) {
        cout << "No schools found matching your criteria.\n";
    }

    cout << "\nThank you for using the system!\n";
    return 0;
}