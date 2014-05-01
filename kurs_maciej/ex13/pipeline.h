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

    void SetPerspectiveProj(float FOV, float Width, float Height, float NearZ, float FarZ)
    {
      m_persProj.FOV    = FOV;
      m_persProj.Width  = Width;
      m_persProj.Height = Height;
      m_persProj.NearZ  = NearZ;
      m_persProj.FarZ   = FarZ;
    }

    void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
    {
      m_camera.Pos = Pos;
      m_camera.Target = Target;
      m_camera.Up = Up;
    }

    const Matrix4f *GetTransformation();

  private:

    Vector3f m_scale;
    Vector3f m_translate;
    Vector3f m_rotate;

    struct {
      float FOV;
      float Width;
      float Height;
      float NearZ;
      float FarZ;
    } m_persProj;

    struct {
      Vector3f Pos;
      Vector3f Target;
      Vector3f Up;
    } m_camera;

    Matrix4f m_transformation;

};

#endif
