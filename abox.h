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
#include "colormod.h"

typedef std::vector<double> Profile;
typedef double Frequency;
typedef std::vector<std::pair<Frequency, double>> Spectrum;
typedef double Height;


class Inhomogeneity;

class Field;

class Averager;

class AttenuationModel;

class ABox {
    private:
        double __x(int i);
        double __y(int j);
        double __z(int k);

        int __i(double x);
        int __j(double y);
        int __k(double z);

        Color::Modifier* fgred = new Color::Modifier(Color::FG_RED);
        Color::Modifier* fggreen = new Color::Modifier(Color::FG_GREEN);
        Color::Modifier* fgblue = new Color::Modifier(Color::FG_BLUE);
        Color::Modifier* fgdef = new Color::Modifier(Color::FG_DEFAULT);

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
        double HP = 7.7, Hrho = 2.1;
        std::map<std::pair<Height, Height>, std::function<double(double)>> beta = {
            {{0, 11}, [](double h){ return -6.5*h; }}, {{11, 20}, [](double h){ return -6.5*11+h*0; }},
            {{20, 32}, [](double h){ return -6.5*11+(h-20); }}, {{32, 47}, [](double h){ return -6.5*11+(32-20)+2.8*(h-32); }},
            {{47, 50}, [](double h){ return -15-3.5+h*0; }}
        };

        ABox(std::tuple<double, double, Height> sizes,
             std::tuple<int, int, int> N,
             std::pair<double, double> inertial_interval);

        void initGrid();

        void setStandardProfiles();

        std::vector<double> xGrid();
        std::vector<double> yGrid();
        std::vector<double> zGrid();
        std::vector<double> grid(std::string axis = "z");

        void setLambdaTemperature(std::function<double(double, Dot3D)> expression);

        void setLambdaPressure(std::function<double(double, Dot3D)> expression);

        void setLambdaHumidity(std::function<double(double, Dot3D)> expression);

        void createStructuralInhomogeneities(int amount = 100, bool verbose = true);

        void applyStructuralInhomogeneities(bool verbose = true);

        Profile getAltitudeProfileTemperature();
        Profile getAltitudeProfileTemperature(int i, int j);
        Profile getAltitudeProfileTemperature(double x, double y);
        Profile getAltitudeProfileTemperature(Averager* avr);

        Profile getAltitudeProfilePressure();
        Profile getAltitudeProfilePressure(int i, int j);
        Profile getAltitudeProfilePressure(double x, double y);
        Profile getAltitudeProfilePressure(Averager* avr);

        Profile getAltitudeProfileHumidity();
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

        Spectrum getBrightnessTemperature(std::vector<Frequency> frequencies,
                                 Averager* avr, AttenuationModel* model, double theta = 0.);

        Profile W_H2O(AttenuationModel* model, Frequency f);
        Profile W_H2O(AttenuationModel* model, Frequency f, int i, int j);
        Profile W_H2O(AttenuationModel* model, Frequency f, double x, double y);
        Profile W_H2O(AttenuationModel* model, Frequency f, Averager* avr);
        static Profile W_H2O(AttenuationModel* model, Frequency f, Profile* T, Profile* P, Profile* rho);

        void moveFieldsPeriodicX(double s);
        void moveFieldsPeriodicX(double v, double t);

};

#endif // ABOX_H
