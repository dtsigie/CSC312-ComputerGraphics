#include "../csc321.h"
#include "MyScene.h"

Tree::Tree() : nodes() {
}

Tree::~Tree() {
	for (int i = 0; i < nodes.size(); i++) 
		delete nodes[i];
}

void Tree::addNode(Node* n) {
	nodes.push_back(n);
}

void Tree::draw() {
	for (int i=0; i < nodes.size(); i++) 
		nodes[i]->draw();
}

FirstHitRecord Tree::intersect(const Point3& o, const Vector3& dir) {
	// return the closest hit amongst all children nodes
	FirstHitRecord fhrMin;
	for (int i=0; i < nodes.size(); i++) {
		FirstHitRecord fhr = nodes[i]->intersect(o, dir);
		if (fhr.hit() && (fhr.t < fhrMin.t) && fhr.t > 1)
			fhrMin = fhr;
	}
	return fhrMin;
}