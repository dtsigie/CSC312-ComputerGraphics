// InteractiveWidget.cpp: implementation of the InteractiveWidget class.
//
//////////////////////////////////////////////////////////////////////
#include "InteractiveWidget.h"
#include "../shapes/ShapesUI.h"
#include <vector>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

inline Point3 CameraPt( const Camera &cam, const Point3 &pt )
{
    Point3 ptProj = cam.getProjection() * cam.getWorldToCamera() * pt;

    return Point3( ptProj[0], ptProj[1], 0.0 );
}
int InteractiveWidget::GetAxis( const Camera &in_camera, const Point3 &in_pt ) const
{
    int iAxis = -1;
    if ( m_typeMove == ROTATE_X ) {
        const Point3 ptOrigX = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,0.0,0) );
        const Vector3 vecMouse = unit( in_pt - ptOrigX );
        const Point3 ptOrigXYP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,0.1,0) );
        const Point3 ptOrigXYN = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,-0.1,0) );
        const Point3 ptOrigXZP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,0,0.1) );
        const Point3 ptOrigXZN = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,0,-0.1) );

        const double dLenYP = fabs( dot( unit( ptOrigXYP - ptOrigX ), vecMouse ) );
        const double dLenYN = fabs( dot( unit( ptOrigXYN - ptOrigX ), vecMouse ) );
        const double dLenZP = fabs( dot( unit( ptOrigXZP - ptOrigX ), vecMouse ) );
        const double dLenZN = fabs( dot( unit( ptOrigXZN - ptOrigX ), vecMouse ) );

        if ( dLenYP > dLenZP && dLenYP > dLenZN ) {
            iAxis = 2;
        } else if ( dLenYN > dLenZP && dLenYN > dLenZN ) {
            iAxis = 2;
        } else {
            iAxis = 1;
        }
    } else if ( m_typeMove == ROTATE_Y ) {
        const Point3 ptOrigY = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,0.0,0) );
        const Vector3 vecMouse = unit( in_pt - ptOrigY );
        const Point3 ptOrigYXP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0.1,1.5,0) );
        const Point3 ptOrigYXN = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(-0.1,1.5,0) );
        const Point3 ptOrigYZP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0,1.5,0.1) );
        const Point3 ptOrigYZN = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0,1.5,-0.1) );

        const double dLenXP = fabs( dot( unit( ptOrigYXP - ptOrigY ), vecMouse ) );
        const double dLenXN = fabs( dot( unit( ptOrigYXN - ptOrigY ), vecMouse ) );
        const double dLenZP = fabs( dot( unit( ptOrigYZP - ptOrigY ), vecMouse ) );
        const double dLenZN = fabs( dot( unit( ptOrigYZN - ptOrigY ), vecMouse ) );

        if ( dLenXP > dLenZP && dLenXP > dLenZN ) {
            iAxis = 2;
        } else if ( dLenXN > dLenZP && dLenXN > dLenZN ) {
            iAxis = 2;
        } else {
            iAxis = 0;
        }
    } else if ( m_typeMove == ROTATE_Z ) {
        const Point3 ptOrigZ = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,0.0,0) );
        const Vector3 vecMouse = unit( in_pt - ptOrigZ );
        const Point3 ptOrigZXP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0.1,0,1.5) );
        const Point3 ptOrigZXN = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(-0.1,0,1.5) );
        const Point3 ptOrigZYP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0,0.1,1.5) );
        const Point3 ptOrigZYN = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0,-0.1,1.5) );

        const double dLenYP = fabs( dot( unit( ptOrigZYP - ptOrigZ ), vecMouse ) );
        const double dLenYN = fabs( dot( unit( ptOrigZYN - ptOrigZ ), vecMouse ) );
        const double dLenXP = fabs( dot( unit( ptOrigZXP - ptOrigZ ), vecMouse ) );
        const double dLenXN = fabs( dot( unit( ptOrigZXN - ptOrigZ ), vecMouse ) );

        if ( dLenXP > dLenYP && dLenXP > dLenYN ) {
            iAxis = 1;
        } else if ( dLenXN > dLenYP && dLenXN > dLenYN ) {
            iAxis = 1;
        } else {
            iAxis = 0;
        }
    }
    return iAxis;
}

