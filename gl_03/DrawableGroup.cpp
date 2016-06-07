#include "DrawableGroup.h"



DrawableGroup::DrawableGroup() {
}

void DrawableGroup::addModel(Drawable * m) {
	ms.push_back(m);
}

void DrawableGroup::draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation) {
	for (auto ii = ms.begin(); ii != ms.end(); ii++) {
		(*ii)->draw(projection, view, parentTransformation);
	}
}

void DrawableGroup::setFrame(const float t) {
	for (auto ii = ms.begin(); ii != ms.end(); ii++) {
		(*ii)->setFrame(t);
	}
}


DrawableGroup::~DrawableGroup() { }
