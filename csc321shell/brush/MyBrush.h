#ifndef _MY_BRUSH_H_
#define _MY_BRUSH_H_

#include "csc321.h"
#include "ScreenPoint.h"
#include "UIInterface.h"
#include <vector>

class BrushInterface;
class MyBrush : public UIInterface {
public:
    enum ToolType {
        TOOL_BRUSH = 0,
        TOOL_LINE,
        TOOL_CIRCLE,
        TOOL_POLYGON,
        TOOL_FILTER
    };

    enum FilterType {
        FILTER_BLUR = 0,
        FILTER_SHARPEN,
        FILTER_EDGE
    };
    
    enum BrushType {
        BRUSH_CONSTANT = 0,
        BRUSH_LINEAR,
        BRUSH_QUADRATIC,
        BRUSH_GAUSSIAN,
        BRUSH_SPECIAL
    };

    MyBrush();
    ~MyBrush();

    // Link to the brush widget
    void setUI( const BrushInterface *in_ui ) { brushUI = in_ui; }

    // Inherited from UIInterface
    // Implemented in MyBrush_UI.cpp
    virtual void resize( int width, int height );
    virtual void draw();
    virtual int  handle( int event );

    void loadImage(Fl_Image* image);
    
    // Called by the UI when the brush type or radius changes
    void changedBrush();


private:
    // read a pixel from the canvas, returns (r, g, b) values in the [0,1] range
    Color getPixel(int x, int y );
    // put a pixel on the canvas, (r, g, b) are to be given in the [0,1] range
    void putPixel(int x, int y, const Color &in_col);

    // Pixel and screen data
    std::vector<unsigned char> pixelData;
    int screenWidth, screenHeight;
    int imageWidth, imageHeight;

    // For interactive drawing, set by handle
    // You do not need to set these; the interface for that is
    // already set up
    bool isMouseDown;
    ScreenPoint mouseDown, mouseDrag;
    std::vector< ScreenPoint > polygon;
    static void draw_callback( void *in_data );

    // All of the widget data can be accessed through
    // methods on this class
    const BrushInterface *brushUI;

    // You need to write these
    // Stubs are in MyBrush.cpp
    void drawBrush( );
    void drawLine( );
    void drawCircle( );
    void drawPolygon( );
    void filterRegion( );

    // declare your extra variables/methods here:
	std::vector<std::vector<float>> mask;
};

inline
Color MyBrush::getPixel( int x, int y ) {
    Color col;
    const int i = (y * imageWidth + x) * 3;
    col[0] = pixelData[i    ] / 255.0f;
    col[1] = pixelData[i + 1] / 255.0f;
    col[2] = pixelData[i + 2] / 255.0f;

    return col;
}

inline
void MyBrush::putPixel(int x, int y, const Color & col) {
    const int i = (y * imageWidth + x) * 3;
    pixelData[i    ] = (unsigned char) (col[0] * 255.0f);
    pixelData[i + 1] = (unsigned char) (col[1] * 255.0f);
    pixelData[i + 2] = (unsigned char) (col[2] * 255.0f);
}


#endif /* _MY_BRUSH_H_ */
