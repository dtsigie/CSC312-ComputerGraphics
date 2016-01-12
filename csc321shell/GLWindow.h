#ifndef _GL_WINDOW_H_
#define _GL_WINDOW_H_

#include "csc321.h"
#include "Color.h"
#include "UIInterface.h"
#include "brush/BrushInterface.h"
#include "shapes/ShapesInterface.h"
#include "camera/CameraInterface.h"
#include "sceneview/SceneviewInterface.h"
#include "intersection/IntersectionInterface.h"
#include "interactive/InteractiveInterface.h"
#include "rendering/RenderingInterface.h"
#include <FL/Fl_Image.H>
#include <FL/Fl_Gl_Window.H>

class GLWindow : public Fl_Gl_Window {
public:
    GLWindow(int x, int y, int w, int h);
    ~GLWindow();
  
    enum Labs {
        LAB_NONE = -1,
        LAB_BRUSH = 6,
        LAB_SHAPES,
        LAB_CAMERA,
        LAB_INTERSECTION,
        LAB_SCENEVIEW,
        LAB_USER_INTERFACE,
        LAB_RENDERING
    };
  
    void setState(Labs s);
    void readImage( const char * filename );
    void saveImage( const char * filename );

    Labs whichLab() const { return state; }

private:  
    // Inherited from Fl_Gl_Window
    void draw();
    void resize(int width, int height);
    int handle(int event);
  
    bool isDrawing;

    // Keep track of which lab we're using
    Labs state;
    UIInterface          * currentUI;

    BrushInterface         brushUI;
    ShapesInterface        shapesUI;
    CameraInterface        cameraUI;
    SceneviewInterface     sceneviewUI;
    IntersectionInterface  intersectionUI;
    InteractiveInterface   interactiveUI;
    RenderingInterface     renderingUI;
};

#endif /* _GL_WINDOW_H_ */
