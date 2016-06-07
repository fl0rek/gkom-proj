#pragma once
#include "Model.h"
#include "DrawableGroup.h"
#include <vector>
#include <memory>

class AnimationController : public DrawableGroup {
public:
	virtual ~AnimationController() = 0;

	virtual void setFrame(const float t) override;
	virtual void draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation = glm::mat4(1)) override;

protected:
	glm::mat4 transformation_current;

private:
	//std::vector<Drawable*> ms;
	//DrawableGroup ms;

};


class LinearAnimationController :
	public AnimationController {
public:
	LinearAnimationController(const glm::mat4 &to, const glm::mat4 &from = glm::mat4(1));
	
	virtual void setFrame(const float t) final override;

private:
	glm::mat4 transformation_start;
	glm::mat4 transformation_delta;

};
