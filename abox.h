#ifndef ABOX_H
#define ABOX_H

#define Dot3D std::tuple<double, double, double>
#define FREQ_EPS 0.00001

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
#include <map>

typedef std::vector<double> Profile;
typedef double Frequency;
typedef std::vector<std::pair<Frequency, double>> Spectrum;


class Inhomogeneity;

class Field;

class Averager;

class AttenuationModel;

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

    void setLambdaHumidity(std::function<double(double, Dot3D)> expression);

    void createStructuralInhomogeneities(int amount = 100, bool verbose = true);

    void applyStructuralInhomogeneities(bool verbose = true);

    Profile getAltitudeProfileTemperature(int i, int j);
    Profile getAltitudeProfileTemperature(double x, double y);
    Profile getAltitudeProfileTemperature(Averager* avr);

    Profile getAltitudeProfilePressure(int i, int j);
    Profile getAltitudeProfilePressure(double x, double y);
    Profile getAltitudeProfilePressure(Averager* avr);

    Profile getAltitudeProfileHumidity(int i, int j);
    Profile getAltitudeProfileHumidity(double x, double y);
    Profile getAltitudeProfileHumidity(Averager* avr);

    void dumpAltitudeProfile(Profile profile, std::string file_path, bool append = true);

    Inhomogeneity* findInhomogeneity(int number);

    void dumpInhomogeneities(std::string file_path,
                             std::tuple<int, int, int> resolution = std::make_tuple(10, 10, 10),
                             bool append = true);

    void moveStructuralInhomogeneities(std::tuple<double, double, double> s);
    void moveStructuralInhomogeneities(std::tuple<double, double, double> v, double t);

    Spectrum getBrightnessTemperature(std::vector<double> frequencies,
                             Averager* avr, AttenuationModel* model, double theta = 0.);

    void moveFieldsPeriodicX(double s);
    void moveFieldsPeriodicX(double v, double t);

};

#endif // ABOX_H
