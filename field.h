#ifndef FIELD_H
#define FIELD_H

#define Dot3D std::tuple<double, double, double>

#include <string>
#include <tuple>
#include <functional>


class Field {
    public:
        std::string name;
        int Nx, Ny, Nz;
        double*** field;

        Field(std::string _name, std::tuple<int, int, int> _N) :
            name(_name) {
            std::tie(Nx, Ny, Nz) = _N;
        }

        void initialize();

        void applyLambda(std::function<double(double, Dot3D)> lambda,
                         std::tuple<int, int, int> indexes, Dot3D point);

        double** sliceX(int i);
        double** sliceY(int j);
        double** sliceZ(int k);

        void circularPermutationX_right();
        void circularPermutationX_left();
        //void circularPermutationY_right();
        //void circularPermutationY_left();
        //void circularPermutationZ_right();
        //void circularPermutationZ_left();

        void movePeriodicX(int i);

};

#endif // FIELD_H
