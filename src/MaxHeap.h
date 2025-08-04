#ifndef MAXHEAP_H
#define MAXHEAP_H

#include "School.h"
#include "Ranking.hpp"
#include <vector>
#include <algorithm>

// helper class for selecting the top N schools based on a weighted score.
class MaxHeap {
public:
    //Returns the top schools from the input list, ranked by a weighted sum
    // of their metrics
    static std::vector<School> getTopSchools(std::vector<School> schools,
                                             float wTest, float wGrad, float wSafe, float wStable,
                                             int topN) {
        if (topN <= 0 || schools.empty()) return {};

        Weights w{wTest, wGrad, wSafe, wStable};
        int n = std::min(topN, static_cast<int>(schools.size()));

        // Comparator
        auto scoreCmp = [&](const School& a, const School& b) {
            return computeScore(a, w) > computeScore(b, w);
        };
        // Reorders it so that the top n scoring elements appear first
        std::nth_element(schools.begin(), schools.begin() + n, schools.end(),
                         [&](const School& a, const School& b) {
                             return computeScore(a, w) > computeScore(b, w);
                         });
        // Copy to a separate vector
        std::vector<School> top(schools.begin(), schools.begin() + n);
        std::sort(top.begin(), top.end(), scoreCmp); // descending order
        return top;
    }
};

#endif