#include "declarations.h"
#include "shared.h"
#include <boost/numeric/odeint.hpp>

Answer adams_explicit(const ConfigSingleton *config)
{
    using namespace boost::numeric::odeint;
    adams_bashforth_moulton<1, state_type> stepper;
    return solve_with_stepper(stepper, config);
}
