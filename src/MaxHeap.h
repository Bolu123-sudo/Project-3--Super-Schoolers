#ifndef MAXHEAP_H
#define MAXHEAP_H

#include "School.h"
#include "Ranking.hpp"
#include <vector>
#include <algorithm>

class MaxHeap {
public:
    static std::vector<School> getTopSchools(std::vector<School> schools,
                                             float wTest, float wGrad, float wSafe, float wStable,
                                             int topN) {
        if (topN <= 0 || schools.empty()) return {};

        Weights w{wTest, wGrad, wSafe, wStable};
        int n = std::min(topN, static_cast<int>(schools.size()));

        auto scoreCmp = [&](const School& a, const School& b) {
            return computeScore(a, w) > computeScore(b, w);
        };

        std::nth_element(schools.begin(), schools.begin() + n, schools.end(),
                         [&](const School& a, const School& b) {
                             return computeScore(a, w) > computeScore(b, w);
                         });

        std::vector<School> top(schools.begin(), schools.begin() + n);
        std::sort(top.begin(), top.end(), scoreCmp); // descending order
        return top;
    }
};

#endif