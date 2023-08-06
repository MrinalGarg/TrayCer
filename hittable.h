#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "constants.h"
#include "aabb.h"
class material;

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    double u;
    double v;
    shared_ptr<material> mat_ptr;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        if(front_face)
        {
            normal = outward_normal;
        }
        else
        {
            normal = -outward_normal;
        }
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

#endif