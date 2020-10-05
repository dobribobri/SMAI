#include "variate.h"
#include "colormod.h"

std::vector<Profile> Variate::gaussian(Profile profile,
                                       std::function<double(unsigned int)> stddevOnIndex,
                                       unsigned int times) {
    clock_t start = clock();
    std::vector<Profile> res(times);
    std::random_device rd{};
    std::mt19937 gen{rd()};
    for (unsigned int i = 0; i < profile.size(); i++) {
        std::normal_distribution<double> d(profile[i], stddevOnIndex(i));
        for (unsigned int k = 0; k < times; k++)
            res[k].push_back(d(gen));
    }
    clock_t end = clock();
    double seconds = double(end - start) / CLOCKS_PER_SEC;
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier def(Color::FG_DEFAULT);
    std::cout << green << "Variate::gaussian\t-\t" << seconds << " sec." << def << std::endl;
    return res;
}

Profile Variate::gaussian(Profile profile, std::function<double(unsigned int)> stddevOnIndex) {
    return Variate::gaussian(profile, stddevOnIndex, 1)[0];
}

std::vector<Profile> Variate::gaussian(Profile profile, double stddev, unsigned int times) {
    return Variate::gaussian(profile,
                             [&stddev](unsigned int i){ return i*0.+stddev; },
                             times);
}

Profile Variate::gaussian(Profile profile, double stddev) {
    return Variate::gaussian(profile, stddev, 1)[0];
}
