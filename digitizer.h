#ifndef DIGITIZER_H
#define DIGITIZER_H

#define Dot3D std::tuple<double, double, double>

#include "inhomogeneity.h"
#include "averager.h"
#include <vector>
#include <tuple>
#include <utility>
#include <functional>

class Digitizer
{
public:
    Digitizer();

    std::vector<Dot3D> digitize(Dot3D center,
                                const std::function<bool(Dot3D)> include_expr,
                                std::pair<double, double> limits_x,
                                std::pair<double, double> limits_y,
                                std::pair<double, double> limits_z,
                                std::tuple<int, int, int> resolution);

    std::vector<Dot3D> digitize(Inhomogeneity* i,
                                std::pair<double, double> limits_x,
                                std::pair<double, double> limits_y,
                                std::pair<double, double> limits_z,
                                std::tuple<int, int, int> resolution);

    std::vector<Dot3D> digitize(Averager* a,
                                std::pair<double, double> limits_x,
                                std::pair<double, double> limits_y,
                                std::pair<double, double> limits_z,
                                std::tuple<int, int, int> resolution);

};

#endif // DIGITIZER_H
