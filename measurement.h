#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#define FREQ_EPS 0.00001

#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <math.h>

typedef double Frequency;
typedef double Timestamp;
typedef std::vector<double> Profile;
typedef std::vector<std::pair<Frequency, double>> Spectrum;
typedef std::vector<std::pair<Timestamp, double>> TimeSeries;
typedef std::map<Frequency, TimeSeries> MDATA;


void remember(Spectrum, Timestamp, MDATA*);

void dumpSpectrum(Spectrum spectrum,
                  std::string file_path, bool append = true);
void dumpSpectrum(Spectrum spectrum, std::vector<Frequency> frequencies,
                  std::string file_path, bool append = true);
void dumpSpectrum(Spectrum spectrum, Frequency frequency, Timestamp t,
                  std::string file_path, bool append = true);




#endif // MEASUREMENT_H
