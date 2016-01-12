#ifndef _csc321_H_
#define _csc321_H_

// this header fixes a number of platform specific problems
// make sure you include it at the top of every file you write

#ifdef _MSC_VER
  // turn off symbol length warnings
  //#pragma warning (disable: 4786)
  //#pragma warning (disable: 4503)
  // conversion double -> float
  //#pragma warning (disable: 4244)
  // truncation double -> float
  //#pragma warning (disable: 4305)
#endif

// define useful constants

#ifndef M_PI
  #define M_PI               3.1415926535897932384626433832795
#endif

#define IZ_EPSILON 1e-16

inline bool isZero(double in_d, double in_dEps = IZ_EPSILON)
{ 
    return (in_d < in_dEps && in_d > -in_dEps) ? true : false; 
}

#include <cassert>
#include <cmath>
#include <iostream>
#include <FL/gl.h>



#ifdef LINUX
using std::min;
using std::max;
#endif

extern void RedrawWindow();

#endif /* _csc321_H_ */
