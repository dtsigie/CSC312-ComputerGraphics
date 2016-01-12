#include "../csc321.h"
#include "Shape.h"
#include <fstream>
#include <FL/glut.h>

//#include "Point3.h"
//#include "Vector3.h"]
using namespace std;

Shape::~Shape() {
}

void Shape::draw() {
	glBegin(GL_TRIANGLES);
	for (int i=0; i < vertices.size(); i +=3) {
		glNormal3dv(&normals[i][0]);
		glVertex3dv(&vertices[i][0]);
		glNormal3dv(&normals[i+1][0]);
		glVertex3dv(&vertices[i+1][0]);
		glNormal3dv(&normals[i+2][0]);
		glVertex3dv(&vertices[i+2][0]);
	}
	glEnd();
}

Shape::Shape() : vertices(), normals() {
}
HitRecord Shape::intersect(const Point3& o, const Vector3& dir) {
	return HitRecord();
}

void Shape::addTriangle(const Point3& p1, const Point3& p2, const Point3& p3) {
	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
}

void Shape::addTriangle(const Point3& p1, const Point3& p2, const Point3& p3, const Vector3& n) {
	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	normals.push_back(n);
}

void Shape::addTriangle(const Point3& p1, const Point3& p2, const Point3& p3, const Vector3& n1, const Vector3& n2, const Vector3& n3) {
	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	normals.push_back(n1);
	normals.push_back(n2);
	normals.push_back(n3);
}

void Shape::addSquare(const Point3& p1, const Point3& p2, const Point3& p3, const Point3& p4) {
	addTriangle(p1, p2, p4);
	addTriangle(p2, p3, p4);
}

void Shape::addSquare(const Point3& p1, const Point3& p2, const Point3& p3, const Point3& p4, const Vector3& n) {
	addTriangle(p1, p2, p4, n);
	addTriangle(p2, p3, p4, n);
}

void Shape::addSquare(const Point3& p1, const Point3& p2, const Point3& p3, const Point3& p4, const Vector3& n1, const Vector3& n2, const Vector3& n3, const Vector3& n4) {
	addTriangle(p1, p2, p4, n1, n2, n4);
	addTriangle(p2, p3, p4, n2, n3, n4);
}

Torus::Torus(double n , double m) : Shape(){
	 int numc = 100, numt = 100;
	 double TWOPI = 2 * M_PI;
        for (int i = 0; i < numc; i++) {
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= numt; j++) {
                for (int k = 1; k >= 0; k--) {

                    double s = (i + k) % numc + 0.5;
                    double t = j % numt;

                    double x = (0.5 + 0.1 * cos(s * TWOPI / numc)) * cos(t * TWOPI / numt);
                    double y = (0.5 + 0.1 * cos(s * TWOPI / numc)) * sin(t * TWOPI / numt);
                    double z = 0.1 * sin(s * TWOPI / numc);

                    glVertex3d(x, y, z);
                }
            }
            glEnd();
        }
    }


