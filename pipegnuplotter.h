#ifndef PIPEGNUPLOTTER_H
#define PIPEGNUPLOTTER_H

#include <string>
#include <iostream>
#include <vector>

#ifdef WIN32
    #define GNUPLOT_NAME "pgnuplot -persist"
#else
    #define GNUPLOT_NAME "gnuplot"
#endif

namespace PipeGnuplotter {
    void scatter3d(std::string file_name) {
        #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif

        if (pipe) {
            fprintf(pipe, "set key off\n");
            std::string command = "splot '" +
                    file_name +
                    "' using 1:2:3 with points palette pointsize 1 pointtype 7\n";
            fprintf(pipe, "%s", command.c_str());

            // fprintf(pipe, "%s\n", "e");
            fflush(pipe);

            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cin.get();

            #ifdef WIN32
                _pclose(pipe);
            #else
                pclose(pipe);
            #endif
        }
        return;
    }

    void plot2d(std::string file_name) {
        #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif

        if (pipe) {
            fprintf(pipe, "set key off\n");
            std::string command = "plot '" +
                    file_name +
                    "' using 1:2 with lines\n";
            fprintf(pipe, "%s", command.c_str());

            // fprintf(pipe, "%s\n", "e");
            fflush(pipe);

            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cin.get();

            #ifdef WIN32
                _pclose(pipe);
            #else
                pclose(pipe);
            #endif
        }
        return;
    }
}


#endif // PIPEGNUPLOTTER_H
