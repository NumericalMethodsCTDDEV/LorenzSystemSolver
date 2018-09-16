#include "frontend.h"
#include <fstream>
#include <iostream>
#include <cmath>

static inline bool check(double a)
{
    if (std::isnan(a) || std::isinf(a)) return false;
    return true;
}

int buildChartGnuPlot(const Answer &ans)
{

    std::ofstream fout(".log.txt");
    if (!ans.solvedCorrect())
    {
        fprintf(stderr, "gnuplot: %s\n", ans.error_msg.c_str());
        return 0;
    }

    for (size_t i = 0; i != ans.x.size(); ++i)
    {
        if (!(check(ans.x[i]) && check(ans.y[i]) && check(ans.z[i]))) {
            fprintf(stderr, "gnuplot: nan or inf in line %d\n", i);
            return 0;
        }
        fout << ans.x[i] << " " << ans.y[i] << " " << ans.z[i] << std::endl;
    }

    FILE *gp = popen("gnuplot", "w");

    if (gp == NULL)
    {
        fprintf(stderr, "gnuplot: Error opening pipe to GNU plot.\n");
        return 1;
    }
    fprintf(gp, "set terminal png\n");
    fprintf(gp, "set terminal png size 1920,1200\n");
    fprintf(gp, "set output \"chart.png\"\n");
    fprintf(gp, "set key inside left top vertical Right noreverse enhanced autotitles box linetype "
                "-1 linewidth 1.000\n");
    fprintf(gp, "set samples 100, 100\n");
    fprintf(gp, "splot \".log.txt\" using 1:2:3 with lines");
    pclose(gp);
    return 0;
}
