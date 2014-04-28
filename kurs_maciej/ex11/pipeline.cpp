#include "pipeline.h"

void Pipeline::InitScale(Matrix4f &m) const
{
  m.m[0][0] = m_scale.x;  m.m[0][1] = 0.0f;       m.m[0][2] = 0.0f;       m.m[0][3] = 0.0f;
  m.m[1][0] = 0.0f;       m.m[1][1] = m_scale.y;  m.m[1][2] = 0.0f;       m.m[1][3] = 0.0f;
  m.m[2][0] = 0.0f;       m.m[2][1] = 0.0f;       m.m[2][2] = m_scale.z;  m.m[2][3] = 0.0f;
  m.m[3][0] = 0.0f;       m.m[3][1] = 0.0f;       m.m[3][2] = 0.0f;       m.m[3][3] = 1.0f;
}

void Pipeline::InitRotate(Matrix4f &m) const
{
  Matrix4f rx, ry, rz;

  const float x = torad(m_rotate.x);
  const float y = torad(m_rotate.y);
  const float z = torad(m_rotate.z);

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

  m = rz * ry * rx;

}

void Pipeline::InitTranslate(Matrix4f &m) const
{
  m.m[0][0] = 1.0f;       m.m[0][1] = 0.0f;       m.m[0][2] = 0.0f;       m.m[0][3] = m_translate.x;
  m.m[1][0] = 0.0f;       m.m[1][1] = 1.0f;       m.m[1][2] = 0.0f;       m.m[1][3] = m_translate.y;
  m.m[2][0] = 0.0f;       m.m[2][1] = 0.0f;       m.m[2][2] = 1.0f;       m.m[2][3] = m_translate.z;
  m.m[3][0] = 0.0f;       m.m[3][1] = 0.0f;       m.m[3][2] = 0.0f;       m.m[3][3] = 1.0f;
}


const Matrix4f *Pipeline::GetTransformation()
{

  Matrix4f scale, rotate, translate;

  InitScale(scale);
  InitRotate(rotate);
  InitTranslate(translate);

  m_transformation = translate * rotate * scale;
  return &m_transformation;
}
