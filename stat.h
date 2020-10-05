#ifndef STAT_H
#define STAT_H

#include <vector>
#include <math.h>
#include <algorithm>


namespace Stat {

    double Mean(std::vector<double> vec);
    double Range(std::vector<double> vec);

    double Variance(std::vector<double> vec);
    double Variance(double mean, std::vector<double> vec);
    double StandardDeviation(std::vector<double> vec);
    double StandardDeviation(double mean, std::vector<double> vec);

    double MSD(std::vector<double> predicted, std::vector<double> obtained);
    std::vector<double> MSD(std::vector<double> predicted, std::vector<std::vector<double>> obtained);

    double RMSD(std::vector<double> predicted, std::vector<double> obtained);
    std::vector<double> RMSD(std::vector<double> predicted, std::vector<std::vector<double>> obtained);

}

#endif // STAT_H
