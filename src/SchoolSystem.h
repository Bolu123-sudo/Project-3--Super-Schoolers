#ifndef PROJECT_3_SUPER_SCHOOLERS_SCHOOLSYSTEM_H
#define PROJECT_3_SUPER_SCHOOLERS_SCHOOLSYSTEM_H

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
    // Loads and parses schools_clean.csv into the BTree and index
    void loadData();

    // Filters schools by county and level
    vector<School> filterSchools(const string& county, const string& level);

    // Ranks top N schools by weighted score
    vector<School> recommendTopSchools(const string& county, const string& level,
                                       float wTest, float wGrad, float wSafe, float wStable, int topN);
};

#endif //PROJECT_3_SUPER_SCHOOLERS_SCHOOLSYSTEM_H