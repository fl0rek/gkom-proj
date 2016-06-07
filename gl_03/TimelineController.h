#pragma once
#include "Drawable.h"
#include "AnimationController.h"
#include <vector>


class TimelineController :
	public Drawable
{
public:
	virtual void draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation = glm::mat4(1)) final override;

	virtual void setFrame(const float t) final override;

	void addAnimation(Drawable * m, const float startFrame, const float endFrame);
	void addDrawable(Drawable * d);

	TimelineController();
	virtual ~TimelineController();

private:
	std::vector<Drawable*> animations;
	std::vector<Drawable*> drawables;
	std::vector<std::pair<float, float>> times;

};

