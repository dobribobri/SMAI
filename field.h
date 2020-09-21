#ifndef FIELD_H
#define FIELD_H

#define Dot3D std::tuple<double, double, double>

#include <string>
#include <tuple>
#include <functional>


class Field
{
public:
    std::string name;
    int Nx, Ny, Nz;
    double*** field;

    Field(std::string _name, std::tuple<int, int, int> _N) :
        name(_name) {
        std::tie(Nx, Ny, Nz) = _N;
    }

    void initialize();

    void applyLambda(const std::function<double(double, Dot3D)> lambda,
                     std::tuple<int, int, int> indexes, Dot3D point);

};

#endif // FIELD_H
