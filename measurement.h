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
typedef double AnyDouble;
typedef std::vector<double> Profile;
typedef std::vector<std::pair<AnyDouble, double>> Spectrum;
typedef std::vector<std::pair<AnyDouble, double>> Series;
typedef Series TimeSeries;
typedef std::map<AnyDouble, Series> MDATA;


class Measurement {
    private:
        Color::Modifier* fgred = new Color::Modifier(Color::FG_RED);
        Color::Modifier* fgdef = new Color::Modifier(Color::FG_DEFAULT);

    public:
        MDATA* DATA;

        Measurement();
        Measurement(MDATA* _DATA) : DATA(_DATA) { }

        std::vector<AnyDouble> keys();
        static std::vector<AnyDouble> getKeys(MDATA* DATA);

        Series* getSeries(AnyDouble frequency_or_key);

        void remember(Spectrum, AnyDouble);
        void remember(AnyDouble, std::vector<double>, std::vector<double>);
        static void remember(Spectrum, AnyDouble, MDATA*);
        static void remember(AnyDouble, std::vector<double>, std::vector<double>, MDATA*);

        void normalize();
        static void normalize(MDATA*);

        void modulus();
        static void modulus(MDATA*);

        void dump(std::string file_path, bool tabular = true, bool titled = true);
        void dump(std::vector<AnyDouble> frequencies_or_keys,
                  std::string file_path, bool tabular = true, bool titled = false);
        static void dump(MDATA* DATA, std::string file_path, bool tabular = true, bool titled = true);
        static void dump(MDATA* DATA, std::vector<AnyDouble> frequencies_or_keys,
                  std::string file_path, bool tabular = true, bool titled = false);

        void clear();
        static void clear(MDATA*);

};


namespace Dump {

    void spectrum(Spectrum spectrum,
                  std::string file_path, bool append = true);
    void spectrum(Spectrum spectrum, std::vector<AnyDouble> frequencies_or_keys,
                  std::ofstream& out);
    void spectrum(Spectrum spectrum, std::vector<AnyDouble> frequencies_or_keys,
                  std::string file_path, bool append = true);

    void peak(Spectrum spectrum, AnyDouble frequency_or_key, AnyDouble time_or_key2,
              std::ofstream& out);
    void peak(Spectrum spectrum, AnyDouble frequency_or_key, AnyDouble time_or_key2,
              std::string file_path, bool append = true);

    void series(Series series, std::ofstream& out);
    void series(Series series, std::string file_path, bool append = true);

    void mData(MDATA* DATA, std::string file_path, bool tabular = true, bool titled = true);
    void mData(MDATA* DATA, std::vector<AnyDouble> frequencies_or_keys,
                   std::string file_path, bool tabular = true, bool titled = false);

}

#endif // MEASUREMENT_H
