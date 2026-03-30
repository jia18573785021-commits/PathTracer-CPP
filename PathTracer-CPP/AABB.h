#pragma once
#include "Interval.h"
#include "Vector3.h"
#include "Ray.h"
class AABB
{
public :
	Interval x, y, z;

	AABB() {}

	AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {}

	// Input two points, which are the opposite corners of the box, and construct the AABB
	// You may ask: Why a coordinate value can build the boudary of the box?
	// The reason is: The face where the coordinate value located in is the boundary of the box
	// For example, if the x value is defined, then the face is the YoZ plane where the x value is the same as the coordinate value 
	AABB(const Point3& a, const Point3& b) : 
		x(std::fmin(a.x(), b.x()), std::fmax(a.x(), b.x())),
		y(std::fmin(a.y(), b.y()), std::fmax(a.y(), b.y())),
		z(std::fmin(a.z(), b.z()), std::fmax(a.z(), b.z())) {}

	AABB(const AABB& box1, const AABB& box2)
	{
		x = Interval(box1.x, box2.x);
		y = Interval(box1.y, box2.y);
		z = Interval(box1.z, box2.z);
	}

	const Interval& axis_interval(int axis) const
	{
		if (axis == 0) return x;
		if (axis == 1) return y;
		return z;
	}

	bool hit(const Ray& ray, Interval ray_t) const
	{
		const Point3& origin = ray.origin();
		const Vector3& direction = ray.direction();

		for (int axis = 0; axis < 3; axis++)
		{
			const Interval& ax = axis_interval(axis);
			const double axDirInv = 1.0 / direction[axis];

			auto t0 = (ax.min - origin[axis]) * axDirInv;
			auto t1 = (ax.max - origin[axis]) * axDirInv;

			ray_t.min = std::fmax(ray_t.min, std::fmin(t0, t1));
			ray_t.max = std::fmin(ray_t.max, std::fmax(t0, t1));
			if (ray_t.max <= ray_t.min) return false;
		}
		return true;
	}
};



