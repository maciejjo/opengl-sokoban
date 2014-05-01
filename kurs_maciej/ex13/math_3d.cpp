#include "math_3d.h"

Vector3f  Vector3f::Cross(const Vector3f & v) const
{
  const float _x = y * v.z - z * v.y;
  const float _y = z * v.x - x * v.z;
  const float _z = x * v.y - y * v.x;

  return Vector3f(_x, _y, _z);
}

Vector3f &Vector3f::Normalize()
{
  const float len = sqrtf(x * x + y * y + z * z);
  x /= len;
  y /= len;
  z /= len;

  return *this;
}

void Matrix4f::InitScaleTransform(float x, float y, float z)
{
  m[0][0] = x;          m[0][1] = 0.0f;       m[0][2] = 0.0f;       m[0][3] = 0.0f;
  m[1][0] = 0.0f;       m[1][1] = y;          m[1][2] = 0.0f;       m[1][3] = 0.0f;
  m[2][0] = 0.0f;       m[2][1] = 0.0f;       m[2][2] = z;          m[2][3] = 0.0f;
  m[3][0] = 0.0f;       m[3][1] = 0.0f;       m[3][2] = 0.0f;       m[3][3] = 1.0f;
}

void Matrix4f::InitRotateTransform(float x, float y, float z)
{
  Matrix4f rx, ry, rz;

  x = torad(x);
  y = torad(y);
  z = torad(z);

  rx.m[0][0] = 1.0f;      rx.m[0][1] = 0.0f;      rx.m[0][2] = 0.0f;      rx.m[0][3] = 0.0f;
  rx.m[1][0] = 0.0f;      rx.m[1][1] = cosf(x);   rx.m[1][2] = -sinf(x);  rx.m[1][3] = 0.0f;
  rx.m[2][0] = 0.0f;      rx.m[2][1] = sinf(x);   rx.m[2][2] = cosf(x);   rx.m[2][3] = 0.0f;
  rx.m[3][0] = 0.0f;      rx.m[3][1] = 0.0f;      rx.m[3][2] = 0.0f;      rx.m[3][3] = 1.0f;

  ry.m[0][0] = cosf(y);   ry.m[0][1] = 0.0f;      ry.m[0][2] = -sinf(y);  ry.m[0][3] = 0.0f;
  ry.m[0][1] = 0.0f;      ry.m[1][1] = 1.0f;      ry.m[1][2] = 0.0f;      ry.m[1][3] = 0.0f;
  ry.m[2][0] = sinf(y);   ry.m[2][1] = 0.0f;      ry.m[2][2] = cosf(y);   ry.m[2][3] = 0.0f;
  ry.m[3][0] = 0.0f;      ry.m[3][1] = 0.0f;      ry.m[3][2] = 0.0f;      ry.m[3][3] = 1.0f;

  rz.m[0][0] = cosf(z);   rz.m[0][1] = -sinf(z);  rz.m[0][2] = 0.0f;      rz.m[0][3] = 0.0f;
  rz.m[1][0] = sinf(z);   rz.m[1][1] = cosf(z);   rz.m[1][2] = 0.0f;      rz.m[1][3] = 0.0f;
  rz.m[2][0] = 0.0f;      rz.m[2][1] = 0.0f;      rz.m[2][2] = 1.0f;      rz.m[2][3] = 0.0f;
  rz.m[3][0] = 0.0f;      rz.m[3][1] = 0.0f;      rz.m[3][2] = 0.0f;      rz.m[3][3] = 1.0f;

  *this = rz * ry * rx;

}

void Matrix4f::InitTranslateTransform(float x, float y, float z)
{
  m[0][0] = 1.0f;       m[0][1] = 0.0f;       m[0][2] = 0.0f;       m[0][3] = x;
  m[1][0] = 0.0f;       m[1][1] = 1.0f;       m[1][2] = 0.0f;       m[1][3] = y;
  m[2][0] = 0.0f;       m[2][1] = 0.0f;       m[2][2] = 1.0f;       m[2][3] = z;
  m[3][0] = 0.0f;       m[3][1] = 0.0f;       m[3][2] = 0.0f;       m[3][3] = 1.0f;
}

void Matrix4f::InitCameraTransform(const Vector3f &target, const Vector3f &up)
{
  Vector3f n = target;
  n.Normalize();
  Vector3f u = up;
  u.Normalize();
  u = u.Cross(n);
  Vector3f v = n.Cross(u);

  m[0][0] = u.x;        m[0][1] = u.y;       m[0][2] = u.z;       m[0][3] = 0.0f;
  m[1][0] = v.x;        m[1][1] = v.y;       m[1][2] = v.z;       m[1][3] = 0.0f;
  m[2][0] = n.x;        m[2][1] = n.y;       m[2][2] = n.z;       m[2][3] = 0.0f;
  m[3][0] = 0.0f;       m[3][1] = 0.0f;      m[3][2] = 0.0f;      m[3][3] = 1.0f;
}

void Matrix4f::InitPersProjTransform(float fov, float width, float height, float z_near, float z_far)
{
  const float ar          = width / height;
  const float z_range     = z_near - z_far;
  const float tanHalfFOV  = tanf(torad(fov / 2.0f));

  m[0][0] = 1.0f/(tanHalfFOV * ar); m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
  m[1][0] = 0.0f; m[1][1] = 1.0f/tanHalfFOV;  m[1][2] = 0.0f;       m[1][3] = 0.0f;
  m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = (-z_near-z_far)/z_range; m[2][3] = 2.0f*z_near*z_far/z_range;
  m[3][0] = 0.0f;       m[3][1] = 0.0f;       m[3][2] = 1.0f;       m[3][3] = 0.0f;
}
