#include "csc321.h"
#include "ShellInterface.h"
#include <FL/Fl.H>
#include <string>

static GLWindow *g_saveWindow = NULL;

static void draw_callback( void *in_data )
{
    if ( g_saveWindow != NULL )
	{
        g_saveWindow->redraw();
    }
}


void RedrawWindow()
{
    if ( g_saveWindow != NULL )
	{
        g_saveWindow->redraw();
    }
}

int main (int argc, char *argv[]) {

    std::string str;

    ShellInterface shell;
    g_saveWindow = shell.glWindow;
  
    shell.m_mainWindow->show();
	Fl::add_idle(draw_callback, NULL);

    return Fl::run();
}
