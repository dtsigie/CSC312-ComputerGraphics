#include "../csc321.h"
#include "IntersectionInterface.h"
#include "HitRecord.h"
#include "../shapes/ShapesUI.h"
#include "../shapes/ShapesInterface.h"

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <fstream>

IntersectionUI::IntersectionUI() {
    width = height = 0;
	current = 0;
	torus = 0;
}

IntersectionUI::~IntersectionUI() {
	if (current != 0)
		delete current;
}

void IntersectionUI::resize(int w, int h) {
    width = w;
    height = h;
}

void IntersectionUI::draw() {
    setup3DDrawing( Color(1,1,1), width, height, true );

    glMatrixMode(GL_MODELVIEW);
    gluLookAt( 3.5 * cos( intersectionUI->getYRot() ) * cos( intersectionUI->getXRot() ), 
               3.5 * sin( intersectionUI->getYRot() ), 
               3.5 * cos( intersectionUI->getYRot() ) * sin( intersectionUI->getXRot() ), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


    glDisable(GL_LIGHTING);
    if ( intersectionUI->m_bGrid->value() ) {
        glBegin(GL_LINES);
        // draw grid
        glColor3f(0.0f, 0.0f, 0.0f);
        for (int i = 0; i <= 10; i++) {
            float s = -2.0f + i / 2.5f;
            glVertex3f(s, 0.0f, -2.0f);
            glVertex3f(s, 0.0f,  2.0f);
            glVertex3f(-2.0f, 0.0f, s);
            glVertex3f( 2.0f, 0.0f, s);
        }
        glEnd();
        // draw (X,Y,Z) axes
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-2.0f, 0.0f, 0.0f);
        glVertex3f( 2.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -2.0f, 0.0f);
        glVertex3f(0.0f,  2.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -2.0f);
        glVertex3f(0.0f, 0.0f,  2.0f);
        glEnd();
        glLineWidth(1.0f);
    }

    // compute ray origin from parameters
    Point3 pAt( intersectionUI->m_dXAt->value(), intersectionUI->m_dYAt->value(), intersectionUI->m_dZAt->value() );

    // compute ray direction from parameters
    Vector3 dir;
    dir[0] = cos(intersectionUI->getPhi()) * cos(intersectionUI->getTheta());
    dir[1] = sin(intersectionUI->getPhi());
    dir[2] = cos(intersectionUI->getPhi()) * sin(intersectionUI->getTheta());

    const Point3 pE1 = pAt - dir * 2.0;
    const Point3 pE2 = pAt + dir * 2.0;
    
    if (intersectionUI->m_bRay->value()) {
        glPointSize(6.0f);
        glLineWidth(3.0f);
        glColor3f(0.5f, 0.5f, 0.0f);
        glBegin(GL_POINTS);
        glVertex3dv( &pE1[0]);
        glEnd();
        glColor3f(0.5f, 0.0f, 0.5f);
        glBegin(GL_LINES);
        glVertex3dv( &pE1[0]);
        glVertex3dv( &pE2[0]);
        glEnd();
        glLineWidth(1.0f);
        glPointSize(1.0f);
    }
    if (intersectionUI->m_bRayShadow->value()) {
        glPointSize(6.0f);
        glLineWidth(2.0f);
        glColor3f(0.1f, 0.1f, 0.0f);
        glBegin(GL_POINTS);
        glVertex3d( pE1[0], 0.0, pE1[2] );
        glEnd();
        glColor3f(0.0f, 0.0f, 0.0f);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xF0F0);
        glBegin(GL_LINES);
        glVertex3d( pE1[0], 0.0, pE1[2] );
        glVertex3d( pE2[0], 0.0, pE2[2] );
        glVertex3d( pE1[0], 0.0, pE1[2] );
        glVertex3dv( &pE1[0]);
        glVertex3d( pE2[0], 0.0, pE2[2] );
        glVertex3dv( &pE2[0]);
        glEnd();
        glLineWidth(1.0f);
        glPointSize(1.0f);
        glDisable(GL_LINE_STIPPLE);
    }
    glEnable(GL_LIGHTING);

    glEnable( GL_BLEND );
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const float colPlane[4] = {0.5, 0.5, 0.75, 0.5};
    const float colObj[4] = {0.5, 0.25, 0.25, 0.5};
    glColor4fv( colPlane );
    glMaterialfv(GL_FRONT, GL_DIFFUSE , colPlane);

    glBegin( GL_POLYGON );
    glVertex3f( -2.0, 0.0, -2.0 );
    glVertex3f(  2.0, 0.0, -2.0 );
    glVertex3f(  2.0, 0.0,  2.0 );
    glVertex3f( -2.0, 0.0,  2.0 );
    glEnd();

    glBegin( GL_POLYGON );
    glVertex3f( -2.0, 0.0001f, -2.0 );
    glVertex3f( -2.0, 0.0001f,  2.0 );
    glVertex3f(  2.0, 0.0001f,  2.0 );
    glVertex3f(  2.0, 0.0001f, -2.0 );
    glEnd();

    
    glColor4fv( colObj );
    glMaterialfv(GL_FRONT, GL_DIFFUSE , colObj);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colObj);

    // ToDo: draw your shape here and perform the intersection
    // then call drawHits so you can see where the ray has hit the shape
    // the origin is in variable 'p' and direction in variable 'dir'
	
	
	if (current != 0){
		if( torus == 0)	{
			current->draw();
			HitRecord hr = current->intersect(pE1, dir);
			drawHits(hr);
		}
		else {
		Torus(5,5);
		//HitRecord hr = current->intersect(pE1, dir);
		//drawHits(hr);
		}
		
	
	}
	
	

    endDrawing();
}

