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
	float yrot = 0;
	float radius = 300;

/*	Tree *tree;
	TreeRenderer *renderer;

	BranchShader branch_shader;
	AttractionPointShader point_shader;*/

	void regen_view();

public:
	glm::mat4 view;

	
	UserInterface();

	void key_handler(int key, int scancode, int action, int mods);
	void mouse_drag(double x, double y);
	void scroll(double ammount);

};