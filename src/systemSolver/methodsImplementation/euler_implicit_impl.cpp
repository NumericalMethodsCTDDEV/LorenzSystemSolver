#include "declarations.h"
#include "shared.h"
#include <vector>

using namespace std;

Answer euler_implicit(const ConfigSingleton *config)
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
    double res_predict[3], res_correct[3];
    for (size_t i = 1; i < sz; ++i) {
        t[i] = t[i - 1] + config -> dt;

        // predict
        dfdt(x[i - 1], y[i - 1], z[i - 1], config, res_predict);
        x[i] = x[i - 1] + res_predict[0] * config -> dt;
        y[i] = y[i - 1] + res_predict[1] * config -> dt;
        z[i] = z[i - 1] + res_predict[2] * config -> dt;

        // correct
        dfdt(x[i], y[i], z[i], config, res_correct);
        x[i] = x[i - 1] + (res_predict[0] + res_correct[0]) / 2 * config -> dt;
        y[i] = y[i - 1] + (res_predict[1] + res_correct[1]) / 2 * config -> dt;
        z[i] = z[i - 1] + (res_predict[2] + res_correct[2]) / 2 * config -> dt;
    }
    return Answer(std::move(t), std::move(x), std::move(y), std::move(z));
}
