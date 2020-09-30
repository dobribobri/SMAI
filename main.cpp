
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
#include "linspace.h"
#include "attenuationmodel.h"
#include "structuralfunction.h"
#include "measurement.h"


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
    double fA = 10.;
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

    AttenuationModel* model = new KP676();


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
    std::string tmp_tbdata = "tmp_tbdata.txt";
    std::string tmp_sfdata = "tmp_sfdata.txt";
    std::remove(tmp00.c_str());
    std::remove(tmp01.c_str());
    std::remove(tmp02.c_str());
    std::remove(tmp_tbdata.c_str());
    std::remove(tmp_sfdata.c_str());

    ab->dumpInhomogeneities(tmp00);
    averager->dump(tmp00);
    //std::thread t0 = PipeGnuplotter::Threaded::scatter3d(tmp00);

    ab->setStandardProfiles(T0, P0, rho0);

    ab->applyStructuralInhomogeneities(false);

    MDATA TBDATA;

    for (int k = 0; k < 10; k++) {
        std::cout << "Iteration: " << k+1 << std::endl;
        double time_step = 11.;

        Profile hum = ab->getAltitudeProfileHumidity(averager);
        ab->dumpAltitudeProfile(hum, tmp01);

        //std::vector<Frequency> freqs = linspace(18.0, 27.2, 47);
        std::vector<Frequency> freqs = {18.0, 19.4, 21.6, 22.2, 25.0, 27.2};
        Spectrum brTemp_k = ab->getBrightnessTemperature(freqs, averager, model, 51);
        dumpSpectrum(brTemp_k, 22.2, k*time_step, tmp02);

        remember(brTemp_k, k*time_step, &TBDATA);

        //ab->moveStructuralInhomogeneities(std::make_tuple(10./s, 0, 0), time_step);
        ab->moveFieldsPeriodicX(10./s*time_step);
    }

    std::thread t1 = PipeGnuplotter::Threaded::plot2d(tmp01);
    std::thread t2 = PipeGnuplotter::Threaded::plot2d(tmp02);

    dumpMDATA(&TBDATA, tmp_tbdata);
    std::thread tbplot = PipeGnuplotter::Threaded::plot2d(tmp_tbdata,
                                         PipeGnuplotter::Palette::set_style_commands);

    MDATA SFDATA = structuralFunctions(TBDATA);

    dumpMDATA(&SFDATA, tmp_sfdata);
    std::thread sfplot = PipeGnuplotter::Threaded::plot2d(tmp_sfdata,
                                         PipeGnuplotter::Palette::set_style_commands);

    //t0.join();
    t1.join();
    t2.join();
    tbplot.join();
    sfplot.join();

    exit(0);

    //return a.exec();
}
