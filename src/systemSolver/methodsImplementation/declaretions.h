#ifndef DECLARETIONS_H
#define DECLARETIONS_H

#include "answer.h"
#include <string>
#include "config.h"

Answer euler_implicit(const ConfigSingleton *);
Answer euler_explicit(const ConfigSingleton *);
Answer euler_implicit(const ConfigSingleton *);
Answer runge_kutta(const ConfigSingleton *);
Answer adams_explicit(const ConfigSingleton *);


#endif // DECLARETIONS_H
