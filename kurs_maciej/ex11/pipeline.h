#ifndef PIPELINE_H
#define PIPELINE_H

#include "math_3d.h"

class Pipeline
{
  public:
    Pipeline()
    {
      m_scale     = Vector3f(1.0f, 1.0f, 1.0f);
      m_translate = Vector3f(0.0f, 0.0f, 0.0f);
      m_rotate    = Vector3f(0.0f, 0.0f, 0.0f);
      m_transformation.InitIdentity();
    }

    void Scale(float x, float y, float z)
    {
      m_scale.x = x;
      m_scale.y = y;
      m_scale.z = z;
    }

    void Translate(float x, float y, float z)
    {
      m_translate.x = x;
      m_translate.y = y;
      m_translate.z = z;
    }

    void Rotate(float x, float y, float z)
    {
      m_rotate.x = x;
      m_rotate.y = y;
      m_rotate.z = z;
    }

    const Matrix4f *GetTransformation();

  private:
    void InitScale(Matrix4f &m) const;
    void InitRotate(Matrix4f &m) const;
    void InitTranslate(Matrix4f &m) const;

    Vector3f m_scale;
    Vector3f m_translate;
    Vector3f m_rotate;

    Matrix4f m_transformation;

};

#endif
