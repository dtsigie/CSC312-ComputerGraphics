#include "../csc321.h"
#include "CameraUI.h"
#include "CameraInterface.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/gl.h>
#include <iostream>
#include "Matrix3.h"

CameraUI::CameraUI( ) : ibar( camera ) {
    playing = false;
    currentFrame = 0;
    bIsMouseOver = false;

    camera.setFrom(Point3(3.0, 2.0, 6.0));
    camera.setAt(Point3(0.0, 0.0, 0.0));
    camera.setUp(Vector3(0.0, 1.0, 0.0));
    camera.setZoom(90.0);
    camera.setNearFar(0.1, 30.0);
}

CameraUI::~CameraUI() {
}

void CameraUI::resize(int width, int height) {
    camera.setWidthHeight(width, height);
}

void CameraUI::draw() {

    setup3DDrawing( Color( 1,1,1 ), camera.getWidth(), camera.getHeight(), true );

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd( &ibar.getCamera().getProjection()(0,0) );    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( &ibar.getCamera().getWorldToCamera()(0,0) );    

    // draw grid
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= 10; i++) {
        float s = -5.0f + i;
        glVertex3f(s, 0.0f, -5.0f);
        glVertex3f(s, 0.0f,  5.0f);
        glVertex3f(-5.0f, 0.0f, s);
        glVertex3f( 5.0f, 0.0f, s);
    }
    glEnd();
    // draw world space axes
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-5.0f, 0.0f, 0.0f);
    glVertex3f( 5.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -5.0f, 0.0f);
    glVertex3f(0.0f,  5.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -5.0f);
    glVertex3f(0.0f, 0.0f,  5.0f);
    glEnd();
    glLineWidth(1.0f);
    // draw cubes
    glEnable(GL_LIGHTING);
    for ( int iI = 3; iI < 8; iI += 2) {
        float s = -5.0f + iI;
        for (int j = 3; j < 8; j += 2) {
          float t = -5.0f + j;
          glPushMatrix();
          glTranslatef(s, 0.0f, t);
          drawCube();
          glPopMatrix();
        }
    }
    if ( (CameraSequence) cameraUI->m_iSequence->value() == IBAR )
        ibar.Draw( bIsMouseOver, true );

    endDrawing();
}

void CameraUI::drawCube() {
    // draw cube
    glBegin(GL_QUADS);
    glNormal3f( 0.0f,  0.0f,  1.0f);
	glVertex3f(-0.5f, -0.5f,  0.5f);
	glVertex3f( 0.5f, -0.5f,  0.5f);
	glVertex3f( 0.5f,  0.5f,  0.5f);
	glVertex3f(-0.5f,  0.5f,  0.5f);
	glNormal3f( 0.0f,  0.0f, -1.0f);	
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f,  0.5f, -0.5f);
	glVertex3f( 0.5f,  0.5f, -0.5f);
	glVertex3f( 0.5f, -0.5f, -0.5f);
	glNormal3f( 0.0f,  1.0f,  0.0f);	
	glVertex3f(-0.5f,  0.5f, -0.5f);
	glVertex3f(-0.5f,  0.5f,  0.5f);
	glVertex3f( 0.5f,  0.5f,  0.5f);
	glVertex3f( 0.5f,  0.5f, -0.5f);
	glNormal3f( 0.0f, -1.0f,  0.0f);	
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f( 0.5f, -0.5f, -0.5f);
	glVertex3f( 0.5f, -0.5f,  0.5f);
	glVertex3f(-0.5f, -0.5f,  0.5f);
	glNormal3f( 1.0f,  0.0f,  0.0f);	
	glVertex3f( 0.5f, -0.5f, -0.5f);
	glVertex3f( 0.5f,  0.5f, -0.5f);
	glVertex3f( 0.5f,  0.5f,  0.5f);
	glVertex3f( 0.5f, -0.5f,  0.5f);
	glNormal3f(-1.0f,  0.0f,  0.0f);	
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f,  0.5f);
	glVertex3f(-0.5f,  0.5f,  0.5f);
	glVertex3f(-0.5f,  0.5f, -0.5f);
	glEnd();
}

