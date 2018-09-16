#include "shared.h"
#include "answer.h"
#include "config.h"
#include <boost/numeric/odeint.hpp>

using namespace boost::numeric::odeint;

template <typename Stepper>
Answer solve_with_stepper(Stepper stepper, const ConfigSingleton *configs)
{

    auto lorenz = [configs](const state_type &x, state_type &dxdt, double) {
        dxdt[0] = configs->sigma * (x[1] - x[0]);
        dxdt[1] = configs->r * x[0] - x[1] - x[0] * x[2];
        dxdt[2] = -configs->b * x[2] + x[0] * x[1];
    };
    std::vector<double> ts;
    std::vector<double> xs;
    std::vector<double> ys;
    std::vector<double> zs;
    auto observer = [&ts, &xs, &ys, &zs](const state_type &x, const double t) {
        ts.push_back(t);
        xs.push_back(x[0]);
        ys.push_back(x[1]);
        zs.push_back(x[2]);
    };
    state_type x = {{configs->x0, configs->y0, configs->z0}}; // initial conditions
    integrate_const(stepper, lorenz, x, 0., configs->t_max, configs->dt, observer);
    return Answer(std::move(ts), std::move(xs), std::move(ys), std::move(zs));
}

double dxdt(double x, double y, const ConfigSingleton *config) {
    return ((y - x) * config -> sigma);
}

double dydt(double x, double y, double z, const ConfigSingleton *config) {
    return (x * (config -> r - z) - y);
}

double dzdt(double x, double y, double z, const ConfigSingleton *config) {
    return (x * y - z * config -> b);
}

// template Answer solve_with_stepper<euler<state_type>>(euler<state_type>, const ConfigSingleton *);
template Answer solve_with_stepper<runge_kutta4<state_type>>(runge_kutta4<state_type>,
                                                             const ConfigSingleton *);
template Answer solve_with_stepper<adams_bashforth_moulton<1, state_type>>(
    adams_bashforth_moulton<1, state_type>, const ConfigSingleton *);
