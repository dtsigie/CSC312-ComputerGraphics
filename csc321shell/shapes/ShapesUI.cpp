#include "../csc321.h"
#include "ShapesUI.h"
#include "ShapesInterface.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>

ShapesUI::ShapesUI() {
    width = height = 0;

    // ToDo: initialize your variables here
	current = 0;
}

ShapesUI::~ShapesUI() {
    // ToDo: delete your variables here
	if (current != 0)
		delete current;
}

ShapesUI::ShapeType ShapesUI::getShapeT(){
	return shapesUI->getShapeType();	
}

void ShapesUI::resize(int w, int h) {
    width = w;
    height = h;
}

void ShapesUI::draw() {
    // Sets up the viewport and background color
    setup3DDrawing( Color( 0,0,0 ), width, height, true );

    // Changes the way triangles are drawn
    switch ( shapesUI->getDisplayType() ) {
        case DISPLAY_WIREFRAME: {
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_LINE);
            glColor3f(1.0f, 1.0f, 1.0f);
        } break;
        case DISPLAY_FLAT_SHADING: {
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0f, 1.0f, 1.0f);
            glShadeModel(GL_FLAT);
        } break;
        case DISPLAY_SMOOTH_SHADING: {
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0f, 1.0f, 1.0f);
            glShadeModel(GL_SMOOTH);
        } break;
        default: break;
    }

    // Setup the camera
    gluLookAt( 3.5 * cos( shapesUI->getYRot() ) * cos( shapesUI->getXRot() ), 
               3.5 * sin( shapesUI->getYRot() ), 
               3.5 * cos( shapesUI->getYRot() ) * sin( shapesUI->getXRot() ), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // ToDo: draw your shape here
    // DO NOT put the actual draw OpenGL code here - put it in the shape class and call the draw method
	if (current != 0){
		if(shapesUI->getShapeType() != SHAPE_TORUS)	{
			current->draw();
		}
		else
			Torus(5,5);
		
	}

    endDrawing();
}

int ShapesUI::handle(int event) {
    return 0;
}

void ShapesUI::changedShape()
{
    // ToDo: Switch shapes
	changedTessel();
    
    RedrawWindow();
}

void ShapesUI::changedTessel( ) {
    // ToDo: tessellate your shape here
	if (current != 0)
		delete current;
    
	switch( shapesUI->getShapeType() ) {
	case SHAPE_CYLINDER: current = new Cylinder( shapesUI->getTessel1(), shapesUI->getTessel2() ); break;
	case SHAPE_CUBE: current = new Cube( shapesUI->getTessel1() ); break;
	case SHAPE_CONE: current = new Cone( shapesUI->getTessel1(), shapesUI->getTessel2() ); break;
	case SHAPE_SPHERE: current = new Sphere( shapesUI->getTessel1() ); break;
	case SHAPE_TORUS: current = new Torus( shapesUI->getTessel1(), shapesUI->getTessel2()); break;
	default:
		current = 0; break;
	}

    RedrawWindow();
}

