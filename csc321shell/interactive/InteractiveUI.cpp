#include "../csc321.h"
#include "InteractiveUI.h"
#include "InteractiveInterface.h"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/gl.h>

InteractiveUI::InteractiveUI() :
ibar( scene.changeCamera() )
{
    widget.SetPosition( Point3(0,0,0) );
    widget.SetScale( 0.5 );
}

InteractiveUI::~InteractiveUI() {
}

void InteractiveUI::resize(int w, int h) {
    scene.resize(w, h);
}

void InteractiveUI::draw() {
    setup3DDrawing( scene.getBackground(), scene.getCamera().getWidth(), scene.getCamera().getHeight(), false );

    // Sets up the camera and model view matrices
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd( &ibar.getCamera().getProjection()(0,0) );    
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( &ibar.getCamera().getWorldToCamera()(0,0) );    

    glPushAttrib( GL_ALL_ATTRIB_BITS );
    // Start drawing at the top of the tree
    scene.draw();
    glPopAttrib();

    if ( interactiveUI->m_bWidget->value() ) {
        glEnable( GL_LIGHT0 );
        widget.DrawWidget();
    }

    if ( interactiveUI->m_bIBar->value() )
        ibar.Draw( bIsMouseOverIBar, interactiveUI->m_bIBarHelp->value() ? true : false );

    endDrawing();
}

int InteractiveUI::handle(int event) {
    static bool s_bDoingCamera = false;
    bool bRedraw = false;
    const bool bRightButton = Fl::event_state( FL_SHIFT ) || Fl::event_button3();

    switch (event) {
        case FL_SHORTCUT: {
            if ( interactiveUI->m_bInteractive->value() ) {
                scene.changeCamera().moveKeyboard();
                RedrawWindow();
            }
            break;
        } return 1;
        case FL_PUSH: {
            if ( bRightButton == false && interactiveUI->m_bIBar->value() ) {
                s_bDoingCamera = ibar.MouseDown( ScreenPoint( Fl::event_x(), Fl::event_y() ), Fl::event_state(FL_SHIFT) ? true : false );
            } else {
                s_bDoingCamera = false;
            }
            if ( s_bDoingCamera == false && bRightButton == false && interactiveUI->m_bWidget->value() ) {
                widget.MouseDown( scene.getCamera(), ScreenPoint( Fl::event_x(), Fl::event_y() ) );
            }
        } return 1;
        case FL_DRAG: {
            if ( s_bDoingCamera == true ) {
                ibar.MouseDrag( ScreenPoint( Fl::event_x(), Fl::event_y() ), Fl::event_state(FL_SHIFT) ? true : false );
            } else if ( s_bDoingCamera == false && bRightButton == false && interactiveUI->m_bWidget->value() ) {
                const Matrix4 mat = widget.MouseDrag( scene.getCamera(), ScreenPoint( Fl::event_x(), Fl::event_y() ) );
                if ( interactiveUI->m_bWidgetMove->value()) {
                    scene.moveSelectedNode( mat );
                }
            }
            RedrawWindow();
              } return 1;
        case FL_MOVE: 
            if ( interactiveUI->m_bIBar->value() ) 
                bIsMouseOverIBar = ibar.MouseMove( ScreenPoint( Fl::event_x(), Fl::event_y() ), bRedraw );
            if ( s_bDoingCamera == false && bRightButton == false && interactiveUI->m_bWidget->value() ) 
                if ( widget.MouseMove( scene.getCamera(), ScreenPoint( Fl::event_x(), Fl::event_y() ) ) )
                    bRedraw = true;

            if ( bRedraw == true )
                RedrawWindow();
            return 1;
        case FL_RELEASE: 
            if ( s_bDoingCamera == true && interactiveUI->m_bIBar->value() ) 
                ibar.MouseRelease( ScreenPoint( Fl::event_x(), Fl::event_y() ), Fl::event_state(FL_SHIFT) ? true : false );
            if ( s_bDoingCamera == false && bRightButton == false && interactiveUI->m_bWidget->value() ) 
                widget.MouseRelease( scene.getCamera(), ScreenPoint( Fl::event_x(), Fl::event_y() ) );

            if ( bRightButton == true ) {
                // ToDo: select object and orient widget
            }

            s_bDoingCamera = false;
            RedrawWindow();
            return 1;
    }
    return 0;
}

void InteractiveUI::readScene(const char *fname) {
    std::string filename(fname);
    if (!scene.loadSceneFile(filename)) {
        std::string message = "Error loading scene file: \"" + filename + "\":\n" + scene.getErrorMessage();
        interactiveUI->m_txtStatus->value(message.c_str());
    } else {
        std::string message = "Scene file: \"" + filename + "\" loaded succesfully.";
        interactiveUI->m_txtStatus->value(message.c_str());
    }
    RedrawWindow();
}

