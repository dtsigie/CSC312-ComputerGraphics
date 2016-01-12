#include "IBar.h"

#include <FL/Fl_Gl_Window.H>

static Camera s_cam;
IBar IBar::m_cameraOrig(s_cam);

Vector3  IBar::GetCOPOffset() const
{
    const double dScl = m_dFocusDist * tan( m_cam.getZoom() / 2.0 );
    const double dXOff = 1.0 / m_cam.getAspectRatioScale() * m_cam.getProjectionCenter()[0] * dScl - m_cam.getSkew() * m_cam.getProjectionCenter()[1];
    const double dYOff = m_cam.getProjectionCenter()[1] * dScl;

    const Vector3 vecOff = m_cam.getRight() * dXOff + m_cam.getUp() * dYOff;

    return vecOff;
}



void IBar::SetFocusPoint( const Point3 &in_pt ) 
{ 
    m_dFocusDist = length( in_pt - m_cam.getEye() ); 
}


IBar &IBar::operator=( const IBar &in_ibar )
{
    m_dClickDistance = in_ibar.m_dClickDistance;
    m_handle = in_ibar.m_handle;
    m_commit = in_ibar.m_commit;
    m_bCameraMode = in_ibar.m_bCameraMode;
    m_iClosest = in_ibar.m_iClosest;
    m_ptDown = in_ibar.m_ptDown;
    m_ptDrag = in_ibar.m_ptDrag;

    m_cam = in_ibar.m_cam;
    return *this;
}

IBar::IBar( const IBar &in_ibar ) :
m_cam( in_ibar.m_cam )
{
    (*this) = in_ibar;
}

IBar::IBar( Camera &cam ) : 
m_cam( cam ),
m_dClickDistance( 0.02 ),
m_handle( NONE ),
m_commit( UNCOMMITTED ),
m_bCameraMode( false ),
m_iClosest( -1 ),
m_dCubeHeight(0.5)
{
    m_dFocusDist = length( Vector3(3.0, 2.0, 6.0) );
    m_cameraOrig = *this;
}

static double Slope( const Point3 &in_p1, const Point3 &in_p2 )
{
    return (in_p1[1] - in_p2[1]) / (in_p1[0] - in_p2[0]);
}
static double Intercept( const Point3 &in_p1, const Point3 &in_p2 )
{
    return in_p1[1] - Slope(in_p1,in_p2) * in_p1[0];
}

// Actually 2D points
static bool Intersect(const Point3 &in_p1, const Point3 &in_p2, const Point3 &in_q1, const Point3 &in_q2, Point3 &out_pt, double &out_dT) 
{
    out_pt = Point3(0,0,0);
    out_dT = 1e30;

    if ( in_p1.approxEqual( in_p2 ) ) return false;
    if ( in_q1.approxEqual( in_q2 ) ) return false;

    // parallel
    const double dDot = unit(in_p2 - in_p1) * unit(in_q2 - in_q1);
    if ( isZero( 1.0 - fabs( dDot ) ) )
        return false;
    
    // Vertical
    if ( isZero( in_p2[0] - in_p1[0] ) ) { 
        out_pt[0] = in_p1[0];
        out_pt[1] = Slope(in_q1, in_q2) * out_pt[0] + Intercept(in_q1,in_q2);
        out_dT = length( out_pt - in_p1 ) / length( in_p2 - in_p1 );
        return true;
    }
    
    // Vertical
    if ( isZero( in_q2[0] - in_q1[0] ) ) {
        out_pt[0] = in_q1[0];
        out_pt[1] = Slope(in_p1, in_p2) * out_pt[0] + Intercept(in_p1,in_p2);
        return true;
    }
    
    out_pt[0] = (Intercept(in_q1, in_q2) - Intercept(in_p1,in_p2)) / (Slope(in_p1,in_p2) - Slope(in_q1, in_q2));
    out_pt[1] = Slope(in_p1,in_p2) * out_pt[0] + Intercept(in_p1,in_p2);
    out_dT = length( out_pt - in_p1 ) / length( in_p2 - in_p1 );
    
    return true;
}

inline Point3 CameraPt( const Matrix4 &mat, const Point3 &pt )
{
    const Point3 ptRet = mat * pt;
    return Point3( ptRet[0], ptRet[1], 0.0 );
}

