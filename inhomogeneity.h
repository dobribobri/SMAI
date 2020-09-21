#ifndef INHOMOGENEITY_H
#define INHOMOGENEITY_H

#define Dot3D std::tuple<double, double, double>
#define TColor std::tuple<float, float, float, float>

#include "abox.h"
#include <tuple>
#include <vector>
#include <iostream>


class Inhomogeneity
{
public:
    double xi, yi, zi;
    double ai, bi, ci;

    TColor color;
    int number;

    Inhomogeneity(Dot3D _vi,
                  double _ai, double _bi, double _ci,
                  TColor _color = std::make_tuple(1,1,1,1),
                  int _number = 0) :
        ai(_ai), bi(_bi), ci(_ci), color(_color), number(_number) {
        std::tie(xi, yi, zi) = _vi;
    }

    bool includesQ(Dot3D coordinates);

    std::vector<Dot3D> digitize(std::tuple<int, int, int> resolution);

    bool disjointQ(Inhomogeneity* e);

    bool withinBoxQ(ABox* aBox);

    void print();

};

#endif // INHOMOGENEITY_H
