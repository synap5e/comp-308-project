#include "shader.hpp"

#include <stdlib.h>

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <SOIL.h>

GLuint create_shader(GLenum type, const char* path) {
    GLuint shader = glCreateShader(type);
    

    std::string shader_code;
    std::ifstream shader_stream(path, std::ios::in);
    std::stringstream shader_data;
    shader_data << shader_stream.rdbuf();
    shader_stream.close();
    shader_code = shader_data.str();


    TRACE("Compiling shader : " << path);
    char const *shader_source = shader_code.c_str();
    GLint shader_length = shader_code.size();
    glShaderSource(shader, 1, &shader_source, &shader_length);
    glCompileShader(shader);


    int log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 1){
        std::vector<char> error_message(log_length);
        glGetShaderInfoLog(shader, log_length, NULL, &error_message[0]);
        TRACE("shader errors: " << std::string(error_message.begin(), error_message.end()));
        throw std::runtime_error(std::string(error_message.begin(), error_message.end()));
    }

    return shader;
}

GLuint load_shaders(const char* vertex_shader_path, const char* geometry_shader_path, const char* fragment_shader_path)
{
    GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_path);
    
    GLuint geometry_shader;
    if (geometry_shader_path){
        geometry_shader = create_shader(GL_GEOMETRY_SHADER, geometry_shader_path);
    }

    GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_path);  

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    if (geometry_shader_path){
        glAttachShader(shader_program, geometry_shader);
    }
    glLinkProgram(shader_program);

    int log_length = 0;
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 1){
        std::vector<char> error_message(log_length);
        glGetProgramInfoLog(shader_program, log_length, NULL, &error_message[0]);
        TRACE("Program errors: " << std::string(error_message.begin(), error_message.end()));
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}


void Shader::load_locations(){
    glUseProgram(shader_id);

    model_location = glGetUniformLocation(shader_id, "model");
    view_location = glGetUniformLocation(shader_id, "view");
    projection_location = glGetUniformLocation(shader_id, "projection");

    glUseProgram(0);
}

void Shader::activate(){
    glUseProgram(shader_id);
}  

void Shader::set_projection(glm::mat4& mat){
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_model(glm::mat4& mat){
    glUniformMatrix4fv(model_location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_view(glm::mat4& mat){
    glUniformMatrix4fv(view_location, 1, GL_FALSE, &mat[0][0]);
}


void BranchShader::load(){
    shader_id = load_shaders("./shaders/branch-vertex.glsl", "./shaders/branch-geometry.glsl", "./shaders/branch-fragment.glsl");
//    shader_id = load_shaders("./shaders/basic-vertex.glsl", "./shaders/branch-debug-geometry.glsl", "./shaders/basic-fragment.glsl");


    glUseProgram(shader_id);
    normal_texture_location = glGetUniformLocation(shader_id, "normal_texture");

	// crashes on windows
	//bark_normal_texture = SOIL_load_OGL_texture("bark-normal.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	
	int width;
	int height;

	glGenTextures(1, &bark_normal_texture);
	glBindTexture(GL_TEXTURE_2D, bark_normal_texture);

	unsigned char* image = SOIL_load_image("bark-normal.png", &width, &height, 0, SOIL_LOAD_RGB);

	if (image == NULL) {
		std::cerr << "Could no load bark-normal.png" << std::endl;
		exit(-1);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);


    load_locations();
}

void BranchShader::activate2(){
    glUseProgram(shader_id);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, bark_normal_texture);
    glUniform1i(normal_texture_location, 1);
}  

void LeafShader::load(){
    shader_id = load_shaders("./shaders/leaf-vertex.glsl", nullptr, "./shaders/basic-fragment.glsl");
    //shader_id = load_shaders("./shaders/leaf-debug-vertex.glsl", "./shaders/leaf-debug-geometry.glsl", "./shaders/basic-fragment.glsl");
    load_locations();
}

void AttractionPointShader::load(){
    shader_id = load_shaders("./shaders/basic-vertex.glsl", "./shaders/sphere-geometry.glsl", "./shaders/basic-fragment.glsl");
    load_locations();
}

void GridShader::load(){
    shader_id = load_shaders("./shaders/grid-vertex.glsl", nullptr, "./shaders/basic-fragment.glsl");
    load_locations();
}
/*

void ShadowMapShader::load(){
    shader_id = load_shaders("./shaders/basic-texture-vertex.glsl", nullptr, "./shaders/basic-texture-fragment.glsl");
    glUseProgram(shader_id);
    
    printf("~~ %d\n", texture_location);
    load_locations();
}

void ShadowMapShader::set_texture(GLuint texID){
    glUniform1i(texture_location, texID);
}
*/
