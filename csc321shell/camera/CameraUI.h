#ifndef _CAMERA_UI_H_
#define _CAMERA_UI_H_

#include "../csc321.h"
#include "Camera.h"
#include "IBar.h"
#include "../UIInterface.h"
#include <FL/Fl_Window.H>

class CameraInterface;
class CameraUI : public UIInterface {
public:
    CameraUI();
    ~CameraUI();

    // Inherited from UIInterface
    void resize(int width, int height);
    void draw();
    int handle(int event);

    // Link to the camera widget
    void setUI( const CameraInterface *in_ui ) { cameraUI = in_ui; }

    void setPlayPause();
    void setNextFrame();
    void print();

private:
    enum CameraSequence {
        FIXED_SEQUENCE,
        EYE_SEQUENCE,
        FOCUS_SEQUENCE,
        ZOOM_SEQUENCE,
        ROLL_SEQUENCE,
        NEAR_FAR_SEQUENCE,
        KEYBOARD,
        IBAR
    };
    
    void drawCube();
    static void animation_callback(void* data);
  
    const CameraInterface *cameraUI;

    int currentFrame;
    bool playing;

    bool bIsMouseOver;
    Camera camera;
    IBar   ibar;
    //Camera camera;
};

#endif /* _CAMERA_UI_H_ */
