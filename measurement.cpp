#include "measurement.h"
#include "colormod.h"


std::vector<Frequency> Measurement::keys() {
    std::vector<Frequency> keys;
    for (std::pair<Frequency, TimeSeries> item : *DATA)
        keys.push_back(item.first);
    return keys;
}

std::vector<Frequency> Measurement::getKeys(MDATA* DATA) {
    Measurement* m = new Measurement(DATA);
    return m->keys();
}

TimeSeries* Measurement::getTimeSeries(Frequency f) {
    for (MDATA::iterator it = DATA->begin(); it != DATA->end(); it++) {
        if (abs(it->first - f) < FREQ_EPS) {
            return &(it->second);
        }
    }
    return nullptr;
}

void Measurement::remember(Spectrum spectrum, Timestamp time) {
    for (std::pair<Frequency, double> peak : spectrum) {
        TimeSeries* series = nullptr;
        if ((series = getTimeSeries(peak.first)) != nullptr)
            series->push_back(std::make_pair(time, peak.second));
        else {
            TimeSeries series;
            series.push_back(std::make_pair(time, peak.second));
            DATA->insert(std::pair<Frequency, TimeSeries>(peak.first, series));
        }
    }
}

void Measurement::remember(Spectrum spectrum, Timestamp time, MDATA* DATA) {
    Measurement* m = new Measurement(DATA);
    m->remember(spectrum, time);
}

//write MDATA - only selected frequencies
//              if tabular=false, then frequency blocks are separated by an empty line,
//              else data is written in a tabular structure
void Measurement::dump(std::vector<Frequency> frequencies, std::string file_path, bool tabular, bool titled) {
    Color::Modifier red(Color::FG_RED);
    Color::Modifier def(Color::FG_DEFAULT);
    if (!DATA->size()) {
        std::cout << red << "Dump Error: No data at all." << def << std::endl;
        return;
    }
    std::ofstream out;
    out.open(file_path);
    TimeSeries* series;
    if (!tabular) {
        for (Frequency f : frequencies) {
            if ((series = getTimeSeries(f)) != nullptr) {
                out << f << std::endl;
                Dump::timeSeries(*series, out);
                out << std::endl;
            }
        }
        return;
    }

    TimeSeries* ts = nullptr;
    int size = 0; bool f = false;
    for (Frequency freq : frequencies) {
        if ((series = getTimeSeries(freq)) != nullptr) {
            if (!f) {
                size = int(series->size());
                f = true;
            }
            if (size != int(series->size())) {
                std::cout << red << "Dump Error: if tabular is set, timeseries must have the same size."
                          << def << std::endl;
                return;
            }
            ts = series;
        }
    }

    if (ts == nullptr) {
        std::cout << red << "Dump Error: No data at such frequencies." << def << std::endl;
        return;
    }

    if (titled) {
        out << "time ";
        for (Frequency freq : frequencies)
            if ((series = getTimeSeries(freq)) != nullptr)
                out << freq << "F ";
        out << std::endl;
    }

    for (unsigned int i = 0; i < ts->size(); i++) {
        out << (*ts).at(i).first << " ";
        for (Frequency freq : frequencies) {
            if ((series = getTimeSeries(freq)) != nullptr)
                out << (*series).at(i).second << " ";
        }
        out << std::endl;
    }
    out << std::endl;

    out.close();
}

//write MDATA - if tabular=false, then frequency blocks are separated by an empty line,
//              else data is written in a tabular structure
void Measurement::dump(std::string file_path, bool tabular, bool titled) {
    dump(keys(), file_path, tabular, titled);
}

void Measurement::dump(MDATA *DATA, std::vector<Frequency> frequencies,
                       std::string file_path, bool tabular, bool titled) {
    Measurement* m = new Measurement(DATA);
    m->dump(frequencies, file_path, tabular, titled);
}

void Measurement::dump(MDATA *DATA, std::string file_path, bool tabular, bool titled) {
    Measurement* m = new Measurement(DATA);
    m->dump(file_path, tabular, titled);
}

//write only several frequencies
void Dump::spectrum(Spectrum spectrum, std::vector<Frequency> frequencies, std::ofstream& out) {
    for (unsigned int k = 0; k < spectrum.size(); k++) {
        for (unsigned int i = 0; i < frequencies.size(); i++)
            if (abs(spectrum[k].first - frequencies[i]) < FREQ_EPS) {
                out << spectrum[k].first << " " << spectrum[k].second << std::endl;
                break;
        }
    }
}

//write only several frequencies
void Dump::spectrum(Spectrum spectrum, std::vector<Frequency> frequencies, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    Dump::spectrum(spectrum, frequencies, out);
    out << std::endl;
    out.close();
}

//write spectrum as-is
void Dump::spectrum(Spectrum spectrum, std::string file_path, bool append) {
    std::vector<Frequency> freqs;
    for (std::pair<Frequency, double> peak : spectrum) freqs.push_back(peak.first);
    Dump::spectrum(spectrum, freqs, file_path, append);
}

//write timestamp and value for only one frequency
void Dump::peak(Spectrum spectrum, Frequency frequency, Timestamp t, std::ofstream& out) {
    for (unsigned int k = 0; k < spectrum.size(); k++)
        if (abs(spectrum[k].first - frequency) < FREQ_EPS)
             out << t << " " << spectrum[k].second << std::endl;
}

//write timestamp and value for only one frequency
void Dump::peak(Spectrum spectrum, Frequency frequency, Timestamp t, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    Dump::peak(spectrum, frequency, t, out);
    out.close();
}

//write TimeSeries as-is
void Dump::timeSeries(TimeSeries series, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    Dump::timeSeries(series, out);
    out << std::endl;
    out.close();
}

//write TimeSeries as-is into ofstream
void Dump::timeSeries(TimeSeries series, std::ofstream& out) {
    for (std::pair<Timestamp, double> item : series)
        out << item.first << " " << item.second << std::endl;
}

void Dump::mData(MDATA* DATA, std::vector<Frequency> frequencies,
                 std::string file_path, bool tabular, bool titled) {
    Measurement::dump(DATA, frequencies, file_path, tabular, titled);
}

void Dump::mData(MDATA* DATA, std::string file_path, bool tabular, bool titled) {
    Measurement::dump(DATA, file_path, tabular, titled);
}

