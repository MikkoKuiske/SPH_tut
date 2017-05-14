#include "calculation_area.hh"
#include "particle.hh"
#include "parameters.hh"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>
#include <algorithm>


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

/*
void
Calculation_area::print_all() {

    for (auto mapiter : particles_) {
        std::cout << mapiter.first << std::endl;
        for (auto vekiter : mapiter.second)
            vekiter.move_particle()
    }

}
*/

void
Calculation_area::move_particles(){
    vector<particle>::iterator itvek;
    map<int, vector<particle>>::iterator itmap;

    for (itmap = particles_.begin(); itmap != particles_.end(); ++itmap) {
        if(itmap->first == 200){
            for (itvek = itmap->second.begin(); itvek != itmap->second.end(); ++itvek){
                //cout << "moved:" << itmap->first << ": "<< itvek->get_x() << "->";
                itvek->move_particle(1.0, 0.0, 2.0);
                //cout << itvek->get_x() << endl;
            }
        }
    }
}


std::map<int,std::vector<particle>>
Calculation_area::hae_alue() {
    return particles_;
}

void
Calculation_area::boundary_conditions(){
    // vector<particle>::iterator itvek;
    // map<int, vector<particle>>::iterator itmap;

    // for(itmap = particles_.begin(); itmap != particles_.end(); ++itmap){
    //     if(itmap->first == 200){
    //         for(itvek = itmap->second.begin(); itvek != itmap->second.end(); ++itvek){
    //             if(itvek->get_x() > ALUE_X or itvek->get_x() < 0){
    //                 itmap->second.erase(itvek);
    //                 cout << "erased" << itvek->get_x() << endl;
    //             }

    //         }
    //     }
    // }

    //Removes particles that are out of X area.
    particles_.at(200).erase(
                std::remove_if(
                    particles_.at(200).begin(),
                    particles_.at(200).end(),
                    [](particle i){ //<- Lambda function. If true,
                                    //particle is removed by remove_if
                                    if(i.get_x()>ALUE_X or i.get_x()<0)
                                    return true;
                                    else
                                    return false;
                }),
            particles_.at(200).end());
    

    //Removes particles that are out of Y area.
    particles_.at(200).erase(
                std::remove_if(
                    particles_.at(200).begin(),
                    particles_.at(200).end(),
                    [](particle i){ //<- Lambda function. If true,
                                    //particle is removed by remove_if
                                    if(i.get_y()>ALUE_Y or i.get_y()<0)
                                    return true;
                                    else
                                    return false;
                }),
            particles_.at(200).end());
}

void
Calculation_area::particle_groupping(){
    int particle_group_size = 6;

    int area_matrix[ALUE_X][ALUE_Y][particle_group_size];

    group_height_ = 2*PARTICLE_DIAMETER;
    group_width_ = 2*PARTICLE_DIAMETER;

    for (int i=0; i < particles_.at(200).size(); ++i){
        int particle_x_value = floor(*particles_.at(200).at(i).get_x() / group_width_ );


    }
}
