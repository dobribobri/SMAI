#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#define FREQ_EPS 0.00001

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>
#include <math.h>
#include <algorithm>
#include "colormod.h"

typedef double Frequency;
typedef double Timestamp;
typedef std::vector<double> Profile;
typedef std::vector<std::pair<Frequency, double>> Spectrum;
typedef std::vector<std::pair<Timestamp, double>> TimeSeries;
typedef std::map<Frequency, TimeSeries> MDATA;


class Measurement {
    private:
        Color::Modifier* fgred = new Color::Modifier(Color::FG_RED);
        Color::Modifier* fgdef = new Color::Modifier(Color::FG_DEFAULT);

    public:
        MDATA* DATA;

        Measurement();
        Measurement(MDATA* _DATA) : DATA(_DATA) { }

        std::vector<Frequency> keys();
        static std::vector<Frequency> getKeys(MDATA* DATA);

        TimeSeries* getTimeSeries(Frequency f);

        void remember(Spectrum, Timestamp);
        void remember(Frequency, std::vector<double>, std::vector<double>);
        static void remember(Spectrum, Timestamp, MDATA*);
        static void remember(Frequency, std::vector<double>, std::vector<double>, MDATA*);

        void normalize();
        static void normalize(MDATA*);

        void modulus();
        static void modulus(MDATA*);

        void dump(std::string file_path, bool tabular = true, bool titled = true);
        void dump(std::vector<Frequency> frequencies,
                  std::string file_path, bool tabular = true, bool titled = false);
        static void dump(MDATA* DATA, std::string file_path, bool tabular = true, bool titled = true);
        static void dump(MDATA* DATA, std::vector<Frequency> frequencies,
                  std::string file_path, bool tabular = true, bool titled = false);

        void clear();
        static void clear(MDATA*);

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
