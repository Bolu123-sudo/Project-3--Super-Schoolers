

#ifndef PROJECT_3_SUPER_SCHOOLERS_SCHOOL_H
#define PROJECT_3_SUPER_SCHOOLERS_SCHOOL_H

#include <string>
using namespace std;

struct School {
    string name;
    string county;
    string level;

    float testScore;
    float graduationRate;
    float safetyScore;
    float stabilityScore;

    //Compute and adds all scores
    float computeScore(float wTest, float wGrad, float wSafe, float wStable) const {
        return testScore * wTest + graduationRate * wGrad + safetyScore * wSafe + stabilityScore * wStable;
    }
};




#endif //PROJECT_3_SUPER_SCHOOLERS_SCHOOL_H