void IntersectionUI::drawHits(HitRecord& hr) {
    double t, u, v;
    Point3 p;
    Vector3 n;
    glDisable(GL_LIGHTING);
    while (hr.getFirstHit(t, u, v, p, n)) {
        glPointSize(8.0f);
        glLineWidth(6.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex3d(p[0], p[1], p[2]);
        glEnd();
        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_LINES);
        glVertex3d(p[0], p[1], p[2]);
        glVertex3d(p[0] + 0.5 * n[0], p[1] + 0.5 * n[1], p[2] + 0.5 * n[2]);
        glEnd();
        glLineWidth(1.0f);
        glPointSize(1.0f);
        hr.removeFirstHit();
    }
}

void IntersectionUI::changeShape( ShapesUI::ShapeType type )
{
    // ToDo: Change which shape
    switch ( type ) {
	case ShapesUI::SHAPE_SPHERE : {current = new Sphere(5);torus = 0;} break;
	case ShapesUI::SHAPE_CYLINDER : {current = new Cylinder(20, 1);torus = 0;} break;
	case ShapesUI::SHAPE_CONE : {current = new Cone(20,1);torus = 0;} break;
	case ShapesUI::SHAPE_CUBE : {current = new Cube(10);torus = 0;} break;
	case ShapesUI::SHAPE_TORUS : {current = new Torus(10,10); torus = 1;} break;
    }
}


int IntersectionUI::handle(int event) {
    return 0;
}


void IntersectionUI::writeTest() const {
    // creates a deterministic sequence of ray positions and directions
    // and writes the resulting intersections to a file
    // you must add the proper intersect calls for this file to be generated
    
	Cylinder cylinder(1,1);
	Sphere sphere(1);
	Cube cube(5);
	Cone cone(1,1);

    double invBase[5] = {1.0 / 2.0, 1.0 / 3.0, 1.0 / 5.0, 1.0 / 7.0, 1.0 / 11.0};
    double values[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
    std::ofstream file("../intersections.txt");
    file.precision(4);

    const int seed = static_cast<int>(intersectionUI->m_iSeed->value());
    // generate a halton sequence to pick position/ray combinations
    // skip the first 'seed' values
    for (int i = 0; i < seed; i++) {
        for (int j = 0; j < 5; j++) {
            double r = 1.0 - values[j] - 1e-10;
            if (invBase[j] < r)
                values[j] += invBase[j];
            else {
                double hh;
                double h = invBase[j];
                do {
                    hh = h;
                    h *= invBase[j];
                } while (h >= r);
                values[j] += ((hh + h) - 1.0);
            }
        }
    }
    for (int i = seed; i < (seed + 1638); i++) {
        for (int j = 0; j < 5; j++) {
            double r = 1.0 - values[j] - 1e-10;
            if (invBase[j] < r)
                values[j] += invBase[j];
            else {
                double hh;
                double h = invBase[j];
                do {
                    hh = h;
                    h *= invBase[j];
                } while (h >= r);
                values[j] += ((hh + h) - 1.0);
            }
        }
        // create the ray from the five random values
        // compute ray origin
        Point3 p;
        p[0] = values[4] * sin(values[0] * M_PI) * cos(values[1] * 2.0 * M_PI);
        p[1] = values[4] * sin(values[0] * M_PI) * sin(values[1] * 2.0 * M_PI);
        p[2] = values[4] * cos(values[0] * M_PI);
        // compute ray direction
        Vector3 dir;
        dir[0] = sin(values[2] * M_PI) * cos(values[3] * 2.0 * M_PI);
        dir[1] = sin(values[2] * M_PI) * sin(values[3] * 2.0 * M_PI);
        dir[2] = cos(values[2] * M_PI);
        
        HitRecord cubeHr, cylinderHr, coneHr, sphereHr;
        // ToDo: intersect with your shapes here and store the result
        // in the appropriate hit record
		cubeHr =  cube.intersect(p, dir);
        cylinderHr = cylinder.intersect(p, dir);
        coneHr = cone.intersect(p, dir);
        sphereHr = sphere.intersect(p, dir);

        // write out
        file << i << " Cube     " << cubeHr     << std::endl;
        file << i << " Cylinder " << cylinderHr << std::endl;
        file << i << " Cone     " << coneHr     << std::endl;
        file << i << " Sphere   " << sphereHr   << std::endl;
    }
    file.close();
}