double InteractiveWidget::GetSign(  const Camera &in_camera, const Point3 &in_pt ) const
{
    const Vector3 vecMouse = in_pt - m_ptLast;

    if ( m_typeMove == ROTATE_X ) {
        const Point3 ptOrigX = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,0,0) );
        if ( m_iAxis == 2 ) {
            const Point3 ptOrigXYP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,0.1,0) );
            if ( dot( ptOrigXYP - ptOrigX, vecMouse ) > 0.0 )
                return -1.0;
        } else {
            const Point3 ptOrigXZP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(1.5,0,0.1) );
            if ( dot( ptOrigXZP - ptOrigX, vecMouse ) < 0.0 )
                return -1.0;
        }
    } else if ( m_typeMove == ROTATE_Y ) {
        const Point3 ptOrigY = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0,1.5,0) );
        if ( m_iAxis == 2 ) {
            const Point3 ptOrigYXP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0.1,1.5,0) );
            if ( dot( ptOrigYXP - ptOrigY, vecMouse ) < 0.0 )
                return -1.0;
        } else {
            const Point3 ptOrigYZP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0,1.5,0.1) );
            if ( dot( ptOrigYZP - ptOrigY, vecMouse ) > 0.0 )
                return -1.0;
        }
    } else if ( m_typeMove == ROTATE_Z ) {
        const Point3 ptOrigZ = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0,0,1.5) );
        if ( m_iAxis == 0 ) {
            const Point3 ptOrigZYP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0,0.1,1.5) );
            if ( dot( ptOrigZYP - ptOrigZ, vecMouse ) < 0.0 )
                return -1.0;
        } else {
            const Point3 ptOrigZXP = CameraPt( in_camera, m_matTrans * m_matRot * m_matScale * Point3(0.1,0,1.5) );
            if ( dot( ptOrigZXP - ptOrigZ, vecMouse ) > 0.0 )
                return -1.0;
        }
    }
    return 1.0;
}

InteractiveWidget::TransformType InteractiveWidget::GetType( const Camera &in_camera, const Point3 &in_pt ) const
{
    const Matrix4 mat = m_matTrans * m_matRot * m_matScale;
    const Point3 ptCenter = CameraPt( in_camera, mat * Point3(0,0,0) );
    const Point3 ptTransX = CameraPt( in_camera, mat * Point3(1,0,0) );
    const Point3 ptTransY = CameraPt( in_camera, mat * Point3(0,1,0) );
    const Point3 ptTransZ = CameraPt( in_camera, mat * Point3(0,0,1) );
    const Point3 ptRotX = CameraPt( in_camera, mat * Point3(1.5,0,0) );
    const Point3 ptRotY = CameraPt( in_camera, mat * Point3(0,1.5,0) );
    const Point3 ptRotZ = CameraPt( in_camera, mat * Point3(0,0,1.5) );
    const Point3 ptScale  = CameraPt( in_camera, mat * Point3(-0.5,0.0,-0.5) );
    const Point3 ptScaleX = CameraPt( in_camera, mat * Point3(0.5,0,0) );
    const Point3 ptScaleY = CameraPt( in_camera, mat * Point3(0,0.5,0) );
    const Point3 ptScaleZ = CameraPt( in_camera, mat * Point3(0,0,0.5) );

    std::vector<double> adDist( NONE );
    std::vector<TransformType> aiType( NONE );

    aiType[ TRANS ] = TRANS;
    aiType[ TRANS_X ] = TRANS_X;
    aiType[ TRANS_Y ] = TRANS_Y;
    aiType[ TRANS_Z ] = TRANS_Z;
    aiType[ ROTATE_X ] = ROTATE_X;
    aiType[ ROTATE_Y ] = ROTATE_Y;
    aiType[ ROTATE_Z ] = ROTATE_Z;
    aiType[ SCALE ] = SCALE;
    aiType[ SCALE_X ] = SCALE_X;
    aiType[ SCALE_Y ] = SCALE_Y;
    aiType[ SCALE_Z ] = SCALE_Z;


    adDist[ TRANS ] = length( ptCenter - in_pt );
    adDist[ TRANS_X ] = length( ptTransX - in_pt );
    adDist[ TRANS_Y ] = length( ptTransY - in_pt );
    adDist[ TRANS_Z ] = length( ptTransZ - in_pt );
    adDist[ ROTATE_X ] = length( ptRotX - in_pt );
    adDist[ ROTATE_Y ] = length( ptRotY - in_pt );
    adDist[ ROTATE_Z ] = length( ptRotZ - in_pt );
    adDist[ SCALE ] = length( ptScale - in_pt );
    adDist[ SCALE_X ] = length( ptScaleX - in_pt );
    adDist[ SCALE_Y ] = length( ptScaleY - in_pt );
    adDist[ SCALE_Z ] = length( ptScaleZ - in_pt );

    int iMin = 0;
    for ( size_t i = 0; i < adDist.size(); i++ ) {
        if ( adDist[iMin] > adDist[i] )
            iMin = i;
    }

    if ( adDist[iMin] < 0.1 )
        return aiType[iMin];

    if ( adDist[iMin] < 0.2 && aiType[iMin] >= ROTATE_X && aiType[iMin] <= ROTATE_Z )
        return aiType[iMin];

    return NONE;
}

