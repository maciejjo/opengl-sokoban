#ifndef MATH_3D_H
#define	MATH_3D_H

#include <math.h>

#define torad(x) ((x) * M_PI / 180.0f)
#define todeg(x) ((x) * 180.0f / M_PI)


struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f()
    {
    }

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

class Matrix4f
{
  public:
    float m[4][4];

    Matrix4f()
    {
    }

    inline void InitIdentity()
    {
      m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
      m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
      m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[1][3] = 0.0f;
      m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][3] = 0.0f; m[1][3] = 1.0f;
    }

    inline Matrix4f operator*(const Matrix4f& right) const
    {
      Matrix4f ret;

      for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
          //ret.m[i][j] = 0;
                ret.m[i][j] = m[i][0] * right.m[0][j] +
                              m[i][1] * right.m[1][j] +
                              m[i][2] * right.m[2][j] +
                              m[i][3] * right.m[3][j];
          /*
          for(int k = 0; k < 4; k++) {
            ret.m[i][j] += m[i][k] * right.m[k][j];
          }
          */
        }
      }

      return ret;
    }
};

#endif	/* MATH_3D_H */

