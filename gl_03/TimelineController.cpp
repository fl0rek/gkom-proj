#include "TimelineController.h"



void TimelineController::draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation) {
	for (unsigned i = 0; i < drawables.size(); i++) {
		drawables[i]->draw(projection, view, parentTransformation);
	}
}

void TimelineController::setFrame(const float t) {
	for (int i = animations.size()-1; i >= 0; i--) {
		if (t <= times[i].first) {
			animations[i]->setFrame(0.);
		} else if (t >= times[i].second) {
			animations[i]->setFrame(1.);
		} else {
			float fromStart = t - times[i].first;
			float total = times[i].second - times[i].first;
			animations[i]->setFrame(fromStart / total);
		}
	}
}

void TimelineController::addAnimation(Drawable * m, const float startFrame, const float endFrame) {
	animations.push_back(m);
	times.push_back(std::make_pair(startFrame, endFrame));
}

void TimelineController::addDrawable(Drawable * d) {
	drawables.push_back(d);
}

TimelineController::TimelineController() {
}


TimelineController::~TimelineController() {
}
