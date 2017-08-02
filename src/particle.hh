#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <iomanip>
#include <vector>


class particle
{
public:

    particle(double x,double y,int type);
    std::vector<double> get_position();
    void tulosta();
    int get_type();
    double get_x();
    double get_y();
    void set_density();

    void move_particle(double nopeus_x, double nopeus_y, double aika);

private:
    //position and type
    double x_pos_;
    double y_pos_;
    int type_;
    double density_;

};

#endif // PARTICLE_H
