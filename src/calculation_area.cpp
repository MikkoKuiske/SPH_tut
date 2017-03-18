#include "calculation_area.hh"

#include <iostream>
#include <iomanip>
#include <map>


Calculation_area::Calculation_area(int height, int width):
    area_height_(height),
    area_width_(width)
{

}



void
Calculation_area::add_particle(particle to_add) {
    if (particles_.find(to_add.get_type()) == particles_.end())
        particles_.insert({to_add.get_type(),{to_add}});
    else
        particles_.at(to_add.get_type()).push_back(to_add);
}


void
Calculation_area::print_all() {

    for (auto mapiter : particles_) {
        std::cout << mapiter.first << std::endl;
        for (auto vekiter : mapiter.second)
            vekiter.tulosta();
    }

}

std::map<int,std::vector<particle>>
Calculation_area::hae_alue() {
    return particles_;
}