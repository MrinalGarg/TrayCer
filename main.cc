#include "constants.h"
#include<cmath>
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include"vec3.h"
#include "material.h"
#include<fstream>
#include <iostream>
#include</mnt/c/Users/mrina/Desktop/My Projects/TrayCer/json-develop/json-develop/single_include/nlohmann/json.hpp>

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

vec3 extract_vec3(nlohmann::json data)
{
    vec3 vec(
    data[0].get<double>(),
    data[1].get<double>(),
    data[2].get<double>()
    );
    return vec;

}

int main(int argc, char * argv[])
{
	//Extracting input
	std::ifstream json_file("scene.json");
	nlohmann::json data;
	json_file >> data;

	// Image

    // Extract image properties from the JSON
    nlohmann::json imageJson = data["image"];
    const auto aspect_ratio = imageJson["aspect_ratio"].get<double>();
    const int image_width = imageJson["image_width"].get<int>();
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = imageJson["samples_per_pixel"].get<int>();
    const int max_depth = imageJson["max_depth"].get<int>();
    const double Gamma = imageJson["Gamma"].get<double>();

    // World
    hittable_list world;
    
    // Extract the "objects" array from the JSON
    nlohmann::json objectsJson = data["objects"];

    // Iterate over each object in the array
    for (const auto& object : objectsJson) {
        std::string obj_type = object["type"].get<std::string>();
    
        if (obj_type == "sphere") {
            // Extract sphere properties
            point3 center = extract_vec3(object["center"]);
            double radius = object["radius"].get<double>();
            std::string material_type = object["material"]["type"].get<std::string>();
            
            // Create the material based on the type
            std::shared_ptr<material> sphere_material;
            if (material_type == "lambertian")
            {
                color albedo = extract_vec3(object["material"]["albedo"]);
                sphere_material = make_shared<lambertian>(albedo);
            } 

            else if (material_type == "metal") 
            {
                color albedo = extract_vec3(object["material"]["albedo"]);
                double fuzz = object["material"]["fuzz"].get<double>();
                sphere_material = make_shared<metal>(albedo, fuzz);
            } 
            else if (material_type == "dielectric") 
            {
                double refractive_index = object["material"]["refractive_index"].get<double>();
                color filter = extract_vec3(object["material"]["filter"]);
                sphere_material = make_shared<dielectric>(refractive_index,filter);
            }

            // Add the sphere to the world
            world.add(make_shared<sphere>(center, radius, sphere_material));
        }
    }


    // Camera
    nlohmann::json cameraJson = data["Camera"];
    point3 lookfrom = extract_vec3(cameraJson["lookfrom"]);
    point3 lookat = extract_vec3(cameraJson["lookat"]);
    point3 vup = extract_vec3(cameraJson["Vertically_up_vector"]);
    auto dist_to_focus = cameraJson["dist_to_focus"].get<double>();
    auto aperture = cameraJson["aperture"].get<double>();

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
    // Render
    std::ofstream my_file(argv[1]);

    my_file << "P6\n" << image_width << " " << image_height << "\n255\n";

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
            write_color(my_file, pixel_color, samples_per_pixel, Gamma);
        }
    }
    
    std::cerr << "\nDone.\n";
    return 0;
}
