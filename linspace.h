#ifndef LINSPACE_H
#define LINSPACE_H

#include <iostream>
#include <vector>

template<typename T>
std::vector<double> linspace(T _start, T _end, int _n) {
    std::vector<double> linspaced;

    double start = static_cast<double>(_start);
    double end = static_cast<double>(_end);
    int num = static_cast<int>(_n);

    if (num == 0) { return linspaced; }
    if (num == 1) {
        linspaced.push_back(start);
        return linspaced;
    }

    double delta = (end - start) / (num - 1);

    for(int i = 0; i < num - 1; ++i) {
        linspaced.push_back(start + delta * i);
    }
    linspaced.push_back(end);

    return linspaced;
}

#endif // LINSPACE_H
