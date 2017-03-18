#ifndef CALCULATION_AREA_H
#define CALCULATION_AREA_H

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

#endif // CALCULATION_AREA_H

/*
virtaus.out: main.o calculation_area.o particle.o
	g++ -o ohjelma.out main.cpp calculation_area.cpp particle.cpp -I. -std=c++11

*/