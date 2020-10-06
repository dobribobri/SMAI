#include "measurement.h"
#include "colormod.h"


std::vector<AnyDouble> Measurement::keys() {
    std::vector<AnyDouble> keys;
    for (std::pair<AnyDouble, Series> item : *DATA)
        keys.push_back(item.first);
    return keys;
}

std::vector<AnyDouble> Measurement::getKeys(MDATA* DATA) {
    Measurement* m = new Measurement(DATA);
    return m->keys();
}

Series* Measurement::getSeries(AnyDouble f) {
    for (MDATA::iterator it = DATA->begin(); it != DATA->end(); it++) {
        if (abs(it->first - f) < FREQ_EPS) {
            return &(it->second);
        }
    }
    return nullptr;
}

void Measurement::remember(Spectrum spectrum, AnyDouble t) {
    for (std::pair<AnyDouble, double> peak : spectrum) {
        Series* series = nullptr;
        if ((series = getSeries(peak.first)) != nullptr)
            series->push_back(std::make_pair(t, peak.second));
        else {
            Series series;
            series.push_back(std::make_pair(t, peak.second));
            DATA->insert(std::pair<AnyDouble, Series>(peak.first, series));
        }
    }
}

void Measurement::remember(Spectrum spectrum, AnyDouble t, MDATA* DATA) {
    Measurement* m = new Measurement(DATA);
    m->remember(spectrum, t);
}

void Measurement::remember(AnyDouble f, std::vector<double> first, std::vector<double> second) {
    if (first.size() != second.size()) {
        std::cout << *fgred << "Arrays should have the same size." << *fgdef << std::endl;
        return;
    }
    for (unsigned int i = 0; i < first.size(); i++) {
        (*DATA)[f].push_back(std::make_pair(first[i], second[i]));
    }
}

void Measurement::remember(AnyDouble f, std::vector<double> first, std::vector<double> second, MDATA* DATA) {
    Measurement* m = new Measurement(DATA);
    m->remember(f, first, second);
}

void Measurement::normalize() {
    for (MDATA::iterator it = DATA->begin(); it != DATA->end(); it++) {
        Series::iterator max = std::max_element((*it).second.begin(), (*it).second.end(),
              [](std::pair<AnyDouble, double> a, std::pair<AnyDouble, double> b){
                  return (a.second < b.second);
        });
        double _max = max->second;
        for (unsigned int i = 0; i < it->second.size(); i++)
            it->second[i].second /= _max;
    }
}

void Measurement::normalize(MDATA* DATA) {
    Measurement* m = new Measurement(DATA);
    m->normalize();
}

void Measurement::modulus() {
    for (MDATA::iterator it = DATA->begin(); it != DATA->end(); it++) {
        for (unsigned int i = 0; i < it->second.size(); i++)
            it->second[i].second = abs(it->second[i].second);
    }
}

void Measurement::modulus(MDATA* DATA) {
    Measurement* m = new Measurement(DATA);
    m->modulus();
}

void Measurement::clear() {
    for (MDATA::iterator it = DATA->begin(); it != DATA->end(); it++) it->second.clear();
    DATA->clear();
}

void Measurement::clear(MDATA* DATA) {
    Measurement* m = new Measurement(DATA);
    m->clear();
}