// Ordering of cube vertices
//  0  1  2  3  4  5  6
//        7  8  9 
//  10 11 12 13 14 15 16
std::vector<Point3> IBar::MakeCube(  double &out_dTStem ) const 
{
    double dZoom = m_cam.getZoom();
    double dFD = m_dFocusDist;
    Point3 ptCOP = m_cam.getProjectionCenter();
    Vector3 vecRight = m_cam.getRight();
    Vector3 vecUp = m_cam.getUp();
    Vector3 vecLook = m_cam.getLook();
    Point3 ptFrom = m_cam.getEye();
    Vector3 vecOffset = GetCOPOffset();

    std::vector<Point3> aptCube(17);

    out_dTStem = 0.5;

    if ( m_handle != NONE ) {
        dZoom = m_cameraOrig.m_cam.getZoom();
        dFD = m_cameraOrig.GetFocusDist();
        ptCOP = m_cameraOrig.m_cam.getProjectionCenter();
        vecRight = m_cameraOrig.m_cam.getRight();
        vecUp = m_cameraOrig.m_cam.getUp();
        vecLook = m_cameraOrig.m_cam.getLook();
        ptFrom = m_cameraOrig.m_cam.getEye();
        vecOffset = m_cameraOrig.GetCOPOffset();
    }
    
    //const double dScl = dFD * tan( dZoom / 2.0 );
    const double dSclHeight = dFD * m_dCubeHeight * tan( dZoom / 2.0 );
    const double dSclLimb   = dFD * m_dCubeHeight * tan( dZoom / 2.0 );

    // Vertical bar
    aptCube[8] = ptFrom + vecLook * dFD + vecOffset; //Point3( 0,  0.0, 0.0 );

    aptCube[3] = aptCube[8] + vecUp * dSclHeight; //Point3( 0,  dHeight,  0.0 );
    aptCube[13] = aptCube[8] - vecUp * dSclHeight; //Point3( 0,-dHeight,  0.0 );

    // limbs are broken into 3 pieces, in (+-, +-, -) direction
    const Point3 ptUL = aptCube[3]  - vecRight * dSclLimb + vecLook * dSclLimb; 
    const Point3 ptUR = aptCube[3]  + vecRight * dSclLimb + vecLook * dSclLimb; 
    const Point3 ptLL = aptCube[13] - vecRight * dSclLimb + vecLook * dSclLimb; 
    const Point3 ptLR = aptCube[13] + vecRight * dSclLimb + vecLook * dSclLimb; 
    for ( int i = 0; i < 3; i++ ) {
        const double dThird = i / 3.0;
        const double dThirdNext = (i+1.0) / 3.0;

        // top left, top right, bottom left, bottom right
        aptCube[i]      = lerp( aptCube[3],  ptUL,  1.0 - dThird );
        aptCube[4 + i]  = lerp( aptCube[3],  ptUR,  dThirdNext );
        aptCube[10 + i] = lerp( aptCube[13], ptLL,  1.0 - dThird );
        aptCube[14 + i] = lerp( aptCube[13], ptLR,  dThirdNext );
    }

    aptCube[7] = aptCube[8] - vecRight * 0.5 * dSclHeight;
    aptCube[9] = aptCube[8] + vecRight * 0.5 * dSclHeight;

    if (  aptCube[3].approxEqual( aptCube[0] ) )
        return aptCube;

    // This calculation is all done with the current camera
    const Matrix4 matProj = m_cam.getProjection() * m_cam.getWorldToCamera();
    Point3 ptIntersectL, ptIntersectR, ptClosest, ptCross;

    // Intersect left and right arms of cube
    Intersect( CameraPt( matProj, aptCube[3] ), CameraPt( matProj, aptCube[0] ), CameraPt( matProj, aptCube[13] ), CameraPt( matProj, aptCube[10] ), ptIntersectL, out_dTStem );
    Intersect( CameraPt( matProj, aptCube[3] ), CameraPt( matProj, aptCube[6] ), CameraPt( matProj, aptCube[13] ), CameraPt( matProj, aptCube[16] ), ptIntersectR, out_dTStem );

    // Intersect the resulting horizontal bar with the stem
    Intersect( CameraPt( matProj, aptCube[3] ), CameraPt( matProj, aptCube[13] ), ptIntersectL, ptIntersectR, ptCross, out_dTStem );

    // Horizontal bar

    aptCube[7] = lerp( aptCube[3], aptCube[13], out_dTStem ) - vecRight * 0.5 * dSclHeight;
    aptCube[9] = lerp( aptCube[3], aptCube[13], out_dTStem ) + vecRight * 0.5 * dSclHeight;

    return aptCube;
}



const Camera &IBar::getCamera() const
{
    if ( m_handle == NONE ) {
        return m_cam;
    } else {
        if ( m_bCameraMode == true )
            return m_cameraOrig.m_cam;
        else
            return m_cam;
    }
}

static void OGLBegin2D( )
{
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib( GL_LIGHTING );
    glDisable( GL_LIGHTING );
}

void OGLEnd2D( )
{
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    glPopAttrib( );
}

void OGLDrawCircle( const Point3 &in_pt, const double in_dR )
{
    const double dDiv = 2.0 * M_PI / 10.0;

    glPushMatrix();
    glTranslated( in_pt[0], in_pt[1], 0 );

    glBegin( GL_LINE_LOOP );
    for ( double dTheta = 0.0; dTheta < 2.0 * M_PI; dTheta += dDiv ) {
        glVertex2d( in_dR * cos( dTheta ), in_dR * sin( dTheta ) );
    }
    glEnd();

    glPopMatrix();

}

