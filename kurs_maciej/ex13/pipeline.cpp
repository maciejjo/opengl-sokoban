#include "pipeline.h"

const Matrix4f *Pipeline::GetTransformation()
{

  Matrix4f scale, rotate, translate, cam_translate, cam_rotate, perspective;

  scale.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
  rotate.InitRotateTransform(m_rotate.x, m_rotate.y, m_rotate.z);
  translate.InitTranslateTransform(m_translate.x, m_translate.y, m_translate.z);
  cam_translate.InitTranslateTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
  cam_rotate.InitCameraTransform(m_camera.Target, m_camera.Up);
  perspective.InitPersProjTransform(m_persProj.FOV, m_persProj.Width, m_persProj.Height, m_persProj.NearZ, m_persProj.FarZ);

  m_transformation = perspective * cam_rotate * cam_translate * translate * rotate * scale;
  return &m_transformation;
}
