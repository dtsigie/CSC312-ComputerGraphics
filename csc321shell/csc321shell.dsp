# Microsoft Developer Studio Project File - Name="csc321shell" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=csc321shell - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "csc321shell.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "csc321shell.mak" CFG="csc321shell - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "csc321shell - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "csc321shell - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "csc321shell - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "d:/temp/Win32R"
# PROP Intermediate_Dir "d:/temp/Win32R"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 fltk.lib fltkgl.lib fltkimages.lib png.lib zlib.lib jpeg.lib opengl32.lib glu32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"csc321shell.exe" /libpath:"..\libs\Win32"

!ELSEIF  "$(CFG)" == "csc321shell - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "d:/temp/Win32DB"
# PROP Intermediate_Dir "d:/temp/Win32DB"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 png.lib jpeg.lib zlib.lib fltk.lib fltkgl.lib fltkimages.lib opengl32.lib glu32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"csc321shellDB.exe" /pdbtype:sept /libpath:"..\libs\Win32"

!ENDIF 

# Begin Target

# Name "csc321shell - Win32 Release"
# Name "csc321shell - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GLWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\UIInterface.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\color.h
# End Source File
# Begin Source File

SOURCE=.\csc321.h
# End Source File
# Begin Source File

SOURCE=.\GLWindow.h
# End Source File
# Begin Source File

SOURCE=.\ShellInterface.h
# End Source File
# Begin Source File

SOURCE=.\UIInterface.h
# End Source File
# Begin Source File

SOURCE=.\vecmath.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\csc321shell.ico
# End Source File
# End Group
# Begin Group "Brush"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\brush\BrushInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\brush\BrushInterface.h
# End Source File
# Begin Source File

SOURCE=.\brush\MyBrush.cpp
# End Source File
# Begin Source File

SOURCE=.\brush\MyBrush.h
# End Source File
# Begin Source File

SOURCE=.\brush\MyBrush_UI.cpp
# End Source File
# End Group
# Begin Group "Shapes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\shapes\ShapesInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\shapes\ShapesInterface.h
# End Source File
# Begin Source File

SOURCE=.\shapes\ShapesUI.cpp
# End Source File
# Begin Source File

SOURCE=.\shapes\ShapesUI.h
# End Source File
# End Group
# Begin Group "rendering"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rendering\MyScene_render.cpp
# End Source File
# Begin Source File

SOURCE=.\rendering\RenderingInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\rendering\RenderingInterface.h
# End Source File
# Begin Source File

SOURCE=.\rendering\RenderingUI.cpp
# End Source File
# Begin Source File

SOURCE=.\rendering\RenderingUI.h
# End Source File
# End Group
# Begin Group "Camera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\camera\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\camera\Camera.h
# End Source File
# Begin Source File

SOURCE=.\camera\CameraInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\camera\CameraInterface.h
# End Source File
# Begin Source File

SOURCE=.\camera\CameraUI.cpp
# End Source File
# Begin Source File

SOURCE=.\camera\CameraUI.h
# End Source File
# Begin Source File

SOURCE=.\camera\IBar.cpp
# End Source File
# Begin Source File

SOURCE=.\camera\IBar.H
# End Source File
# End Group
# Begin Group "Sceneview"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sceneview\Light.h
# End Source File
# Begin Source File

SOURCE=.\sceneview\MyScene.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneview\MyScene.h
# End Source File
# Begin Source File

SOURCE=.\sceneview\MyScene_draw.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneview\parser.h
# End Source File
# Begin Source File

SOURCE=.\sceneview\SceneviewInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneview\SceneviewInterface.h
# End Source File
# Begin Source File

SOURCE=.\sceneview\SceneviewUI.cpp
# End Source File
# Begin Source File

SOURCE=.\sceneview\SceneviewUI.h
# End Source File
# End Group
# Begin Group "Intersect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\intersection\HitRecord.h
# End Source File
# Begin Source File

SOURCE=.\intersection\IntersectionInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\intersection\IntersectionInterface.h
# End Source File
# Begin Source File

SOURCE=.\intersection\IntersectionUI.cpp
# End Source File
# Begin Source File

SOURCE=.\intersection\IntersectionUI.h
# End Source File
# End Group
# Begin Group "Interactive"

# PROP Default_Filter "*.cpp *.h"
# Begin Source File

SOURCE=.\interactive\InteractiveInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\interactive\InteractiveInterface.h
# End Source File
# Begin Source File

SOURCE=.\interactive\InteractiveUI.cpp
# End Source File
# Begin Source File

SOURCE=.\interactive\InteractiveUI.h
# End Source File
# Begin Source File

SOURCE=.\interactive\InteractiveWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\interactive\InteractiveWidget.h
# End Source File
# Begin Source File

SOURCE=.\interactive\MyScene_select.cpp
# End Source File
# End Group
# End Target
# End Project
