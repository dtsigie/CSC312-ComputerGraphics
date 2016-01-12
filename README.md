Author- Dawit Tsigie    Title - Project 1

1. No additional methods, classes or files added.

2. The mask was implementd using a 2d Vector container. I used a double for loop to iterate through the mask and set the respective distribution value. To assign the correct distribution value for the selected brush type i used a switch statement to switch between the different brush types.

3.  The algorithm for the scanline line drawing and circle drawing is quadratic at worst case.
The algorithm for the scanline line drawing and circle drawing is linear at worst case.
4. I did the scanline line drawing and circle drawing.
5. Extra credit - I implemented a special brush which is a derivative of the gaussian function.

Project 2 - Tesselation

 Classes added to Shape.cpp - Cylinder - Draws a cylinder for different the tesselation values.  Draws top and bot faces first. Then itetartively draws the side faces.
							 - Cone - Draws a cone- similarly to the cylinder. Draws the bot face and draws the sides iteratively. When it reaches the top it just draws a single triangle
							 - Cube - Draws a cube- draws each face separately.
							 - Sphere - draws an icosahedron but the  sphere has not actually been drawn.



Project 3 - Camera

1. I basically added member variables for all the characteristics of the camera: up, right, look, at, eye, projCenter, zoom(height angle) skew aspectRatio, nearP, farP, width, height.

2. I didn't store any transformation matrix. For the inversion of matrices i mostly used the shortcuts on the slides i.e transpose for the rotation matrix , reciprocals for the scaling matrix and inversion of the signs of the points(last column with the exception of the last row) for the translation matrix.

3. I also implemented all the member functions provided in the template. I didnt have to add any member functions.

I did the last two extra credits. I didn't use parallel projection.
For the extra credits I modified the  rotateAtPoint(), setSkew(),getSkew, getCenterProjection(), setCenterProjection(), setAspectRatioScale(), and getAspectRatio() methods. This allowed me to get full functionality of the Ibar.


No known Bugs


Project 4 - Intersection

1. Added a virtual intersect method to the Shape class- virtual HitRecord intersect(const Point3&, const Vector3&);
And implemented the intersect method for each of the shapes.

2 - No known bugs.

3 - Tried to do the intersection for the torus but didn't implement the quartic equation. I drew the the torus using the code from the opengl website


Project 5 - Sceneview

1. Used the classes provided in the skeleton- Tree, Object and Shape
2. Utilized a map to store all the subgraphs.
3. No known bugs

4. MySCENE.cs  - an attempt at a wedding cake


Project 6 - Ray tracing

1. I added in attenuation vals for the diffusion calculation, reflection ray and specular shading. Also incorporated shadowing.

2. Bugs- The ray trace is not exactly the same as the one in the demo.

3. No extra credits.

 