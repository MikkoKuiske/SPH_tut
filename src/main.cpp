#include "particle.hh"
#include "calculation_area.hh"
#include "file_operations.hh"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

const int ALUE_Y{70};
const int ALUE_X{70};
const int SEINAN_PAKSUUS{ALUE_Y/10};

using namespace std;


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