#ifndef ROTATION_H
#define ROTATION_H

#define Dot3D std::tuple<double, double, double>

#include <math.h>
#include <vector>
#include <tuple>

class Rotation
{
private:
    double ca = 1, sa = 0, cb = 1, sb = 0, cg = 1, sg = 0;
    std::vector<std::vector<double>> Rx;
    std::vector<std::vector<double>> Ry;
    std::vector<std::vector<double>> Rz;
    std::vector<std::vector<double>> M;

public:  
    double alpha_rad = 0, beta_rad = 0, gamma_rad = 0;

    Rotation(double alpha_z = 0., double beta_y = 0., double gamma_x = 0.) {
        alpha_rad = alpha_z * M_PI / 180;
        beta_rad = beta_y * M_PI / 180;
        gamma_rad = gamma_x * M_PI / 180;
        ca = cos(alpha_rad); sa = sin(alpha_rad);
        cb = cos(beta_rad); sb = sin(beta_rad);
        cg = cos(gamma_rad); sg = sin(gamma_rad);
        Rx.resize(3); Ry.resize(3); Rz.resize(3);
        M.resize(3);
        for (unsigned int i = 0 ; i < 3; i++) {
           Rx[i].resize(3); Ry[i].resize(3); Rz[i].resize(3);
           M[i].resize(3);
        }
        getRx(); getRy(); getRz();
        getFullMatrix();
    }

    std::vector<std::vector<double>> getRx() {
        Rx[0][0] = 1; Rx[0][1] = 0; Rx[0][2] = 0;
        Rx[1][0] = 0; Rx[1][1] = cg; Rx[1][2] = -sg;
        Rx[2][0] = 0; Rx[2][1] = sg; Rx[2][2] = cg;
        return this->Rx;
    }

    std::vector<std::vector<double>> getRy() {
        Ry[0][0] = cb; Ry[0][1] = 0; Ry[0][2] = sb;
        Ry[1][0] = 0; Ry[1][1] = 1; Ry[1][2] = 0;
        Ry[2][0] = -sb; Ry[2][1] = 0; Ry[2][2] = cb;
        return this->Ry;
    }

    std::vector<std::vector<double>> getRz() {
        Rz[0][0] = ca; Rz[0][1] = -sa; Rz[0][2] = 0;
        Rz[1][0] = sa; Rz[1][1] = ca; Rz[1][2] = 0;
        Rz[2][0] = 0; Rz[2][1] = 0; Rz[2][2] = 1;
        return this->Rz;
    }

    std::vector<std::vector<double>> getFullMatrix() {
        M[0][0] = ca*cb; M[0][1] = ca*sb*sg - sa*cg; M[0][2] = ca*sb*cg + sa*sg;
        M[1][0] = sa*cb; M[1][1] = sa*sb*sg + ca*cg; M[1][2] = sa*sb*cg - ca*sg;
        M[2][0] = -sb; M[2][1] = cb*sg; M[2][2] = cb*cg;
        return this->M;
    }

    Dot3D rotate(Dot3D coords) {
        double xyz[3] = {std::get<0>(coords), std::get<1>(coords), std::get<2>(coords)};
        double res[3];
        for (unsigned int i = 0; i < 3; i++) {
            res[i] = 0;
            for (unsigned int j = 0; j < 3; j++)
                res[i] += this->M[i][j] * xyz[j];
        }
        double x = res[0], y = res[1], z = res[2];
        return std::make_tuple(x, y, z);
    }

};

#endif // ROTATION_H
