#pragma once
#include "Drawable.h"
#include <vector>
class DrawableGroup :
	public Drawable {
public:
	DrawableGroup();
	void addModel(Drawable *m);
	virtual void draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation = glm::mat4(1)) override;
	virtual void setFrame(const float t) override;
	virtual ~DrawableGroup();
private:
	std::vector<Drawable*> ms;
};

