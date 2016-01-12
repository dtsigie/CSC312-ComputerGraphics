# Original by Chris & Zach
# Modified by Doug Hyde, 1/19/2009
# Modified by Stephen Schuh, 1/19/2011

# Include the correct .mk file for your operating system:
include linux.mk       # Linux (Fedora, Ubuntu)
#include darwin.mk      # Mac OS X

MAINOBJ = $(OBJDIR)main.o $(OBJDIR)GLWindow.o $(OBJDIR)ShellInterface.o $(OBJDIR)UIInterface.o
VECMATHOBJ = $(OBJDIR)ScreenVector.o $(OBJDIR)ScreenPoint.o $(OBJDIR)Vector3.o $(OBJDIR)Point3.o $(OBJDIR)Matrix3.o $(OBJDIR)Vector4.o $(OBJDIR)Matrix4.o
BRUSHOBJ = $(OBJDIR)MyBrush_UI.o $(OBJDIR)MyBrush.o  $(OBJDIR)BrushInterface.o 
SHAPESOBJ = $(OBJDIR)ShapesInterface.o $(OBJDIR)ShapesUI.o
CAMERAOBJ = $(OBJDIR)CameraInterface.o $(OBJDIR)CameraUI.o $(OBJDIR)IBar.o $(OBJDIR)Camera.o
SCENEOBJ = $(OBJDIR)SceneviewInterface.o $(OBJDIR)SceneviewUI.o $(OBJDIR)MyScene.o $(OBJDIR)MyScene_draw.o 
INTERSECTOBJ  = $(OBJDIR)IntersectionInterface.o $(OBJDIR)IntersectionUI.o
USEROBJ =  $(OBJDIR)InteractiveInterface.o $(OBJDIR)InteractiveUI.o $(OBJDIR)InteractiveWidget.o $(OBJDIR)MyScene_select.o 
RENDEROBJ = $(OBJDIR)MyScene_render.o $(OBJDIR)RenderingUI.o $(OBJDIR)RenderingInterface.o

OBJ = $(MAINOBJ) $(VECMATHOBJ) $(RENDEROBJ)  $(USEROBJ)  $(SCENEOBJ) $(INTERSECTOBJ) $(SHAPESOBJ) $(CAMERAOBJ) $(BRUSHOBJ)

.PHONY: all all-before all-after clean clean-custom

all: all-before cse452shell all-after


clean: clean-custom
	rm -f $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o "cse452shell" $(LIBS) $(LDFLAGS)

$(MAINOBJ): $(OBJDIR)%.o: %.cpp cse452.h
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(VECMATHOBJ): $(OBJDIR)%.o: vecmath/%.cpp 
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BRUSHOBJ): $(OBJDIR)%.o: brush/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(SHAPESOBJ): $(OBJDIR)%.o: shapes/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(CAMERAOBJ): $(OBJDIR)%.o: camera/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(SCENEOBJ): $(OBJDIR)%.o: sceneview/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(INTERSECTOBJ): $(OBJDIR)%.o: intersection/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(USEROBJ): $(OBJDIR)%.o: interactive/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(RENDEROBJ): $(OBJDIR)%.o: rendering/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
