#include "particle.hh"


#include <iostream>
#include <iomanip>
#include <vector>

particle::particle(double x,double y,int type):
    x_pos_(x),
    y_pos_(y),
    type_(type)
{

}

std::vector<double>
particle::get_position() {
    std::vector<double> position;
    position.push_back(x_pos_);
    position.push_back(y_pos_);
    return position;
}

int
particle::get_type() {
    return type_;
}

void
particle::tulosta() {
    std::cout << "x: " << x_pos_ << " y: " << y_pos_ << std::endl;
}

double
particle::get_x() {
	return x_pos_;
}

double
particle::get_y() {
	return y_pos_;
}

void
particle::move_particle(double nopeus_x, double nopeus_y, double aika) {
    x_pos_ += nopeus_x*aika;
    y_pos_ += nopeus_y*aika;
}

