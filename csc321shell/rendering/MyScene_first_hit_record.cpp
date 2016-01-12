#include "../csc321.h"
#include "../sceneview/MyScene.h"

FirstHitRecord::FirstHitRecord() {
	t = 1e40;
	u = v = 0.0;
	p = Point3();
	n = Vector3();
	node = 0;
}

FirstHitRecord::~FirstHitRecord() {
}

bool FirstHitRecord::hit() {
	return node != 0;
}