int CameraUI::handle(int event) {
    if ( (CameraSequence) cameraUI->m_iSequence->value() == IBAR ) {
        bool bRedraw = false;
        static bool s_bDoingCamera = false;

        switch (event) {
            case FL_PUSH: 
                s_bDoingCamera = ibar.MouseDown( ScreenPoint( Fl::event_x(), Fl::event_y() ), Fl::event_state(FL_SHIFT) ? true : false );
                return 1;

            case FL_DRAG: 
                if ( s_bDoingCamera == true ) {
                    ibar.MouseDrag( ScreenPoint( Fl::event_x(), Fl::event_y() ), Fl::event_state(FL_SHIFT) ? true : false );
                    RedrawWindow();
                }
                return 1;

            case FL_MOVE: 
                bIsMouseOver = ibar.MouseMove( ScreenPoint( Fl::event_x(), Fl::event_y() ), bRedraw );
                if ( bRedraw == true )
                    RedrawWindow();
                return 1;

            case FL_RELEASE: {
                ibar.MouseRelease( ScreenPoint( Fl::event_x(), Fl::event_y() ), Fl::event_state(FL_SHIFT) ? true : false );
                RedrawWindow();
                return 1;
            }
            default: return 0;
        }
    } else if ( (CameraSequence) cameraUI->m_iSequence->value() == KEYBOARD ) {
        if (event == FL_SHORTCUT ) {
            camera.moveKeyboard();
            RedrawWindow();
            return 1;
        }
    }
    return 0;
}

