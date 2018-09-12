#ifndef ABSTRACTMETHODSIMPL_H
#define ABSTRACTMETHODSIMPL_H

#include <string>
#include "answer.h"
#include "config.h"

struct AbstractMethodsImpl {
private:
    std::string error_msg;
    bool ok;
public:
    virtual Answer solveLorenzAttractor(const ConfigSingleton *) = 0;

    std::string get_error_msg() const
    {
        return error_msg;
    }

    bool isSolvedSuccessful() const
    {
        return ok;
    }
};

#endif // ABSTRACTMETHODSIMPL_H
