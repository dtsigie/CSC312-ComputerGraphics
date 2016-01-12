#include "csc321.h"
#include "MyBrush.h"
#include "BrushInterface.h"
#include <cmath>
#include <iostream>
//using namespace std;

void MyBrush::changedBrush() {
    // this is called anytime the brush type or brush radius changes
    // it should recompute the brush mask appropriately
    const int radius = brushUI->getRadius();
	ToolType tType = brushUI->getToolType();
	int size = 2*radius;
	mask.resize(size);
	for (int i = 0; i < size; i++)
		mask[i].resize(size);
	
//	if (tType == 0){
	for (int x=0; x<size; x++){
		for(int y=0; y<size; y++){
			float dx = (float) x - radius;
			float dy = (float) y - radius;
			float d = sqrt(dx*dx + dy*dy);
			if(d<=radius){
				switch (brushUI->getBrushType())
				{
					case BRUSH_CONSTANT:
						mask[x][y] = 1.0;
						break;
					case BRUSH_LINEAR:
						mask[x][y] = 1.0f - d/radius;
						break;
					case BRUSH_QUADRATIC:
						mask[x][y] = 1.0f - (d/radius)*(d/radius);
						//cout<<"ad";
						break;
					case BRUSH_GAUSSIAN:
						mask[x][y] = exp(-(d*d)/(radius*radius))/sqrt(2*M_PI)*radius;
						break;
					default:
						mask[x][y] = 0.0;
						

				}
			}
			else
				mask[x][y] = 0.0;
			//cout<<"2";
	//	i++;
		}
	//cout<<"lkk";
	}
	}
	
		


void MyBrush::drawBrush( ) {
    // apply the current brush mask to image location (x,y)
    // the mouse location is in mouseDrag

    const int radius = brushUI->getRadius();
    const float pixelFlow = brushUI->getFlow();
    const Color colBrush = brushUI->getColor();
	std::cout<<"imageHeight"<<imageHeight;
	//image location
	ScreenPoint startPt = mouseDrag + ScreenVector(-radius,-radius);
	ScreenPoint endPt = mouseDrag + ScreenVector(radius,radius);

	// boundary check
	int XoFF=0;
	int YoFF=0;
	int ymin = startPt[1] ,ymax = endPt[1], xmin = startPt[0],xmax = endPt[0];
	if (ymin < 0) {
		YoFF = -ymin;
		ymin=0;
	}	
	if (ymax > imageHeight){
		ymax = imageHeight;
	}
	if (xmin < 0 ){
		XoFF = -xmin;
		xmin = 0;
	}
	if (xmax > imageWidth){
		xmax = imageWidth;
	}

	for(int i=XoFF , x  = xmin; x < xmax; x++,i++){
		
		for(int y = ymin, j= YoFF; y < ymax; y++,j++){
			const Color colImage = getPixel(x,y);
			float d = sqrt((float) (mouseDrag[0]-x)*(mouseDrag[0]-x) + (mouseDrag[1]-y)*(mouseDrag[1]-y));
			if ( d <= radius){
				putPixel(x,y, colBrush*mask[i][j]*pixelFlow + colImage*(1.0f - mask[i][j]*pixelFlow));
			}
			
			
		}

	//	j=YoFF; // reseting the mask
		
		
	}

}


