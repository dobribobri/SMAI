#ifndef AVERAGER_H
#define AVERAGER_H

#define Dot3D std::tuple<double, double, double>
#define TColor std::tuple<float, float, float, float>

#include "rotation.h"
#include <tuple>
#include <vector>
#include <utility>
#include <fstream>
#include <time.h>
#include <string>


class ABox;


class Averager
{
public:
    ABox* aBox;
    double xi, yi, zi;
    double xr, yr, zr;
    double height;
    Rotation* r;
    TColor color;

    Averager(ABox* _aBox, Dot3D _center, double _height,
             std::tuple<double, double, double> _rotation = std::make_tuple(0,0,0),
             TColor _color = std::make_tuple(1,1,1,1));

    virtual bool includesQ(Dot3D coordinates) = 0;

    std::vector<Dot3D> digitize(std::tuple<int, int, int> resolution);

    void dump(std::string file_path, std::tuple<int, int, int> resolution = std::make_tuple(100, 100, 100));

    virtual ~Averager() { }

};

class ConeAverager: public Averager
{
public:
    double a, b, c;

    ConeAverager(ABox* _aBox, Dot3D _center,
                 double _height,
                 double _ax, double _by, double _cz = 1.,
                 std::tuple<double, double, double> _rotation = std::make_tuple(0,0,0),
                 TColor _color = std::make_tuple(1,1,1,1)) :
        Averager(_aBox, _center, _height, _rotation, _color),
        a(_ax), b(_by), c(_cz) { }

    bool includesQ(Dot3D coordinates);

};

class CylinderAverager: public Averager
{
public:
    double a, b;

    CylinderAverager(ABox* _aBox, std::pair<double, double> _center,
                     double _height,
                     double _ax, double _by,
                     std::tuple<double, double, double> _rotation = std::make_tuple(0,0,0),
                     TColor _color = std::make_tuple(1,1,1,1)) :
        Averager(_aBox, std::make_tuple(std::get<0>(_center), std::get<1>(_center), 0), _height, _rotation, _color),
        a(_ax), b(_by) { }

    bool includesQ(Dot3D coordinates);

};


#endif // AVERAGER_H