Point3 FlTkToCamera( const Camera &in_camera, const ScreenPoint &in_ptScreen )
{
    return Point3( -1.0 + 2.0 * (in_ptScreen[0]) / (double) in_camera.getWidth(),
                   -1.0 + 2.0 * (in_camera.getHeight() - in_ptScreen[1] - 1) / (double) in_camera.getHeight(),
                   0.0 );
}

void InteractiveWidget::MouseDown( const Camera &in_camera, const ScreenPoint &in_pt )
{
    m_typeTransform = GetType( in_camera, FlTkToCamera( in_camera, in_pt ) );
    m_typeMove = m_typeTransform;

    m_ptLast = m_ptDown = FlTkToCamera( in_camera, in_pt );
    m_iAxis = GetAxis( in_camera, m_ptDown );
}

bool InteractiveWidget::MouseMove( const Camera &in_camera, const ScreenPoint &in_pt )
{
    const TransformType typeMove = GetType( in_camera, FlTkToCamera( in_camera, in_pt ) );
    const int iAxis = GetAxis( in_camera, FlTkToCamera( in_camera, in_pt ) );
    if ( typeMove != m_typeMove || iAxis != m_iAxis ) {
        m_typeMove = typeMove;
        m_iAxis = iAxis;
        return true;
    }
    return false;
}

Matrix4 InteractiveWidget::MouseDrag( const Camera &in_camera, const ScreenPoint &in_pt )
{
    const Point3 ptIn = FlTkToCamera( in_camera, in_pt );

    Matrix4 matNew;
    matNew = Matrix4::identity();
    switch( m_typeTransform ) {
        // ToDo: Set matNew for every case; this is what is returned
        /// Also need to set one of m_matTrans/Rot/ScaleDelta, 
    case TRANS :
        break;
    case TRANS_X :
        break;
    case TRANS_Y :
        break;
    case TRANS_Z :
        break;
    case ROTATE_X :
    case ROTATE_Y :
    case ROTATE_Z :
        break;
    case SCALE :
        break;
    case SCALE_X :
        break;
    case SCALE_Y :
        break;
    case SCALE_Z :
        break;
    case NONE :
        break;

    default : assert(false); break;
    }

    m_ptLast = ptIn;

    return m_matTrans * m_matRot * matNew * m_matRot.transpose() * Matrix4::translation( Vector3( m_matTrans(0,3), m_matTrans(1,3), m_matTrans(2,3) ) * -1.0 );
}

void InteractiveWidget::MouseRelease( const Camera &in_camera, const ScreenPoint &in_pt )
{
    m_matTrans = m_matTrans * m_matTransDelta;
    m_matTransDelta = Matrix4::identity();
    m_matRot = m_matRot * m_matRotDelta;
    m_matRotDelta = Matrix4::identity();
    const double dScale = ( m_matScale(0,0) * m_matScaleDelta(0,0) +
                            m_matScale(1,1) * m_matScaleDelta(1,1) +
                            m_matScale(2,2) * m_matScaleDelta(2,2) ) / 3.0;
    m_matScale = Matrix4::scaling( dScale, dScale, dScale );
    m_matScaleDelta = Matrix4::identity();

    m_typeTransform = NONE;
    m_iAxis = -1;
}

