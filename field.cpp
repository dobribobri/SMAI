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

double** Field::sliceX(int i) {
    double** slice = new double* [Ny];
    for (int j = 0; j < Ny; j++) slice[j] = new double [Nz];
    for (int j = 0; j < Ny; j++) {
        for (int k = 0; k < Nz; k++) slice[j][k] = field[i][j][k];
    }
    return slice;
}

double** Field::sliceY(int j) {
    double** slice = new double* [Nx];
    for (int i = 0; i < Nx; i++) slice[i] = new double [Nz];
    for (int i = 0; i < Nx; i++) {
        for (int k = 0; k < Nz; k++) slice[i][k] = field[i][j][k];
    }
    return slice;
}

double** Field::sliceZ(int k) {
    double** slice = new double* [Nx];
    for (int i = 0; i < Nx; i++) slice[i] = new double [Ny];
    for (int i = 0; i < Nx; i++) {
        for (int j = 0; j < Ny; j++) slice[i][j] = field[i][j][k];
    }
    return slice;
}

void Field::circularPermutationX() {
    double** slice = this->sliceX(Nx-1);
    for (int i = Nx - 1; i > 0; i--) {
        for (int j = 0; j < Ny; j++) {
            for (int k = 0; k < Nz; k++) {
                   field[i][j][k] = field[i-1][j][k];
            }
        }
    }
    for (int j = 0; j < Ny; j++) {
        for (int k = 0; k < Nz; k++) {
               field[0][j][k] = slice[j][k];
        }
    }
}

void Field::movePeriodic(int _i, int _j, int _k) {
    for (int i = 0; i < Nx; ++i) {

    }
}
