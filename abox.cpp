#include "abox.h"
#include "colormod.h"
#include "averager.h"
#include "inhomogeneity.h"

void remember(Spectrum spectrum, Timestamp time, MDATA* DATA) {
    for (std::pair<Frequency, double> peak : spectrum) {
        Frequency freq;
        double val;
        std::tie(freq, val) = peak;
        for (std::pair<Frequency, TimeSeries> item : *DATA) {
            //TBD
        }
    }
}


ABox::ABox(std::tuple<double, double, double> sizes,
           std::tuple<int, int, int> N,
           std::pair<double, double> inertial_interval) {
          std::tie(PX, PY, PZ) = sizes;
          std::tie(Nx, Ny, Nz) = N;
          std::tie(l0, L0) = inertial_interval;
          temperature = new Field("temperature", N);
          pressure = new Field("pressure", N);
          humidity = new Field("humidity", N);
          initGrid();
          std::function<double(double, Dot3D)> defaultLambda =
                  [](double field_val, Dot3D point){
              std::tie(std::ignore, std::ignore, std::ignore) = point;
              return field_val;
          };
          lambdaTemperature = defaultLambda;
          lambdaPressure = defaultLambda;
          lambdaHumidity = defaultLambda;
}

double ABox::__x(int i) { return i * this->PX / (this->Nx - 1); }

double ABox::__y(int j) { return j * this->PY / (this->Ny - 1); }

double ABox::__z(int k) { return k * this->PZ / (this->Nz - 1); }

int ABox::__i(double x) { return int(x / this->PX * (this->Nx - 1)); }

int ABox::__j(double y) { return int(y / this->PY * (this->Ny - 1)); }

int ABox::__k(double z) { return int(z / this->PZ * (this->Nz - 1)); }


void ABox::initGrid() {
    clock_t start = clock();
    this->temperature->initialize();
    this->pressure->initialize();
    this->humidity->initialize();
    clock_t end = clock();
    double seconds = double(end - start) / CLOCKS_PER_SEC;
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);
    std::cout << green << "ABox.initGrid()\t-\t" << seconds << " sec." << def << std::endl;
}

void ABox::setStandardProfiles(double T0, double P0, double rho0, double beta, double HP, double Hrho) {
    clock_t start = clock();
    this->T0 = T0; this->P0 = P0; this->rho0 = rho0;
    this->beta = beta; this->HP = HP; this->Hrho = Hrho;
    for (int i = 0; i < Nx; i++) {
        for (int j = 0; j < Ny; j++) {
            for (int k = 0; k < Nz; k++) {
                this->temperature->field[i][j][k] = T0 - beta * this->__z(k);
                this->pressure->field[i][j][k] = P0 * exp(-this->__z(k)/HP);
                this->humidity->field[i][j][k] = rho0 * exp(-this->__z(k)/Hrho);
            }
        }
    }
    clock_t end = clock();
    double seconds = double(end - start) / CLOCKS_PER_SEC;
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);
    std::cout << green << "ABox.setStandardProfiles(..)\t-\t" << seconds << " sec." << def << std::endl;
}

void ABox::setLambdaTemperature(const std::function<double (double, std::tuple<double, double, double>)> expression) {
    this->lambdaTemperature = expression;
}

void ABox::setLambdaPressure(const std::function<double (double, std::tuple<double, double, double>)> expression) {
    this->lambdaPressure = expression;
}

void ABox::setLambdaHumidity(std::function<double (double, std::tuple<double, double, double>)> expression) {
    this->lambdaHumidity = expression;
}

void ABox::createStructuralInhomogeneities(int amount, bool verbose) {
    Color::Modifier red(Color::FG_RED);
    Color::Modifier blue(Color::FG_BLUE);
    Color::Modifier def(Color::FG_DEFAULT);
    std::default_random_engine g;
    std::uniform_real_distribution<double> upx(0.0, PX);
    std::uniform_real_distribution<double> upy(0.0, PY);
    std::uniform_real_distribution<double> upz(0.0, PZ);
    std::uniform_real_distribution<double> ul(l0, L0);
    int k = 0;
    while (k < amount) {
        Inhomogeneity* item;
        while (true) {
            double xi = upx(g);
            double yi = upy(g);
            double zi = upz(g);
            double ai = ul(g);
            double bi = ai * PY/PX;
            double ci = ai * PZ/PX/2;
            item = new Inhomogeneity(std::make_tuple(xi, yi, zi), ai, bi, ci);
            if (item->withinBoxQ(this)) break;
            else if (verbose) {
                std::cout << blue << "..." << def << std::endl;
            }
        }
        std::uniform_real_distribution<double> c(0, 1);
        item->color = std::make_tuple(c(g), c(g), c(g), 1);
        item->number = k + 1;
        if (verbose) item->print();
        bool f = true;
        for (Inhomogeneity* e: this->inhomogeneities)
            if (!item->disjointQ(e)) {
                if (verbose) std::cout << red << "Deleted due to intersections" << def << std::endl;
                f = false;
                break;
        }
        if (f) {
            this->inhomogeneities.push_back(item);
            k++;
        }
    }
}

