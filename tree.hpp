#pragma once

#include "branch.hpp"
#include "shader.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <map>
#include <set>

using namespace glm;

/*struct vec3_comparator{
	bool operator()(const vec3 &vecA, const vec3 &vecB){ 
		const double epsilion = 0.01;
		return  	fabs(vecA[0] -vecB[0]) < epsilion   
		      	&& 	fabs(vecA[1] -vecB[1]) < epsilion   
		      	&& 	fabs(vecA[2] -vecB[2]) < epsilion;
	} 
};*/


class Tree{
private:
	vec3 position;

	float radius 				= 200.f;
	float height 				= 300.f;
	float trunk_height 			= 50.f;

	int attraction_point_count 	= 2000;
	float branch_length 		= 2.f;

	// actual distance is * branch_length
	float kill_distance 		= 2;
	float influence_distance 	= 10;

	float initial_radius 		= 0.01;
	float radius_growth 		= 0.005;

	// how much wieght is given to the branches existing direction
	// as opposed to the pull from the attraction points
	float soft_bends_weight = 3;

    Branch *root;
    std::vector<AttractionPoint*> attraction_points;
    std::vector<Branch*> branches;

    std::vector<vec3> branch_locations;

public:
	Tree();

	void generate_crown();

	void generate_trunk();

	void grow();

	void draw(BranchShader bs, AttractionPointShader ls);

	void render(std::vector<vec3> verts, std::vector<float> sizes, GLenum type);


};
