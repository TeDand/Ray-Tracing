#ifndef COLOUR_H
#define COLUOR_H

#include "vec3.hpp"

#include <iostream>

void write_color(std::ostream &out, colour pixel_colour) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_colour.x()) << ' '
        << static_cast<int>(255.999 * pixel_colour.y()) << ' '
        << static_cast<int>(255.999 * pixel_colour.z()) << '\n';
}

#endif