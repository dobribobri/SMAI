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


class Measurement {
public:
    MDATA* DATA;

    Measurement();
    Measurement(MDATA* _DATA) : DATA(_DATA) { }

    std::vector<Frequency> keys();
    static std::vector<Frequency> getKeys(MDATA* DATA);

    TimeSeries* getTimeSeries(Frequency f);

    void remember(Spectrum, Timestamp);
    static void remember(Spectrum, Timestamp, MDATA*);

    void dump(std::string file_path, bool tabular = true, bool titled = true);
    void dump(std::vector<Frequency> frequencies,
              std::string file_path, bool tabular = true, bool titled = false);
    static void dump(MDATA* DATA, std::string file_path, bool tabular = true, bool titled = true);
    static void dump(MDATA* DATA, std::vector<Frequency> frequencies,
              std::string file_path, bool tabular = true, bool titled = false);

};


namespace Dump {

    void spectrum(Spectrum spectrum,
                  std::string file_path, bool append = true);
    void spectrum(Spectrum spectrum, std::vector<Frequency> frequencies,
                  std::ofstream& out);
    void spectrum(Spectrum spectrum, std::vector<Frequency> frequencies,
                  std::string file_path, bool append = true);

    void peak(Spectrum spectrum, Frequency frequency, Timestamp t,
              std::ofstream& out);
    void peak(Spectrum spectrum, Frequency frequency, Timestamp t,
              std::string file_path, bool append = true);

    void timeSeries(TimeSeries series, std::ofstream& out);
    void timeSeries(TimeSeries series, std::string file_path, bool append = true);

    void mData(MDATA* DATA, std::string file_path, bool tabular = true, bool titled = true);
    void mData(MDATA* DATA, std::vector<Frequency> frequencies,
                   std::string file_path, bool tabular = true, bool titled = false);
}




#endif // MEASUREMENT_H