///
void IBar::Draw( const bool in_bMouseOver, const bool in_help ) const
{
    double dTStem = 0.5;
    std::vector<Point3> aptCube = MakeCube(dTStem);

    float fColBack[4];
    glGetFloatv( GL_COLOR_CLEAR_VALUE, fColBack );

    fColBack[0] = 1.0f - fColBack[0];
    fColBack[1] = 1.0f - fColBack[1];
    fColBack[2] = 1.0f - fColBack[2];

    glPushAttrib( GL_ALL_ATTRIB_BITS );

    ::glMatrixMode( GL_PROJECTION );
    ::glPushMatrix();
    ::glMatrixMode( GL_MODELVIEW );
    ::glPushMatrix();
    
    ::glDisable( GL_LIGHTING );
    ::glDisable( GL_DEPTH_TEST );
    ::glEnable( GL_LINE_SMOOTH );

    const Matrix4 matProj = m_cam.getProjection() * m_cam.getWorldToCamera();
    OGLBegin2D();
    if (in_help) {
        Point3 p;
        gl_draw("Object-Centric = OC", (float) ((matProj * aptCube[ 3])[0] - 0.2), (float) ((matProj * aptCube[ 3])[1] + 0.2)); 
        gl_draw("Camera-Centric = CC", (float) ((matProj * aptCube[13])[0] - 0.25), (float) ((matProj * aptCube[13])[1] - 0.2)); 

        p = CameraPt( matProj,  lerp( aptCube[3], aptCube[13], min( dTStem * 0.5, 0.1) ) );
        gl_draw("Spin OC", (float) (p[0] + 0.04), (float) (p[1] - 0.04) );

        p = CameraPt( matProj,  lerp( aptCube[3], aptCube[13], 0.9 ) );
        gl_draw("Spin CC", (float) (p[0] + 0.04), (float) (p[1]) );

        p = CameraPt( matProj,  lerp( aptCube[3], aptCube[13], dTStem * 0.5 ) );
        gl_draw("Aspect ratio/skew (shift)", (float) (p[0] + 0.07), (float) (p[1] - 0.02) );

        p = CameraPt( matProj, aptCube[7]);
        gl_draw("Zoom CC", (float) (p[0] - 0.3), (float) (p[1]) );

        p = CameraPt( matProj, aptCube[9]);
        gl_draw("Zoom OC", (float) (p[0] + 0.05), (float) (p[1]) );  
 
        p = CameraPt( matProj,  lerp( aptCube[3], aptCube[13], dTStem * 0.9 ) );
        gl_draw("Pan OC", (float) (p[0] + 0.05), (float) (p[1] + 0.04) );  
                
        p = CameraPt( matProj,  lerp( aptCube[3], aptCube[13], dTStem * 1.1 ) ) ;
        gl_draw("Pan CC", (float) (p[0] + 0.05), (float) (p[1] - 0.08) );  

        p = CameraPt( matProj, aptCube[0]);
        gl_draw("COP L/R", (float) (p[0] - 0.25), (float) (p[1]) );

        p = CameraPt( matProj, aptCube[1]);
        gl_draw("COP U/D", (float) (p[0] - 0.2), (float) (p[1] + 0.03) );

        p = CameraPt( matProj, aptCube[5]);
        gl_draw("Rot U/D", (float) (p[0] - 0.05), (float) (p[1] + 0.03) );

        p = CameraPt( matProj, aptCube[6]);
        gl_draw("Rot L/R", (float) (p[0] + 0.03), (float) (p[1] - 0.01) );
    }
    OGLEnd2D();

    if ( in_bMouseOver == true ) {
        OGLBegin2D();

	    //const double dScl1 = 0.01;
	    //const double dScl2 = -dScl1 / 4.0;
        std::vector<Point3> apt(10);
	    apt[0] = CameraPt( matProj,  aptCube[0] );
	    apt[1] = CameraPt( matProj,  aptCube[3] );
	    apt[2] = CameraPt( matProj,  aptCube[3] );
	    apt[3] = CameraPt( matProj,  aptCube[6] );
	    apt[4] = CameraPt( matProj,  aptCube[7] );
	    apt[5] = CameraPt( matProj,  aptCube[9] );
	    apt[6] = CameraPt( matProj,  aptCube[10] );
	    apt[7] = CameraPt( matProj,  aptCube[13] );
	    apt[8] = CameraPt( matProj,  aptCube[13] );
	    apt[9] = CameraPt( matProj,  aptCube[16] );

        glColor3fv(fColBack);
	    for ( int iY = -2; iY < 3; iY++ ) {
		    glBegin( GL_LINES );
		    for ( size_t i = 0; i < apt.size(); i++ ) 
			    glVertex3d( apt[i][0], apt[i][1] + iY * 0.003, 0.0 );
		    glVertex3dv( &aptCube[16][0] );
		    glEnd();
	    }
        glColor3f(0,0,1);
        glLineWidth( 1.0f );
	    
        glColor3fv(fColBack);
        glLineWidth( 1.0f );
        switch (m_iClosest) {
        case 0 :
        case 1 :
        case 2 :
        case 10 :
        case 11 :
        case 12 :
            OGLDrawCircle( lerp( CameraPt( matProj,  aptCube[m_iClosest] ), CameraPt( matProj,  aptCube[m_iClosest+1] ), 0.5 ), 0.025 ); 
            break;
        case 4 :
        case 5 :
        case 6 :
        case 14 :
        case 15 :
        case 16 :
            OGLDrawCircle( lerp( CameraPt( matProj,  aptCube[m_iClosest] ), CameraPt( matProj,  aptCube[m_iClosest-1] ), 0.5 ), 0.025 ); 
            break;
        case 7 :
        case 9 :
            OGLDrawCircle( CameraPt( matProj,  aptCube[m_iClosest] ), 0.025 ); 
            break;
        case 8 :
            OGLDrawCircle( CameraPt( matProj,  lerp( aptCube[3], aptCube[13], dTStem * 0.9 ) ), 0.025 ); 
            break;
        case 3 :
            OGLDrawCircle( CameraPt( matProj,  lerp( aptCube[3], aptCube[13], min( dTStem * 0.5, 0.1) ) ), 0.025 ); 
            break;
        case 13 :
            OGLDrawCircle( CameraPt( matProj,  lerp( aptCube[3], aptCube[13], 0.9 ) ), 0.025 ); 
            break;
        case 17 :
            OGLDrawCircle( CameraPt( matProj,  lerp( aptCube[3], aptCube[13], dTStem * 0.5 ) ), 0.025 ); 
            break;
        case 18 :
            OGLDrawCircle( CameraPt( matProj,  lerp( aptCube[3], aptCube[13], dTStem * 1.1 ) ), 0.025 ); 
            break;
        case -1 :
            break;
        }

        OGLEnd2D();
    }

    // Perspective, focal length, and aspect ratio part of camera matrix
    glMatrixMode( GL_PROJECTION );
    glLoadMatrixd( & m_cam.getProjection()(0,0));
    glMatrixMode( GL_MODELVIEW );
    glLoadMatrixd( & m_cam.getWorldToCamera()(0,0) );

    const float fDef = (m_handle == PAN) ? 3.0f : 2.0f;
    const float fSel = 2.0f;

    static float colGreen[] = {0.0, 1.0, 0.0, 1.0 };
    ::glBegin( GL_LINES );
    ::glColor3f( 1.0,0,1.0 ); glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colGreen );
    if ( m_handle == LEFT || m_handle == TOP_L || m_handle == TOP_R )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[0][0] );
    ::glVertex3dv( &aptCube[1][0] );

    if ( m_handle == LEFT || m_handle == BOTTOM_L || m_handle == BOTTOM_R )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[10][0] );
    ::glVertex3dv( &aptCube[11][0] );

    ::glColor3f( 0,1.0,0 );
    if ( m_handle == TOP_L || m_handle == TOP_R )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[1][0] );
    ::glVertex3dv( &aptCube[2][0] );

    ::glVertex3dv( &aptCube[4][0] );
    ::glVertex3dv( &aptCube[5][0] );

    ::glColor3f( 1.0,0,0 );
    if ( m_handle == BOTTOM_L || m_handle == BOTTOM_R )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[11][0] );
    ::glVertex3dv( &aptCube[12][0] );

    ::glVertex3dv( &aptCube[14][0] );
    ::glVertex3dv( &aptCube[15][0] );

    ::glColor3f( 0,1.0,1.0 );
    if ( m_handle == TOP_L || m_handle == TOP_R || m_handle == RIGHT)
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[5][0] );
    ::glVertex3dv( &aptCube[6][0] );

    ::glVertex3dv( &aptCube[15][0] );
    ::glVertex3dv( &aptCube[16][0] );

    ::glColor3f( 0,0,1.0 );
    if ( m_handle == SKEW || m_handle == ASPECT_RATIO )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    const Point3 ptL = lerp( aptCube[3], aptCube[13], dTStem * 0.5 ) + ( aptCube[9] - aptCube[7] ) * 0.1;
    const Point3 ptR = lerp( aptCube[3], aptCube[13], dTStem * 0.5 ) - ( aptCube[9] - aptCube[7] ) * 0.1;
    ::glVertex3dv( &ptL[0] );
    ::glVertex3dv( &ptR[0] );

    if ( m_handle == TOP_L || m_handle == TOP_R || m_handle == LEFT )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[2][0] );
    ::glVertex3dv( &aptCube[3][0] );

    if ( m_handle == TOP_L || m_handle == TOP_R || m_handle == RIGHT )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[3][0] );
    ::glVertex3dv( &aptCube[4][0] );

    if ( m_handle == BOTTOM_L || m_handle == BOTTOM_R || m_handle == LEFT )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[12][0] );
    ::glVertex3dv( &aptCube[13][0] );

    if ( m_handle == BOTTOM_L || m_handle == BOTTOM_R || m_handle == RIGHT )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[13][0] );
    ::glVertex3dv( &aptCube[14][0] );

    if ( m_handle == ZOOM )
        glLineWidth( fSel );
    else
        glLineWidth( fDef );

    ::glVertex3dv( &aptCube[3][0] );
    ::glVertex3dv( &aptCube[13][0] );

    ::glVertex3dv( &aptCube[7][0] );
    ::glVertex3dv( &aptCube[9][0] );

    ::glEnd();

    const Point3 ptAt = m_cam.getEye() + m_cam.getLook() * GetFocusDist();
    //const double dScl = length( ptAt - m_cam.getEye() ) * tan( m_cam.getZoom() / 2.0 );

    const Point3 ptLL = ptAt - m_cam.getRight() * 0.1 - m_cam.getUp() * 0.1 + GetCOPOffset();
    const Point3 ptRR = ptAt + m_cam.getRight() * 0.1 - m_cam.getUp() * 0.1 + GetCOPOffset();
    const Point3 ptUU = ptAt + m_cam.getRight() * 0.1 + m_cam.getUp() * 0.1 + GetCOPOffset();
    const Point3 ptBB = ptAt - m_cam.getRight() * 0.1 + m_cam.getUp() * 0.1 + GetCOPOffset();
    ::glColor4f( 0.5, 0.5, 0.5, 0.5 );

    ::glEnable( GL_DEPTH_TEST );
    ::glEnable( GL_BLEND );
    ::glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );


    ::glMatrixMode( GL_PROJECTION );
    :: glPopMatrix();

    ::glMatrixMode( GL_MODELVIEW );
    :: glPopMatrix();

    
    glPopAttrib();
}
///