//write MDATA - only selected frequencies
//              if tabular=false, then frequency blocks are separated by an empty line,
//              else data is written in a tabular structure
void Measurement::dump(std::vector<AnyDouble> fs, std::string file_path, bool tabular, bool titled) {
    if (!DATA->size()) {
        //std::cout << *fgred << "Dump Error: No data at all." << *fgdef << std::endl;
        //return;
        throw "Dump Error: No data at all.";
    }
    std::ofstream out;
    out.open(file_path);
    Series* series;
    if (!tabular) {
        for (AnyDouble f : fs) {
            if ((series = getSeries(f)) != nullptr) {
                out << f << std::endl;
                Dump::series(*series, out);
                out << std::endl;
            }
        }
        return;
    }

    Series* ts = nullptr;
    int size = 0; bool f = false;
    for (AnyDouble freq : fs) {
        if ((series = getSeries(freq)) != nullptr) {
            if (!f) {
                size = int(series->size());
                f = true;
            }
            if (size != int(series->size())) {
                //std::cout << *fgred << "Dump Error: if tabular is set, timeseries must have the same size."
                //          << *fgdef << std::endl;
                //return;
                throw "Dump Error: if tabular is set, timeseries must have the same size.";
            }
            ts = series;
        }
    }

    if (ts == nullptr) {
        //std::cout << *fgred << "Dump Error: No data at such frequencies." << *fgdef << std::endl;
        //return;
        throw "Dump Error: No data at such frequencies.";
    }

    if (titled) {
        out << "T ";
        for (AnyDouble freq : fs)
            if ((series = getSeries(freq)) != nullptr)
                out << freq << "F ";
        out << std::endl;
    }

    for (unsigned int i = 0; i < ts->size(); i++) {
        out << (*ts).at(i).first << " ";
        for (AnyDouble freq : fs) {
            if ((series = getSeries(freq)) != nullptr)
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

void Measurement::dump(MDATA *DATA, std::vector<AnyDouble> fs,
                       std::string file_path, bool tabular, bool titled) {
    Measurement* m = new Measurement(DATA);
    m->dump(fs, file_path, tabular, titled);
}

void Measurement::dump(MDATA *DATA, std::string file_path, bool tabular, bool titled) {
    Measurement* m = new Measurement(DATA);
    m->dump(file_path, tabular, titled);
}

//write only several frequencies
void Dump::spectrum(Spectrum spectrum, std::vector<AnyDouble> fs, std::ofstream& out) {
    for (unsigned int k = 0; k < spectrum.size(); k++) {
        for (unsigned int i = 0; i < fs.size(); i++)
            if (abs(spectrum[k].first - fs[i]) < FREQ_EPS) {
                out << spectrum[k].first << " " << spectrum[k].second << std::endl;
                break;
        }
    }
}

//write only several frequencies
void Dump::spectrum(Spectrum spectrum, std::vector<AnyDouble> fs, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    Dump::spectrum(spectrum, fs, out);
    out << std::endl;
    out.close();
}

//write spectrum as-is
void Dump::spectrum(Spectrum spectrum, std::string file_path, bool append) {
    std::vector<AnyDouble> fs;
    for (std::pair<AnyDouble, double> peak : spectrum) fs.push_back(peak.first);
    Dump::spectrum(spectrum, fs, file_path, append);
}

//write timestamp and value for only one frequency
void Dump::peak(Spectrum spectrum, AnyDouble f, AnyDouble t, std::ofstream& out) {
    for (unsigned int k = 0; k < spectrum.size(); k++)
        if (abs(spectrum[k].first - f) < FREQ_EPS)
             out << t << " " << spectrum[k].second << std::endl;
}

//write timestamp and value for only one frequency
void Dump::peak(Spectrum spectrum, AnyDouble f, AnyDouble t, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    Dump::peak(spectrum, f, t, out);
    out.close();
}

//write TimeSeries as-is
void Dump::series(Series series, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    Dump::series(series, out);
    out << std::endl;
    out.close();
}

//write TimeSeries as-is into ofstream
void Dump::series(Series series, std::ofstream& out) {
    for (std::pair<AnyDouble, double> item : series)
        out << item.first << " " << item.second << std::endl;
}

void Dump::mData(MDATA* DATA, std::vector<AnyDouble> fs,
                 std::string file_path, bool tabular, bool titled) {
    Measurement::dump(DATA, fs, file_path, tabular, titled);
}

void Dump::mData(MDATA* DATA, std::string file_path, bool tabular, bool titled) {
    Measurement::dump(DATA, file_path, tabular, titled);
}