Point3 InteractiveWidget::Center() const
{
    return Point3( m_matTrans(0,3) + m_matTransDelta(0,3), 
                 m_matTrans(1,3) + m_matTransDelta(1,3),
                 m_matTrans(2,3) + m_matTransDelta(2,3) );
}

void InteractiveWidget::SetSelectColor( const TransformType in_type, const int in_iAxis ) const
{
    const float colYellow[4] = {1,1,0,1};
    const float colGreen[4] = {0,1,0,1};
    const float colBlue[4] = {0,0,1,1};
    const float colRed[4] = {1,0,0,1};
    const float colGrey[4] = {0.5f,0.5f,0.5f,1};
    if ( in_type == m_typeMove && in_iAxis == m_iAxis ) {
        glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colYellow );
    } else {
        switch( in_type ) {
        case TRANS : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colGrey ); break;
        case TRANS_X : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colRed ); break;
        case TRANS_Y : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colGreen ); break;
        case TRANS_Z : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colBlue ); break;
        case ROTATE_X : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, &Color(1.0f, 0.3f, 0.3f)[0] ); break;
        case ROTATE_Y : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, &Color(0.3f, 1.0f, 0.3f)[0] ); break;
        case ROTATE_Z : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, &Color(0.3f, 0.3f, 1.0f)[0] ); break;
        case SCALE : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colGrey ); break;
        case SCALE_X : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, &Color(1.0f, 0.5f, 0.5f)[0] ); break;
        case SCALE_Y : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, &Color(0.5f, 1.0f, 0.5f)[0] ); break;
        case SCALE_Z : glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, &Color(0.5f, 0.5f, 1.0f)[0] ); break;
        case NONE :
            break;

        default : assert(false); break;
        }
    }
}

inline void DrawArrow( const double  in_dHeight, 
                       const double  in_dRad, 
                       const int     in_iDivsTheta )
{
    // ToDo: use your cylinder and cone class to draw an arrow
    glPushMatrix();
    glScaled( in_dRad * 0.4, in_dHeight, in_dRad * 0.4 );
    glTranslated( 0, 0.5, 0.0);
    //cyl.draw();
    glPopMatrix();
    glPushMatrix();
    glTranslated( 0, in_dHeight, 0 );
    glScaled( in_dRad, in_dHeight * 0.2, in_dRad );
    glTranslated( 0, 0.5, 0.0);
    //cone.draw();
    glPopMatrix();

}

