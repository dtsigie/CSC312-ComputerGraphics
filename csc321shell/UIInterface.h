#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include "csc321.h"
#include "Color.h"
#include <FL/Fl_Widget.H>

class UIInterface {
public:
    UIInterface( );
    virtual ~UIInterface();

    void setup2DDrawing( const Color &colBackground, int width, int height );
    void setup3DDrawing( const Color &colBackground, int width, int height, bool bOneLight );
    void endDrawing();

    // These get called when something happens in the drawing window,
    // not in the lab's widget window
    virtual void resize(int width, int height) = 0;
    virtual void draw() = 0;
    virtual int handle(int event) = 0;

    virtual Fl_Color fontColor() const { return FL_BLACK; }
};

#endif /* _USER_INTERFACE_H_ */