Sphere::Sphere(int n) : Shape() {
	
	unsigned int i;
	int j;
	if (n > 5)
		n = 5;
	double a = 2.0 / (1.0 + sqrt(5.0));

	addTriangle(Point3( 0,  a, -1), Point3(-a,  1,  0), Point3( a,  1,  0));
	addTriangle(Point3( 0,  a,  1), Point3( a,  1,  0), Point3(-a,  1,  0));
	addTriangle(Point3( 0,  a,  1), Point3(-1,  0,  a), Point3( 0, -a,  1));
	addTriangle(Point3( 0,  a,  1), Point3( 0, -a,  1), Point3( 1,  0,  a));
	addTriangle(Point3( 0,  a, -1), Point3( 1,  0, -a), Point3( 0, -a, -1));
	addTriangle(Point3( 0,  a, -1), Point3( 0, -a, -1), Point3(-1,  0, -a));
	addTriangle(Point3( 0, -a,  1), Point3(-a, -1,  0), Point3( a, -1,  0));
	addTriangle(Point3( 0, -a, -1), Point3( a, -1,  0), Point3(-a, -1,  0));
	addTriangle(Point3(-a,  1,  0), Point3(-1,  0, -a), Point3(-1,  0,  a));
	addTriangle(Point3(-a, -1,  0), Point3(-1,  0,  a), Point3(-1,  0, -a));
	addTriangle(Point3( a,  1,  0), Point3( 1,  0,  a), Point3( 1,  0, -a));
	addTriangle(Point3( a, -1,  0), Point3( 1,  0, -a), Point3( 1,  0,  a));
	addTriangle(Point3( 0,  a,  1), Point3(-a,  1,  0), Point3(-1,  0,  a));
	addTriangle(Point3( 0,  a,  1), Point3( 1,  0,  a), Point3( a,  1,  0));
	addTriangle(Point3( 0,  a, -1), Point3(-1,  0, -a), Point3(-a,  1,  0));
	addTriangle(Point3( 0,  a, -1), Point3( a,  1,  0), Point3( 1,  0, -a));
	addTriangle(Point3( 0, -a, -1), Point3(-a, -1,  0), Point3(-1,  0, -a));
	addTriangle(Point3( 0, -a, -1), Point3( 1,  0, -a), Point3( a, -1,  0));
	addTriangle(Point3( 0, -a,  1), Point3(-1,  0,  a), Point3(-a, -1,  0));
	addTriangle(Point3( 0, -a,  1), Point3( a, -1,  0), Point3( 1,  0,  a));

	std::vector<Point3> vOld;
	for (j = 1; j < n; j++) {
		// normalize to sphere
		Point3 origin;
		for (i = 0; i < vertices.size(); i++) {
			Vector3 v = unit( vertices[i] - origin );
			vertices[i] = origin + v;
		}
		vOld = vertices;
		vertices.clear();
		for (i = 0; i < vOld.size(); i += 3) {
			Point3 v1 = vOld[i];
			double x1 = v1[0], y1 = v1[1], z1 = v1[2];
			Point3 v2 = vOld[i + 1];
			double x2 = v2[0], y2 = v2[1], z2 = v2[2];
			Point3 v3 = vOld[i + 2];
			double x3 = v3[0], y3 = v3[1], z3 = v3[2];

			addTriangle(v1, Point3((x1 + x2) * 0.5, (y1 + y2) * 0.5, (z1 + z2) * 0.5), Point3((x1 + x3) * 0.5, (y1 + y3) * 0.5, (z1 + z3) * 0.5));
			addTriangle(Point3((x1 + x2) * 0.5, (y1 + y2) * 0.5, (z1 + z2) * 0.5), v2, Point3((x3 + x2) * 0.5, (y3 + y2) * 0.5, (z3 + z2) * 0.5));
			addTriangle(Point3((x1 + x3) * 0.5, (y1 + y3) * 0.5, (z1 + z3) * 0.5), Point3((x3 + x2) * 0.5, (y3 + y2) * 0.5, (z3 + z2) * 0.5), v3);
			addTriangle(Point3((x1 + x3) * 0.5, (y1 + y3) * 0.5, (z1 + z3) * 0.5), Point3((x1 + x2) * 0.5, (y1 + y2) * 0.5, (z1 + z2) * 0.5), Point3((x3 + x2) * 0.5, (y3 + y2) * 0.5, (z3 + z2) * 0.5));
		}
	}
	// compute normals and scale to 0.5 radius
	Point3 origin;
	for (i = 0; i < vertices.size(); i++) {
		Vector3 n = unit( vertices[i] - origin );
		normals.push_back(n);
		vertices[i] = origin + n * 0.5;
	}
}

