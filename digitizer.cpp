#include "digitizer.h"
#include "linspace.h"


Digitizer::Digitizer() { }

std::vector<Dot3D> Digitizer::digitize(Dot3D center,
                                        const std::function<bool(Dot3D)> include_expr,
                                        std::pair<double, double> limits_x,
                                        std::pair<double, double> limits_y,
                                        std::pair<double, double> limits_z,
                                        std::tuple<int, int, int> resolution) {
    std::vector<Dot3D> points;
    double xi, yi, zi;
    std::tie(xi, yi, zi) = center;
    int nx, ny, nz;
    std::tie(nx, ny, nz) = resolution;
    for (double x: linspace(xi + std::get<0>(limits_x),
                            xi + std::get<1>(limits_x),
                            nx)) {
        for (double y: linspace(yi + std::get<0>(limits_y),
                                yi + std::get<1>(limits_y),
                                ny)) {
            for (double z: linspace(zi + std::get<0>(limits_z),
                                    zi + std::get<1>(limits_z),
                                    nz)) {
                Dot3D point = std::make_tuple(x, y, z);
                if (include_expr(point)) {
                    points.push_back(point);
                }
            }
        }
    }
    return points;
}

std::vector<Dot3D> Digitizer::digitize(Inhomogeneity* i,
                                       std::pair<double, double> limits_x,
                                       std::pair<double, double> limits_y,
                                       std::pair<double, double> limits_z,
                                       std::tuple<int, int, int> resolution) {
    return digitize(std::make_tuple(i->xi, i->yi, i->zi), [&i](Dot3D p){ return i->includesQ(p); }, limits_x, limits_y, limits_z, resolution);
}

std::vector<Dot3D> Digitizer::digitize(Averager* a,
                                       std::pair<double, double> limits_x,
                                       std::pair<double, double> limits_y,
                                       std::pair<double, double> limits_z,
                                       std::tuple<int, int, int> resolution) {
    return digitize(std::make_tuple(a->xi, a->yi, a->zi), [&a](Dot3D p){ return a->includesQ(p); }, limits_x, limits_y, limits_z, resolution);
}