void IBar::SetClickDistance( const double in_d ) 
{
    m_dClickDistance = in_d;
}

static int m_aiMapLimb[6] = {0, 6, 10, 16, 9, 13};
static int m_aiMapLimbBase[6] = {3, 3, 13, 13, 7, 3};

Point3 IBar::FlTkToCamera( const ScreenPoint &in_ptScreen )
{
    return Point3( -1.0 + 2.0 * (in_ptScreen[0]) / (double) m_cam.getWidth(),
                   -1.0 + 2.0 * (m_cam.getHeight() - in_ptScreen[1] - 1) / (double) m_cam.getHeight(),
                   0.0 );
}

static double FindPtOnSeg( const Point3 &pt0, const Point3 &pt1, const Point3 &ptClick, double &out_dDist )
{
	const Vector3 vec = pt1 - pt0;
	const double dLenVec = vec.lengthSquared();
    double dT = 0.0;
	if ( isZero( dLenVec ) ) {
		out_dDist = length( ptClick - pt0 );
		return dT;
	}

	dT = ( (ptClick - pt0) * vec ) / (dLenVec);

	if ( dT < 0.0 ) {
		dT = 0.0;
		out_dDist = length( ptClick - pt0 );
		return dT;
	}

	if ( dT > 1.0 ) {
		dT = 1.0;
		out_dDist = length( ptClick - pt1 );
		return dT;
	}

	const Point3 pt = lerp( pt0, pt1, dT );
    const Vector3 vecTo = pt - ptClick;
    const Vector3 vecPerp = pt1 - pt0;
    //double dot = vecTo * vecPerp;
	out_dDist = length( pt - ptClick );

    return dT;
}

