#pragma once
#include "Drawable.h"
class Transformation :
	public Drawable {
public:
	Transformation(const glm::mat4 &t, Drawable* m);

	virtual void draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation = glm::mat4(1));
	virtual void setFrame(const float) override;

	virtual ~Transformation();
private:
	glm::mat4 t;
	Drawable *m;
};