void ABox::applyStructuralInhomogeneities(bool verbose) {
    clock_t start = clock();
    if (!this->inhomogeneities.size()) this->createStructuralInhomogeneities(100, verbose);
    int progress = 0;
    int b = 0;
    for (int i = 0; i < Nx; i++) {
        double x = this->__x(i);
        for (int j = 0; j < Ny; j++) {
            double y = this->__y(j);
            for (int k = 0; k < Nz; k++) {
                double z = this->__z(k);
                for (Inhomogeneity* e: this->inhomogeneities) {
                    progress++;
                    if (e->includesQ(std::make_tuple(x, y, z))) {
                        this->temperature->applyLambda(this->lambdaTemperature,
                                                       std::make_tuple(i, j, k), std::make_tuple(x-e->xi, y-e->yi, z-e->zi));
                        this->pressure->applyLambda(this->lambdaPressure,
                                                    std::make_tuple(i, j, k), std::make_tuple(x-e->xi, y-e->yi, z-e->zi));
                        this->humidity->applyLambda(this->lambdaHumidity,
                                                    std::make_tuple(i, j, k), std::make_tuple(x-e->xi, y-e->yi, z-e->zi));
                        b++;
                    }
                }
            }
        }
        if (verbose)
            std::cout << float(progress) / (Nx*Ny*Nz*int(inhomogeneities.size())) * 100
                      << "%\t" << b << std::endl;
    }
    clock_t end = clock();
    double seconds = double(end - start) / CLOCKS_PER_SEC;
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);
    std::cout << green << "ABox.applyStructuralInhomogeneities(..)\t-\t" << seconds << " sec." << def << std::endl;
}

Profile ABox::getAltitudeProfileTemperature(int i, int j) {
    std::vector<double> profile;
    for (int k = 0; k < Nz; k++)
        profile.push_back(this->temperature->field[i][j][k]);
    return profile;
}

Profile ABox::getAltitudeProfileTemperature(double x, double y) {
    return getAltitudeProfileTemperature(__i(x), __j(y));
}

Profile ABox::getAltitudeProfileTemperature(Averager *avr) {
    std::vector<double> profile;
    for (int k = 0; k < Nz; k++) {
        double val = 0.; int count = 0;
        for (int i = 0; i < Nx; i++) {
            for (int j = 0; j < Ny; j++)
                if (avr->includesQ(std::make_tuple(__x(i), __y(j), __z(k)))) {
                    val += this->temperature->field[i][j][k];
                    count++;
            }
        }
        if (count) profile.push_back(val / count);
        else profile.push_back(T0 - beta * this->__z(k));
    }
    return profile;
}

Profile ABox::getAltitudeProfilePressure(int i, int j) {
    std::vector<double> profile;
    for (int k = 0; k < Nz; k++)
        profile.push_back(this->pressure->field[i][j][k]);
    return profile;
}

Profile ABox::getAltitudeProfilePressure(double x, double y) {
    return getAltitudeProfilePressure(__i(x), __j(y));
}

Profile ABox::getAltitudeProfilePressure(Averager *avr) {
    std::vector<double> profile;
    for (int k = 0; k < Nz; k++) {
        double val = 0.; int count = 0;
        for (int i = 0; i < Nx; i++) {
            for (int j = 0; j < Ny; j++)
                if (avr->includesQ(std::make_tuple(__x(i), __y(j), __z(k)))) {
                    val += this->pressure->field[i][j][k];
                    count++;
            }
        }
        if (count) profile.push_back(val / count);
        else profile.push_back(P0 * exp(-this->__z(k)/HP));
    }
    return profile;
}

Profile ABox::getAltitudeProfileHumidity(int i, int j) {
    std::vector<double> profile;
    for (int k = 0; k < Nz; k++)
        profile.push_back(this->humidity->field[i][j][k]);
    return profile;
}

Profile ABox::getAltitudeProfileHumidity(double x, double y) {
    return getAltitudeProfileHumidity(__i(x), __j(y));
}

Profile ABox::getAltitudeProfileHumidity(Averager *avr) {
    clock_t start = clock();
    std::vector<double> profile;
    for (int k = 0; k < Nz; k++) {
        double val = 0.; int count = 0;
        for (int i = 0; i < Nx; i++) {
            for (int j = 0; j < Ny; j++)
                if (avr->includesQ(std::make_tuple(__x(i), __y(j), __z(k)))) {
                    val += this->humidity->field[i][j][k];
                    count++;
            }
        }
        if (count) profile.push_back(val / count);
        else profile.push_back(rho0 * exp(-this->__z(k)/Hrho));
    }
    clock_t end = clock();
    double seconds = double(end - start) / CLOCKS_PER_SEC;
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);
    std::cout << green << "ABox.getAltitudeProfileHumidity(..)\t-\t" << seconds << " sec." <<  def << std::endl;
    return profile;
}

