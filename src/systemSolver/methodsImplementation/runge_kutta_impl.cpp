#include "declarations.h"
#include "shared.h"
#include <boost/numeric/odeint.hpp>

Answer runge_kutta(const ConfigSingleton *config)
{
    using namespace boost::numeric::odeint;
    runge_kutta4<state_type> stepper;
    return solve_with_stepper(stepper, config);
}