bool IBar::MouseMove( const ScreenPoint &in_ipt, bool &out_bRedraw ) 
{
    double dTStem = 0.5;
    std::vector<Point3> aptCube = MakeCube( dTStem );

    // Convert to (-1,1) X (-1,1)
    const Point3 ptClick = FlTkToCamera( in_ipt );

    const int iClosestSave = m_iClosest;

    double dDist;
    m_iClosest = -1;
    double dClosest = 1e30;
    const Matrix4 matProj = m_cam.getProjection() * m_cam.getWorldToCamera();
    for ( int i = 0; i < 6; i++ ) {
        double dT = FindPtOnSeg( CameraPt( matProj, aptCube[ m_aiMapLimbBase[i]] ),  CameraPt( matProj, aptCube[ m_aiMapLimb[i] ] ), ptClick, dDist );

        if ( dT < 1e-16 ) dT = 1e-16;
        if ( dT > 1.0 - 1e-16 ) dT = 1.0 - 1e-16;

        if ( dDist < dClosest ) {
            switch (i) {
            case 0 :
            case 2 :
                m_iClosest = (int) ( dT * m_aiMapLimb[i] + (1.0 - dT) * m_aiMapLimbBase[i] );
                break;
            case 1 :
            case 3 :
                m_iClosest = 1 + (int) ( dT * m_aiMapLimb[i] + (1.0 - dT) * m_aiMapLimbBase[i] );
                break;
            case 4 :
                m_iClosest = 7 + (int) ( dT * 3.0 );
                break;
            case 5 :
                if ( dT < ( 0.5 * dTStem - 0.05) ) {
                    m_iClosest = 3;  // Spin object-centric
                } else if ( dT < (0.5 * dTStem + 0.05) ) {
                    m_iClosest = 17; // skew/aspect ratio
                } else if ( dT < dTStem ) {
                    m_iClosest = 8; // Pan, either object or camera-centric
                } else if ( dT < 0.5 * (dTStem + 1.0) ) {
                    m_iClosest = 18; // Pan, either object or camera-centric
                } else  {
                    m_iClosest = 13; // Spin camera-centric
                }
                break;
            }
            dClosest = dDist;
        }
    }

    if ( dClosest >= m_dClickDistance ) {
        m_iClosest = -1;
    }

    if ( m_iClosest != iClosestSave ) {
        out_bRedraw = true;
    } else { 
        out_bRedraw = false;
    }

    if ( dClosest < m_dClickDistance ) {
        return true;
    }

    return false;
}

