#include "TexturedModel.h"
#include <SOIL.h>
#include <iostream>


extern float lightIntensity;
extern glm::vec3 lightPosition;

TexturedModel::TexturedModel(const GLfloat * vertices, size_t vertice_count, const GLuint * indices, size_t indice_count, ShaderProgram & shader, const char* textureFile, GLuint textId) :
		shader(shader) {
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertice_count * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	// vertex geometry data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	// vertex normal data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	// vertex uv data
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_count * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

	int width, height;
	unsigned char* image = SOIL_load_image(textureFile, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw std::exception("Failed to load texture file");

	glGenTextures(1, &this->texture);

	this->textureID = GL_TEXTURE0;

	glActiveTexture(textureID);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->shader.Use();

	GLuint programID = this->shader.get_programID();
	this->MVP_location = glGetUniformLocation(programID, "MVP");
	this->V_location = glGetUniformLocation(programID, "V");
	this->M_location = glGetUniformLocation(programID, "M");

	this->lightPositionLocation = glGetUniformLocation(programID, "LightPosition_worldspace");
	this->lightIntensityLocation = glGetUniformLocation(programID, "lightIntensity");

	this->indice_num = indice_count;
}

void TexturedModel::draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation) {
	glm::mat4 M = parentTransformation * this->model;
	glm::mat4 MVP = projection * view * M;

	this->shader.Use();

	glUniformMatrix4fv(this->MVP_location, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniformMatrix4fv(this->V_location, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(this->M_location, 1, GL_FALSE, glm::value_ptr(M));

	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));
	glUniform1f(lightIntensityLocation, lightIntensity);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glUniform1i(glGetUniformLocation(shader.get_programID(), "texture0"), 0);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glDrawElements(GL_TRIANGLES, this->indice_num, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void TexturedModel::setFrame(const float t) {
}

void TexturedModel::setModelMatrix(const glm::mat4 & m) {
	this->model = m;
}

TexturedModel::~TexturedModel() {
}
