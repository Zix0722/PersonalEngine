#pragma once
#include <Engine/Math/Vec2.hpp>
#include "Engine/Math/Mat44.hpp"
#include "../Math/Vec3.hpp"
#include "../Math/EulerAngles.hpp"
#include "../Math/AABB2.hpp"

class Camera
{
public:
	enum Mode
	{
		eMode_Orthographic,
		eMode_Perspective,

		eMode_Count
	};


	Camera();
	Camera(float width, float length);
	~Camera();

	void ChangeSize(float width, float length);
	void Translate2D(Vec2 const& translation2D);
	float GetWidth() const;
	float GetLength() const;
	Vec2 GetPosAtUV(Vec2 uv) const;
	void SetPosition(Vec2 position);
	void SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight);
	Vec2 GetOrthoBottomLeft() const;
	Vec2 GetOrthoTopRight() const;

	void SetOrthographicView(Vec2 const& bottomLeft, Vec2 const& topRight, float near = 0.f, float far = 1.0f);
	void SetPerspectiveView(float aspect, float fov, float near, float far);

	float GetClipNear() const;
	float GetFOV() const;
	float GetRatio() const;

	Mat44 GetOrthographicMatrix() const;
	Mat44 GetPerspectiveMatrix() const;
	Mat44 GetProjectionMaxtrix() const;

	void SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis);
	Mat44 GetRenderMatrix() const;

	void SetTransform(Vec3 const& position, EulerAngles const& orientation);
	Mat44 GetViewMatrix() const;
	void SetViewport(Vec2 const& min, Vec2 const& max);

	Vec3 m_renderIBasis = Vec3(1.f, 0.f, 0.f);
	Vec3 m_renderJBasis = Vec3(0.f, 1.f, 0.f);
	Vec3 m_renderKBasis = Vec3(0.f, 0.f, 1.f);

	Vec3 m_position;
	EulerAngles m_orientation;

	AABB2 m_viewport;

private:
	Mode m_mode = eMode_Orthographic;

	Vec2 m_orthographicBottomLeft;
	Vec2 m_orthographicTopRight;
	Vec2 m_cameraPosition;
	float m_width = 0.f;
	float m_length = 0.f;

	float m_orthographicNear;
	float m_orthographicFar;

	float m_perspectiveAspect;
	float m_perspectiveFOV;
	float m_perspectiveNear;
	float m_perspectiveFar;

};