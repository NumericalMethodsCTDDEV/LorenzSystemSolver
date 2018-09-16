#include "solver.h"
#include "config.h"
#include "internal_method_names.h"
#include "methodsImplementation/declarations.h"
#include <unordered_map>

using SolverMethodPtr = Answer (*)(const ConfigSingleton *);

static const std::unordered_map<std::string, SolverMethodPtr> solverMethodsFunctions = {
    {internal_method_names::euler_explicit, &euler_explicit},
    {internal_method_names::euler_implicit, &euler_implicit},
    {internal_method_names::runge_kutta, &runge_kutta},
    {internal_method_names::adams_explicit, &adams_explicit}};

Answer solve()
{
    const ConfigSingleton *configs = ConfigSingleton::getConfigs();
    auto it = solverMethodsFunctions.find(configs->method_name);
    if (it == solverMethodsFunctions.end())
    {
        std::string error_msg;
        error_msg += "Dont have method: ";
        error_msg += configs->method_name;
        return Answer(error_msg);
    }
    SolverMethodPtr solver = it->second;
    return (*solver)(configs);
}