MoebiusStrip::MoebiusStrip(int n, int m) : Shape() {
	unsigned int k;
	int i, j;
	if (n < 3)
		n = 3;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			double s0 = 2 * M_PI * i / n;
			double t0 = -0.4 + 0.8 * j / m;

			double s1 = 2 * M_PI * (i + 1) / n;
			double t1 = -0.4 + 0.8 * (j + 1)  / m;

			double x1 = cos(s0) + t0 * cos(s0 * 0.5) * cos(s0);
			double y1 = sin(s0) + t0 * cos(s0 * 0.5) * sin(s0);
			double z1 = t0 * sin(s0 * 0.5);

			double x2 = cos(s1) + t0 * cos(s1 * 0.5) * cos(s1);
			double y2 = sin(s1) + t0 * cos(s1 * 0.5) * sin(s1);
			double z2 = t0 * sin(s1 * 0.5);

			double x3 = cos(s1) + t1 * cos(s1 * 0.5) * cos(s1);
			double y3 = sin(s1) + t1 * cos(s1 * 0.5) * sin(s1);
			double z3 = t1 * sin(s1 * 0.5);

			double x4 = cos(s0) + t1 * cos(s0 * 0.5) * cos(s0);
			double y4 = sin(s0) + t1 * cos(s0 * 0.5) * sin(s0);
			double z4 = t1 * sin(s0 * 0.5);

			// add the square both ways (because surface is two sided)
			addSquare(Point3(x1, y1, z1), Point3(x2, y2, z2), Point3(x3, y3, z3), Point3(x4, y4, z4));
			addSquare(Point3(x2, y2, z2), Point3(x1, y1, z1), Point3(x4, y4, z4), Point3(x3, y3, z3));
		}
	}

	// calculate only one normal per face, the explicit expression for the surface normal is
	// a little too messy
	for (k = 0; k < vertices.size(); k += 3) {
		vertices[k    ] *= 0.70;
		vertices[k + 1] *= 0.70;
		vertices[k + 2] *= 0.70;
		Vector3 a = vertices[k + 1] - vertices[k];
		Vector3 b = vertices[k + 2] - vertices[k];
		Vector3 n = a ^ b;
		n.normalize();
		normals.push_back(n);
	}
}

Cow::Cow() : Shape() {
	std::ifstream cowFile("../data/cow.raw");
	if ( !cowFile.good() )
		return;

	bool done = !cowFile;
	while (!done) {
		Point3 p1, p2, p3;
		cowFile >> p1[0];
		cowFile >> p1[1];
		cowFile >> p1[2];
		cowFile >> p2[0];
		cowFile >> p2[1];
		cowFile >> p2[2];
		cowFile >> p3[0];
		cowFile >> p3[1];
		done = !(cowFile >> p3[2]);
		addTriangle(p1, p2, p3);
	}
	for (unsigned int i = 0; i < vertices.size(); i += 3) {
		vertices[i    ] *= 0.20;
		vertices[i + 1] *= 0.20;
		vertices[i + 2] *= 0.20;
		Vector3 a = vertices[i + 1] - vertices[i];
		Vector3 b = vertices[i + 2] - vertices[i];
		Vector3 n = a ^ b;
		n.normalize();
		normals.push_back(n);
	}
}



