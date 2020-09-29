#include "measurement.h"


void remember(Spectrum spectrum, Timestamp time, MDATA* DATA) {
    for (std::pair<Frequency, double> peak : spectrum) {
        bool f = false;
        for (MDATA::iterator it = DATA->begin(); it != DATA->end(); it++) {
            if (abs(it->first - peak.first) < FREQ_EPS) {
                it->second.push_back(std::make_pair(time, std::get<1>(peak)));
                f = true;
                break;
            }
        }
        if (!f) {
            TimeSeries series;
            series.push_back(std::make_pair(time, std::get<1>(peak)));
            DATA->insert(std::pair<Frequency, TimeSeries>(std::get<0>(peak), series));
        }
    }
}

void dumpSpectrum(Spectrum spectrum, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    for (unsigned int k = 0; k < spectrum.size(); k++) {
        double freq, val;
        std::tie(freq, val) = spectrum[k];
        out << freq << " " << val << std::endl;
    }
    out << std::endl;
    out.close();
}

void dumpSpectrum(Spectrum spectrum, std::vector<Frequency> frequencies, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    for (unsigned int k = 0; k < spectrum.size(); k++) {
        double freq, val;
        std::tie(freq, val) = spectrum[k];
        for (unsigned int i = 0; i < frequencies.size(); i++)
            if (abs(freq - frequencies[i]) < 0.00001)
                out << freq << " " << val << std::endl;
    }
    out << std::endl;
    out.close();
}

void dumpSpectrum(Spectrum spectrum, Frequency frequency, Timestamp t, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    for (unsigned int k = 0; k < spectrum.size(); k++) {
        double freq, val;
        std::tie(freq, val) = spectrum[k];
        if (abs(freq - frequency) < FREQ_EPS)
             out << t << " " << val << std::endl;
    }
    out.close();
}

