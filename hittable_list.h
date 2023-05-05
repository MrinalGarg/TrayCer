#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>
#include <memory>

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

#endif