Cone::Cone(int n, int m) : Shape(){
	double radius = 0.5;
	double h = 1;
	double x = 0.5;
	double z = 0.0;

	double y = 0.0;

	for (int i = 1; i <= n; i++) {
		double a = (2 * M_PI * i) / n;

		double x1 = 0.5* cos(a);
		double z1 = 0.5* sin(a);

		double norm = 0.5*sqrt((x1*x1) + (z1*z1));

		//BOT FACE
		addTriangle(Point3(0.0, -0.5, 0.0),   Point3(x, -0.5, z),  Point3(x1, -0.5, z1),
			Vector3(0.0, -1.0, 0.0), Vector3(0.0, -1.0, 0.0), Vector3(0.0, -1.0, 0.0));

		//SIDE FACES

		//calculation for each row
		for( int k = 0; k < m  ; k++){
			double y = -0.5 + (double) k * 1.0/ m;
			double yl = -0.5 + (double) (k+1) * 1.0/ m;
			double g1 =(double) 0.5 * 1- y/1.0;
			double g2 = (double) 0.5 *1 -yl/1.0;

			double norm = 0.5*sqrt((x*x) + (z*z));
			double norm1 = 0.5*sqrt((x1*x1) + (z1*z1));


			if ( k == m-1){
				addTriangle(Point3(0, 0.5, 0),  Point3(g1*x1, y, g1*z1), Point3(g1*x, y, g1*z),
					Vector3(0, 1, 0),  Vector3(x, norm, z), Vector3(x1, norm1, z1));

			}

			else{
				addTriangle(Point3(g1*x1, y, g1*z1),  Point3(g1*x, y, g1*z), Point3(g2*x1, yl, g2*z1),
					Vector3(x1, norm1, z1), Vector3(x, norm, z), Vector3(x1, norm1, z1));
				addTriangle(Point3(x1*g2, yl, z1*g2),  Point3(x*g1, y, z*g1), Point3(x*g2, yl, z*g2),
					Vector3(x1, norm1, z1), Vector3(x, norm, z), Vector3(x1, norm1, z1));
				//	Vector3(x1, norm1, z1), Vector3(x, norm, z), Vector3(x, norm, z));
			}
		}

		x = x1;
		z = z1;

	}


}
Cube::Cube(int n) : Shape(){
	double x = 0.5;
	double xl = 0.5- 1.0/ (double) n;
	double z = 0.5;
	double zl = 0.5 - 1.0/ (double) n;
	//y normal
	for (int i = 1; i <= n; i++) {
		for(int j = 1; j <= n; j++){

			//bot Face - positive normal
			addTriangle(Point3(x, -0.5, -z),Point3(x, -0.5, -zl), Point3(xl, -0.5, -zl),  
				Vector3(0.0, -1.0, 0.0), Vector3(0.0, -1.0, 0.0), Vector3(0.0, -1.0, 0.0));
			addTriangle(Point3(xl, -0.5, -zl),  Point3(xl, -0.5, -z), Point3(x, -0.5, -z),
				Vector3(0.0, -1.0, 0.0), Vector3(0.0, -1.0, 0.0), Vector3(0.0, -1.0, 0.0));
			//top Face - negative normal
			addTriangle(Point3(x, 0.5, -z), Point3(xl, 0.5, -zl),  Point3(x, 0.5, -zl),
				Vector3(0.0, 1.0, 0.0), Vector3(0.0, 1.0, 0.0), Vector3(0.0, 1.0, 0.0));
			addTriangle(Point3(xl, 0.5, -zl), Point3(x, 0.5, -z), Point3(xl, 0.5, -z),  
				Vector3(0.0, 1.0, 0.0), Vector3(0.0, 1.0, 0.0), Vector3(0.0, 1.0, 0.0));

			z -= 1.0/(double) n;
			zl -= 1.0/ (double) n;
		}
		z = 0.5;
		zl = 0.5 - 1.0/ (double) n;
		x -= 1.0/(double) n;
		xl -= 1.0/ (double) n;
	}


	//	x normal faces
	double y = 0.5;
	double yl = 0.5- 1.0/ (double) n;
	z = 0.5;
	zl = 0.5 - 1.0/ (double) n;

	for (int i = 1; i <= n; i++) {
		for(int j = 1; j <= n; j++){

			//positive normal face
			addTriangle(Point3(0.5, y, -z), Point3(0.5, y, -zl), Point3(0.5, yl, -zl), 
				Vector3(1.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0));
			addTriangle(Point3(0.5, yl, -zl),  Point3(0.5, yl, -z), Point3(0.5, y , -z),
				Vector3(1.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0), Vector3(1.0, 0.0, 0.0));

			////negative normal Face
			addTriangle(Point3(-0.5, y, -z), Point3(-0.5, yl, -zl), Point3(-0.5, y, -zl),  
				Vector3(-1.0, 0.0, .0), Vector3(-1.0, 0.0, 0.0), Vector3(-1.0, 0.0, 0.0));
			addTriangle(Point3(-0.5, yl, -zl), Point3(-0.5, y , -z), Point3(-0.5, yl, -z), 
				Vector3(-1.0, 0.0, 0.0), Vector3(-1.0, 0.0, 0.0), Vector3(-1.0, 0.0, 0.0));

			z -= 1.0/(double) n;
			zl -= 1.0/ (double) n;
		}
		z = 0.5;
		zl = 0.5 - 1.0/ (double) n;
		y -= 1.0/(double) n;
		yl -= 1.0/ (double) n;
	}

	//z normal faces

	y = 0.5;
	yl = 0.5- 1.0/ (double) n;
	x = 0.5;
	xl = 0.5 - 1.0/ (double) n;

	for (int i = 1; i <= n; i++) {
		for(int j = 1; j <= n; j++){

			//negative normal Face
			addTriangle(Point3(-x, -y, -0.5),Point3(-x, -yl, -0.5), Point3(-xl, -yl, -0.5),  
				Vector3(0.0, 0.0, -1.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 0.0, -1.0));
			addTriangle(Point3(-xl, -yl, -0.5),  Point3(-xl, -y, -0.5), Point3(-x, -y, -0.5),
				Vector3(0.0, 0.0, -1.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 0.0, -1.0));
			//postive normal
			addTriangle(Point3(-x, -y, 0.5), Point3(-xl, -yl, 0.5),  Point3(-x, -yl, 0.5),
				Vector3(0.0, 0.0, 1.0), Vector3(0.0, 0.0, 1.0), Vector3(0.0, 0.0, 1.0));
			addTriangle(Point3(-xl, -yl, 0.5), Point3(-x, -y, 0.5), Point3(-xl, -y, 0.5),  
				Vector3(0.0, 0.0, 1.0), Vector3(0.0, 0.0, 1.0), Vector3(0.0, 0.0, 1.0));

			y -= 1.0/(double) n;
			yl -= 1.0/ (double) n;
		}
		y = 0.5;
		yl = 0.5 - 1.0/ (double) n;
		x -= 1.0/(double) n;
		xl -= 1.0/ (double) n;

	}}
