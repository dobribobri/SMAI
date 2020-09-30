#include "structuralfunction.h"
#include <math.h>
#include <iostream>


TimeSeries structuralFunction(TimeSeries xi, double part, double rightLimit) {
    int nsize = int(xi.size());
    double t0, t1, dt_avg = 0.;
    for (int i = 0; i < nsize - 1; i++) {
        std::tie(t0, std::ignore) = xi[unsigned(i)];
        std::tie(t1, std::ignore) = xi[unsigned(i+1)];
        dt_avg += (t1 - t0);
    }
    dt_avg /= (nsize - 1);

    std::vector<std::pair<double, double>> out;
    for (int m = 0; m < part * nsize; m++) {
        if (m * dt_avg > rightLimit) break;
        double I = 0.;
        for (int k = 0; k < nsize - m; k++) {
            double x0 = 0., x1 = 0.;
            std::tie(std::ignore, x1) = xi[unsigned(k + m)];
            std::tie(std::ignore, x0) = xi[unsigned(k)];
            I += (x1 - x0)*(x1 - x0);
        }
        I = sqrt(I / (nsize - m));

        out.push_back(std::make_pair(m * dt_avg, I));
    }
    return out;
}

MDATA structuralFunctions(MDATA DATA, double part, double rightShowLimit) {
    std::map<Frequency, TimeSeries> out = DATA;
    for (MDATA::iterator it = out.begin(); it != out.end(); it++) {
        it->second = structuralFunction(it->second, part, rightShowLimit);
    }
    return out;
}

void structuralFunctions(MDATA* DATA, double part, double rightShowLimit) {
    for (MDATA::iterator it = DATA->begin(); it != DATA->end(); it++)
        it->second = structuralFunction(it->second, part, rightShowLimit);
}
