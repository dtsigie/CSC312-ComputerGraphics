// generated by Fast Light User Interface Designer (fluid) version 1.0107

#include "RenderingInterface.h"

void RenderingInterface::cb_Load_i(Fl_Button*, void*) {
  const char *strBack = fl_file_chooser("Scene name?", "*.sc", strSceneName.c_str() );

if ( strBack ) {
    renderingUI.readScene( strBack );
    strSceneName = strBack;
}
RedrawWindow();
}
void RenderingInterface::cb_Load(Fl_Button* o, void* v) {
  ((RenderingInterface*)(o->parent()->user_data()))->cb_Load_i(o,v);
}

void RenderingInterface::cb_Scanline_i(Fl_Menu_*, void*) {
  RedrawWindow();
}
void RenderingInterface::cb_Scanline(Fl_Menu_* o, void* v) {
  ((RenderingInterface*)(o->parent()->user_data()))->cb_Scanline_i(o,v);
}

void RenderingInterface::cb_Ray_i(Fl_Menu_*, void*) {
  RedrawWindow();
}
void RenderingInterface::cb_Ray(Fl_Menu_* o, void* v) {
  ((RenderingInterface*)(o->parent()->user_data()))->cb_Ray_i(o,v);
}

void RenderingInterface::cb_Path_i(Fl_Menu_*, void*) {
  RedrawWindow();
}
void RenderingInterface::cb_Path(Fl_Menu_* o, void* v) {
  ((RenderingInterface*)(o->parent()->user_data()))->cb_Path_i(o,v);
}

Fl_Menu_Item RenderingInterface::menu_m_iRenderType[] = {
 {"Scanline", 0,  (Fl_Callback*)RenderingInterface::cb_Scanline, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Ray trace", 0,  (Fl_Callback*)RenderingInterface::cb_Ray, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Path trace", 0,  (Fl_Callback*)RenderingInterface::cb_Path, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

void RenderingInterface::cb_m_bRenderOn_i(Fl_Button*, void*) {
  if ( m_bRenderOn->value() )
   renderingUI.startRendering();
else
   renderingUI.stopRendering();
}
void RenderingInterface::cb_m_bRenderOn(Fl_Button* o, void* v) {
  ((RenderingInterface*)(o->parent()->user_data()))->cb_m_bRenderOn_i(o,v);
}

Fl_Double_Window* RenderingInterface::make_window() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = m_renderWindow = new Fl_Double_Window(310, 250, "Rendering UI");
    w = o;
    o->user_data((void*)(this));
    { Fl_Button* o = new Fl_Button(5, 5, 100, 25, "Load scene");
      o->callback((Fl_Callback*)cb_Load);
    }
    { Fl_Output* o = m_txtStatus = new Fl_Output(5, 80, 300, 100, "Status");
      o->align(FL_ALIGN_TOP_LEFT);
    }
    { Fl_Choice* o = m_iRenderType = new Fl_Choice(140, 25, 135, 25, "Render Type");
      o->down_box(FL_BORDER_BOX);
      o->align(FL_ALIGN_TOP_LEFT);
      o->menu(menu_m_iRenderType);
    }
    { Fl_Value_Input* o = m_iWidth = new Fl_Value_Input(40, 185, 40, 25, "Width");
      o->minimum(10);
      o->maximum(1024);
      o->step(1);
      o->value(256);
    }
    { Fl_Value_Input* o = m_iHeight = new Fl_Value_Input(125, 185, 40, 25, "Height");
      o->minimum(10);
      o->maximum(1024);
      o->step(1);
      o->value(256);
    }
    { Fl_Progress* o = m_progress = new Fl_Progress(5, 215, 300, 30, "Progress");
      o->selection_color(FL_DARK_GREEN);
    }
    m_iTime = new Fl_Value_Output(215, 185, 90, 25, "Time");
    { Fl_Button* o = m_bRenderOn = new Fl_Button(5, 35, 100, 25, "Start rendering");
      o->type(1);
      o->callback((Fl_Callback*)cb_m_bRenderOn);
    }
    o->end();
  }
  return w;
}

RenderingInterface::RenderingInterface() {
  m_renderWindow = make_window();
m_progress->minimum(0);
m_progress->maximum(1);
strSceneName = "../data/scenes/bujans_raquel.sc";
renderingUI.setUI( this );
}

UIInterface * RenderingInterface::getUI() {
  return &renderingUI;
}
