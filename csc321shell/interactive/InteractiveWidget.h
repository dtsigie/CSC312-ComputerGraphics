// OGLTransformWidget.h: interface for the OGLTransformWidget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGLTRANSFORMWIDGET_H__754FCF56_AF21_48B1_B149_DB7C54D3C56C__INCLUDED_)
#define AFX_OGLTRANSFORMWIDGET_H__754FCF56_AF21_48B1_B149_DB7C54D3C56C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "camera/Camera.h"
#include "ScreenPoint.h"

class InteractiveWidget 
{
public:
	InteractiveWidget();
	virtual ~InteractiveWidget();

    ///
    const Point3 GetCenter() const { return Center(); }
    ///
    double GetWidth() const { return length( m_matScale * Vector3(0,1,0) ); }
    ///
    const Vector3 GetNorm() const { return m_matRot * Vector3(0,1,0); }
    ///
    const Matrix4 GetMatrix() const { return m_matTrans * m_matRot * m_matScale; }

    bool IsSelected() const { return m_typeTransform == NONE ? false : true; }

    void MouseDown( const Camera &in_camera, const ScreenPoint &in_pt );
    bool MouseMove( const Camera &in_camera, const ScreenPoint &in_pt );
    Matrix4 MouseDrag( const Camera &in_camera, const ScreenPoint &in_pt );
    void MouseRelease( const Camera &in_camera, const ScreenPoint &in_pt );

    void DrawWidget() const;

    void SetPosition( const Point3 &in_pt );
    void SetAxis( const Vector3 &in_vecAxis, const Vector3 &in_vecUp ) ;
    void SetScale( const double in_d ) ;

private:
    Matrix4 m_matRot, m_matTrans, m_matScale;
    Matrix4 m_matRotDelta, m_matTransDelta, m_matScaleDelta;

    Point3  m_ptDown, m_ptLast;
    int     m_iAxis;

    typedef enum {
        TRANS, 
        TRANS_X, 
        TRANS_Y, 
        TRANS_Z, 
        ROTATE_X,
        ROTATE_Y,
        ROTATE_Z,
        SCALE,
        SCALE_X,
        SCALE_Y,
        SCALE_Z,
        NONE
    } TransformType;

    TransformType m_typeTransform, m_typeMove;
    void SetSelectColor( const TransformType in_type, const int in_iAxis = -1 ) const;

    int GetAxis( const Camera &in_camera, const Point3 &in_pt ) const;
    double GetSign(  const Camera &in_camera, const Point3 &in_pt ) const;

    Point3 Center() const;
    TransformType GetType( const Camera &in_camera, const Point3 &in_pt ) const;

};

#endif // !defined(AFX_OGLTRANSFORMWIDGET_H__754FCF56_AF21_48B1_B149_DB7C54D3C56C__INCLUDED_)
