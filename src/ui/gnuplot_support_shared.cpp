#include "frontend.h"
#include <iostream>

int buildChartGnuPlot()
{
    FILE *gp = popen("gnuplot", "w");

    if (gp == NULL)
    {
        printf("Error opening pipe to GNU plot.\n");
        return 1;
    }
    fprintf(gp, "set terminal pngcairo\n");
    fprintf(gp, "set terminal png size 1920,1200\n");
    fprintf(gp, "set output \"chart.png\"\n");
    fprintf(gp, "set key inside left top vertical Right noreverse enhanced autotitles box linetype -1 linewidth 1.000\n");
    fprintf(gp, "set samples 100, 100\n");
    fprintf(gp, "splot \".logs.txt\" using 1:2:3 with lines");
    pclose(gp);
    return 0;
}
