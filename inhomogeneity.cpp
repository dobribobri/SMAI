#include "abox.h"
#include "inhomogeneity.h"
#include "digitizer.h"
#include "colormod.h"
#include <math.h>
#include <algorithm>
#include <random>

bool Inhomogeneity::includesQ(std::tuple<double, double, double> coords) {
    double x, y, z;
    std::tie(x, y, z) = coords;
    return (x-xi)*(x-xi)/(ai*ai) + (y-yi)*(y-yi)/(bi*bi) +
            (z-zi)*(z-zi)/(ci*ci) - 1 <= 0;
}

bool Inhomogeneity::disjointQ(Inhomogeneity *e) {
    double xj = e->xi; double yj = e->yi; double zj = e->zi;
    double aj = e->ai; double bj = e->bi; double cj = e->ci;
    return (xj-xi)*(xj-xi)/((ai+aj)*(ai+aj)) +
            (yj-yi)*(yj-yi)/((bi+bj)*(bi+bj)) +
             (zj-zi)*(zj-zi)/((ci+cj)*(ci+cj)) >= 0;
}

std::vector<Dot3D> Inhomogeneity::digitize(std::tuple<int, int, int> resolution) {
    Digitizer d;
    return d.digitize(this, std::make_pair(-ai, ai),
                            std::make_pair(-bi, bi),
                            std::make_pair(-ci, ci),
                      resolution);
}

bool Inhomogeneity::withinBoxQ(ABox *aBox) {
    return std::min({ -xi + aBox->PX - ai, -yi + aBox->PY - bi,
                      -zi + aBox->PZ - ci, xi + aBox->PX - ai,
                      yi + aBox->PY - bi, zi + aBox->PZ - ci
                    }) >= 0;
}

void Inhomogeneity::print() {
    if (number)
        std::cout << number << ":\t(" << xi << ", " << yi << ", " << zi << "), "
                  << ai << ", " << bi << ", " << ci << std::endl;
    else
        std::cout << "(" << xi << ", " << yi << ", " << zi << "), "
                  << ai << ", " << bi << ", " << ci << std::endl;
}
