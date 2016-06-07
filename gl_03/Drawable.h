#pragma once

#include <glm/glm.hpp>

class Drawable {
public:
	//virtual void draw(const glm::mat4 projection, const glm::mat4 view) = 0;
	virtual void draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation = glm::mat4(1)) = 0;
	virtual void setFrame(const float t) = 0;
	virtual ~Drawable() {};
};

