#include "Camera.h"

Camera::Camera(int id,                      // Id of the camera
               const char* imName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
     int i = 0;
     
     while(*(imName + i)) {
          imageName[i++] = *(imName + i);
     }
     
     imageName[i] = 0;
     this->id = id;
     this->imgPlane = imgPlane;
     this->pos = pos;
     this->gaze = gaze; // w
     this->up = up; // v

     u = up.crossProduct(gaze * -1);
     print("gaze", gaze);
     m = pos + gaze * imgPlane.distance;
     q = m + u * imgPlane.left + up * imgPlane.top;
     s_u_const = (imgPlane.right - imgPlane.left) / imgPlane.nx;
     s_v_const = (imgPlane.top - imgPlane.bottom) / imgPlane.ny;
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int row, int col) const
{
     float s_u = (col + .5) * s_u_const;
     float s_v = (row + .5) * s_v_const;

     Vector3f s = q + u * s_u - up * s_v;
	
     return {pos, s - pos};
     
}

