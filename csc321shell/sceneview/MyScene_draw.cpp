#include "../csc321.h"
#include "MyScene.h"

void MyScene::resize(int w, int h) {
    // resize the film plane to the specified width/height
    camera.setWidthHeight(w, h);
}

/// Note: your camera and screen clear, etc, will be set up by
/// SceneviewUI.cpp *before* this gets called
void MyScene::draw() {
    // render the scene using OpenGL
    if (!isLoaded) // Don't draw if loadSceneFile hasn't been called yet
        return;


    // Turn off all lights
    for ( int i = 0; i < 7; i++ )
        glDisable( GL_LIGHT0 + i );

    //  .. and reset
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, &ambientLight[0] );
    for (unsigned int i = 0; i < lights.size(); i++) {
        lights[i].setOpenGLLight( GL_LIGHT0 + i );
    }

    // TODO: draw the rest of the scene here
	masters->get("root")->draw();
}
