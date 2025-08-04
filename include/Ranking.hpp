//
// Created by Bolu Abegunde on 8/2/25.
//

#ifndef RANKING_HPP
#define RANKING_HPP

#include "School.h"

struct Weights {
    float test;
    float grad;
    float safe;
    float stable;
};

inline float computeScore(const School& s, const Weights& w) {
    return s.testScore * w.test + s.graduationRate * w.grad + s.safetyScore * w.safe + s.stabilityScore * w.stable;
}

#endif