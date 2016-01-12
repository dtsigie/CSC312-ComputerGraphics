#ifndef _SCENEVIEW_UI_H_
#define _SCENEVIEW_UI_H_

#include "../csc321.h"
#include "MyScene.h"
#include "../UIInterface.h"
#include "../camera/IBar.h"
#include <FL/Fl_Window.H>
#include <FL/Fl_Multiline_Output.H>

class SceneviewInterface;
class SceneviewUI : public UIInterface {
public:
    SceneviewUI();
    ~SceneviewUI();

    // Inherited from UIInterface
    void resize(int width, int height);
    void draw();
    int handle(int event);
    Fl_Color fontColor() const { return (scene.getBackground()[0]  > 0.5) ? FL_BLACK : FL_WHITE; }

    // Link to the sceneview widget
    void setUI( const SceneviewInterface *in_ui ) { sceneviewUI = in_ui; }

    void readScene( const char * filename);

private:
    const SceneviewInterface *sceneviewUI;

    // Your scene
    MyScene scene;

    // These should appear after scene so camera is init'd
    bool bIsMouseOver;
    IBar ibar;
};

#endif /* SCENEVIEW_UI_H_ */
