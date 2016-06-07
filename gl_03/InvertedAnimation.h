#pragma once
#include "Drawable.h"
class InvertedAnimation :
	public Drawable
{
public:

	virtual void setFrame(const float t) override;
	
	virtual void draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation = glm::mat4(1));

	InvertedAnimation(Drawable *sourceAnimation);
	virtual ~InvertedAnimation();

private:
	Drawable* m;
};

