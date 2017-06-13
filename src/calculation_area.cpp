#include "calculation_area.hh"
#include "particle.hh"
#include "parameters.hh"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>
#include <algorithm>
#include <vector>


Calculation_area::Calculation_area(int height, int width, int wall_thickness):
    area_height_(height),
    area_width_(width),
    area_wall_thickness_(wall_thickness),
    vector_size_limit_(height * width)
{
    //int limit = height * width;
    vector<particle> dum_vec_1;
    vector<particle> dum_vec_2;
    dum_vec_1.reserve(vector_size_limit_);
    dum_vec_2.reserve(vector_size_limit_);

    particles_.insert({TYPE_WATER,dum_vec_1});
    particles_.insert({TYPE_WALL,dum_vec_2});

}

void
Calculation_area::initialize_calculation_area() {

    for (double y = 1; y < area_height_;++y ) {
        for (double x = 1; x < area_width_;++x ) {

            //Tee seinät
            if (y < area_wall_thickness_ || y > area_height_-area_wall_thickness_)
                add_particle(particle (x,y,TYPE_WALL));

            //Tee "seinä"pallo keskelle (x-xo)²+(y-yo)² = säde
            else if (sqrt(pow(x-area_width_/2.0,2)+
                          pow(y-area_height_/2.0,2)) < double(area_height_)/4.0)
                add_particle(particle (x,y,TYPE_WALL));

            //Kaikki muu on vettä
            else
                add_particle(particle (x,y,TYPE_WATER));
        }
    }
}

void
Calculation_area::add_particle(particle to_add) {

    //if (particles_.find(to_add.get_type()) == particles_.end())
    //    particles_.insert({to_add.get_type(),{to_add}});
    //else
    if (particles_.at(to_add.get_type()).size() >= vector_size_limit_){
        //vector<particle> temp_vec;
        //temp_vec.reserve(particles_.at(to_add.get_type()).size() + 1000);
        //copy(particles_.at(to_add.get_type()).begin(), particles_.at(to_add.get_type()).end(), back_inserter(temp_vec));

        particles_.at(to_add.get_type()).reserve(vector_size_limit_ + 1000);

        //particles_.at(to_add.get_type()) = temp_vec;

        vector_size_limit_ = vector_size_limit_ + 1000;
        //cout << "kaytiin kasvattaan kokoa" << endl;
        //cout << "koko nyt " << vector_size_limit_ << endl;

    }
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
        if(itmap->first == TYPE_WATER){
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
    //     if(itmap->first == TYPE_WATER){
    //         for(itvek = itmap->second.begin(); itvek != itmap->second.end(); ++itvek){
    //             if(itvek->get_x() > ALUE_X or itvek->get_x() < 0){
    //                 itmap->second.erase(itvek);
    //                 cout << "erased" << itvek->get_x() << endl;
    //             }

    //         }
    //     }
    // }

    //Removes particles that are out of X area.
    particles_.at(TYPE_WATER).erase(
                std::remove_if(
                    particles_.at(TYPE_WATER).begin(),
                    particles_.at(TYPE_WATER).end(),
                    [](particle i){ //<- Lambda function. If true,
                                    //particle is removed by remove_if
                                    if(i.get_x()>ALUE_X or i.get_x()<0)
                                    return true;
                                    else
                                    return false;
                }),
            particles_.at(TYPE_WATER).end());
    

    //Removes particles that are out of Y area.
    particles_.at(TYPE_WATER).erase(
                std::remove_if(
                    particles_.at(TYPE_WATER).begin(),
                    particles_.at(TYPE_WATER).end(),
                    [](particle i){ //<- Lambda function. If true,
                                    //particle is removed by remove_if
                                    if(i.get_y()>ALUE_Y or i.get_y()<0)
                                    return true;
                                    else
                                    return false;
                }),
            particles_.at(TYPE_WATER).end());
}

void
Calculation_area::particle_grouping(){
    //int particle_group_size;
    int particle_x_index;
    int particle_y_index;

    //int x_koko = floor(ALUE_X / (2*PARTICLE_DIAMETER));
    //int y_koko = floor(ALUE_Y / (2*PARTICLE_DIAMETER));

    vector<particle*> area_matrix[ALUE_X][ALUE_Y];

    for (unsigned int i=0; i < particles_.at(TYPE_WATER).size(); ++i){


        particle* p_Pointer = new particle{particles_.at(TYPE_WATER).at(i)};

        particle_x_index = floor(((particles_.at(TYPE_WATER).at(i))).get_x() / group_width_ ); // int particle_x_index = floor(*particles_.at(TYPE_WATER).at(i).get_x() / group_width_ );

        particle_y_index = floor(((particles_.at(TYPE_WATER).at(i))).get_y() / group_height_);

        area_matrix[particle_x_index][particle_y_index].push_back(p_Pointer); // (particles_.at(TYPE_WATER).at(i))*

        cout << particle_x_index << " : " << particle_y_index << ": " << area_matrix[particle_x_index][particle_y_index].size() << endl;

        delete p_Pointer;
    }
}
