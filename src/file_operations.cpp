#include "file_operations.hh"

#include <iostream>
#include <fstream>

using namespace std;

void writefile(Calculation_area laskenta_alue) {
    double x,y;
    int tyyppi;

    ofstream datafile;
    datafile.open ("./data/datafile.dat");

    for (auto mapiter : laskenta_alue.hae_alue()) {
        datafile <<"\"type "<< mapiter.first << "\"\n";
        for (auto vekiter : mapiter.second) {
            x = vekiter.get_x();
            y = vekiter.get_y();
            datafile << x << " " << y << "\n";
        }
        datafile << "\n\n";
    }
    datafile.close();
}
