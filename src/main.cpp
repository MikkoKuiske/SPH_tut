#include "particle.hh"
#include "calculation_area.hh"
#include "file_operations.hh"
#include "parameters.hh"

#include <iostream>

//const int ALUE_Y{70};
//const int ALUE_X{70};
//const int SEINAN_PAKSUUS{ALUE_Y/10};

using namespace std;

int main()
{
    int timestep{1};

    //Create the calculation area by giving it outer borders
    Calculation_area laskenta_alue(ALUE_Y,ALUE_X, WALL_THICKNESS);
    
    //initialize particles for the area
    laskenta_alue.initialize_calculation_area();
    //laskenta_alue.print_all();

    writefile(laskenta_alue,timestep);

    for (timestep = START_TIME; timestep < END_TIME; ++timestep) {
        cout << "  Timestep" << timestep << endl;
        laskenta_alue.move_particles();
        laskenta_alue.boundary_conditions();
        writefile(laskenta_alue,timestep);
    }



}
