#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class ConfigSingleton
{

    static const ConfigSingleton *configSingletonPtr;

    ConfigSingleton(double x0, double y0, double z0, double sigma, double r, double b, double dt,
                    double t_max, const char *method_name)
        : x0(x0), y0(y0), z0(z0), sigma(sigma), r(r), b(b), dt(dt), t_max(t_max),
          method_name(method_name)
    {
    }
    ConfigSingleton()
        : x0(0), y0(0), z0(0), sigma(0), r(0), b(0), dt(0), t_max(t_max), method_name(){};

public:
    const double x0;
    const double y0;
    const double z0;
    const double sigma;
    const double r;
    const double b;
    const double dt;
    const double t0 = 0;
    const double t_max;

    const std::string method_name;

    static const ConfigSingleton *getConfigs()
    {
        if (!configSingletonPtr)
            configSingletonPtr = new ConfigSingleton;
        return configSingletonPtr;
    }

    static const ConfigSingleton *createConfigs(double x0, double y0, double z0, double sigma,
                                                double r, double b, double dt, double t_max,
                                                const char *method_name)
    {
        if (configSingletonPtr)
            return configSingletonPtr;
        configSingletonPtr = new ConfigSingleton(x0, y0, z0, sigma, r, b, dt, t_max, method_name);
        return configSingletonPtr;
    }

    static void releaseConfigs()
    {
        delete configSingletonPtr;
        configSingletonPtr = nullptr;
    }
};

#endif // CONFIG_H
