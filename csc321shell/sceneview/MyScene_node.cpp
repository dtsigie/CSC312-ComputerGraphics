#include "../csc321.h"
#include "MyScene.h"

Node::Node() : object(0), tree(0) {
	trans = Matrix4::identity();
	transInv = Matrix4::identity();
	transInvT = Matrix4::identity();
}

Node::~Node() {
	if (object != 0)
		delete object;
}

void Node::draw() {
	glPushMatrix();
	glMultMatrixd( &trans(0,0) );

	if (object != 0)
		object->draw();
	else if (tree != 0)
		tree->draw();
	
	glPopMatrix();
}

FirstHitRecord Node::intersect(const Point3& o, const Vector3& dir) {
	HitRecord hr;
	FirstHitRecord fhr;
	Point3 no = transInv * o;
	Vector3 ndir = transInv * dir;

	if (object != 0 ) {
		hr = object->intersect(no, ndir);
		hr.sortHits();
		if (hr.getFirstHit(fhr.t, fhr.u, fhr.v, fhr.p, fhr.n)) {
			fhr.node = this;
			fhr.p = trans * fhr.p;
			fhr.n = transInvT * fhr.n;
		}
	}
	else if (tree != 0) 
		return tree->intersect(no, ndir);

	return fhr;
}