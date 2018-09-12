#include "../frontend.h"
#include "config.h"
#include "show_method_names.h"
#include "internal_method_names.h"
#include <fstream>
#include <iostream>

using namespace std;

void printUsage()
{
    cout << "Pass input file name as first argument, the default file is input.txt" << endl;
    cout << "Write to file in exactly the same order:" << endl;
    cout << "x0\ny0\nz0\nsigma\nr\nb\ndt\nt0\nt_max\nmethod_name" << endl;
}

int showUi(int argc, char **argv)
{
    ifstream fin;
    if (argc > 1) {
        fin.open(argv[1]);
    } else {
        fin.open("input.txt");
    }

    if (!fin.good()) {
        printUsage();
        return 1;
    }

    double x0;
    double y0;
    double z0;
    double sigma;
    double r;
    double b;
    double dt;
    double t0 = 0;
    double t_max;

    std::string method_name;

    fin >> x0 >> y0 >> z0 >> sigma >> r >> b >> dt >> t0 >> t_max;
    fin >> method_name;

    fin.close();

    ConfigSingleton::createConfigs(x0, y0, z0, sigma, r, b, dt, t_max, method_name.c_str());

    return 0;
}
