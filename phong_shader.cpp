#include "light.h"
#include "object.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"

//4.5.2 Blinn-Phong Shading
//4.7 Shading

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    
	//Ambient color calculation
	//ambient color = RaLa 
	//		= object color * color of lighting *intensity of lighting
	vec3 ambient = this->color_ambient * world.ambient_color * world.ambient_intensity;
	color = ambient;
	
	for(auto light : world.lights) {
		vec3 light_source = light->position - intersection_point; 
		vec3 light_color = light->Emitted_Light(light_source);
	
	//check for shadows...
	if(world.enable_shadows) {
		Ray check_ray(intersection_point, light_source.normalized());
		Hit check_hit = world.Closest_Intersection(check_ray);
		
		//if theres an object blocking all light sources, 
		//return only ambient light component
		if(check_hit.object != NULL && check_hit.dist < light_source.magnitude()) {
			//exit for loop and return color = ambient;
			continue;
		}
	}
	
	//Diffuse color calculation
	//diffuse = RdLdmax(n.l,0)
	// = object color * diffuse intensity * color of light
	double diffuse_intensity = std::max(dot(light_source.normalized(),normal), 0.0);
	vec3 diffuse = this->color_diffuse * diffuse_intensity * light_color;
	
	//Specular color calculation 
	//specular = RsLsmax(dot(c,reflection), 0)^alpha
	// = object color * specular intensity * color of light
	
	// reflection = 2n*(n.l)/||n^2|| - l but ||n|| = 1 so it can be ignored
	vec3 reflection = (2*dot(light_source.normalized(), normal)*normal) - light_source.normalized();
	//specular intensity = max(reflection*c,0) ^ alpha
	//c = - ray direction, alpha = specular_power
	double specular_intensity = pow(std::max(dot(reflection.normalized(), -ray.direction.normalized()), 0.0), specular_power); 
	vec3 specular = color_specular * specular_intensity * light_color;
	
	//combine all to get final color
	color += diffuse + specular;
	}
	
    return color;
}
