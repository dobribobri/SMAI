#include "field.h"
#include <iostream>
#include <math.h>

void Field::initialize() {
    field = new double** [this->Nx];
    for (int i = 0; i < this->Nx; i++) {
        field[i] = new double* [this->Ny];
        for (int j = 0; j < this->Ny; j++) {
            field[i][j] = new double [this->Nz];
            for (int k = 0; k < this->Nz; k++) {
                field[i][j][k] = 0.;
            }
        }
    }
}

void Field::applyLambda(std::function<double (double, Dot3D)> lambda,
                        std::tuple<int, int, int> indexes, Dot3D point) {
    int i, j, k;
    std::tie(i, j, k) = indexes;
    field[i][j][k] = lambda(field[i][j][k], point);
}
