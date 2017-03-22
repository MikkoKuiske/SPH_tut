#ifndef CALCULATION_AREA_HH
#define CALCULATION_AREA_HH

#include "particle.hh"

#include <iostream>
#include <map>
#include <vector>



class Calculation_area
{
public:
    Calculation_area(int height,int width);
    std::map<int,std::vector<particle>> hae_alue();
    void add_particle(particle to_add);
    void print_all();


private:
    int area_height_;
    int area_width_;

    std::map<int,std::vector<particle>> particles_;
};

#endif // CALCULATION_AREA_HH
