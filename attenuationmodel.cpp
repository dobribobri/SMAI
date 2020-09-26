#include "attenuationmodel.h"

AttenuationModel::AttenuationModel() { }

AttenuationModel::~AttenuationModel() { }


P676::P676() { }

double P676::gammaOxygen(double f, double T, double P) {
    double rt = 288 / (273 + T);
    double rp = P / 1013;
    double gamma = 0.;
    if (f <= 57.)
        gamma = (7.27 * rt / (f * f + 0.351 * rp * rp * rt * rt) +
                 7.5 / ((f - 57)*(f - 57) + 2.44 * rp * rp * rt*rt*rt*rt*rt)) *
                 f * f * rp * rp * rt * rt / 1000;
    else if ((63. <= f) && (f <= 350.))
        gamma = (2 / 10000 * pow(rt, 1.5) * (1 - 1.2 / 100000 * pow(f, 1.5)) +
                 4 / ((f - 63)*(f - 63) + 1.5 * rp * rp * rt*rt*rt*rt*rt) +
                 0.28 * rt * rt / ((f - 118.75)*(f - 118.75) + 2.84 * rp * rp * rt * rt)) *
                 f * f * rp * rp * rt * rt / 1000;
    else if ((57. < f) && (f < 63.))
        gamma = (f - 60) * (f - 63) / 18 * gammaOxygen(57, T, P) -
                 1.66 * rp * rp * pow(rt, 8.5) * (f - 57) * (f - 63) +
                 (f - 57) * (f - 60) / 18 * gammaOxygen(63, T, P);
    return gamma;
}

double P676::gammaWVapor(double f, double T, double P, double rho) {
    double rt = 288 / (273 + T);
    double rp = P / 1013;
    double gamma = 0.;
    if (f <= 350.)
        gamma = (3.27 / 100 * rt +
                 1.67 / 1000 * rho * rt*rt*rt*rt*rt*rt*rt / rp +
                 7.7 / 10000 * pow(f, 0.5) +
                 3.79 / ((f - 22.235)*(f - 22.235) + 9.81 * rp * rp * rt) +
                 11.73 * rt / ((f - 183.31)*(f - 183.31) + 11.85 * rp * rp * rt) +
                 4.01 * rt / ((f - 325.153)*(f - 325.153) + 10.44 * rp * rp * rt)) *
                 f * f * rho * rp * rt / 10000;
    return gamma;
}
