
#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tuple>
#include <utility>
#include <random>
#include <string>
#include <thread>
#include "abox.h"
#include "averager.h"
#include "pipegnuplotter.h"
//#include "plotter.h"
#include "linspace.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //std::ifstream pysrce("../SMAI/plotter.py", std::ios::binary);
    //std::ofstream pydest("plotter.py", std::ios::binary);
    //pydest << pysrce.rdbuf();

    double s = 1000;
    double PX = 10000 / s, PY = 5000 / s, PZ = 2100 / s;
    double Nx = 100*2, Ny = 50*2, Nz = 21*2;
    double l0 = 0.1 / s, L0 = 500 / s;
    int numberInhomogeneities = 1000;
    double fA = 10;
//    double Crho = 1;
    unsigned int seed = 42;
    double T0 = 19.65;
    double P0 = 755.66 * 1.333;
    double rho0 = 10.57;


    std::srand(seed);
    std::default_random_engine g;
    std::uniform_real_distribution<double> fluctuation(-fA, fA);

    ABox* ab = new ABox(std::make_tuple(PX, PY, PZ),
                        std::make_tuple(Nx, Ny, Nz),
                        std::make_pair(l0, L0));

    Averager* averager = new ConeAverager(ab,
                                          std::make_tuple(PX/2, PY/2, 0.),
                                          1.25*PZ, 0.148492, 0.148492, 1,
                                          std::make_tuple(0, -51, 0));

    AttenuationModel* model = new P676();

    ab->setStandardProfiles(T0, P0, rho0);

    ab->createStructuralInhomogeneities(numberInhomogeneities, false);

    ab->setLambdaHumidity([&](double rho, Dot3D point)
    /* mutable */ {
        std::tie(std::ignore, std::ignore, std::ignore) = point;
        return rho - fluctuation(g);
    });


    std::string tmp00 = "tmp00.txt";
    std::string tmp01 = "tmp01.txt";
    std::string tmp02 = "tmp02.txt";
    std::remove(tmp00.c_str());
    std::remove(tmp01.c_str());
    std::remove(tmp02.c_str());

    ab->dumpInhomogeneities(tmp00);
    averager->dump(tmp00);
    std::thread t0 = PipeGnuplotter::Threaded::scatter3d(tmp00);


    ab->applyStructuralInhomogeneities(false);

    for (int k = 0; k < 100; k++) {
        std::cout << "Номер итерации: " << k << std::endl;
        double time_step = 11.;

        Profile hum = ab->getAltitudeProfileHumidity(averager);
        ab->dumpAltitudeProfile(hum, tmp01);

        Spectrum brTemp = ab->getBrightnessTemperature(linspace(18.0, 27.2, 47), averager, model, 51);
        ab->dumpSpectrum(brTemp, 22.2, k*time_step, tmp02);

        ab->moveFieldsPeriodic(std::make_tuple(10./s, 0, 0), time_step);
    }

    std::thread t1 = PipeGnuplotter::Threaded::plot2d(tmp01);
    PipeGnuplotter::plot2d(tmp02);

    t0.join();
    t1.join();

    exit(0);

    //return a.exec();
}
