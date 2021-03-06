#ifndef SHARED_H
#define SHARED_H

#include "answer.h"
#include "config.h"
#include <array>

using state_type = std::array<double, 3>;

template <typename Stepper>
Answer solve_with_stepper(Stepper stepper, const ConfigSingleton *);

void dfdt(double x, double y, double z, const ConfigSingleton *config, double ans[]);

#endif // SHARED_H
