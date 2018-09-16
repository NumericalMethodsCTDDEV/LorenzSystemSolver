#include "declarations.h"
#include "shared.h"
#include <vector>

using namespace std;

Answer euler_explicit(const ConfigSingleton *config)
{
    size_t sz = (config -> t_max - config -> t0) / config -> dt;
    vector <double> t(sz);
    vector <double> x(sz);
    vector <double> y(sz);
    vector <double> z(sz);
    t[0] = config -> t0;
    x[0] = config -> x0;
    y[0] = config -> y0;
    z[0] = config -> z0;
    double res[3];
    for (size_t i = 1; i < sz; ++i) {
        t[i] = t[i - 1] + config -> dt;

        dfdt(x[i - 1], y[i - 1], z[i - 1], config, res);
        x[i] = x[i - 1] + res[0] * config -> dt;
        y[i] = y[i - 1] + res[1] * config -> dt;
        z[i] = z[i - 1] + res[2] * config -> dt;
    }
    return Answer(std::move(t), std::move(x), std::move(y), std::move(z));
}
