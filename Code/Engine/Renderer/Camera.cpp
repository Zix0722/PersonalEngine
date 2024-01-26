#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/Mat44.hpp"
#include "../Math/EulerAngles.hpp"
#include <corecrt_math.h>
#include <string>
#include "DebugRender.hpp"

Camera::Camera()
{
	m_cameraPosition = Vec2(0.f, 0.f);
	
}

Camera::Camera(float width, float length)
	:m_width(width)
	,m_length(length)
{
	m_cameraPosition = Vec2(0.f, 0.f);
}

Camera::~Camera()
{
}

void Camera::ChangeSize(float width, float length)
{
	m_width = width;
	m_length = length;
}

void Camera::Translate2D(Vec2 const& translation2D)
{
	float uniformScale = 1.f;
	float rotatedDegrees = 0.f;
	Vec2 targetPos = m_cameraPosition;
	TransformPosition2D(targetPos, uniformScale, rotatedDegrees, translation2D);
	SetPosition(targetPos);
}

float Camera::GetWidth() const
{
	return m_width;
}

float Camera::GetLength() const
{
	return m_length;
}



Vec2 Camera::GetPosAtUV(Vec2 uv) const
{
	Vec2 result;
	float width = GetWidth();
	float length = GetLength();
	float xOffset = width * uv.x;
	float yOffset = length * uv.y;

	result = m_orthographicBottomLeft + Vec2(xOffset, yOffset);
	return result;
}

void Camera::SetPosition(Vec2 position)
{
	Vec2 displacement = position - m_cameraPosition;
	Vec2 BL = GetOrthoBottomLeft();
	Vec2 TR = GetOrthoTopRight();
	
	SetOrthographicView(Vec2(BL + displacement), Vec2(TR + displacement));
}

//Only used in 2D
void Camera::SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight)
{
	m_width = topRight.x - bottomLeft.x;
	m_length = topRight.y - bottomLeft.y;

	m_cameraPosition.x = bottomLeft.x + m_width / 2;
	m_cameraPosition.y = bottomLeft.y + m_length / 2;
}

Vec2 Camera::GetOrthoBottomLeft() const
{
	Vec2 result(m_cameraPosition.x-m_width/2, m_cameraPosition.y - m_length /2);
	
	return result;
}

Vec2 Camera::GetOrthoTopRight() const
{
	Vec2 result(m_cameraPosition.x + m_width /2, m_cameraPosition.y + m_length/2);

	return result;
}

void Camera::SetOrthographicView(Vec2 const& bottomLeft, Vec2 const& topRight, float near /*= 0.f*/, float far /*= 1.0f*/)
{
	m_orthographicBottomLeft = bottomLeft;
	m_orthographicTopRight = topRight;
	m_orthographicNear = near;
	m_orthographicFar = far;
	SetOrthoView(m_orthographicBottomLeft, m_orthographicTopRight);
	m_mode = eMode_Orthographic;
}

void Camera::SetPerspectiveView(float aspect, float fov, float near, float far)
{
	m_perspectiveAspect = aspect;
	m_perspectiveFOV = fov;
	m_perspectiveNear = near;
	m_perspectiveFar = far;
	m_mode = eMode_Perspective;
}

float Camera::GetClipNear() const
{
	return m_perspectiveNear;
}

float Camera::GetFOV() const
{
	//float tanHalfHorizontalFOV = tanf(m_perspectiveFOV / 2.f);
	//float tanHalfVerticalFOV = tanHalfHorizontalFOV * GetRatio();
	//float verticalFOV = 2.0f * atanf(tanHalfVerticalFOV);
	return m_perspectiveFOV;
	//return verticalFOV;
}

float Camera::GetRatio() const
{
	return m_perspectiveAspect;
}

Mat44 Camera::GetOrthographicMatrix() const
{
	return Mat44::CreateOrthoProjection(m_orthographicBottomLeft.x, m_orthographicTopRight.x, m_orthographicBottomLeft.y, m_orthographicTopRight.y, m_orthographicNear, m_orthographicFar);
}

Mat44 Camera::GetPerspectiveMatrix() const
{
	return Mat44::CreatePerspectiveProjection(m_perspectiveFOV, m_perspectiveAspect, m_perspectiveNear, m_perspectiveFar);
}

Mat44 Camera::GetProjectionMaxtrix() const
{
	if (m_mode == eMode_Orthographic)
	{
		Mat44 result = GetOrthographicMatrix();
		result.Append(GetRenderMatrix());
		return result;
	}
	else if (m_mode == eMode_Perspective)
	{
		Mat44 result = GetPerspectiveMatrix();
		result.Append(GetRenderMatrix());
		return result;
	}
	return Mat44();
}

void Camera::SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis)
{
	m_renderIBasis = iBasis;
	m_renderJBasis = jBasis;
	m_renderKBasis = kBasis;
}

Mat44 Camera::GetRenderMatrix() const
{
	Mat44 result;
	result.SetIJK3D(m_renderIBasis, m_renderJBasis, m_renderKBasis);
	return result;
}

void Camera::SetTransform(Vec3 const& position, EulerAngles const& orientation)
{
	m_orientation = orientation;
	m_position = position;
}

Mat44 Camera::GetViewMatrix() const
{
	Mat44 result;
	result = m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
	result.SetTranslation3D(m_position);
	result = result.GetOrthonormalInverse();
	return result;
}



void Camera::SetViewport(Vec2 const& min, Vec2 const& max)
{
	m_viewport.m_mins = min;
	m_viewport.m_maxs = max;
}
