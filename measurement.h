#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#define FREQ_EPS 0.00001

#include <iostream>
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
//TBD

void dumpTimeSeries(TimeSeries series, std::ofstream& out);
void dumpTimeSeries(TimeSeries series, std::string file_path, bool append = true);

void dumpMDATA(MDATA* DATA, std::string file_path, bool tabular = true, bool titled = false);
//void dumpMDATA(MDATA* DATA, std::vector<Frequency> frequencies,
//               std::string file_path, bool tabular = true, bool titled = false);

std::vector<Frequency> getKeys(MDATA* MDATA);




#endif // MEASUREMENT_H
