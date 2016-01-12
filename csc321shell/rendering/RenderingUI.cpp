#include "../csc321.h"
#include "RenderingUI.h"
#include "RenderingInterface.h"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstring>
#include <ctime>
#include <sstream>

RenderingUI::RenderingUI() 
{
    isRendering = false;

    //glPixelStorei(GL_PACK_ALIGNMENT, 1);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

RenderingUI::~RenderingUI() {
}

void RenderingUI::resize(int w, int h) {
    scene.changeCamera().setWidthHeight(w,h);
}

void RenderingUI::render_callback( void *in_data )
{
    RenderingUI *opMe = static_cast< RenderingUI * >( in_data );

    const double dProgress = opMe->scene.getRenderProgress();

    const clock_t curr = clock();
    opMe->renderingUI->m_iTime->value( (double) (curr - opMe->startTime) / CLOCKS_PER_SEC );
    opMe->renderingUI->m_progress->value( static_cast<float>(dProgress) );

    if ( dProgress < 1.0 ) {
        Fl::repeat_timeout( 0.05, render_callback, in_data );
        RedrawWindow();
    } else {
        opMe->isRendering = false;
        opMe->renderingUI->m_txtStatus->value( "Ending rendering"  );
        opMe->renderingUI->m_bRenderOn->value(0);
        RedrawWindow();
    }

}

void RenderingUI::draw() {

    const unsigned int w = (unsigned int) renderingUI->m_iWidth->value();
    const unsigned int h = (unsigned int) renderingUI->m_iHeight->value();

    setup2DDrawing( Color(1,1,1) - scene.getBackground(), scene.getCamera().getWidth(), scene.getCamera().getHeight() );

    switch ( (RenderType) (renderingUI->m_iRenderType->value()) ) {
        case RENDER_SCANLINE :
            // display image
            if (pixelsScan.size() == w * h * 3 )
                glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, &pixelsScan[0]);
            break;
        case RENDER_RAY_TRACING :
            // display image
            if (pixelsRay.size() == w * h * 3 )
                glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, &pixelsRay[0]);
            break;
        case RENDER_PATH_TRACING :
            // display image
            if (pixelsPath.size() == w * h * 3 )
                glDrawPixels(w, h, GL_RGB, GL_UNSIGNED_BYTE, &pixelsPath[0]);
            break;
    }
    endDrawing();
}

int RenderingUI::handle(int event) {
    return 0;
}

void RenderingUI::readScene(const char * fname) {
    if (isRendering) {
        renderingUI->m_txtStatus->value( "Can't load file while rendering\n" );
        return;
    }

    std::string filename(fname);
    if (!scene.loadSceneFile(filename)) {
        std::string message = "Error loading scene file: \"" + filename + "\":\n" + scene.getErrorMessage();
        renderingUI->m_txtStatus->value(message.c_str());
    } else {
        std::string message = "Scene file: \"" + filename + "\" loaded succesfully.";
        renderingUI->m_txtStatus->value(message.c_str());
    }
    RedrawWindow();
}

void RenderingUI::stopRendering()
{
    scene.stopRender();
    isRendering = false;
}

void RenderingUI::startRendering()
{
    if ( isRendering == true ) {
        return;
    }

    startTime = clock();
    renderingUI->m_progress->value(0);
    renderingUI->m_iTime->value(0);

    const int w = (int) renderingUI->m_iWidth->value();
    const int h = (int) renderingUI->m_iHeight->value();
    const RenderType type = (RenderType) (renderingUI->m_iRenderType->value());

	resize(w, h);

    renderingUI->m_txtStatus->value( "Starting rendering");
    isRendering = true;

    Fl::add_timeout( 0.1, render_callback, this );

    switch ( type ) {
    case RENDER_SCANLINE :
        pixelsScan.resize( w * h * 3, 0 );
        scene.render( type, w, h, &pixelsScan[0] );
        break;
    case RENDER_RAY_TRACING :
        pixelsRay.resize( w * h * 3, 0 );
        scene.render( type, w, h, &pixelsRay[0] );
        break;
    case RENDER_PATH_TRACING :
        pixelsPath.resize( w * h * 3, 0 );
        scene.render( type, w, h, &pixelsPath[0] );
        break;
    }

}
