// OGLTransformWidget.h: interface for the OGLTransformWidget class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _INTERACTIVEINTERFACE_H
#define _INTERACTIVEINTERFACE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InteractiveWidget.h"
#include "../UIInterface.h"
#include "../sceneview/MyScene.h"
#include "../camera/IBar.h"

class InteractiveInterface;
class InteractiveUI : public UIInterface  
{
public:
	InteractiveUI();
	virtual ~InteractiveUI();

    // Inherited from UIInterface
    void resize(int width, int height);
    void draw();
    int handle(int event);
    Fl_Color fontColor() const { return (scene.getBackground()[0]  > 0.5) ? FL_BLACK : FL_WHITE; }

    // Link to the interactive widget
    void setUI( const InteractiveInterface *in_ui ) { interactiveUI = in_ui; }

    void readScene( const char * filename);

private:
    const InteractiveInterface *interactiveUI;

    // Your scene
    MyScene scene;

    // These should appear after scene so camera is init'd
    bool bIsMouseOverIBar;
    IBar ibar;

    InteractiveWidget widget;
};

#endif // !defined(AFX_OGLTRANSFORMWIDGET_H__754FCF56_AF21_48B1_B149_DB7C54D3C56C__INCLUDED_)