bool IBar::MouseDown( const ScreenPoint &in_ipt, const bool in_bShift ) 
{
    double dTStem = 0.5;
    std::vector<Point3> aptCube = MakeCube(dTStem);

    m_cameraOrig = *this;

    // Convert to (-1,1) X (-1,1)
    m_ptDown = FlTkToCamera( in_ipt );
    m_commit = UNCOMMITTED;
    m_bCameraMode = false;

    // Project each point 
    m_iClosest = -1;

    //double dT = 0.0;
	double dDist = 0.0, dClosest = 1e30;
    const Matrix4 matProj = m_cam.getProjection() * m_cam.getWorldToCamera();
    for ( int i = 0; i < 6; i++ ) {
        double dT = FindPtOnSeg( CameraPt( matProj, aptCube[ m_aiMapLimbBase[i]] ),  CameraPt( matProj, aptCube[ m_aiMapLimb[i] ] ), FlTkToCamera( in_ipt ), dDist );

        if ( dT < 1e-16 ) dT = 1e-16;
        if ( dT > 1.0 - 1e-16 ) dT = 1.0 - 1e-16;

        if ( dDist < dClosest ) {
            switch (i) {
            case 0 :
            case 2 :
                m_iClosest = (int) ( dT * m_aiMapLimb[i] + (1.0 - dT) * m_aiMapLimbBase[i] );
                break;
            case 1 :
            case 3 :
                m_iClosest = 1 + (int) ( dT * m_aiMapLimb[i] + (1.0 - dT) * m_aiMapLimbBase[i] );
                break;
            case 4 :
                m_iClosest = 7 + (int) ( dT * 3.0 );
                break;
            case 5 :
                if ( dT < ( 0.5 * dTStem - 0.05) ) {
                    m_iClosest = 3;  // Spin object-centric
                } else if ( dT < (0.5 * dTStem + 0.05) ) {
                    m_iClosest = 17; // skew/aspect ratio
                } else if ( dT < dTStem ) {
                    m_iClosest = 8; // Pan, either object or camera-centric
                } else if ( dT < 0.5 * (dTStem + 1.0) ) {
                    m_iClosest = 18; // Pan, either object or camera-centric
                } else  {
                    m_iClosest = 13; // Spin camera-centric
                }
                break;
            }
            dClosest = dDist;
        }
    }

    if ( dClosest > m_dClickDistance )
        m_iClosest = -1;

    switch( m_iClosest ) {
    case -1 : m_handle = NONE; break;
    case 8 : m_handle = PAN; m_bCameraMode = false; break;
    case 18 : m_handle = PAN; m_bCameraMode = true; break;
    case 3 :  m_handle = ROTATE; break;
    case 13 : m_handle = ROTATE; m_bCameraMode = true; break;
    case 2 : 
    case 4 : 
    case 14 : 
    case 12 : m_handle = ALL;   break;
    case 7 : m_bCameraMode = true; 
    case 9 : m_handle = ZOOM; break;
    case 0 :
    case 10 : m_handle = LEFT; break;
    case 1 : m_handle = TOP_L; break;
    case 5 : m_handle = TOP_R; break;
    case 6 : 
    case 16 : m_handle = RIGHT; break;
    case 11 : m_handle = BOTTOM_L; break;
    case 15 : m_handle = BOTTOM_R; break;
    case 17 : 
            if ( in_bShift ) 
                m_handle = SKEW;
            else
                m_handle = PAN;
            break;

    default : assert(false);
    }

	if ( m_iClosest == -1 ) return false;
	return true;
}

