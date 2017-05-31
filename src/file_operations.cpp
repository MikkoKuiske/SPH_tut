#include "file_operations.hh"
#include "parameters.hh"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string> 
#include <vector>
#include <sstream>

using namespace std;

void writefile(Calculation_area laskenta_alue, int time) {
    double x,y;

    //Format filename for "./data/datafile.xxxx.dat"; 
    //where xxxx = current timestep
    string time_as_string{std::to_string(time)};
    stringstream temp_stream;
    temp_stream << setw(4) << setfill('0') << time_as_string;

    time_as_string = temp_stream.str();
    string data_name = "./data/datafile" + time_as_string + ".txt";
    string tyyppi;

    //open datafile with dataname
    ofstream datafile;
    datafile.open(data_name);

    for (auto mapiter : laskenta_alue.hae_alue()) {
        if (mapiter.first == TYPE_WALL)
            tyyppi = "wall";
        else
            tyyppi = "water";

        datafile <<"type "<< tyyppi << "\n";
        for (auto vekiter : mapiter.second) {
            x = vekiter.get_x();
            y = vekiter.get_y();
            datafile << x << " " << y << "\n";
        }
    }
    datafile.close();

}
