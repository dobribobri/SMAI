#ifndef PLOTTER_H
#define PLOTTER_H

#include <string>
#include <thread>


namespace Plotter {
    void scatter3d(std::string file_path, int every = 3) {
        std::string command = "python3 plotter.py --scatter3d --colored --colortype RGBA --filepath " + file_path +
                " --every " + std::to_string(every) +
                " --xlabel X --ylabel Y --zlabel Z";
        std::system(command.c_str());
        return;
    }

    void altitudeScatter2d(std::string file_path, int every = 1, std::string xlabel = "x", std::string ylabel = "x") {
        std::string command = "python3 plotter.py --scatter2d --filepath " + file_path +
                " --every " + std::to_string(every) +
                " --xlabel \'" + xlabel + "\' --ylabel \'" + ylabel + "\' --altitude";
        std::system(command.c_str());
        return;
    }

    void altitudePlot2d(std::string file_path, int every = 1, std::string xlabel = "x", std::string ylabel = "x") {
        std::string command = "python3 plotter.py --plot2d --filepath " + file_path +
                " --every " + std::to_string(every) +
                " --xlabel \'" + xlabel + "\' --ylabel \'" + ylabel + "\' --altitude";
        std::system(command.c_str());
        return;
    }


    namespace Threaded {
        std::thread scatter3d(std::string file_path, int every = 3) {
            std::thread t(Plotter::scatter3d, file_path, every);
            return t;
        }

        std::thread altitudeScatter2d(std::string file_path, int every = 1, std::string xlabel = "x", std::string ylabel = "x") {
            std::thread t(Plotter::altitudeScatter2d, file_path, every, xlabel, ylabel);
            return t;
        }

        std::thread altitudePlot2d(std::string file_path, int every = 1, std::string xlabel = "x", std::string ylabel = "x") {
            std::thread t(Plotter::altitudePlot2d, file_path, every, xlabel, ylabel);
            return t;
        }
    }
}

#endif // PLOTTER_H