void ABox::dumpAltitudeProfile(Profile profile, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    for (unsigned int k = 0; k < profile.size(); k++)
        out << profile[k] << " " << this->__z(int(k)) << std::endl;
    out << std::endl;
    out.close();
}

Inhomogeneity* ABox::findInhomogeneity(int number) {
    for (Inhomogeneity* e: this->inhomogeneities) {
        if (e->number == number) return e;
    }
    return nullptr;
}

void ABox::dumpInhomogeneities(std::string file_path, std::tuple<int, int, int> resolution, bool append) {
    clock_t start = clock();
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    double x, y, z;
    for (Inhomogeneity* e: this->inhomogeneities) {
        std::vector<Dot3D> points = e->digitize(resolution);
        for (unsigned int i = 0; i < points.size(); i++) {
            std::tie(x, y, z) = points[i];
            out << x << " " << y << " " << z << " ";
            float r, g, b, a;
            std::tie(r, g, b, a) = e->color;
            out << r << " " << g << " " << b << " " << a << std::endl;
        }
        out << std::endl;
    }
    out << std::endl;
    out.close();
    clock_t end = clock();
    double seconds = double(end - start) / CLOCKS_PER_SEC;
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);
    std::cout << green << "ABox.dumpInhomogeneities(..)\t-\t" << seconds << " sec." << def << std::endl;
}

void ABox::moveStructuralInhomogeneities(std::tuple<double, double, double> s) {
    double x, y, z;
    std::tie(x, y, z) = s;
    for (Inhomogeneity* e: this->inhomogeneities) {
        e->xi += x; e->yi += y; e->zi += z;
    }
}

void ABox::moveStructuralInhomogeneities(std::tuple<double, double, double> v, double t) {
    double x, y, z;
    std::tie(x, y, z) = v;
    x *= t; y *= t; z *= t;
    this->moveStructuralInhomogeneities(std::make_tuple(x, y, z));
}

void ABox::moveFieldsPeriodicX(double s) {
    int i = __i(s) % __i(Nx-1);
    this->temperature->movePeriodicX(i);
    this->pressure->movePeriodicX(i);
    this->humidity->movePeriodicX(i);
}

Spectrum ABox::getBrightnessTemperature(std::vector<double> frequencies,
                                        Averager* avr, AttenuationModel* model, double theta) {
    std::vector<double> T = this->getAltitudeProfileTemperature(avr);
    std::vector<double> P = this->getAltitudeProfilePressure(avr);
    std::vector<double> Rho = this->getAltitudeProfileHumidity(avr);
    double dh = PZ / (Nz - 1);

    std::function<double(int, int)> gamma = [&](int i, int k){
        double f = frequencies[unsigned(i)], t = T[unsigned(k)], p = P[unsigned(k)], rho = Rho[unsigned(k)];
        double ox = model->gammaOxygen(f, t, p);
        double wv = model->gammaWVapor(f, t, p, rho);
        return (ox + wv) / cos(theta * M_PI / 180) * model->dB2np;
    };
    std::function<double(int, int)> Igamma = [&dh,gamma](int i, int H){
        if (H == 0) return 0.;
        double integ = dh * (gamma(i, 0) + gamma(i, H - 1)) / 2.;
        for (int k = 1; k < H - 1; k++) integ += dh * gamma(i, k);
        return integ;
    };
    std::function<double(int, int)> f = [&T,gamma,Igamma](int i, int h){
        return (T[unsigned(h)] + 273.15) * gamma(i, h) * exp(-Igamma(i, h));
    };
    std::vector<std::pair<double, double>> res;
    for (int i = 0; i < int(frequencies.size()); i++) {
        double integ = dh * (f(i, 0) + f(i, Nz - 1)) / 2.;
        for (int k = 1; k < Nz - 1; k++) integ += dh * f(i, k);
        res.push_back(std::make_pair(frequencies[unsigned(i)], integ));
    }
    return res;
}

void ABox::dumpSpectrum(Spectrum spectrum, std::string file_path, bool append) {
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

void ABox::dumpSpectrum(Spectrum spectrum, std::vector<double> frequencies, std::string file_path, bool append) {
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

void ABox::dumpSpectrum(Spectrum spectrum, double frequency, double t, std::string file_path, bool append) {
    std::ofstream out;
    if (append) out.open(file_path, std::ios::app);
    else out.open(file_path);
    for (unsigned int k = 0; k < spectrum.size(); k++) {
        double freq, val;
        std::tie(freq, val) = spectrum[k];
        if (abs(freq - frequency) < 0.00001)
             out << t << " " << val << std::endl;
    }
    out.close();
}



