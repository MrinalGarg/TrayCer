#include "constants.h"
#include<cmath>
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include"vec3.h"
#include "material.h"
#include<iostream>
#include<fstream>
#include<string>

color ray_color(const ray& r, const hittable_list &world, int depth) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    hit_record t_rec;
    
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if(depth<=0)
    {
        return color(0,0,0);
    }

    if(world.hit(r,0.001,infinity,t_rec))
    {
        ray scattered;
        color attenuation;
        if (t_rec.mat_ptr->scatter(r, t_rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main(int argc, char*argv[]) {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 512;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    const double Gamma = 0.5;

    //world
    hittable_list world;
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    //auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_right  = make_shared<lambertian>(color(0.1, 0.2, 0.5));

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    //world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1,    0.5, -2.0),   1, material_left));
    //world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(point3( 1,    0.5, -2.0),   1, material_right));


    //camera
    point3 lookfrom(0,0,3);
    point3 lookat(0,0,-1);
    vec3 vup(0,1,0);
    auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 20;

    camera cam(lookfrom, lookat, vup, 60, aspect_ratio, aperture, dist_to_focus);

    // Render
    std::ofstream image_file(argv[1],std::ios::binary);
    image_file << "P6\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(image_file, pixel_color, samples_per_pixel, Gamma);
        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}