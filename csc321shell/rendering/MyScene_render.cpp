#include "../csc321.h"
#include "../sceneview/MyScene.h"
#include "RenderingInterface.h"
#include <FL/gl.h>
#include <cfloat>
#define RECURSIVE_LIMIT 5

void MyScene::render(int type, int width, int height, unsigned char* pixels) {
    if (!isLoaded) {
		progress = 1.0;
        return;
    }
	bDoRender = true;
    // Add your rendering code here.
    // Keep track of your progress as a value between 0 and 1
    // so the progress bar can update as the rendering progresses
	progress = 0.0;
    switch (type) {
        case RenderingUI::RENDER_SCANLINE:  scanline(width, height, pixels); break;
        case RenderingUI::RENDER_RAY_TRACING:  raytrace(width, height, pixels); break;
        case RenderingUI::RENDER_PATH_TRACING:  break;
        default: break;
    }
	progress = 1.0;
}

void MyScene::stopRender()
{
    // Because this is threaded code, this function
    // can be called in the middle of your rendering code.
    // You should then stop at the next scanline
	bDoRender = false;
}

double MyScene::getRenderProgress() {
    // return the current progress as a value between 0 and 1
    return progress;
}

// add extra methods here
void MyScene::scanline(int w, int h, unsigned char* pixels) {
	resize(w, h);

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glClearColor( background[0], background[1], background[2], 1.0f);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd( &getCamera().getProjection()(0,0) );
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd( &getCamera().getWorldToCamera()(0,0) );

	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT, GL_FILL);

	draw();

	glPopAttrib();
	glReadPixels( 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0] );
	progress = 1.0;
}

void MyScene::raytrace(int w, int h, unsigned char* pixels) {
	resize(w, h);

	for (int y=0; y < h; y++) {
		if (bDoRender == false)
			break;

		for (int x = 0; x < w; x++) {
			// determine the color of the pixel (x,y) by raytracing

			// form the ray
			Point3 pixel (x, y, -1.0);
			pixel[0] = -1.0 + 2.0 * pixel[0] / (camera.getWidth() - 1);
			pixel[1] = -1.0 + 2.0 * pixel[1] / (camera.getHeight() - 1);
			pixel = camera.getCameraToWorld() * pixel;
			Vector3 dir = pixel - camera.getEye();
			Point3 o = camera.getEye();

			// trace the ray
			Color c = traceRay(o, dir,RECURSIVE_LIMIT);

			// clamp and store the color value
			c[0] = (c[0] > 0.0) ? ((c[0] < 1.0) ? c[0] : 1.0) : 0.0;
			c[1] = (c[1] > 0.0) ? ((c[1] < 1.0) ? c[1] : 1.0) : 0.0;
			c[2] = (c[2] > 0.0) ? ((c[2] < 1.0) ? c[2] : 1.0) : 0.0;
			*pixels++ = (unsigned char) (c[0] * 255.0);
			*pixels++ = (unsigned char) (c[1] * 255.0);
			*pixels++ = (unsigned char) (c[2] * 255.0);
		}

		progress = (double) y / (double) h;
		Fl::check();
	}

	progress = 1.0;
}

Color MyScene::traceRay(Point3& o, Vector3& dir, int  limit) {
	FirstHitRecord fhr = masters->get("root")->intersect(o, dir);
	if (!fhr.hit())
		return background;

	Color c;
	//ambientLight = Ia, ambient = Ka
	Color colAmbient = fhr.node->object->ambient * ambientLight;
	
	fhr.n.normalize();
	Vector3 nWorld = fhr.n;
	Point3 pWorld =  fhr.p;
	Color colSpec;
	Color colDiffuse;
	for (int i = 0; i < lights.size(); i++) {
		
		Vector3 l;  // light vector pointing from point to source
		l = lights[i].getPos() - pWorld; // vector from light source to intersection point
		Vector3 lightDist = l;
		//atttenuation values;
		double c0 = lights[i].getFalloff()[0];
		double c1 = lights[i].getFalloff()[1];
		double c2 = lights[i].getFalloff()[2];
		double fatt = 1/ ( c0 + c1*l.length() + c2*l.length()*l.length());
			
		l.normalize();
		double d = nWorld * l;
		
		// check if in front( d = N.L)
		if ( d <= 0)
			continue;
		
		// reflection ray
		Vector3 V = nWorld*(l*nWorld);
		Vector3 R = -l + V*2;
		R.normalize();
		// diffuse 
		
		colDiffuse = fhr.node->object->diffuse * d;
		
		colSpec = fhr.node->object->specular*pow((R*V),fhr.node->object->shine);
	
		// shadow check
		FirstHitRecord inShadow = masters->get("root")->intersect(pWorld, l);
		if (inShadow.hit()){
			colSpec = Color(0,0,0);
			colDiffuse = Color(0,0,0);
		}
	
	

		
		// refraction ray
	/*	Vector3 Refr = 
		Color colRefract = Color(0,0,0);
		if( limit>0){
			Color c3 = Color(0,0,0);
			c3 = traceRay(pWorld,Refr,limit-1);
			colRefract = c3 * fhr.node->object->ior;
		
		}*/
		
		// total color
		//c += colAmbient  + colReflect + (colDiffuse + colSpec) * lights[i].getColor()* fatt;
		c +=   (colDiffuse + colSpec) * lights[i].getColor()* fatt;
		
	}
	// reflection ray
		Color colReflect = Color(0,0,0);
		if( limit>0){
			Vector3 R;
			R = dir;
			R = 2*(R*nWorld) * nWorld - R;
			R.normalize();
			Color c3 = Color(0,0,0);
			c3 = traceRay(pWorld+(R*0.000001),R,limit-1);
			colReflect = c3 * fhr.node->object->reflect;
			
		}
		c += colAmbient +  colReflect ; 

	return c;
}

