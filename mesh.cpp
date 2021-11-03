#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    float min_t = std::numeric_limits<float>::max();
	Hit intersect;
	intersect.object = NULL;
	double distance;
	if(part >= 0) 
	{
		if(Intersect_Triangle(ray,part,distance)) 
		{
			intersect = {this, distance, part};
		}
	}
	else 
	{ //part < 0
		for(unsigned i = 0; i < triangles.size(); i++) 
		{ 
			if(Intersect_Triangle(ray,i,distance))
			{ 
				if(distance < min_t) 
				{
					//std::cout << "Intersected with part " << i << " dist:" << distance <<std::endl;
					min_t = distance;
					intersect = {this, distance, i};
				}
			}
				
		}
	}
	
    return intersect;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
	ivec3 triangle = triangles[part];
	vec3 a = vertices[triangle[0]];
	vec3 b = vertices[triangle[1]];
	vec3 c = vertices[triangle[2]];
	
	vec3 ac = c - a;
	vec3 ab = b - a;
	
    return cross(ab,ac).normalized();
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    //4.4.2 Ray-Triangle Intersection
	//& Barycentric Coordinates notes
	
	ivec3 triangle = triangles[tri];
	vec3 A = vertices[triangle[0]];
	vec3 B = vertices[triangle[1]];
	vec3 C = vertices[triangle[2]];
	vec3 AB = B - A;
	vec3 AC = C - A;
	
	vec3 d = ray.direction;
	vec3 e = ray.endpoint - A;
	
	dist = -(dot(cross(AB,AC),e)) / (dot(cross(AB,AC), d));
	
	if(dist > small_t) 
	{ 
		//beta = β
		double beta = (dot(cross(d,AC), e)) / (dot(cross(d,AC), AB));
		//gamma = γ
		double gamma = (dot(cross(d,AB), e)) / (dot(cross(d,AB), AC));
		//intersection if β > 0, γ > 0, and β + γ < 1 => 0 < 1 - β - γ
		//in this case 0 is our -weight_tol
		//i noticed weight tolerance was defined here and in mesh.h so i 
		//just used the one in mesh.h bc it was shorter
		if(beta>weight_tol && gamma>weight_tol && (1-gamma-beta)>weight_tol)
		{
			//std::cout << "returns true" << std::endl;
			return true;
		}
	}	
    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
