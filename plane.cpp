#include "plane.h"
#include "hit.h"
#include "ray.h"
#include <cfloat>
#include <limits>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
Hit Plane::Intersection(const Ray& ray, int part) const
{
	vec3 d = x1 - ray.endpoint;
	double lower = dot(ray.direction, normal);
	double upper = dot(d, normal);
	if(lower == 0) {
		if(upper == 0) {
			return {this, 0, 0}; //on plane
		}
		return {this, -1, part}; //no intersection
	}
	return {this, upper / lower, part}; //intersection at t
}


vec3 Plane::Normal(const vec3& point, int part) const
{
    return normal;
}

// There is not a good answer for the bounding box of an infinite object.
// The safe thing to do is to return a box that contains everything.
Box Plane::Bounding_Box(int part) const
{
    Box b;
    b.hi.fill(std::numeric_limits<double>::max());
    b.lo=-b.hi;
    return b;
}

