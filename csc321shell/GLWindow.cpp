#include "csc321.h"
#include "GLWindow.h"
#include <FL/fl_ask.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <string>

GLWindow::GLWindow(int x, int y, int w, int h) : Fl_Gl_Window(x, y, w, h) {
//    mode(FL_RGB8 | FL_DOUBLE | FL_DEPTH);
    state = LAB_NONE;
    currentUI = 0;
    isDrawing = false;

    resize( w, h );
}

GLWindow::~GLWindow() {
}

void GLWindow::setState(Labs s) {
    if (state == s)
        return;

    brushUI.m_brushWindow->hide();
    shapesUI.m_shapesWindow->hide();
    cameraUI.m_cameraWindow->hide();
    sceneviewUI.m_sceneviewWindow->hide();
    intersectionUI.m_intersectionWindow->hide();
    interactiveUI.m_interactiveWindow->hide();
    renderingUI.m_renderWindow->hide();

    state = s;

    currentUI = 0;
    switch (state) {
        case LAB_BRUSH:          brushUI.m_brushWindow->show();               currentUI = brushUI.getUI();         break;
        case LAB_SHAPES:         shapesUI.m_shapesWindow->show();             currentUI = shapesUI.getUI();        break;
        case LAB_CAMERA:         cameraUI.m_cameraWindow->show();             currentUI = cameraUI.getUI();        break;
        case LAB_INTERSECTION:   intersectionUI.m_intersectionWindow->show(); currentUI = intersectionUI.getUI();  break;
        case LAB_SCENEVIEW:      sceneviewUI.m_sceneviewWindow->show();       currentUI = sceneviewUI.getUI();     break;
        case LAB_USER_INTERFACE: interactiveUI.m_interactiveWindow->show();   currentUI = interactiveUI.getUI();   break;
        case LAB_RENDERING:      renderingUI.m_renderWindow->show();          currentUI = renderingUI.getUI();     break;
        default: break;
    }
    RedrawWindow();
}

void GLWindow::readImage( const char * fname ) {
    if (state != LAB_BRUSH) {
        fl_alert("Must be in brush mode to open image");
        return;
    }
    std::string filename(fname);
    std::string ext(filename.substr(filename.length() - 4, 4));
    Fl_Image* image;
    if (ext == ".bmp")
        image = new Fl_BMP_Image(filename.c_str());
    else if (ext == ".jpg")
        image = new Fl_JPEG_Image(filename.c_str());
    else if (ext == ".png")
        image = new Fl_PNG_Image(filename.c_str());
    else {
        fl_alert("Not a valid image file!");
        return;
    }

    brushUI.loadImage(image);
    delete image;
}

void GLWindow::saveImage( const char *filename ) {
    // static int screenshotNum = 0;
    // grab pixels
    make_current();
    int width = w();
    int height = h();
    unsigned char* pixelData = new unsigned char[width * height * 3];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    // get filename
    // write bitmap file
    // (quick hack because fltk doesn't have support for saving images)
    int infoSize = 40;
    int offset = 14 + infoSize;
    // make sure each line is aligned on a 4 byte boundary
    int paddedWidth = ((width * 3) + 3) & (~3);
    int scanPadding = paddedWidth - (width * 3);
    int filesize = offset + (paddedWidth * height);
    int zero = 0;  // used for all 0 fields
    short planes = 1, bitCount = 24;
    FILE* fp = fopen(filename, "wb");
    // header: 14 bytes
    fwrite("BM",      1, 2, fp); // magic header
    fwrite(&filesize, 4, 1, fp);
    fwrite(&zero,     4, 1, fp); // reserved
    fwrite(&offset,   4, 1, fp);
    // info header: 40 bytes
    fwrite(&infoSize, 4, 1, fp);
    fwrite(&width,    4, 1, fp);
    fwrite(&height,   4, 1, fp);
    fwrite(&planes,   2, 1, fp);
    fwrite(&bitCount, 2, 1, fp);
    fwrite(&zero,     4, 1, fp); // compression
    fwrite(&zero,     4, 1, fp); // compressed image size
    fwrite(&zero,     4, 1, fp); // pixel resolution
    fwrite(&zero,     4, 1, fp); // pixel resolution
    fwrite(&zero,     4, 1, fp); // number of colours
    fwrite(&zero,     4, 1, fp); // important colours
    // raster image data
    unsigned char padding = 0, tmp;
    int n = width * height * 3;
    // swap red & blue
    unsigned char* src = pixelData;
    for (int i = 0; i < n; i += 3, src += 3) {
        tmp = *src;
        *src = *(src + 2);
        *(src + 2) = tmp;
    }
    // write out raster data
    src = pixelData;
    int delta = width * 3;
    for (int y = 0; y < height; y++) {
        fwrite(src, 1, delta, fp);
        src += delta;
        for (int i = 0; i < scanPadding; i++)
            fwrite(&padding, 1, 1, fp);
    }
    fclose(fp);
    delete [] pixelData;
}

void GLWindow::resize(int w, int h)
{
    brushUI.getUI()->resize( w, h );
    shapesUI.getUI()->resize( w, h );
    cameraUI.getUI()->resize( w, h );
    sceneviewUI.getUI()->resize( w, h );
    intersectionUI.getUI()->resize( w, h );
    interactiveUI.getUI()->resize( w, h );
    renderingUI.getUI()->resize( w, h );
}

void GLWindow::draw() {
    // prevent nested calls to draw from creating infinite loops
    // Also prevents threads from creating invalid data
	// This is not atomic, so not perfect, but it should do the job for now
    if (isDrawing)
        return;
    isDrawing = true;

    // Set up fonts for drawing text
    float fColBack[4];
    glGetFloatv( GL_COLOR_CLEAR_VALUE, fColBack );

    gl_color( FL_BLACK );

    gl_font(labelfont(), labelsize());

    if (currentUI != 0) {
        gl_color( currentUI->fontColor() );

        currentUI->draw();
        
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
    }

	// We're done, so allow other calls into this method.
	isDrawing = false;
}

int GLWindow::handle(int event) {

    // Send event to appropriate UI
    const int r = (currentUI != 0) ? currentUI->handle(event) : 0;

    return r ? r : Fl_Gl_Window::handle(event);
}
