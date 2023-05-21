#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

const point3 C0 = {0,0,-1};
const double Radius = 0.5;

bool HitSphere (point3 C,double R,ray r,double &t0)
{
    double a = (r.direction()).length_squared();
    double h = (dot(r.direction(),r.origin()-C));
    double c = (r.origin()-C).length_squared()-R*R;
    double discriminant = h*h-a*c;
    if (discriminant>=0)
    {t0 = (-h-std::sqrt(discriminant))/a;}
    return (discriminant>=0);
}

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    double l;
    if(HitSphere(C0, Radius, r, l))
    {
        vec3 n = r.at(l)-C0;
        n=unit_vector(n);

        //double t1 = 0.5*(1 + n.x());
        //double t2 = 0.5*(1 + n.y());
        //double t3 = 0.5*(1 + n.z());

        //std::cout<<t1<<t2<<t3;

        return color(0.5*(1 + n.x()),0.5*(1 + n.y()),0.5*(1 + n.z()));

    }
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 256;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        //std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    //std::cerr << "\nDone.\n";
}