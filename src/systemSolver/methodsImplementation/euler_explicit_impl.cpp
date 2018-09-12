#include "declaretions.h"
#include "shared.h"
#include <array>

#include <boost/numeric/odeint.hpp>

Answer euler_explicit(const ConfigSingleton *configs)
{
    using namespace boost::numeric::odeint;
    euler<state_type> steper;
    return solve_with_stepper(steper, configs);
}
