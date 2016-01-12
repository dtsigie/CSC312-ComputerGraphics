#ifndef _RENDERING_UI_H_
#define _RENDERING_UI_H_

#include "../UIInterface.h"
#include "../sceneview/MyScene.h"
#include "../csc321.h"
#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Window.H>
#include <ctime>

class RenderingInterface;
class RenderingUI : public UIInterface {
public:
    RenderingUI();
    ~RenderingUI();

    // Inherited from UIInterface
    void resize(int width, int height);
    void draw();
    int handle(int event);
    
    void refresh();
    
    enum RenderType {
        RENDER_SCANLINE,
        RENDER_RAY_TRACING,
        RENDER_PATH_TRACING
    };

    // Link to the rendering widget
    void setUI( const RenderingInterface *in_ui ) { renderingUI = in_ui; }

    void readScene( const char * filename);
    void stopRendering();
    void startRendering();
    
private:
    const RenderingInterface *renderingUI;

    static void render_callback( void *in_data );
    clock_t startTime;
    bool    isRendering;

    std::vector<unsigned char> pixelsScan;
    std::vector<unsigned char> pixelsRay;
    std::vector<unsigned char> pixelsPath;

    MyScene scene;
};

#endif /* _RENDERING_UI_H_ */
