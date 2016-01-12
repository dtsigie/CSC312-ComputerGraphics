#include "UIInterface.h"
#include <FL/Fl.H>
#include <GL/glu.h>
#include <FL/Fl_Widget.H>


UIInterface::UIInterface( )
{
}

UIInterface::~UIInterface()
{
}

void UIInterface::setup2DDrawing( const Color &colBackground, int width, int height )
{
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    glClearColor( colBackground[0], colBackground[1], colBackground[2], 1.0f);

    // No light needed for 2D drawing
    glDisable( GL_LIGHT0 );

    // Set up camera and viewport for 2D images
    // Ensures that glVertex2d(x,y) will go to x,y on the screen
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH );

    // Clear the frame buffer (color is set to whatever glClearColor (DrawRoutines::setBackground) was set to
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void UIInterface::setup3DDrawing( const Color &colBackground, int width, int height, bool bOneLight )
{
    //Save attributes
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    glClearColor( colBackground[0], colBackground[1], colBackground[2], 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    // Normalizes normals
    glEnable(GL_NORMALIZE);

    // Setup a basic projective camera
    // Will be overwritten by UIs that have cameras
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat) width /(GLfloat) height, 0.1, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if ( bOneLight == true ) {
        // Make this light white
        GLfloat colWhite[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat colGrey[4]  = {0.2f, 0.2f, 0.2f, 1.0f};
        // Last number zero forces this to be a directional light
        GLfloat pos[4] = {0.1f, 0.1f, 1.0f, 0.0f}; 

        // Set light color
        glLightfv(GL_LIGHT0, GL_DIFFUSE       , colWhite);
        glLightfv(GL_LIGHT0, GL_SPECULAR      , colWhite);
        glLightfv(GL_LIGHT0, GL_AMBIENT       , colGrey);
        // Set light direction
        glLightfv(GL_LIGHT0, GL_POSITION      , pos  );

        //Enable light
        glEnable(GL_LIGHT0);
    }

    // Clear the frame buffer (color is set to whatever glClearColor (DrawRoutines::setBackground) was set to
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void UIInterface::endDrawing()
{
    // Return to previous attribute state
    glPopAttrib();
}
