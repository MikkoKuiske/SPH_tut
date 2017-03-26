#include "particle.hh"
#include "calculation_area.hh"
#include "file_operations.hh"

#include <iostream>

const int ALUE_Y{70};
const int ALUE_X{70};
const int SEINAN_PAKSUUS{ALUE_Y/10};

using namespace std;

int main()
{
    int timestep{1};

    //Create the calculation area by giving it outer borders
    Calculation_area laskenta_alue(ALUE_Y,ALUE_X, SEINAN_PAKSUUS);
    
    //initialize particles for the area
    laskenta_alue.initialize_calculation_area();
    //laskenta_alue.print_all();

    //write datafile with number data_timestep, where timestep a is number from 0001 to 9999
    writefile(laskenta_alue,timestep);

}
