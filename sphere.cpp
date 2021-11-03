#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
	vec3 w = ray.endpoint - center;
    //quadratic equation components
	double b = 2*dot(ray.direction, w);
	double c = dot(w,w) - (radius*radius);
	
	//discriminant is value under sqrt
	//used to determine how many intersections
	double discriminant = b*b - 4*c;
	
	if(discriminant < 0) {
		//no intersection
		return {this, -1, part};
	}
	else {
		//an intersection occurred!
		double t0 = ((-b - sqrt(discriminant)) / 2);
		double t1 = ((-b + sqrt(discriminant)) / 2);
		if(discriminant == 0) {
			return {this, t1, part};
		}
		else if(discriminant > 0 && t0 < 0) {
			//if t0 is neg then t1
			return {this, t1, part};
		}
		else if(discriminant > 0 && t1 < 0) {
			//if t1 is neg then t0
			return {this, t0, part};
		}
		//if both pos then get smallest dist
		else {return {this, std::min(t0,t1), part};
		}
	}
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    // compute the normal direction
	vec3 n= (point - center) / radius;
	normal = n.normalized();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}