void MyBrush::drawLine( ) {
    // draw a thick line from mouseDown to mouseDrag
    // the width of the line is given by the current brush radius
    const int radius = brushUI->getRadius();
    const Color colBrush = brushUI->getColor();
	int x1 = mouseDrag[0];
	int x0 = mouseDown[0];
	int y1 = mouseDrag[1];
	int y0 = mouseDown[1];
		
	if (mouseDrag[1] > imageHeight) {
		y1 = imageHeight;
	}	
	if (y1 < 0){
		y1 = 0;
	}
	if (x1 < 0 ){
		x1 = 0;
	}
	if (mouseDrag[0] > imageWidth){
		x1 = imageWidth;
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int x = x0; 
	int	y = y0;
	putPixel(x, y,colBrush);
	
	//special cases

	//vertical line
	if( abs(dy) > 0  && dx == 0){
	
		
		if(dy>0){
			while (y < y1) {

					putPixel(x,y,colBrush);	
					y++;
			}
		}
		else{
			while (y > y1) {

					putPixel(x,y,colBrush);	
					y--;
			}
		}

	}

	

	//octant 1
	else if(dy >= 0 && dy <= dx && dx > 0){
	
		int d = 2 * dy - dx;
 		int incrE = 2 * dy;
		int incrNE = 2 * (dy - dx);
		//ScreenVector line1 = ScreenPoint(x1,y1) - ScreenPoint(x,y);
		//ScreenVector perpline = (ScreenPoint(y,x1) - ScreenPoint(y1,y));
		//ScreenVector normperpline = perpline* (1/perpline.length());
		////new points
		//ScreenPoint pt3 =  ScreenPoint(x,y) + normperpline*(radius/2);
		//ScreenPoint pt4 =  ScreenPoint(x,y) - normperpline*(radius/2);
		//ScreenPoint pt5 =  ScreenPoint(x1,y1) + normperpline*(radius/2);
		//ScreenPoint pt6 =  ScreenPoint(x1,y1) - normperpline*(radius/2);
		//
		while (x < x1) {
			
				if (d <= 0) d = d + incrE; // East Case
				else { d = d + incrNE; ++y;/*pt3[1]++;*/ } // Northeast Case
				++x;
				//pt3[0]++;
				putPixel(x,y,colBrush);	
				//putPixel(pt3[0],pt3[1],colBrush);
		}
	}
	//octant 2
	else if(dy >= dx && dx >  0  ){
		std::swap(x,y);
		std::swap(x1,y1);
		dx = x1 - x;
		dy = abs(y1 - y);

		int d = 2 * dy - dx;
 		int incrE = 2 * dy;
		int incrNE = 2 * (dy - dx);
		
	
		while (x < x1) {
			
				if (d <= 0) d = d + incrE; // East Case
				else { d = d + incrNE; ++y; } // Northeast Case
				++x;
				putPixel(y,x,colBrush);	
	
		}
	}
	
	////octant 3
	else if((dy >= abs(dx)) && dx <= 0 && dy >0){
		
		

		//std::swap(x,x1);
		std::swap(x,y);
		std::swap(x1,y1);
		std::swap(y,y1);
		std::swap(x1,x);
		

		dx = x1 - x;
		dy = abs(y1 - y);

		int d = 2 * (dy) - dx;
 		int incrE = 2 * (dy);
		int incrNE = 2 * (dy - dx);
		
		std::cout<<"x: "<<x<<std::endl;
		std::cout<<"x1: "<<x1<<std::endl;
		std::cout<<"start: "<<mouseDown[0]<<"\n";
		std::cout<<"end: "<<mouseDrag[0]<<"\n";
		while (x < x1) {
			
				if (d <= 0) d = d + incrE; // west Case
				else { d = d + incrNE; ++y; } // Northwest Case
				++x;
				putPixel(y,x,colBrush);	
	
		}
	
	
	}
	//octant 4
	else if((dy <= abs(dx)  && dx <= 0 && dy >= 0)){
		
		std::swap(x,x1);
		dx = x1 - x;
		dy = y1 - y;

		int d = 2 * (dy) - dx;
 		int incrE = 2 * (dy);
		int incrNE = 2 * (dy - dx);
		
		std::cout<<"x: "<<x<<std::endl;
		std::cout<<"x1: "<<x1<<std::endl;
		std::cout<<"start: "<<mouseDown[0]<<"\n";
		std::cout<<"end: "<<mouseDrag[0]<<"\n";
		while (x < x1) {
			
				if (d <= 0) d = d + incrE; // 
				else { d = d + incrNE; ++y; } //
				++x;
				putPixel(-x,y,colBrush);	
	
		}
		
	}
	//octant 5
	else if(dy < 0 && dx < 0 && abs(dy) <= abs(dx)  ){
		
		std::swap(x1,x);
		std::swap(y1,y);
		dx = x1 - x;
		dy = y1 - y;
		int d = 2 * dy - dx;
 		int incrE = 2 * dy;
		int incrNE = 2 * (dy - dx);
		
	
		while (x < x1) {
			
				if (d <= 0) d = d + incrE; // East Case
				else { d = d + incrNE; ++y; } // Northeast Case
				++x;
				putPixel(x,y,colBrush);	
	
		}
	
	}
	// octant 6 
	else if(dy < 0 && dx < 0 && abs(dy) >= abs(dx)){
		std::swap(x,y);
		std::swap(x1,y1);
		std::swap(x,x1);
		std::swap(y,y1);
		dx = x1 - x;
		dy = y1 - y;

		int d = 2 * dy - dx;
 		int incrE = 2 * dy;
		int incrNE = 2 * (dy - dx);
		
	
		while (x < x1) {
			
				if (d <= 0) d = d + incrE; // East Case
				else { d = d + incrNE; ++y; } // Northeast Case
				++x;
				putPixel(y,x,colBrush);	
	
		}
	}
	//octant 7 
	else if(dy<0 && dx>0 && abs(dy) >= abs(dx)){
		std::swap(x,x1);
		std::swap(x,y);
		std::swap(x1,y1);

		dx = x1 - x;
		dy = y1 - y;

		int d = 2 * dy - dx;
 		int incre = 2 * dy;
		int incrne = 2 * (dy - dx);
	
	
		while (x > x1) {
			
				if (d <= 0) d = d + incre; // east case
				else { d = d + incrne; ++y; } // southeast case
				++x;
				putPixel(-y,x,colBrush);	
	
		}
	
	}
	//octant 8 
	else if(dy <= 0 && dx > 0 && dx > abs(dy)){
		
		std::swap(x,x1);
		std::swap(y1,y);
		std::swap(x1,x);
	
		dx = x1 - x;
		dy = y1 - y;

		int d = 2 * (dy) - dx;
 		int incrE = 2 * (dy);
		int incrNE = 2 * (dy - dx);
		
	
		while (x < x1) {
			
				if (d <= 0) d = d + incrE; // west Case
				else { d = d + incrNE; ++y; } // Northwest Case
				++x;
				putPixel(-x,y,colBrush);	
	
		}
	}
	//octant 8
	/*if(dy < 0 ){
			y = -y;
			y1 = -y1;
	}
	else{
		x = x;
		y = y;
	}*/
	
	//vertical line
	/*if(dx = 0 && dy > 0){
		while(x < x1){
			putPixel(x,y,colBrush);
			y++;
		}
	}*/
//	else{
	
//	}	

}




void MyBrush::drawCircle() {
  //   draw a thick circle at mouseDown with radius r
  //   the width of the circle is given by the current brush radius
	 int width = brushUI->getRadius();
	 for(int i  = 0; i<=width; i++){
	 const Color colBrush = brushUI->getColor();

	
	 int x0 = mouseDown[0];
	 int y0 = mouseDown[1];
	 int radius =  mouseDown[0] + abs( mouseDrag[0]);
	 radius += width; 
	 int x;
	 x = 0;
	
	
	 // top canvas
	 if((mouseDown[1]+radius) > imageHeight){
		radius =  imageHeight - mouseDown[1] - 1;
	 }
	 // bot canvas
	 if((mouseDown[1]-radius) < 0){
		radius = mouseDown[1] - 1;
	 }
	 //left canvas
	 if((mouseDown[0]-radius) < 0){
		radius =  mouseDown[0] - 1;
	 }
	 // right canvas
	 if((mouseDown[0]+radius) > imageWidth){
		radius = imageWidth - mouseDown[0] -1;
	 }
	 int y =  radius;

	 
	 float decision = 1 - radius ;
	 
	  putPixel(x0 + x,y0 + y,colBrush);
	  putPixel(x0 + x,y0 - y,colBrush);
	  putPixel(x0 - x,y0 + y,colBrush);
	  putPixel(x0 - x,y0 - y,colBrush);
	  putPixel(x0 + y,y0 + x,colBrush);
	  putPixel(x0 - y,y0 + x,colBrush);
	  putPixel(x0 + y,y0 - x,colBrush);
	  putPixel(x0 - y,y0 - x,colBrush);
	 while ( y >= x ) {
		 x++; 
		 if (decision < 0) { // Move East
			decision += 2 * x + 3;
			//decision += deltaE;
			//deltaE += 2; 
			//deltaSE += 2; // Update delta
		 } 
		 else { // Move SouthEast
			y--;
		
			decision += 2 * (x - y) + 5;
		
		}
		
		  putPixel(x0 + x,y0 + y,colBrush);
		  putPixel(x0 + x,y0 - y,colBrush);
		  putPixel(x0 - x,y0 + y,colBrush);
		  putPixel(x0 - x,y0 - y,colBrush);
		  putPixel(x0 + y,y0 + x,colBrush);
		  putPixel(x0 - y,y0 + x,colBrush);
		  putPixel(x0 + y,y0 - x,colBrush);
		  putPixel(x0 - y,y0 - x,colBrush);
	 }
	 }
}



void MyBrush::drawPolygon() {
    // draw a polygon with numVertices whos coordinates are stored in the
    // polygon array: {x0, y0, x1, y1, ...., xn-1, yn-1}
}

void MyBrush::filterRegion( ) {
    // apply the filter indicated by filterType to the square
    // defined by the two corner points mouseDown and mouseDrag
    // these corners are not guarenteed to be in any order
    // The filter width is given by the brush radius
	FilterType fType = brushUI->getFilterType();
	int p1x = mouseDown[0];
	int p1y = mouseDown[1];
	int p2x = mouseDrag[0];
	int p2y = mouseDrag[1];
	int xStart = min( p1x, p2x );
	int yStart = min( p1y, p2y );
	int xEnd = max( p1x, p2x );
	int yEnd = max( p1y, p2y );
	int r = p2x - p1x;
	int iSrc;
	int jSrc;
	Color origColor;
	Color **outColor;
	Color colSum;

	/*switch (fType){
	case FILTER_BLUR:*/
		
	for ( int x = xStart; x <= xEnd; x++ ){
		for ( int y = yStart; y <= yEnd; y++ ){
			colSum = Color(0,0,0);
			for ( int i = -r/2; i <= r/2; i++ ){
				iSrc = x + i;
				if ( iSrc < 0 ) iSrc = 0;
				if ( iSrc >= imageWidth ) iSrc = imageWidth - 1;
				for ( int j = -r/2; j <= r/2; j++ ){
					jSrc = y + j;
					if ( jSrc < 0 ) jSrc = 0;
					if ( jSrc >= imageHeight ) jSrc = imageHeight - 1;
					colSum += getPixel( iSrc, jSrc );
				}
				//outColor[x][y] = colSum;
			 putPixel(x,y,colSum);
			}
		}
	}
}

		//}
		/*break;
	default:
		break;*/
		/*for ( int x = xStart; x <= xEnd; x++ ){
			for ( int y = yStart; y <= yEnd; y++ ){
				putPixel(x,y,outColor[x][y]);
			}
		}*/

	


