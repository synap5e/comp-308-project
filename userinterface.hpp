#pragma once

//#include "node.hpp"
//#include "stem.hpp"
/*#include "shader.hpp"
#include "tree.hpp"
#include "treerenderer.hpp"*/

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class UserInterface {
private:
	float xrot = 90;
	float yrot = 90;
	float radius = 400;
	glm::vec3 centre;


/*	//124.899925, 47.431683, 14.000000
	float xrot = 124.899925;
	float yrot = 47.431683;
	float radius = 14.000000;*/

/*	Tree *tree;
	TreeRenderer *renderer;

	BranchShader branch_shader;
	AttractionPointShader point_shader;*/

	void regen_view();

public:
	glm::mat4 view;

	bool enable[9];
	
	UserInterface();

	void key_handler(int key, int scancode, int action, int mods);
	void mouse_drag(double x, double y);
	void mouse2_drag(double x, double y);
	void scroll(double ammount);

};
