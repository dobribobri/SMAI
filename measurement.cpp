#include "measurement.h"
#include "colormod.h"


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

//write spectrum as it is
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

//write only several frequencies
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

//write timestamp and value for only one frequency
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

//write TimeSeries as it is
void dumpTimeSeries(TimeSeries series, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    dumpTimeSeries(series, out);
    out << std::endl;
    out.close();
}

//write TimeSeries as it is into ofstream
void dumpTimeSeries(TimeSeries series, std::ofstream& out) {
    for (std::pair<Timestamp, double> item : series)
        out << item.first << " " << item.second << std::endl;
}

std::vector<Frequency> getKeys(MDATA* DATA) {
    std::vector<double> freqs;
    for (std::pair<Frequency, TimeSeries> item : *DATA)
        freqs.push_back(std::get<0>(item));
    return freqs;
}

//write MDATA - if tabular=false, then frequency blocks are separated by an empty line,
//              else data is written in a tabular structure
void dumpMDATA(MDATA* DATA, std::string file_path, bool tabular, bool titled) {
    Color::Modifier red(Color::FG_RED);
    Color::Modifier def(Color::FG_DEFAULT);
    if (!DATA->size()) {
        std::cout << red << "Dump Error: No data" << def << std::endl;
        return;
    }
    std::ofstream out;
    out.open(file_path);
    if (!tabular) {
        for (MDATA::iterator it = DATA->begin(); it != DATA->end(); it++) {
            out << it->first << std::endl;
            dumpTimeSeries(it->second, out);
            out << std::endl;
        }
        return;
    }
    if (tabular) {
        int size = int(DATA->begin()->second.size());
        for (std::pair<Frequency, TimeSeries> item : *DATA) {
            if (size != int(std::get<1>(item).size())) {
                std::cout << red << "Dump Error: if tabular is set, timeseries must have the same size"
                          << def << std::endl;
                return;
            }
        }
        std::vector<Frequency> freqs = getKeys(DATA);
        if (titled) {
            out << "time ";
            for (unsigned int i = 0; i < freqs.size(); i++)
                out << freqs[i] << "GHz ";
            out << std::endl;
        }
        for (unsigned int i = 0; i < unsigned(size); i++) {
            out << (*DATA)[freqs[0]][i].first << " ";
            for (Frequency f : freqs) {
                out << (*DATA)[f][i].second << " ";
            }
            out << std::endl;
        }
        out << std::endl;
    }
    out.close();
}

