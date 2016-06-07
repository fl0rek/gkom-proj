#pragma once

#define GLEW_STATIC
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "shprogram.h"
#include "Drawable.h"

class Model : public Drawable {
public:
	Model(const GLfloat *vertices, size_t vertice_count, const GLuint *indices, size_t indice_count, ShaderProgram &shader, const glm::vec3 &colour);
	virtual ~Model();

	//virtual void draw(glm::mat4 projection, glm::mat4 view);
	virtual void draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation = glm::mat4(1));

	virtual void setFrame(const float t) override;

	void setModelMatrix(const glm::mat4 &m);
private :
	size_t indice_num;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	GLuint MVP_location;
	GLuint V_location;
	GLuint M_location;

	GLuint colourLocation;
	GLuint lightPositionLocation;

	//GLuint lightColourLocation;
	//GLuint diffuseIntensityLocation;
	//GLuint specularIntensityLocation;
	GLuint lightIntensityLocation;


	glm::vec3 colour;
	//glm::vec3 lightPosition;
	//glm::vec3 lightColour;
	//glm::vec3 diffuseIntensity;
	//glm::vec3 specularIntensity;
	//GLfloat lightIntensity;

	glm::mat4 model = glm::mat4(1.f);

	ShaderProgram &shader;
};

