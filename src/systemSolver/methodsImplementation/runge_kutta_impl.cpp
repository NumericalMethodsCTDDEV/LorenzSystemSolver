#include "declarations.h"
#include "shared.h"
#include <vector>

using namespace std;

Answer runge_kutta(const ConfigSingleton *config)
{
    // explicit runge_kutta
    size_t sz = (config -> t_max - config -> t0) / config -> dt;
    vector <double> t(sz);
    vector <double> x(sz);
    vector <double> y(sz);
    vector <double> z(sz);
    t[0] = config -> t0;
    x[0] = config -> x0;
    y[0] = config -> y0;
    z[0] = config -> z0;

    double k1[3], k2[3], k3[3], k4[3];
    for (size_t i = 1; i < sz; ++i) {
        t[i] = t[i - 1] + config -> dt;

        dfdt(x[i - 1], y[i - 1], z[i - 1], config, k1);
        dfdt(x[i - 1] + config -> dt / 2 * k1[0], y[i - 1] + config -> dt / 2 * k1[1], z[i - 1] + config -> dt / 2 * k1[2], config, k2);
        dfdt(x[i - 1] + config -> dt / 2 * k2[0], y[i - 1] + config -> dt / 2 * k2[1], z[i - 1] + config -> dt / 2 * k2[2], config, k3);
        dfdt(x[i - 1] + config -> dt * k3[0], y[i - 1] + config -> dt * k3[1], z[i - 1] + config -> dt * k3[2], config, k4);

        x[i] = x[i - 1] + config -> dt / 6 * (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]);
        y[i] = y[i - 1] + config -> dt / 6 * (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]);
        z[i] = z[i - 1] + config -> dt / 6 * (k1[2] + 2 * k2[2] + 2 * k3[2] + k4[2]);
    }
    return Answer(std::move(t), std::move(x), std::move(y), std::move(z));
}