inline void DrawSceptre( const double  in_dHeight, 
                         const double  in_dRad, 
                         const int     in_iDivsTheta,
                         const int     in_iDivsPhi )
{
    // ToDo: use your cylinder and sphere class to draw a sceptre

    glPushMatrix();
    glScaled( in_dRad * 0.4, in_dHeight, in_dRad * 0.4 );
    glTranslated( 0, 0.5, 0.0);
    //cyl.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslated( 0, in_dHeight, 0 );
    glScaled( in_dRad, in_dRad, in_dRad );
    glTranslated( 0, 0.5, 0.0);
    //sphere.draw();
    glPopMatrix();

}
inline void DrawCylinderOpen( const Point3 &in_ptCenter, const double in_dHeight, const double in_dRad, const int in_iDivsTheta )
{
    glPushMatrix();

    glTranslated( in_ptCenter[0], in_ptCenter[1], in_ptCenter[2] );
    glScaled( in_dRad, in_dHeight, in_dRad );

    const int iThetaDiv = max( in_iDivsTheta, 4 );
    const double dThetaDiv = 2.0 * M_PI / (double) iThetaDiv;

    glBegin( GL_TRIANGLE_STRIP );
    for (int iT = 0; iT < iThetaDiv + 1; iT++ ) {
        glNormal3d( cos( -iT * dThetaDiv ), 1.0, sin( -iT * dThetaDiv ) );
        glVertex3d( cos( -iT * dThetaDiv ), 1.0, sin( -iT * dThetaDiv ) );
        glNormal3d( cos( -iT * dThetaDiv ), 0.0, sin( -iT * dThetaDiv ) );
        glVertex3d( cos( -iT * dThetaDiv ), 0.0, sin( -iT * dThetaDiv ) );
    }
    glEnd( ); //GL_TRIANGLE_STRIP

    const double dScl = 0.95;
    glPushMatrix();
    glScaled( dScl, 1.0, dScl );
    glBegin( GL_TRIANGLE_STRIP );
    for (int iT = 0; iT < iThetaDiv + 1; iT++ ) {
        glNormal3d( cos( iT * dThetaDiv ), 1.0, sin( iT * dThetaDiv ) );
        glVertex3d( cos( iT * dThetaDiv ), 1.0, sin( iT * dThetaDiv ) );
        glNormal3d( cos( iT * dThetaDiv ), 0.0, sin( iT * dThetaDiv ) );
        glVertex3d( cos( iT * dThetaDiv ), 0.0, sin( iT * dThetaDiv ) );
    }
    glEnd( ); //GL_TRIANGLE_STRIP
    glPopMatrix();

    glBegin( GL_TRIANGLE_STRIP );
    for (int iT = 0; iT < iThetaDiv + 1; iT++ ) {
        glNormal3d( cos( iT * dThetaDiv ), 0.0, sin( iT * dThetaDiv ) );
        glVertex3d( cos( iT * dThetaDiv ) * dScl, 0.0, sin( iT * dThetaDiv ) * dScl );
        glNormal3d( cos( iT * dThetaDiv ), 0.0, sin( iT * dThetaDiv ) );
        glVertex3d( cos( iT * dThetaDiv ), 0.0, sin( iT * dThetaDiv ) );
    }
    glEnd( ); //GL_TRIANGLE_STRIP

    glBegin( GL_TRIANGLE_STRIP );
    for (int iT = 0; iT < iThetaDiv + 1; iT++ ) {
        glNormal3d( cos( -iT * dThetaDiv ), 1.0, sin( -iT * dThetaDiv ) );
        glVertex3d( cos( -iT * dThetaDiv ) * dScl, 1.0, sin( -iT * dThetaDiv ) * dScl );
        glNormal3d( cos( -iT * dThetaDiv ), 1.0, sin( -iT * dThetaDiv ) );
        glVertex3d( cos( -iT * dThetaDiv ), 1.0, sin( -iT * dThetaDiv ) );
    }
    glEnd( ); //GL_TRIANGLE_STRIP

    glPopMatrix();
}


