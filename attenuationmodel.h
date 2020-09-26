#ifndef ATTENUATIONMODEL_H
#define ATTENUATIONMODEL_H

#include <math.h>
#include <iostream>


class AttenuationModel {
    public:
        double c = 299792458;
        double dB2np = 0.23255814;

        AttenuationModel();

        virtual double gammaOxygen(double f, double temperature, double pressure) = 0;

        virtual double gammaWVapor(double f, double temperature, double pressure, double humidity) = 0;

        virtual ~AttenuationModel() = 0;

};


class P676 : public AttenuationModel {
    public:
        double H1 = 5.;
        double H2 = 1.8;

        P676();

        double gammaOxygen(double f, double temperature, double pressure);

        double gammaWVapor(double f, double temperature, double pressure, double humidity);

};


#endif // ATTENUATIONMODEL_H
