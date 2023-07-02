#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <functional>

#include "hitobject.hpp"

namespace accuracy_analyzer
{

struct Stats {
    int num_300 = 0;
    int num_100 = 0;
    int num_50 = 0;
    int num_miss = 0;
    float accuracy = 0.f;
    float avg_early = 0.f;
    float avg_late = 0.f;
    float unstable_rate = 0.f;
};

void analyze(Stats* stats, bool do_trace);
int next_hitobject(std::function<bool(const hitobject_t&)> func);

}  // namespace accuracy_analyzer
