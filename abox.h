#ifndef ABOX_H
#define ABOX_H

#define Dot3D std::tuple<double, double, double>

#include "field.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <tuple>
#include <utility>
#include <random>
#include <functional>
#include <time.h>
#include <math.h>
#include <string>


class Inhomogeneity;

class Averager;

class ABox
{
private:
    double __x(int i);
    double __y(int j);
    double __z(int k);

    int __i(double x);
    int __j(double y);
    int __k(double z);

public:
    double PX, PY, PZ;
    int Nx, Ny, Nz;
    double l0, L0;

    Field* temperature;
    Field* pressure;
    Field* humidity;

    std::vector<Inhomogeneity*> inhomogeneities;

    std::function<double(double, Dot3D)> lambdaTemperature;
    std::function<double(double, Dot3D)> lambdaPressure;
    std::function<double(double, Dot3D)> lambdaHumidity;

    double T0 = 15., P0 = 1013., rho0 = 7.5;
    double beta = 6.5, HP = 7.7, Hrho = 2.1;

    ABox(std::tuple<double, double, double> sizes,
         std::tuple<int, int, int> N,
         std::pair<double, double> inertial_interval);

    void initGrid();

    void setStandardProfiles(double T0 = 15., double P0 = 1013., double rho0 = 7.5,
                             double beta = 6.5, double HP = 7.7, double Hrho = 2.1);

    void setLambdaTemperature(const std::function<double(double, Dot3D)> expression);

    void setLambdaPressure(const std::function<double(double, Dot3D)> expression);

    void setLambdaHumidity(const std::function<double(double, Dot3D)> expression);

    void createStructuralInhomogeneities(int amount = 100, bool verbose = true);

    void applyStructuralInhomogeneities(bool verbose = true);

    std::vector<double> getAltitudeProfileTemperature(int i, int j);
    std::vector<double> getAltitudeProfileTemperature(double x, double y);
    std::vector<double> getAltitudeProfileTemperature(Averager* avr);

    std::vector<double> getAltitudeProfilePressure(int i, int j);
    std::vector<double> getAltitudeProfilePressure(double x, double y);
    std::vector<double> getAltitudeProfilePressure(Averager* avr);

    std::vector<double> getAltitudeProfileHumidity(int i, int j);
    std::vector<double> getAltitudeProfileHumidity(double x, double y);
    std::vector<double> getAltitudeProfileHumidity(Averager* avr);

    void dumpAltitudeProfile(std::vector<double> profile, std::string file_path);

    Inhomogeneity* findInhomogeneity(int number);

    void dumpInhomogeneities(std::string file_path,
                             std::tuple<int, int, int> resolution = std::make_tuple(10, 10, 10));

};

#endif // ABOX_H
