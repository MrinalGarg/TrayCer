#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include<fstream>

#include <iostream>

void write_color(std::ofstream &out, color pixel_color, int samples_per_pixel, double Gamma)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

     //Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = pow((scale * r),Gamma);
    g = pow((scale * g),Gamma);
    b = pow((scale * b),Gamma);


    
    // Write the translated [0,255] value of each color component.
    out.put(static_cast<int>(256 * clamp(r, 0.0, 0.999)));
    out.put(static_cast<int>(256 * clamp(g, 0.0, 0.999)));
    out.put(static_cast<int>(256 * clamp(b, 0.0, 0.999)));
}


#endif