Vector3 IBar::Limb( const std::vector<Point3> &in_aptCube, const Matrix4 &in_mat ) const
{
    Vector3 vecProj(0,0,1);

    //  Vector directions (* indicates arrow head)
    //
    //      ---* ----*
    //          *
    //          |
    //      ---------*
    //          |
    //      ---* ----*

    switch ( m_iClosest ) {
    case 0 :
    case 1 :
    case 2 :
        vecProj = in_mat * in_aptCube[3] - in_mat * in_aptCube[0];
        break;
    case 4 :
    case 5 :
    case 6 :
        vecProj = in_mat * in_aptCube[6] - in_mat * in_aptCube[3];
        break;
    case 10 :
    case 11 :
    case 12 :
        vecProj = in_mat * in_aptCube[13] - in_mat * in_aptCube[10];
        break;
    case 14 :
    case 15 :
    case 16 :
        vecProj = in_mat * in_aptCube[16] - in_mat * in_aptCube[13];
        break;
    case 7 :
    case 8 :
    case 9 :
    case 18 :
        vecProj = in_mat * in_aptCube[9] - in_mat * in_aptCube[7];
        break;
    case 3 :
    case 13 :
        vecProj = in_mat * in_aptCube[3] - in_mat * in_aptCube[13];
        break;
    case 17 :
        vecProj = in_mat * in_aptCube[3] - in_mat * in_aptCube[8];
        break;
    default :
        assert(false);
        break;
    }

    return Vector3( vecProj[0], vecProj[1], 0.0 );
}

Point3 IBar::LimbBase( const std::vector<Point3> &in_aptCube, const Matrix4 &in_mat ) const
{
    Point3 ptLimb;

    if ( m_iClosest < 7 )
        ptLimb = in_mat * in_aptCube[3];
    else if ( m_iClosest < 10 )
        ptLimb = in_mat * lerp( in_aptCube[7], in_aptCube[9], 0.5 );
    else if ( m_iClosest < (int) in_aptCube.size() )
        ptLimb = in_mat * in_aptCube[13];
    else
        ptLimb = in_mat * in_aptCube[8];

    return Point3( ptLimb[0], ptLimb[1], 0.0 );
}