Cylinder::Cylinder(int n, int m) : Shape() {
	double ca = 0.5;
	double sa = 0.0;
	for (int i = 1; i <= n; i++) {
		double a = (2 * M_PI * i) / n;
		double ca1 = 0.5 * cos(a);
		double sa1 = 0.5 * sin(a);

		//TOP AND BOT FACES
		addTriangle(Point3(0.0, 0.5, 0.0), Point3(ca1, 0.5, sa1), Point3(ca, 0.5, sa), 
			Vector3(0.0, 1.0, 0.0), Vector3(0.0, 1.0, 0.0), Vector3(0.0, 1.0, 0.0));
		addTriangle(Point3(0.0, -0.5, 0.0), Point3(ca, -0.5, sa), Point3(ca1, -0.5, sa1), 
			Vector3(0.0, -1.0, 0.0), Vector3(0.0, -1.0, 0.0), Vector3(0.0, -1.0, 0.0));

		//SIDE FACES
		double y = 0.5;
		double yl = 0.5 - 1.0/(double) m;
		for( int k = 1; k <= m ; k++){
			addTriangle(Point3(ca1, yl, sa1),  Point3(ca, y, sa), Point3(ca1, y, sa1),
				Vector3(ca, 0.0, sa), Vector3(ca, 0.0, sa), Vector3(ca, 0.0, sa));
			addTriangle(Point3(ca, -y, sa),  Point3(ca, -yl, sa), Point3(ca1, -y, sa1),
				Vector3(ca, 0.0, sa), Vector3(ca, 0.0, sa), Vector3(ca, 0.0, sa));
			y -= 1.0/(double) m;
			yl -= 1.0/(double) m;
		}

		ca = ca1;
		sa = sa1;
	}
}
HitRecord Cylinder::intersect(const Point3& o, const Vector3& d) {
	HitRecord hr;
	//Bot and Top cap
	Point3 p;
	Vector3 n;
	double tTop = (0.5 - o[1])/ d[1];
	double tBot = (-0.5 - o[1])/ d[1];

	// Top cap
	if(tTop > 0){
	p = o + tTop*d;
	
	if((p[0]*p[0] + p[2]*p[2]) <= 0.25){	

		n = Vector3(0, p[1], 0);
		n.normalize();
		hr.addHit(tTop, 0.0, 0.0, p, n);
	}}
	// Bot cap
	if(tBot > 0){
	p = o + tBot*d;
	
	if((p[0]*p[0] + p[2]*p[2]) <= 0.25){	

		n = Vector3(0, p[1], 0);
		n.normalize();
		hr.addHit(tBot, 0.0, 0.0, p, n);
	}}
	//Sides
	// solve a*t^2 + b*t + c = 0
	double a = d[0]*d[0] + d[2]*d[2];
	double b = 2.0*( d[0]*o[0] + d[2]*o[2] );
	double c = o[0]*o[0] + o[2]*o[2] - 0.25;

	double dis = b*b - 4.0*a*c;
	if (dis >= 0) {
		dis = sqrt(dis);
		double t1 = ( dis - b) / (2.0*a);
		double t2 = (-dis - b) / (2.0*a);

		//Point3 p;
		//Vector3 n;
		if (t1 > 0.0) {
			p = o + t1*d;
			if ((p[1] < 0.5) && (p[1] > -0.5)) {
				n = Vector3(p[0], 0, p[2]);
				n.normalize();
				hr.addHit(t1, 0.0, 0.0, p, n);
			}
		}
		if (t2 > 0.0) {
			p = o + t2*d;
			if ((p[1] < 0.5) && (p[1] > -0.5)) {
				n = Vector3(p[0], 0, p[2]);
				n.normalize();
				hr.addHit(t2, 0.0, 0.0, p, n);
			}
		}

	}

	hr.sortHits();
	return hr;
}
HitRecord Sphere::intersect(const Point3& o, const Vector3& d) {
	HitRecord hr;

	// solve a*t^2 + b*t + c = 0
	double a = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
	double b = 2.0*( d[0]*o[0] + d[1]*o[1] + d[2]*o[2] );
	double c = o[0]*o[0] + o[1]*o[1] + o[2]*o[2] - 0.25;

	double dis = b*b - 4.0*a*c;
	if (dis >= 0) {
		dis = sqrt(dis);
		double t1 = (dis - b) / (2.0*a);
		double t2 = (-dis - b) / (2.0*a);

		Point3 p;
		Vector3 n;
		if (t1 < 0.0) return hr;

		if (t2 > 0.0) {
			p = o + t2*d;
			n = Vector3(p[0], p[1], p[2]);
			n.normalize();
			hr.addHit(t2, 0.0, 0.0, p, n);

		}
		p = o + t1*d;
		n = Vector3(p[0], p[1], p[2]);
		n.normalize();
		hr.addHit(t1, 0.0, 0.0, p, n);

	}

	hr.sortHits();
	return hr;
}


