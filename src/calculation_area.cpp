#include "calculation_area.hh"
#include "particle.hh"
#include "parameters.hh"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>
#include <algorithm>
#include <vector>
/*
#------------------------------------------------
# <short overview of the function/script>
#
#	syntax: output1 = exampleFuntion(param1,param2)
#
#	input: 	param1:
#		param2:
#
#	output: output1:
#
#	example: output1 = exampleFunction(param1,param2)
#
#	Copyright 2017 Your Name
#	Version 0.1 by Your Name dd.mm.yyyy
#	...
#
#	Change log:
#	Version 0.1 dd.mm.yyyy Initial version
#	...
#
#	to be done:
#	- ...
#
#------------------------------------------------
*/
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

/*
#------------------------------------------------
# <short overview of the function/script>
#
#	syntax: output1 = exampleFuntion(param1,param2)
#
#	input: 	param1:
#		param2:
#
#	output: output1:
#
#	example: output1 = exampleFunction(param1,param2)
#
#	Copyright 2017 Your Name
#	Version 0.1 by Your Name dd.mm.yyyy
#	...
#
#	Change log:
#	Version 0.1 dd.mm.yyyy Initial version
#	...
#
#	to be done:
#	- ...
#
#------------------------------------------------
*/

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

/*
#------------------------------------------------
# <short overview of the function/script>
#
#	syntax: output1 = exampleFuntion(param1,param2)
#
#	input: 	param1:
#		param2:
#
#	output: output1:
#
#	example: output1 = exampleFunction(param1,param2)
#
#	Copyright 2017 Your Name
#	Version 0.1 by Your Name dd.mm.yyyy
#	...
#
#	Change log:
#	Version 0.1 dd.mm.yyyy Initial version
#	...
#
#	to be done:
#	- ...
#
#------------------------------------------------
*/

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

/*
#------------------------------------------------
# <short overview of the function/script>
#
#	syntax: output1 = exampleFuntion(param1,param2)
#
#	input: 	param1:
#		param2:
#
#	output: output1:
#
#	example: output1 = exampleFunction(param1,param2)
#
#	Copyright 2017 Your Name
#	Version 0.1 by Your Name dd.mm.yyyy
#	...
#
#	Change log:
#	Version 0.1 dd.mm.yyyy Initial version
#	...
#
#	to be done:
#	- ...
#
#------------------------------------------------
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

/*
#------------------------------------------------
# <short overview of the function/script>
#
#	syntax: output1 = exampleFuntion(param1,param2)
#
#	input: 	param1:
#		param2:
#
#	output: output1:
#
#	example: output1 = exampleFunction(param1,param2)
#
#	Copyright 2017 Your Name
#	Version 0.1 by Your Name dd.mm.yyyy
#	...
#
#	Change log:
#	Version 0.1 dd.mm.yyyy Initial version
#	...
#
#	to be done:
#	- ...
#
#------------------------------------------------
*/

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

/*
#------------------------------------------------
# <short overview of the function/script>
#
#	syntax: output1 = exampleFuntion(param1,param2)
#
#	input: 	param1:
#		param2:
#
#	output: output1:
#
#	example: output1 = exampleFunction(param1,param2)
#
#	Copyright 2017 Your Name
#	Version 0.1 by Your Name dd.mm.yyyy
#	...
#
#	Change log:
#	Version 0.1 dd.mm.yyyy Initial version
#	...
#
#	to be done:
#	- Area_matrix twice the size needed. Atm 70*70 -> could be 35*35
#
#------------------------------------------------
*/

