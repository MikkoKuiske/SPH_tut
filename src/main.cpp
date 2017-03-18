#include "particle.hh"
#include "calculation_area.hh"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

const int ALUE_Y{70};
const int ALUE_X{70};
const int SEINAN_PAKSUUS{ALUE_Y/10};

using namespace std;

void writefile(Calculation_area laskenta_alue);



int main()
{
    //Create the calculation area by giving it borders
    Calculation_area laskenta_alue(ALUE_Y,ALUE_X);

    //initialize particles for the area
    for (double y = 1; y < ALUE_Y;++y ) {
        for (double x = 1; x < ALUE_X;++x ) {

            if (y < SEINAN_PAKSUUS || y > ALUE_Y-SEINAN_PAKSUUS)
                laskenta_alue.add_particle(particle (x,y,100));
            else
                laskenta_alue.add_particle(particle (x,y,200));
        }
    }
    writefile(laskenta_alue);
    laskenta_alue.print_all();
}

























void writefile(Calculation_area laskenta_alue) {
    double x,y;
    int tyyppi;

    /*ofstream vesifile;
    ofstream seinafile;
    vesifile.open ("./data/vesi.dat");
    seinafile.open ("./data/seina.dat");
    vesifile << "#Veden x ja y\n";
    seinafile << "#Veden x ja y\n";

    for (auto mapiter : laskenta_alue.hae_alue()) {
        for (auto vekiter : mapiter.second) {
            x = vekiter.get_x();
            y = vekiter.get_y();
            tyyppi = vekiter.get_type();
            if (tyyppi == 100)
                seinafile << x << " " << y << "\n";
            else 
                vesifile << x << " " << y << "\n";

        }
    }
    vesifile.close();
    seinafile.close();*/


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
