#ifndef PIPEGNUPLOTTER_H
#define PIPEGNUPLOTTER_H

#include <string>
#include <iostream>
#include <vector>
#include <thread>

#ifdef WIN32
    #define GNUPLOT_NAME "pgnuplot -persist"
#else
    #define GNUPLOT_NAME "gnuplot"
#endif

void __wait() {
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
}

namespace PipeGnuplotter {

    class Palette {
    public:
        static std::vector<std::string> set_style_commands;

        static void initialize(FILE* pipe, std::vector<std::string> set_style_commands) {
            std::string command = "";
            for (std::string s : set_style_commands) {
                fprintf(pipe, "%s", s.c_str());
            }
        }
    };

    std::vector<std::string> Palette::set_style_commands = {
        "set style line 1 linecolor rgb '#0000A0' linetype 1 linewidth 1\n", //darkblue
        "set style line 2 linecolor rgb '#32ACB4' linetype 1 linewidth 1\n", //cyan
        "set style line 3 linecolor rgb '#228B22' linetype 1 linewidth 1\n", //forestgreen
        "set style line 4 linecolor rgb '#ff8C00' linetype 1 linewidth 1\n", //orange
        "set style line 5 linecolor rgb '#FF4500' linetype 1 linewidth 1\n", //orangered
        "set style line 6 linecolor rgb '#DC143C' linetype 1 linewidth 1\n"  //crimson
    };

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

            __wait();

            #ifdef WIN32
                _pclose(pipe);
            #else
                pclose(pipe);
            #endif
        }
        return;
    }

    void _plot2d(std::string file_name) {
        #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif

        if (pipe) {
            fprintf(pipe, "set key off\n");
            std::string command = "plot '" +
                    file_name +
                    "' using 1:2 with lines linestyle 1\n";
            fprintf(pipe, "%s", command.c_str());

            // fprintf(pipe, "%s\n", "e");
            fflush(pipe);

            __wait();

            #ifdef WIN32
                _pclose(pipe);
            #else
                pclose(pipe);
            #endif
        }
        return;
    }

    void plot2d(std::string file_name,
                std::vector<std::string> set_style_commands = {}) {
        if (!set_style_commands.size()) {
            _plot2d(file_name);
            return;
        }
        #ifdef WIN32
            FILE *pipe = _popen(GNUPLOT_NAME, "w");
        #else
            FILE *pipe = popen(GNUPLOT_NAME, "w");
        #endif

        if (pipe) {
            fprintf(pipe, "set key off\n");
            Palette::initialize(pipe, set_style_commands);
            std::string command = "plot '";
            for (unsigned int i = 0; i < set_style_commands.size(); i++) {
                command += file_name +
                        "' using 1:" + std::to_string(i+2) +
                        " with lines linestyle " + std::to_string(i+1);
                if (i != set_style_commands.size() - 1) command += " , '";
            }
            command += "\n";
            fprintf(pipe, "%s", command.c_str());

            // fprintf(pipe, "%s\n", "e");
            fflush(pipe);

            __wait();

            #ifdef WIN32
                _pclose(pipe);
            #else
                pclose(pipe);
            #endif
        }
        return;
    }


    namespace Threaded {
        std::thread scatter3d(std::string file_name) {
            std::thread t(PipeGnuplotter::scatter3d, file_name);
            return t;
        }

        std::thread plot2d(std::string file_name,
                           std::vector<std::string> set_style_commands = {}) {
            std::thread t(PipeGnuplotter::plot2d, file_name, set_style_commands);
            return t;
        }
    }
}


#endif // PIPEGNUPLOTTER_H
