#ifndef SHARED_H
#define SHARED_H

#include "answer.h"
#include "config.h"
#include <array>

using state_type = std::array<double, 3>;

template <typename Stepper>
Answer solve_with_stepper(Stepper stepper, const ConfigSingleton *);

double dxdt(double x, double y, const ConfigSingleton *);

double dydt(double x, double y, double z, const ConfigSingleton *);

double dzdt(double x, double y, double z, const ConfigSingleton *);

#endif // SHARED_H
