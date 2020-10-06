
#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tuple>
#include <map>
#include <utility>
#include <random>
#include <string>
#include <thread>
#include <locale>
#include <sstream>
#include "abox.h"
#include "averager.h"
#include "pipegnuplotter.h"
#include "linspace.h"
#include "attenuationmodel.h"
#include "structuralfunction.h"
#include "measurement.h"
#include "variate.h"
#include "stat.h"


int main(int argc, char *argv[])
{
    setlocale(LC_ALL, nullptr);

    QCoreApplication a(argc, argv);

    //std::ifstream pysrce("../SMAI/plotter.py", std::ios::binary);
    //std::ofstream pydest("plotter.py", std::ios::binary);
    //pydest << pysrce.rdbuf();

    double s = 1000;
    //double PX = 10000 / s, PY = 5000 / s, PZ = 2100 / s;
    //double Nx = 100*2, Ny = 50*2, Nz = 21*2;
    double PX = 10000 / s, PY = 5000 / s, PZ = 10000 / s;
    double Nx = 100*2, Ny = 50*2, Nz = 50*10;
    double l0 = 0.1 / s, L0 = 500 / s;
    int numberInhomogeneities = 1000;
    double fA = 10.;
    unsigned int seed = 42;
    double T0 = 15.;
    //double T0 = 19.65;
    double P0 = 760. * 1.333;
    //double P0 = 755.66 * 1.333;
    double rho0 = 7.5;
    //double rho0 = 10.57;

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

    AttenuationModel* model = new P676K();


    ab->setStandardProfiles(T0, P0, rho0);

    //ab->createStructuralInhomogeneities(numberInhomogeneities, false);

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

//    Plot the created inhomogeneities as ellipsoids and the figure of averager in 3D.
//    ab->dumpInhomogeneities(tmp00);
//    averager->dump(tmp00);
//    std::thread t0 = PipeGnuplotter::Threaded::scatter3d(tmp00);

// Get variated altitude profiles of absolute humidity and exit.
//    std::vector<double> z = ab->grid("z");
//    Profile rho = ab->getAltitudeProfileHumidity();
//    //int h = int(rho.size());
//    double H2 = 2.1/PZ*Nz;
//    for (unsigned int i = 0; i < 50; i++) {
//        MDATA rho_variated;
//        double stddev = (i+1)*0.1;
//        std::vector<Profile> profiles =
//                Variate::gaussian(rho,
//                                  [&H2,stddev](unsigned int i){ return exp(-(double(i)+1)/H2)*stddev; },
//                                  1000);
//        for (unsigned int k = 0; k < profiles.size(); k++)
//            Measurement::remember(k, z, profiles[k], &rho_variated);
//        Measurement::modulus(&rho_variated);
//        std::string filename = "abs_humidity_profiles_stddev" + std::to_string(i+1) + "x0.1.txt";
//        std::remove(filename.c_str());
//        Dump::mData(&rho_variated, filename);
//    }
//    exit(0);

// Get H2O weighting functions and exit.
//    MDATA WH2O;
//    std::string tmp_wh2odata;
//    for (double f = 18.0; f <= 27.2; f += 0.2) {
//        std::vector<double> z = ab->grid("z");
//        Profile W = ab->W_H2O(model, f);
//        Measurement::remember(f, z, W, &WH2O);
//    }
//    tmp_wh2odata = "tmp_wh2odata_18.0_27.2_GHz_non_normalized.txt";
//    std::remove(tmp_wh2odata.c_str());
//    Dump::mData(&WH2O, tmp_wh2odata);
//    Measurement::normalize(&WH2O);
//    tmp_wh2odata = "tmp_wh2odata_18.0_27.2_GHz_normalized.txt";
//    std::remove(tmp_wh2odata.c_str());
//    Dump::mData(&WH2O, tmp_wh2odata);
//    Measurement::clear(&WH2O);
//    for (double f = 21.0; f <= 23.0; f += 0.1) {
//        std::vector<double> z = ab->grid("z");
//        Profile W = ab->W_H2O(model, f);
//        Measurement::remember(f, z, W, &WH2O);
//    }
//    tmp_wh2odata = "tmp_wh2odata_21.0_23.0_GHz_non_normalized.txt";
//    std::remove(tmp_wh2odata.c_str());
//    Dump::mData(&WH2O, tmp_wh2odata);
//    Measurement::normalize(&WH2O);
//    tmp_wh2odata = "tmp_wh2odata_21.0_23.0_GHz_normalized.txt";
//    std::remove(tmp_wh2odata.c_str());
//    Dump::mData(&WH2O, tmp_wh2odata);
//    exit(0);

    std::vector<double> z = ab->grid("z");
    Profile T = ab->getAltitudeProfileTemperature();
    Profile P = ab->getAltitudeProfilePressure();
    Profile rho = ab->getAltitudeProfileHumidity();
    double H2 = 2.1/PZ*(Nz-1);
    MDATA DATA;
    Measurement* m = new Measurement(&DATA);
    for (unsigned int j = 0; j < 50; j++) {
        double stddev = (j+1)*0.1;
        std::function<double(unsigned int)> lambda = [&H2,stddev](unsigned int i){ return exp(-double(i)/H2)*stddev; };
        std::vector<Profile> profiles = Variate::gaussian(rho, lambda, 1000);
        for (unsigned int i = 0; i < profiles[0].size(); i++) {
            std::vector<double> layerRho;
            for (unsigned int k = 0; k < profiles.size(); k++) layerRho.push_back(profiles[k][i]);
            std::map<Frequency, std::vector<double>> layerWH2O;
            for (double f = 18.0; f <= 27.2; f += 0.2) {
                for (unsigned int k = 0; k < profiles.size(); k++)
                    layerWH2O[f].push_back(model->gammaWVapor(f, T[i], P[i], profiles[k][i]) / profiles[k][i]);
            }
            double height = i*(ab->PZ/(ab->Nz-1));
            std::cout << "\nHeight: " << height << std::endl;
            double stddev_recalc = Stat::StandardDeviation(rho[i], layerRho);
            std::cout << "StdDev declared: " << lambda(i) << "\t|\tStdDev obtained: " << stddev_recalc << std::endl;
            Spectrum wh2o_stddev;
            for (double f = 18.0; f <= 27.2; f += 0.2) {
                double mean_wh2o = model->gammaWVapor(f, T[i], P[i], rho[i]) / rho[i];
                wh2o_stddev.push_back(std::make_pair(f, Stat::StandardDeviation(mean_wh2o, layerWH2O[f])));
            }
            m->remember(wh2o_stddev, height);
        }
        std::ostringstream out;
        out.precision(1);
        out << std::fixed << stddev;
        m->dump("WH2O_deviations_on_height_10km__rho(0)_stddev_" + out.str() + ".txt");
        m->clear();
    }
    exit(0);

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
        Dump::peak(brTemp_k, 22.2, k*time_step, tmp02);

        Measurement::remember(brTemp_k, k*time_step, &TBDATA);

        //ab->moveStructuralInhomogeneities(std::make_tuple(10./s, 0, 0), time_step);
        ab->moveFieldsPeriodicX(10./s*time_step);
    }

    std::thread t1 = PipeGnuplotter::Threaded::plot2d(tmp01);
    std::thread t2 = PipeGnuplotter::Threaded::plot2d(tmp02);

    Dump::mData(&TBDATA, tmp_tbdata);
    std::thread tbplot = PipeGnuplotter::Threaded::plot2d(tmp_tbdata,
                                         PipeGnuplotter::Palette::set_style_commands);

    MDATA SFDATA = structuralFunctions(TBDATA);

    Dump::mData(&SFDATA, tmp_sfdata);
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
