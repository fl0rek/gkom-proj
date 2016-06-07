#include "InvertedAnimation.h"




void InvertedAnimation::setFrame(const float t) {
	m->setFrame(1 - t);
}

void InvertedAnimation::draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation) {
	m->draw(projection, view, parentTransformation);
}

InvertedAnimation::InvertedAnimation(Drawable *sourceAnimation) {
	m = sourceAnimation;
}

InvertedAnimation::~InvertedAnimation()
{
}