void
Calculation_area::particle_grouping(){
    //int particle_group_size;
    int particle_x_index;
    int particle_y_index;

    //int x_koko = floor(ALUE_X / (2*PARTICLE_DIAMETER));
    //int y_koko = floor(ALUE_Y / (2*PARTICLE_DIAMETER));

    //vector<particle*> area_matrix[ALUE_X][ALUE_Y];

    for (unsigned int clear_x=0; clear_x < ALUE_X; ++clear_x){
        for (unsigned int clear_y =0; clear_y < ALUE_Y; ++clear_y){
            area_matrix[clear_x][clear_y].clear();
        }
    }

    for (unsigned int j=0; j < particles_.at(TYPE_WALL).size(); ++j){


        particle* p_Pointer = new particle{particles_.at(TYPE_WALL).at(j)};

        particle_x_index = floor(((particles_.at(TYPE_WALL).at(j))).get_x() / group_width_ );

        particle_y_index = floor(((particles_.at(TYPE_WALL).at(j))).get_y() / group_height_);

        area_matrix[particle_x_index][particle_y_index].push_back(p_Pointer);

        //cout << particle_x_index << " : " << particle_y_index << ": " << area_matrix[particle_x_index][particle_y_index].size() << endl;

        //delete p_Pointer;
    }

    for (unsigned int i=0; i < particles_.at(TYPE_WATER).size(); ++i){


        particle* p_Pointer = new particle{particles_.at(TYPE_WATER).at(i)};

        particle_x_index = floor(((particles_.at(TYPE_WATER).at(i))).get_x() / group_width_ ); // int particle_x_index = floor(*particles_.at(TYPE_WATER).at(i).get_x() / group_width_ );

        particle_y_index = floor(((particles_.at(TYPE_WATER).at(i))).get_y() / group_height_);

        area_matrix[particle_x_index][particle_y_index].push_back(p_Pointer); // (particles_.at(TYPE_WATER).at(i))*

        //cout << particle_x_index << " : " << particle_y_index << ": " << area_matrix[particle_x_index][particle_y_index].size() << endl;

        //delete p_Pointer;
    }
}

/*
#------------------------------------------------
# <short overview of the function/script>
#
#	syntax: marco = comparable particle_1
#               Particle that is fetched from
#               grid.
#           polo = comparable particle_2
#               Particle that is fetched
#               from the same grid element
#               or surrounding elements.
#           temp_density = temporal variable
#               for saving density of particles
#
#   output1 = exampleFuntion(param1,param2)
#
#	input: 	param1:
#		param2:
#
#	output: output1:
#
#	example: output1 = exampleFunction(param1,param2)
#
#	Copyright 2017 Your Name
#	Version 0.1 by Your Name dd.mm.yyyy
#	...
#
#	Change log:
#	Version 0.1 dd.mm.yyyy Initial version
#	...
#
#	to be done:
#	-
#
#------------------------------------------------
*/

void
Calculation_area::calculate_density(){
    double M_x = 0;
    double x_h = 0;
    double distance_between_particles = 0;
    double temp_density = 0;

    for (unsigned int x_grid_element = 0; x_grid_element < ALUE_X; ++x_grid_element){

        for (unsigned int y_grid_element = 0; y_grid_element < ALUE_Y; ++y_grid_element){

            for (unsigned int particle_num = 0; particle_num < area_matrix[x_grid_element][y_grid_element].size(); ++particle_num){

                particle* particle_marco = area_matrix[x_grid_element][y_grid_element].at(particle_num);

                // Particle is compared to other particles that are within range of
                // one grid element of it in any direction. Particle is also compared to other particles that are
                // within the same grid element.
                for (unsigned int i = x_grid_element - 1; i < x_grid_element + 2; ++i){
                    if (i < 0 or i > ALUE_X){
                       continue;
                    }
                    for (unsigned int j = y_grid_element - 1; j < y_grid_element + 2; ++j){
                        if (j < 0 or j > ALUE_Y){
                           continue;
                        }
                        for (auto particle_polo:area_matrix[i][j]){
                            // If the comparison is done to different particles
                            // calculation proceeds. If particle is the same one
                            // calculation is skipped.
                            if (particle_polo != particle_marco){
                                //Physics!!!!!!!
                                distance_between_particles = sqrt(pow(particle_polo->get_x() - particle_marco->get_x(), 2) + pow(particle_polo->get_y() - particle_marco->get_y(), 2));
                                x_h = distance_between_particles / PARTICLE_DIAMETER;
                               
                                if (x_h <= 1 or x_h >= 0) {
                                    M_x = AUXILIARY_VARIABLE*(pow((2 - x_h), 3) - 4*(pow((1 - x_h), 3)));
                                }
                                else if (x_h <= 2 or x_h > 1) {
                                    M_x = AUXILIARY_VARIABLE*(pow((2 - x_h), 3));
                                }
                                else if (x_h > 2){
                                    M_x = 0;
                                }

                                temp_density = PARTICLE_MASS * (M_x / pow(PARTICLE_DIAMETER, 2));

                                // JATKA TASTA!!!
                                particle_polo->set_density(temp_density);

                            }

                        }
                    }
                }
            }
        }
    }

}
