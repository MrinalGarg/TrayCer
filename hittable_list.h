#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>
#include <memory>
#include "aabb.h"
using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
	public:
		hittable_list(){};

		void add (shared_ptr<hittable> object){objects.push_back(object);};
		void clear(){objects.clear();};
		hittable_list(shared_ptr<hittable> object){add(object);};

		virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;
		virtual bool bounding_box(
            double time0, double time1, aabb& output_box) const override;

	public:
		std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	double closest_till_now = t_max;
	bool hit_at_all = false;

	for(const auto& object : objects)
	{
		if(object->hit(r,t_min,closest_till_now,rec))
		{
			hit_at_all = true;
			closest_till_now = rec.t;
		}
	} 
	return hit_at_all;
}

bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

#endif