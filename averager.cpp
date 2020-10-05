#include "averager.h"
#include "abox.h"
#include "digitizer.h"
#include "colormod.h"
#include "rotation.h"


Averager::Averager(ABox* _aBox, Dot3D _center, double _height,
                   std::tuple<double, double, double> _rotation,
                   TColor _color) :
              aBox(_aBox), height(_height), color(_color) {
              std::tie(xi, yi, zi) = _center;
              double alpha_z, beta_y, gamma_x;
              std::tie(alpha_z, beta_y, gamma_x) = _rotation;
              r = new Rotation(alpha_z, beta_y, gamma_x);
              std::tie(xr, yr, zr) = r->rotate(_center);
          }

Averager::~Averager() { }


std::vector<Dot3D> Averager::digitize(std::tuple<int, int, int> resolution) {
    Digitizer d;
    return d.digitize(this, std::make_pair(-aBox->PX/2, aBox->PX/2),
                      std::make_pair(-aBox->PY/2, aBox->PY/2),
                      std::make_pair(0., aBox->PZ),
                      resolution);
}

void Averager::dump(std::string file_path, std::tuple<int, int, int> resolution, bool append) {
    clock_t start = clock();
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path, std::ios::app);
    double x, y, z;
    std::vector<Dot3D> points = this->digitize(resolution);
    for (unsigned int i = 0; i < points.size(); i++) {
        std::tie(x, y, z) = points[i];
        out << x << " " << y << " " << z << " ";
        float r, g, b, a;
        std::tie(r, g, b, a) = this->color;
        out << r << " " << g << " " << b << " " << a << std::endl;
    }
    out << std::endl;
    out.close();
    clock_t end = clock();
    double seconds = double(end - start) / CLOCKS_PER_SEC;
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);
    std::cout << green << "Averager::dump\t-\t" << seconds << " sec." << def << std::endl;
}


bool ConeAverager::includesQ(Dot3D coords) {
    double x, y, z;
    std::tie(x, y, z) = r->rotate(coords);
    return (x-xr)*(x-xr)/(a*a) +
            (y-yr)*(y-yr)/(b*b) -
             (z-zr)*(z-zr)/(c*c) < 0;
}


bool CylinderAverager::includesQ(Dot3D coords) {
    double x, y;
    std::tie(x, y, std::ignore) = r->rotate(coords);
    return (x-xr)*(x-xr)/(a*a) + (y-yr)*(y-yr)/(b*b) <= 1;
}
