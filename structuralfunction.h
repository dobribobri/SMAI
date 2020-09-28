#ifndef STRUCTURALFUNCTION_H
#define STRUCTURALFUNCTION_H

#include <vector>
#include <tuple>
#include <utility>

typedef double Frequency;
typedef double Timestamp;
typedef std::vector<std::pair<Timestamp, double>> TimeSeries;
typedef std::vector<std::pair<Frequency, TimeSeries>> MDATA;


TimeSeries structuralFunction(TimeSeries xi, double part = 1., double rightShowLimit = 500.);

MDATA structuralFunctions(MDATA DATA, double part = 1., double rightShowLimit = 500.);


#endif // STRUCTURALFUNCTION_H
