#include "../csc321.h"
#include "SceneviewUI.h"
#include "SceneviewInterface.h"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/gl.h>

SceneviewUI::SceneviewUI() : bIsMouseOver(false), ibar( scene.changeCamera() ) 
{
}

SceneviewUI::~SceneviewUI() {
}

void SceneviewUI::resize(int w, int h) 
{
    scene.resize(w, h);
}

/*
 * The main draw routine. This gets called whenever the window is initialized,
 * un-iconified, re-shown, etc. It is also called (eventually) whenever redraw()
 * is called (once control has returned to the processor). 
 */
void SceneviewUI::draw() {
    if ( sceneviewUI->m_bLighting->value() ) {
        setup3DDrawing( scene.getBackground(), scene.getCamera().getWidth(), scene.getCamera().getHeight(), false );
    } else {
        setup3DDrawing( Color(0,0,0), scene.getCamera().getWidth(), scene.getCamera().getHeight(), false );
    }

    // Sets up the camera and model view matrices
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd( &ibar.getCamera().getProjection()(0,0) );    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( &ibar.getCamera().getWorldToCamera()(0,0) );    


    // Toggle lighting calculations (fill the rendered triangles, or just draw lines)
    if ( sceneviewUI->m_bLighting->value() ) {
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_FILL);
    } else {
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPolygonMode(GL_FRONT, GL_LINE);
    }

    // Start drawing at the top of the tree
    scene.draw();

    if ( sceneviewUI->m_bIBar->value() )
        ibar.Draw( bIsMouseOver, sceneviewUI->m_bIBarHelp->value() ? true : false );

    endDrawing();
}

int SceneviewUI::handle(int event) {
    if ( sceneviewUI->m_bInteractive->value() && event == FL_SHORTCUT ) {
        scene.changeCamera().moveKeyboard();

        RedrawWindow();
    }
    if ( sceneviewUI->m_bIBar->value() ) {
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
    }
    return 0;
}

void SceneviewUI::readScene( const char * fname) {

    std::string filename( fname );
    // Call the scene parse routine (loadSceneFile)
    if (!scene.loadSceneFile(filename)) {
        std::string message = "Error loading scene file: \"" + filename + "\":\n" + scene.getErrorMessage();
        sceneviewUI->m_txtStatus->value(message.c_str());
    } else {
        std::string message = "Scene file: \"" + filename + "\" loaded succesfully.";
        sceneviewUI->m_txtStatus->value(message.c_str());
    }

    RedrawWindow();
}

