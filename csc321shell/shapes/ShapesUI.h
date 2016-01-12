#ifndef _SHAPES_UI_H_
#define _SHAPES_UI_H_


#include "../UIInterface.h"
#include "../csc321.h"
#include <FL/Fl_Window.H>
#include "Shape.h"

class ShapesInterface;
class ShapesUI : public UIInterface {
public:
    ShapesUI();
    ~ShapesUI();

    // Inherited from UIInterface
    void resize(int width, int height);
    void draw();
    int handle(int event);
    const ShapesInterface *shapesUI;
    enum ShapeType {
        SHAPE_SPHERE,
        SHAPE_CONE,
        SHAPE_CYLINDER,
		SHAPE_CUBE,
		SHAPE_TORUS		
    };

    enum DisplayType {
        DISPLAY_WIREFRAME,
        DISPLAY_FLAT_SHADING,
        DISPLAY_SMOOTH_SHADING
    };
	ShapeType getShapeT() ;
    void changedTessel();
    void changedShape();
    
    // Link to the shapes widget
    void setUI( const ShapesInterface *in_ui ) { shapesUI = in_ui; }
	ShapeType getShapeT() const;

private:
    int width, height;
    

    // declare your variables here
	Shape* current;
};

#endif /* _SHAPES_UI_H_ */
