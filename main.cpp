
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
//#include "pipegnuplotter.h"
#include "plotter.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    double s = 1000;
    double PX = 10000 / s, PY = 5000 / s, PZ = 2100 / s;
    double Nx = 100*2, Ny = 50*2, Nz = 21*2;
    double l0 = 0.1 / s, L0 = 500 / s;
    int numberInhomogeneities = 100;
    double fluctuationAmplitude = 10.;

    std::srand(42);

    ABox* ab = new ABox(std::make_tuple(PX, PY, PZ),
                        std::make_tuple(Nx, Ny, Nz),
                        std::make_pair(l0, L0));

    Averager* averager = new ConeAverager(ab,
                                          std::make_tuple(PX/2, PY/2, 0.),
                                          1.25*PZ, 0.148492, 0.148492, 1,
                                          std::make_tuple(0, -51, 0)
                                          );

    ab->setStandardProfiles();
    ab->createStructuralInhomogeneities(numberInhomogeneities, false);

    std::string dumped3DPic = "tmp00.txt";
    ab->dumpInhomogeneities(dumped3DPic);
    averager->dump(dumped3DPic);

    //PipeGnuplotter::scatter3d(dumped3DPic);
    std::thread t1 = Plotter::Threaded::scatter3d(dumped3DPic, 3);

    std::default_random_engine g;
    std::uniform_real_distribution<double> dist(0, fluctuationAmplitude);
    double drho = dist(g);
    ab->setLambdaHumidity([&drho,fluctuationAmplitude](double rho, Dot3D point){ return rho - drho + fluctuationAmplitude/2.; });

    ab->applyStructuralInhomogeneities(false);

    std::string dumpedHumidityProfile = "tmp01.txt";
    ab->dumpAltitudeProfile(ab->getAltitudeProfileHumidity(averager), dumpedHumidityProfile);

    //PipeGnuplotter::plot2d(dumpedHumidityProfile);
    std::thread t2 = Plotter::Threaded::altitudeScatter2d(dumpedHumidityProfile, 1, "absolute humidity", "height");
    std::thread t3 = Plotter::Threaded::altitudePlot2d(dumpedHumidityProfile, 1, "absolute humidity", "height");

    t1.join();
    t2.join();
    t3.join();

    std::remove(dumped3DPic.c_str());
    std::remove(dumpedHumidityProfile.c_str());

    exit(0);

//    return a.exec();
}
