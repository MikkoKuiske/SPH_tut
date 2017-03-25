#include "calculation_area.hh"
#include "particle.hh"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>


Calculation_area::Calculation_area(int height, int width, int wall_thickness):
    area_height_(height),
    area_width_(width),
    area_wall_thickness_(wall_thickness)
{
}

void
Calculation_area::initialize_calculation_area() {

    for (double y = 1; y < area_height_;++y ) {
        for (double x = 1; x < area_width_;++x ) {

            //Tee seinät
            if (y < area_wall_thickness_ || y > area_height_-area_wall_thickness_)
                add_particle(particle (x,y,100));

            //Tee "seinä"pallo keskelle (x-xo)²+(y-yo)² = säde
            else if (sqrt(pow(x-area_width_/2.0,2)+
                    pow(y-area_height_/2.0,2)) < double(area_height_)/4.0)
                add_particle(particle (x,y,100));

            //Kaikki muu on vettä
            else
                add_particle(particle (x,y,200));
        }
    }
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