void InteractiveWidget::DrawWidget() const
{

    glPushAttrib( GL_LIGHTING_BIT | GL_ENABLE_BIT );
    glEnable( GL_LIGHTING );

    glPushMatrix();

    glMultMatrixd( &m_matTrans(0,0) );
    glMultMatrixd( &m_matTransDelta(0,0) );
    glMultMatrixd( &m_matRot(0,0) );
    glMultMatrixd( &m_matRotDelta(0,0) );
    glMultMatrixd( &m_matScale(0,0) );
    glMultMatrixd( &m_matScaleDelta(0,0) );

    // ToDo: use sphere class to draw widget center
    //static Sphere sphere(2);
    SetSelectColor( TRANS );
    glPushMatrix();
    glScaled(0.1, 0.1, 0.1);
    //sphere.draw();
    glPopMatrix();

    if ( m_typeTransform == NONE || m_typeTransform == TRANS_X ) {
        SetSelectColor( TRANS_X );
        glPushMatrix();
        glRotated( -90, 0,0,1 );
        DrawArrow( 1.0, 0.075, 10 );
        glPopMatrix();
    }

    if ( m_typeTransform == NONE || m_typeTransform == TRANS_Y ) {
        SetSelectColor( TRANS_Y );
        DrawArrow( 1.0, 0.075, 10 );
    }

    if ( m_typeTransform == NONE || m_typeTransform == TRANS_Z ) {
        SetSelectColor( TRANS_Z );
        glPushMatrix();
        glRotated( 90, 1,0,0 );
        DrawArrow( 1.0, 0.075, 10 );
        glPopMatrix();
    }

    if ( m_typeTransform == NONE || m_typeTransform == SCALE_X ) {
        SetSelectColor( SCALE_X );
        glPushMatrix();
        glRotated( -90, 0,0,1 );
        DrawSceptre( 0.5, 0.1, 10, 4 );
        glPopMatrix();
    }

    if ( m_typeTransform == NONE || m_typeTransform == SCALE_Y ) {
        SetSelectColor( SCALE_Y );
        DrawSceptre( 0.5, 0.1, 10, 4 );
    }

    if ( m_typeTransform == NONE || m_typeTransform == SCALE_Z ) {
        SetSelectColor( SCALE_Z );
        glPushMatrix();
        glRotated( 90, 1,0,0 );
        DrawSceptre( 0.5, 0.1, 10, 4 );
        glPopMatrix();
    }

    SetSelectColor( ROTATE_X );
    glPushMatrix();
    glRotated( -90, 0,0,1 );
    DrawSceptre(1.5, 0.05, 10, 4 );
    if ( m_typeMove == ROTATE_X ) {
        glRotated( 90, 0,0,1 );
        SetSelectColor( ROTATE_X, 1 );
        DrawCylinderOpen( Point3(0,-0.1,0), 0.2, 1.5, 40 );
        glRotated( 90, 1,0,0 );
        SetSelectColor( ROTATE_X, 2 );
        DrawCylinderOpen( Point3(0,-0.1,0), 0.2, 1.5, 40 );
    }
    glPopMatrix();

    glPushMatrix();
    SetSelectColor( ROTATE_Y );
    DrawSceptre( 1.5, 0.05, 10, 4 );
    if ( m_typeMove == ROTATE_Y ) {
        glRotated( 90, 0,0,1 );
        SetSelectColor( ROTATE_Y, 0 );
        DrawCylinderOpen( Point3(0,-0.1,0), 0.2, 1.5, 40 );
        glRotated( 90, 1,0,0 );
        SetSelectColor( ROTATE_Y, 2 );
        DrawCylinderOpen( Point3(0,-0.1,0), 0.2, 1.5, 40 );
    }
    glPopMatrix();

    SetSelectColor( ROTATE_Z );
    glPushMatrix();
    glRotated( 90, 1,0,0 );
    DrawSceptre( 1.5, 0.05, 10, 4 );
    if ( m_typeMove == ROTATE_Z ) {
        glRotated( 90, 0,0,1 );
        SetSelectColor( ROTATE_Z, 0 );
        DrawCylinderOpen( Point3(0,-0.1,0), 0.2, 1.5, 40 );
        glRotated( 90, 1,0,0 );
        SetSelectColor( ROTATE_Z, 1 );
        DrawCylinderOpen( Point3(0,-0.1,0), 0.2, 1.5, 40 );
    }
    glPopMatrix();    

    if ( m_typeTransform == NONE || m_typeTransform == SCALE ) {
        SetSelectColor( SCALE );
        glPushMatrix();
        glRotated( 90, 0,0,1 );
        glRotated( -45, 1,0,0 );
        DrawSceptre( sqrt(2.0) * 0.5, 0.075, 10, 4 );
        glPopMatrix();
    }

    glPopMatrix();

    glPopAttrib();
}

void InteractiveWidget::SetPosition( const Point3 &in_pt ) 
{ 
    m_matTrans = Matrix4::translation( in_pt - Point3(0,0,0) ); 
}

void InteractiveWidget::SetAxis( const Vector3 &in_vecAxis, const Vector3 &in_vecUp ) 
{ 
    const Vector3 vecAxis = unit( in_vecAxis );
    const Vector3 vecUp = unit( in_vecUp - vecAxis * dot( in_vecUp, vecAxis ) );

    m_matRot = Matrix4( vecAxis, vecUp, cross( vecAxis, vecUp) ).transpose();
    Vector3 vecCheck = m_matRot * Vector3( 1,0,0);
    Vector3 vecCheck2 = m_matRot * Vector3( 0,1,0);
    assert( vecCheck2.approxEqual( vecUp, 1e-12 ) );
    assert( vecCheck.approxEqual( vecAxis, 1e-12 ) );
}

void InteractiveWidget::SetScale( const double in_d ) 
{
    if ( fabs( in_d ) > 1e-6 )
        m_matScale = Matrix4::scaling( in_d, in_d, in_d );
}

InteractiveWidget::InteractiveWidget()
: m_typeTransform( NONE ),
  m_typeMove( NONE )
{
    m_matRot = Matrix4::identity();
    m_matTrans = Matrix4::identity();
    m_matScale = Matrix4::identity();
    m_matTransDelta = Matrix4::identity();
    m_matRotDelta = Matrix4::identity();
    m_matScaleDelta = Matrix4::identity();
}

InteractiveWidget::~InteractiveWidget()
{

}