void IBar::MouseMoveObject()  
{
    if ( m_iClosest == -1 ) return;

    const Vector3 vec = m_ptDrag - m_ptDown;
    double vecmag = length( vec );
    if ( fabs( vec[0] ) > fabs( vec[1] ) ) {
        if ( vec[0] < 0.0 ) vecmag = -vecmag;
    } else {
        if ( vec[1] < 0.0 ) vecmag = -vecmag;
    }

    double dTStem = 0.5;
    std::vector<Point3> aptCube = MakeCube( dTStem );

    m_cam = m_cameraOrig.m_cam;

    const Matrix4 matProj = m_cam.getProjection() * m_cam.getWorldToCamera();
    const Vector3 vecMiddle = unit( Limb( aptCube, m_cam.getProjection() * m_cam.getWorldToCamera() ) );
    const double dLenOrig = 2.0 * ( vecMiddle * (m_ptDown - LimbBase( aptCube,matProj ) ) );
    const double dLenNew  = 2.0 * ( vecMiddle * (m_ptDrag - LimbBase( aptCube,matProj ) ) );
    const double dRatio = dLenOrig / dLenNew;

    if ( m_handle == PAN ) {
        // Since cube is at film plane, move by mouse movement in film plane
        const double dTrans = m_dFocusDist * tan( m_cam.getZoom() / 2.0 );
        switch (m_commit) {
        case SCALE_ONLY :
            m_cam.moveSideways( -vec[0] * dTrans ); break;
        case ANGLE_ONLY :
            m_cam.moveVertical( -vec[1] * dTrans ); break;
        case BOTH :
            m_cam.moveSideways( -vec[0] * dTrans ); 
            m_cam.moveVertical( -vec[1] * dTrans ); break;
        case UNCOMMITTED :
            break;
        }
    } 
    
    else if ( m_handle == ZOOM ) {
        const double dFocusLen = m_cam.getHeight() / tan( m_cam.getZoom() / 2.0 );
        const double dNewFL = dFocusLen / dRatio;
        double dZNew = 2.0 * atan( m_cam.getHeight() / dNewFL );
        if ( dZNew < 1e-4 ) dZNew = 1e-4;
        if ( dZNew > M_PI - 1e-4 ) dZNew = M_PI - 1e-4;
		m_cam.setZoom( 180.0 * dZNew / M_PI );
    } 
    
    else if ( m_handle == ROTATE ) {
        // Angle of the current point to the center of the screen (minus 90 degrees)
        // is desired rotation of object
        const double dAng = atan2( m_ptDrag[1], m_ptDrag[0] ) + ((m_iClosest == 3) ? - M_PI / 2.0 : M_PI / 2.0);
        m_cam.rotateAroundAtPoint( 2, -dAng, m_dFocusDist );
    } 

    // simplified IBar behavior -- COP works only from the left side
    else if ( m_handle == LEFT) {
        m_cam.moveSideways( vecmag * m_dFocusDist * tan( m_cam.getZoom() / 2.0 ) );
        m_cam.setProjectionCenter( m_cam.getProjectionCenter() + Vector3( -vecmag, 0, 0 ) );
    } 
    
    // simplified IBar behavior -- ... and Rotate works only from the right side
    else if ( m_handle == RIGHT ) {
        m_cam.rotateAroundAtPoint( 1, vecmag * M_PI / 2.0, m_dFocusDist );
    } 
    
    // simplified IBar behavior for top vs. bottom handles
    else if ( m_handle == TOP_L || m_handle == BOTTOM_L ) {
        m_cam.moveVertical( vecmag * m_cam.getAspectRatioScale() * m_dFocusDist * tan( m_cam.getZoom() / 2.0 ) );
        m_cam.setProjectionCenter( m_cam.getProjectionCenter() + Vector3( 0, -vecmag, 0.0 ) );
    }
    else if ( m_handle == TOP_R || m_handle == BOTTOM_R ) {
        m_cam.rotateAroundAtPoint( 0, -vecmag * M_PI / 2.0, m_dFocusDist );
    } 
 
    else if ( m_handle == ALL ) {
        double dNewFD = m_dFocusDist;
        const double dScalePerc = fabs( m_ptDown[1] - m_ptDrag[1] ) / 0.5;
        if ( m_ptDown[1] > m_ptDrag[1] )
            dNewFD = max(0.01 * m_dFocusDist, 1.0 - 0.5 * dScalePerc) * m_dFocusDist;
        else 
            dNewFD = (1.0 + 2.0 * dScalePerc) * m_dFocusDist;

        // Adjust the focal length so that the IBar still shows up the same size
        const double dF = 1.0 / tan( m_cam.getZoom() / 2.0 );
        const double dFNew = dF * dNewFD / m_dFocusDist;
        const double dAngNew = 2.0 * atan( 1.0 / dFNew );

        switch ( m_commit ) {
        case SCALE_ONLY :
            if ( m_cam.getZoom() * dRatio < 0.0001 )
                m_cam.setZoom( 180.0 * 0.0001 / M_PI );
            else if ( m_cam.getZoom() * dRatio > M_PI - 0.001 )
                m_cam.setZoom( 180.0 * (M_PI - 0.001) / M_PI );
            else
                m_cam.setZoom( 180.0 * (m_cam.getZoom() * dRatio) / M_PI );
            break;
        case ANGLE_ONLY :
            m_cam.moveForward( dNewFD - m_dFocusDist );
            break;
        case BOTH :
            m_cam.setZoom(180.0 * dAngNew / M_PI );
            m_cam.moveForward( -(dNewFD - m_dFocusDist) );
            break;
        case UNCOMMITTED :
            break;
        }
    } 
    
    else if ( m_handle == ASPECT_RATIO ) {
        m_cam.setAspectRatioScale( m_cam.getAspectRatioScale() * dRatio );
    } 
    
    else if ( m_handle == SKEW ) {
        m_cam.setSkew( m_cam.getSkew() + vec[0] );
    }
}
///
void IBar::MouseDrag( const ScreenPoint &in_ipt, const bool in_bShift ) 
{
    m_ptDrag = FlTkToCamera( in_ipt );


    if ( m_iClosest == -1 )
        return;

    const Vector3 vec = m_ptDrag - m_ptDown;

    if ( (m_handle == LEFT || m_handle == RIGHT) && in_bShift == true ) {
        m_commit = BOTH;
    }
    if ( ( m_handle == TOP_L || m_handle == BOTTOM_L || m_handle == TOP_R || m_handle == BOTTOM_R ) && in_bShift == true ) {
        m_commit = BOTH;
    }

    if ( m_commit == UNCOMMITTED ) {
        if ( fabs( vec[0] ) > fabs( vec[1] ) + 0.009 || fabs( vec[1] ) > fabs( vec[0] ) + 0.009 ) {
            if ( fabs( vec[0] ) > fabs( vec[1] ) + 0.002 ) {
                m_commit = SCALE_ONLY;
            } else  {
                m_commit = ANGLE_ONLY;
            }

            if (m_handle == ALL && in_bShift == false)
                m_commit = BOTH;
            if ( m_handle == PAN && in_bShift == false )
                m_commit = BOTH;
        }
    }
    if ( m_commit != UNCOMMITTED && in_bShift == true && m_handle == SKEW ) {
        if ( m_commit == ANGLE_ONLY ) {
            m_handle = ASPECT_RATIO; 
        } else {
            m_handle = SKEW;
       }
    }

	MouseMoveObject();
}
///
void IBar::MouseRelease( const ScreenPoint &in_ipt, const bool in_bShift ) 
{
    m_ptDrag = FlTkToCamera( in_ipt );
	if ( m_bCameraMode == true ) {
        switch (m_handle) {
        case PAN :
        case ZOOM :
            m_ptDrag = m_ptDown;
            m_ptDown = FlTkToCamera( in_ipt );
            break;
        case ROTATE :
            m_ptDrag[0] = -m_ptDrag[0];
            break;
		default :
			break;
		}
	}
	MouseMoveObject();


    m_iClosest = -1;
    m_handle = NONE;
    m_commit = UNCOMMITTED;
}

