
#ifndef PROJECT_3_SUPER_SCHOOLERS_SCHOOLSYSTEM_H
#define PROJECT_3_SUPER_SCHOOLERS_SCHOOLSYSTEM_H

#ifndef SCHOOLSYSTEM_H
#define SCHOOLSYSTEM_H

#include "School.h"
#include "BTree.h"
#include "MaxHeap.h"
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class SchoolSystem {
private:
    BTree tree;
    unordered_map<string, vector<School>> countyIndex;

public:

    void loadData();


    vector<School> filterSchools(const string& county, const string& level);


    vector<School> recommendTopSchools(const string& county, const string& level,
                                       float wTest, float wGrad, float wSafe, float wStable, int topN);


    void normalize(School& school);

    // helpers
    void parseTestScores(const string& filename);
    void parseGraduationRates(const string& filename);
    void parseSafetyReports(const string& filename);
    void parseStabilityScores(const string& filename);
    void parseSchoolGrades(const string& filename);
};


#endif //PROJECT_3_SUPER_SCHOOLERS_SCHOOLSYSTEM_H
