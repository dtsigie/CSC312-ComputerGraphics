#include "../csc321.h"
#include "MyScene.h"

Object::Object() {
	shape = 0;
	ambient = Color();
	diffuse = Color();
	specular = Color();
	reflect = Color();
	transparent = Color();
	emit = Color();
	shine = 0.0;
	ior = 1.0;
	textureU = 0.0;
	textureV = 0.0;
}

Object::~Object() {
}

void Object::draw() {
	glMaterialfv(GL_FRONT, GL_AMBIENT, &ambient[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, &diffuse[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, &specular[0]);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	shape->draw();
}

HitRecord Object::intersect(const Point3& o, const Vector3& dir) {
	return shape->intersect(o, dir);
}