#ifndef ATTENUATIONMODEL_H
#define ATTENUATIONMODEL_H

#define C 299792458.
#define dB2np 0.23255814

#include <math.h>
#include <iostream>
#include <tuple>


class AttenuationModel {
    public:

        AttenuationModel();

        virtual double gammaOxygen(double f, double temperature, double pressure) = 0;
        virtual double tauOxygen(double f, double temperature, double pressure, double theta = 0.) = 0;

        virtual double gammaWVapor(double f, double temperature, double pressure, double humidity) = 0;
        virtual double tauWVapor(double f, double temperature, double pressure, double humidity, double theta = 0.) = 0;

        virtual double gammaLWater(double f, double tcl) = 0;

        virtual double tauClearSky(double f, double temperature, double pressure, double humidity, double theta = 0.) = 0;

        virtual double opacity(double brT, double Tavg) = 0;

        virtual ~AttenuationModel() = 0;

};


class P676K : public AttenuationModel {
    public:
        double H1 = 5.;
        double H2 = 1.8;

        P676K();

        double gammaOxygen(double f, double temperature, double pressure) override;
        double tauOxygen(double f, double temperature, double pressure, double theta) override;

        double gammaWVapor(double f, double temperature, double pressure, double humidity) override;
        double tauWVapor(double f, double temperature, double pressure, double humidity, double theta) override;

        std::tuple<double, double, double> epsilon_(double temperature, double saltiness = 0.);

        double gammaLWater(double f, double tcl) override;

        double tauClearSky(double f, double temperature, double pressure, double humidity, double theta) override;

        double opacity(double brT, double Tavg) override;
};


#endif // ATTENUATIONMODEL_H
