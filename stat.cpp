#include "stat.h"

double Stat::Mean(std::vector<double> vec) {
    if (!vec.size())
        throw "Stat::Mean: Vector should have at least one element.";
    double mean = 0.;
    for (double val : vec) mean += val;
    return mean /= vec.size();
}

double Stat::Range(std::vector<double> vec) {
    if (!vec.size())
        throw "Stat::Range: Vector should have at least one element.";
    std::vector<double>::iterator max = std::max_element(vec.begin(), vec.end());
    std::vector<double>::iterator min = std::min_element(vec.begin(), vec.end());
    return abs(*max - *min);
}

double Stat::Variance(std::vector<double> vec) {
    if (vec.size() < 2)
        throw "Stat::Variance: Vector should have at least two elements if the mean value is not set.";
    double s_lin = 0., s_squared = 0.;
    for (double x : vec) {
        s_lin += x;
        s_squared += x*x;
    }
    return (s_squared - s_lin*s_lin/vec.size()) / (vec.size());
}

double Stat::StandardDeviation(std::vector<double> vec) {
     return sqrt(Stat::Variance(vec));
}

double Stat::Variance(double mean, std::vector<double> vec) {
    if (!vec.size())
        throw "Stat::Variance: Vector should have at least one element.";
    double variance = 0.;
    for (double x : vec) variance += (x - mean)*(x - mean);
    return variance / vec.size();
}

double Stat::StandardDeviation(double mean, std::vector<double> vec) {
    return sqrt(Stat::Variance(mean, vec));
}

double Stat::MSD(std::vector<double> predicted, std::vector<double> obtained) {
    if (predicted.size() != obtained.size())
        throw "Stat::MSD: Sizes of vectors should be the same.";
    double error = 0.;
    for (unsigned int i = 0; i < predicted.size(); i++) {
        error += (predicted[i] - obtained[i]) * (predicted[i] - obtained[i]);
    }
    error /= predicted.size();
    return error;
}

double Stat::RMSD(std::vector<double> predicted, std::vector<double> obtained) {
    return sqrt(Stat::MSD(predicted, obtained));
}

std::vector<double> Stat::MSD(std::vector<double> predicted, std::vector<std::vector<double>> obtained) {
    std::vector<double> errors;
    for (std::vector<double> vec : obtained)
        errors.push_back(Stat::MSD(predicted, vec));
    return errors;
}

std::vector<double> Stat::RMSD(std::vector<double> predicted, std::vector<std::vector<double>> obtained) {
    std::vector<double> errors;
    for (std::vector<double> vec : obtained)
        errors.push_back(sqrt(Stat::MSD(predicted, vec)));
    return errors;
}