HitRecord Cone::intersect(const Point3& o, const Vector3& d) {
	HitRecord hr;
	//Bot and Top cap
	Point3 p ;
	Vector3 n;
	
	double tBot = (-0.5 - o[1])/ d[1];
	
	if((tBot) > 0){
		p = o + tBot*d;
	if((p[0]*p[0] + p[2]*p[2]) <= 0.25){	
		n = Vector3(0, p[1], 0);
		n.normalize();
		hr.addHit(tBot, 0.0, 0.0, p, n);
	}
	}
	//Sides
	// solve a*t^2 + b*t + c = 0
	double a = d[0]*d[0] + d[2]*d[2] - 0.25*d[1]*d[1];
	double b = 2.0*( d[0]*o[0] + d[2]*o[2] ) + 0.5*d[1]*(0.5 - o[1]) ;
	double c = o[0]*o[0] + o[2]*o[2] - 0.25*(0.5 - o[1])*(0.5 - o[1]);

	double dis = b*b - 4.0*a*c;
	if (dis >= 0) {
		dis = sqrt(dis);
		double t1 = ( dis - b) / (2.0*a);
		double t2 = (-dis - b) / (2.0*a);

		//Point3 p;
		//Vector3 n;
		if (t1 > 0.0) {
			p = o + t1*d;
			if (p[1] < 0.5 && p[1] > -0.5 && p[0]*p[0] + p[2]*p[2] < 0.25) {
				n = Vector3(p[0],0.5*sqrt(p[0]*p[0] + p[2]*p[2]), p[2]);
				n.normalize();
				hr.addHit(t1, 0.0, 0.0, p, n);
			}
		}
		if (t2 > 0.0) {
			p = o + t2*d;
			if (p[1] < 0.5 && p[1] > -0.5 && p[0]*p[0] + p[2]*p[2] < 0.25) {
				n = Vector3(p[0], 0.5*sqrt(p[0]*p[0] + p[2]*p[2]), p[2]);
				n.normalize();
				hr.addHit(t2, 0.0, 0.0, p, n);
			}
		}

	}

	hr.sortHits();
	return hr;
}
HitRecord Cube::intersect(const Point3& o, const Vector3& d) {
	HitRecord hr;
	//Bot and Top cap
	Point3 p;
	Vector3 n;
	double tTop = (0.5 - o[1])/ d[1];
	double tBot = (-0.5 - o[1])/ d[1];
	double tZpos = (0.5 - o[2])/ d[2];
	double tZneg = (-0.5 - o[2])/ d[2];
	double tXpos = (0.5 - o[0])/ d[0];
	double tXneg = (-0.5 - o[0])/ d[0];

	// Top cap
	if(tTop > 0){
		p = o + tTop*d;
		if( p[2] <= 0.5 && p[2] >= -0.5 && p[0] <= 0.5 && p[0] >= -0.5){	

			n = Vector3(0, p[1], 0);
			n.normalize();
			hr.addHit(tTop, 0.0, 0.0, p, n);
		}}
	// Bot cap
	if(tBot > 0){
		p = o + tBot*d;
		
		if( p[2] <= 0.5 && p[2] >= -0.5 && p[0] <= 0.5 && p[0] >= -0.5){	
		
			n = Vector3(0, p[1], 0);
			n.normalize();
			hr.addHit(tBot, 0.0, 0.0, p, n);
		}
	}
	//Z positive
	if( tZpos > 0){
		p = o + tZpos*d;
		if( p[1] <= 0.5 && p[1] >= -0.5 && p[0] <= 0.5 && p[0] >= -0.5){	

			n = Vector3(0.0, 0.0, p[2]);
			n.normalize();
			hr.addHit(tZpos, 0.0, 0.0, p, n);
		}}

	//Z negative
	if( tZneg > 0){
		p = o + tZneg*d;
		if(p[1] <= 0.5 && p[1] >= -0.5 && p[0] <= 0.5 && p[0] >= -0.5){	

			n = Vector3(0.0, 0, p[2]);
			n.normalize();
			hr.addHit(tZneg, 0.0, 0.0, p, n);
		}}

	//X positive
	if( tXpos > 0){
		p = o + tXpos*d;
		if(p[1] < 0.5 && p[1] >= -0.5 && p[2] <= 0.5 && p[2] >= -0.5){	

			n = Vector3(p[0], 0.0, 0.0);
			n.normalize();
			hr.addHit(tXpos, 0.0, 0.0, p, n);
		}}
	//X negative
	if( tXneg > 0){
		p = o + tXneg*d;
		if( p[1] <= 0.5 && p[1] >= -0.5 && p[2] <= 0.5 && p[2] >= -0.5){	

			n = Vector3(p[0], 0.0, 0.0);
			n.normalize();
			hr.addHit(tXneg, 0.0, 0.0, p, n);
		}
	}



	hr.sortHits();
	return hr;
}

HitRecord Torus::intersect(const Point3& o, const Vector3& d){
	HitRecord hr;
	// solve a*t^4 + b*t^3 + c*t^2 + d*t + e = 0
	/*double a = (d*d)*(d*d);
	double b = 4*(d*d)*(o*d);
	double c = 4*(d*o)*(o*d) + 2*(d*d)*((o*o) - 0.1*0.1 - 0.5*0.5) + 4*0.5*0.5*d[2]*d[2] ;
	double d = 4*(o*d)*((o*o) - 0.1*0.1 - 0.5*0.5) + 8*0.5*0.5*o[2]*d[2];
	double e = (o*o - 0.1*0.1 - 0.5*0.5)*(o*o - 0.1*0.1 - 0.5*0.5) + 4*0.5*0.5*o[2] - 4*0.5*0.5*0.1*0.1;*/

	hr.sortHits();
	return hr;

}