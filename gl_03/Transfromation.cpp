#include "Transfromation.h"



Transformation::Transformation(const glm::mat4 & t, Drawable *m) :
	t(t), m(m) {

}

void Transformation::draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 parentTransformation) {
	this->m->draw(projection, view, parentTransformation * this->t);
}

void Transformation::setFrame(float t) {
	this->m->setFrame(t);
}

Transformation::~Transformation() {
}