void CameraUI::animation_callback(void* data) {
    CameraUI* cui = static_cast<CameraUI *>(data);
    switch ( (CameraUI::CameraSequence) cui->cameraUI->m_iSequence->value() ) {
        case CameraUI::FIXED_SEQUENCE: {
            cui->currentFrame = 0;
            cui->camera.setFrom(Point3(3.0, 2.0, 6.0));
            cui->camera.setAt(Point3(0.0, 0.0, 0.0));
            cui->camera.setUp(Vector3(0.0, 1.0, 0.0));
            cui->camera.setZoom(90.0);
            cui->camera.setProjectionCenter( Point3(0,0,0) );
            cui->camera.setSkew(0.0);
            cui->camera.setAspectRatioScale(1.0);
            cui->camera.setNearFar(0.01, 100000.0);
        } break;
        case CameraUI::EYE_SEQUENCE: {
            cui->currentFrame++;
            cui->currentFrame %= 250;
            double a = (M_PI * cui->currentFrame) / 125.0;
            cui->camera.setFrom(Point3(6.0 * cos(a), sin(4.0 * a), 6.0 * sin(a)));
            cui->camera.setAt(Point3(0.0, 0.0, 0.0));
            cui->camera.setUp(Vector3(0.0, 1.0, 0.0));
			 cui->camera.setZoom(90.0);
            cui->camera.setProjectionCenter( Point3(0,0,0) );
            cui->camera.setSkew(0.0);
            cui->camera.setAspectRatioScale(1.0);
            cui->camera.setNearFar(0.01, 10000.0);
        } break;
        case CameraUI::FOCUS_SEQUENCE: {
            cui->currentFrame++;
            cui->currentFrame %= 250;
            double a = (M_PI * cui->currentFrame) / 125.0;
            cui->camera.setFrom(Point3(0.0, 2.0, 0.0));
            cui->camera.setAt(Point3(3.5 * cos(a), 0.0, 3.5 * sin(a)));
            cui->camera.setUp(Vector3(0.0, 1.0, 0.0));
            cui->camera.setZoom(90.0);
            cui->camera.setProjectionCenter( Point3(0,0,0) );
            cui->camera.setSkew(0.0);
            cui->camera.setAspectRatioScale(1.0);
            cui->camera.setNearFar(0.01, 30.0);
        } break;
        case CameraUI::ZOOM_SEQUENCE: {
            cui->currentFrame++;
            cui->currentFrame %= 250;
            double x = (double) cui->currentFrame / 250.0;
            cui->camera.setFrom(Point3(3.0, 2.0, 6.0));
            cui->camera.setAt(Point3(0.0, 0.0, 0.0));
            cui->camera.setUp(Vector3(0.0, 1.0, 0.0));
            cui->camera.setZoom(4.0 * 115.0 * x * (1 - x) + 5.0);
			std::cout<<4.0 * 115.0 * x * (1 - x) + 5.0<<std::endl;
            cui->camera.setProjectionCenter( Point3(0,0,0) );
            cui->camera.setSkew(0.0);
            cui->camera.setAspectRatioScale(1.0);
            cui->camera.setNearFar(0.1, 30.0);
        } break;
        case CameraUI::ROLL_SEQUENCE: {
            cui->currentFrame++;
            cui->currentFrame %= 250;
            double a = (M_PI * cui->currentFrame) / 125.0;
            cui->camera.setFrom(Point3(3.0, 2.0, 6.0));
            cui->camera.setAt(Point3(0.0, 0.0, 0.0));
            Vector3 up(0.0, 1.0, 0.0);
            cui->camera.setUp(Matrix4::zrotation(a) * up);
            cui->camera.setZoom(90.0);
            cui->camera.setProjectionCenter( Point3(0,0,0) );
            cui->camera.setSkew(0.0);
            cui->camera.setAspectRatioScale(1.0);
            cui->camera.setNearFar(0.1, 30.0);
        } break;
        case CameraUI::NEAR_FAR_SEQUENCE: {
            cui->currentFrame++;
            cui->currentFrame %= 250;
            double x = (double) cui->currentFrame / 250.0;
            cui->camera.setFrom(Point3(3.0, 2.0, 6.0));
            cui->camera.setAt(Point3(0.0, 0.0, 0.0));
            cui->camera.setUp(Vector3(0.0, 1.0, 0.0));
            cui->camera.setZoom(90.0);
            cui->camera.setProjectionCenter( Point3(0,0,0) );
            cui->camera.setSkew(0.0);
            cui->camera.setAspectRatioScale(1.0);
            if (x < 0.5) {
                x /= 0.5;
                cui->camera.setNearFar(4.0 * 14.4 * x * (1 - x) + 0.5, 15.0);
            } else {
                x = (x - 0.5) / 0.5;
                cui->camera.setNearFar(0.5,  15.0 - (4.0 * 14.4 * x * (1 - x)));
            }
        } break;
        case CameraUI::KEYBOARD: {
            cui->currentFrame = 0;
            cui->camera.moveKeyboard();
        } break;
        default: break;
    }
    if (cui->playing)
        Fl::repeat_timeout(0.02, CameraUI::animation_callback, data);

    RedrawWindow();
}

void CameraUI::setPlayPause()
{
    if ( playing ) {
        playing = false;
    } else {
        playing = true;
        Fl::add_timeout(0, animation_callback, this);
    }
}

void CameraUI::setNextFrame()
{
    animation_callback( this );
}

void CameraUI::print( ) {
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Frame # " << currentFrame << " of sequence " << cameraUI->m_iSequence->value() << ":" << std::endl;
    std::cout << "Projection matrix" << std::endl;
    std::cout << camera.getProjection() << std::endl;
    std::cout << "World to Camera" << std::endl;
    std::cout << camera.getWorldToCamera() << std::endl;
	std::cout << "RotationFromXYZ" << std::endl;
	std::cout << camera.getRotationFromXYZ() << std::endl;
	std::cout << "RotationToXYZ" << std::endl;
//	std::cout << camera.getRotationToXYZ() << std::endl;
	std::cout << "aspectRatioScale" << std::endl;
	std::cout << camera.getAspectRatioScale() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl << std::endl;

}

