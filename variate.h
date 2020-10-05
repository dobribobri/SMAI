#ifndef VARIATE_H
#define VARIATE_H

#include <iostream>
#include <vector>
#include <random>
#include <functional>

typedef std::vector<double> Profile;


namespace Variate {

    Profile gaussian(Profile profile, double stddev);
    Profile gaussian(Profile profile, std::function<double(unsigned int)> stddevOnIndex);
    std::vector<Profile> gaussian(Profile profile, double stddev, unsigned int times);
    std::vector<Profile> gaussian(Profile profile,
                                  std::function<double(unsigned int)> stddevOnIndex,
                                  unsigned int times);

}

#endif // VARIATE_H
