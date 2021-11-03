#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    
	vec3 d = ray.direction;
	vec3 r = (d - (2*dot(d,normal)*normal)).normalized(); //r = (d - 2(d.n)n)
	Ray temp(intersection_point,r);
	
	if(recursion_depth < world.recursion_depth_limit) 
	{
		vec3 reflection_color = world.Cast_Ray(temp, ++recursion_depth);
		color = (1-reflectivity) * color + reflectivity * reflection_color;
	}
	else {color = (1-reflectivity) * color;}
	
    return color;
}
