#include "Engine/Math/Mat44.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "MathUtils.hpp"

Mat44::Mat44()
{
	m_values[Ix] = 1.f;
	m_values[Iy] = 0.f;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = 0.f;
	m_values[Jy] = 1.f;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;

	m_values[Kx] = 0.f;
	m_values[Ky] = 0.f;
	m_values[Kz] = 1.f;
	m_values[Kw] = 0.f;

	m_values[Tx] = 0.f;
	m_values[Ty] = 0.f;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;

	m_values[Kx] = 0.f;
	m_values[Ky] = 0.f;
	m_values[Kz] = 1.f;
	m_values[Kw] = 0.f;

	m_values[Tx] = translation2D.x;
	m_values[Ty] = translation2D.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;

	m_values[Tx] = translation3D.x;
	m_values[Ty] = translation3D.y;
	m_values[Tz] = translation3D.z;
	m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;

	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}

Mat44::Mat44(float const* sixteenValuesBasis)
{
	for (int valIndex = 0; valIndex < 16; ++valIndex)
	{
		m_values[valIndex] = sixteenValuesBasis[valIndex];
	}
}

Mat44 const Mat44::CreateTranslation2D(Vec2 const& translationXY)
{
	Mat44 result;
	result.SetTranslation2D(translationXY);
	return result;
}

Mat44 const Mat44::CreateTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 result;
	result.SetTranslation3D(translationXYZ);
	return result;
}

Mat44 const Mat44::CreateUniformScale2D(float uniformScaleXY)
{
	Mat44 result;
	result.m_values[Ix] *= uniformScaleXY;
	result.m_values[Jy] *= uniformScaleXY;
	return result;
}

Mat44 const Mat44::CreateUniformScale3D(float uniformScaleXYZ)
{
	Mat44 result;
	result.m_values[Ix] *= uniformScaleXYZ;
	result.m_values[Jy] *= uniformScaleXYZ;
	result.m_values[Kz] *= uniformScaleXYZ;
	return result;
}

Mat44 const Mat44::CreateNonUniformScale2D(Vec2 const& nonUniformscaleXY)
{
	Mat44 result;
	result.m_values[Ix] *= nonUniformscaleXY.x;
	result.m_values[Jy] *= nonUniformscaleXY.y;
	return result;
}

Mat44 const Mat44::CreateNonUniformScale3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 result;
	result.m_values[Ix] *= nonUniformScaleXYZ.x;
	result.m_values[Jy] *= nonUniformScaleXYZ.y;
	result.m_values[Kz] *= nonUniformScaleXYZ.z;
	return result;
}

Mat44 const Mat44::CreateZRotationDegrees(float rotationDegreesAboutZ)
{
	float cosine = CosDegrees(rotationDegreesAboutZ);
	float sine = SinDegrees(rotationDegreesAboutZ);

	Mat44 result;
	result.m_values[Ix] = cosine;
	result.m_values[Iy] = sine;
	result.m_values[Jx] = -sine;
	result.m_values[Jy] = cosine;
	return result;
}

Mat44 const Mat44::CreateYRotationDegrees(float rotationDegreesAboutY)
{
	float cosine = CosDegrees(rotationDegreesAboutY);
	float sine = SinDegrees(rotationDegreesAboutY);

	Mat44 result;
	result.m_values[Ix] = cosine;
	result.m_values[Iz] = -sine;
	result.m_values[Kx] = sine;
	result.m_values[Kz] = cosine;
	return result;
}

Mat44 const Mat44::CreateXRotationDegrees(float rotationDegreesAboutX)
{
	float cosine = CosDegrees(rotationDegreesAboutX);
	float sine = SinDegrees(rotationDegreesAboutX);

	Mat44 result;
	result.m_values[Jy] = cosine;
	result.m_values[Jz] = sine;
	result.m_values[Ky] = -sine;
	result.m_values[Kz] = cosine;
	return result;
}

Mat44 const Mat44::CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float mat[] =
	{
		2.f / (right - left),				0.f,								0.f,									0.f,
		0.f,								2.f / (top - bottom),				0.f,									0.f,
		0.f,								0.f,								1.f / (zFar - zNear),					0.f,
		(left + right) / (left - right),	(bottom + top) / (bottom - top)	,	-zNear / (zFar - zNear),				1.f,
	};
	return Mat44(mat);
}

Mat44 const Mat44::CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar)
{
	Mat44 result;
	float scaleY = CosDegrees(fovYDegrees * 0.5f) / SinDegrees(fovYDegrees * 0.5f);
	float scaleX = scaleY / aspect;
	float scaleZ = zFar / (zFar - zNear);
	float translateZ = (zNear * zFar) / (zNear - zFar);
	result.m_values[Jy] = scaleY;
	result.m_values[Ix] = scaleX;
	result.m_values[Kz] = scaleZ;
	result.m_values[Kw] = 1.0f;
	result.m_values[Tz] = translateZ;
	result.m_values[Tw] = 0.0f;
	return result;
}

void Mat44::SetTranslation2D(Vec2 const& translationXY)
{
	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetTranslation3D(Vec3 const& translationXYZ)
{
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;
}

void Mat44::SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;

	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;
}

void Mat44::SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;

	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;

	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;

	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;

	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}

void Mat44::Transpose()
{
	for (int i = 0; i < 3; i++) 
	{
		for (int j = i + 1; j < 4; j++)
		{
			int idxA = 4 * i + j;
			int idxB = 4 * j + i;
			float temp = m_values[idxA];
			m_values[idxA] = m_values[idxB];
			m_values[idxB] = temp;
		}
	}
}

void Mat44::Orthonormalize_XFwd_YLeft_ZUp()
{
	Vec3 I = GetIBasis3D();
	Vec3 J = GetJBasis3D();
	Vec3 K = GetKBasis3D();
	I = I.GetNormalized();
	m_values[Kx] = DotProduct3D(K, I);
	Vec3 Ki = m_values[Kx] * I;
	K -= Ki;
	SetIJKT3D(I, J, K, GetTranslation3D());
	I = GetIBasis3D();
	J = GetJBasis3D();
	K = GetKBasis3D();
	K = K.GetNormalized();
	m_values[Jx] = DotProduct3D(J, I);
	Vec3 Ji = m_values[Jx] * I;
	J -= Ji;
	Vec3 Jk = DotProduct3D(J, K) * K;
	J -= Jk;
	J = J.GetNormalized();
	SetIJKT3D(I, J, K, GetTranslation3D());
}

void Mat44::Append(Mat44 const& appendThis)
{
	Mat44 oldMatrix(&m_values[Ix]);

	m_values[Ix] = oldMatrix.m_values[Ix] * appendThis.m_values[Ix] + oldMatrix.m_values[Jx] * appendThis.m_values[Iy] + oldMatrix.m_values[Kx] * appendThis.m_values[Iz] + oldMatrix.m_values[Tx] * appendThis.m_values[Iw];
	m_values[Iy] = oldMatrix.m_values[Iy] * appendThis.m_values[Ix] + oldMatrix.m_values[Jy] * appendThis.m_values[Iy] + oldMatrix.m_values[Ky] * appendThis.m_values[Iz] + oldMatrix.m_values[Ty] * appendThis.m_values[Iw];
	m_values[Iz] = oldMatrix.m_values[Iz] * appendThis.m_values[Ix] + oldMatrix.m_values[Jz] * appendThis.m_values[Iy] + oldMatrix.m_values[Kz] * appendThis.m_values[Iz] + oldMatrix.m_values[Tz] * appendThis.m_values[Iw];
	m_values[Iw] = oldMatrix.m_values[Iw] * appendThis.m_values[Ix] + oldMatrix.m_values[Jw] * appendThis.m_values[Iy] + oldMatrix.m_values[Kw] * appendThis.m_values[Iz] + oldMatrix.m_values[Tw] * appendThis.m_values[Iw];

	m_values[Jx] = oldMatrix.m_values[Ix] * appendThis.m_values[Jx] + oldMatrix.m_values[Jx] * appendThis.m_values[Jy] + oldMatrix.m_values[Kx] * appendThis.m_values[Jz] + oldMatrix.m_values[Tx] * appendThis.m_values[Jw];
	m_values[Jy] = oldMatrix.m_values[Iy] * appendThis.m_values[Jx] + oldMatrix.m_values[Jy] * appendThis.m_values[Jy] + oldMatrix.m_values[Ky] * appendThis.m_values[Jz] + oldMatrix.m_values[Ty] * appendThis.m_values[Jw];
	m_values[Jz] = oldMatrix.m_values[Iz] * appendThis.m_values[Jx] + oldMatrix.m_values[Jz] * appendThis.m_values[Jy] + oldMatrix.m_values[Kz] * appendThis.m_values[Jz] + oldMatrix.m_values[Tz] * appendThis.m_values[Jw];
	m_values[Jw] = oldMatrix.m_values[Iw] * appendThis.m_values[Jx] + oldMatrix.m_values[Jw] * appendThis.m_values[Jy] + oldMatrix.m_values[Kw] * appendThis.m_values[Jz] + oldMatrix.m_values[Tw] * appendThis.m_values[Jw];

	m_values[Kx] = oldMatrix.m_values[Ix] * appendThis.m_values[Kx] + oldMatrix.m_values[Jx] * appendThis.m_values[Ky] + oldMatrix.m_values[Kx] * appendThis.m_values[Kz] + oldMatrix.m_values[Tx] * appendThis.m_values[Kw];
	m_values[Ky] = oldMatrix.m_values[Iy] * appendThis.m_values[Kx] + oldMatrix.m_values[Jy] * appendThis.m_values[Ky] + oldMatrix.m_values[Ky] * appendThis.m_values[Kz] + oldMatrix.m_values[Ty] * appendThis.m_values[Kw];
	m_values[Kz] = oldMatrix.m_values[Iz] * appendThis.m_values[Kx] + oldMatrix.m_values[Jz] * appendThis.m_values[Ky] + oldMatrix.m_values[Kz] * appendThis.m_values[Kz] + oldMatrix.m_values[Tz] * appendThis.m_values[Kw];
	m_values[Kw] = oldMatrix.m_values[Iw] * appendThis.m_values[Kx] + oldMatrix.m_values[Jw] * appendThis.m_values[Ky] + oldMatrix.m_values[Kw] * appendThis.m_values[Kz] + oldMatrix.m_values[Tw] * appendThis.m_values[Kw];

	m_values[Tx] = oldMatrix.m_values[Ix] * appendThis.m_values[Tx] + oldMatrix.m_values[Jx] * appendThis.m_values[Ty] + oldMatrix.m_values[Kx] * appendThis.m_values[Tz] + oldMatrix.m_values[Tx] * appendThis.m_values[Tw];
	m_values[Ty] = oldMatrix.m_values[Iy] * appendThis.m_values[Tx] + oldMatrix.m_values[Jy] * appendThis.m_values[Ty] + oldMatrix.m_values[Ky] * appendThis.m_values[Tz] + oldMatrix.m_values[Ty] * appendThis.m_values[Tw];
	m_values[Tz] = oldMatrix.m_values[Iz] * appendThis.m_values[Tx] + oldMatrix.m_values[Jz] * appendThis.m_values[Ty] + oldMatrix.m_values[Kz] * appendThis.m_values[Tz] + oldMatrix.m_values[Tz] * appendThis.m_values[Tw];
	m_values[Tw] = oldMatrix.m_values[Iw] * appendThis.m_values[Tx] + oldMatrix.m_values[Jw] * appendThis.m_values[Ty] + oldMatrix.m_values[Kw] * appendThis.m_values[Tz] + oldMatrix.m_values[Tw] * appendThis.m_values[Tw];
}

void Mat44::AppendZRotation(float degreesRotationAboutZ)
{
	float c = CosDegrees(degreesRotationAboutZ);
	float s = SinDegrees(degreesRotationAboutZ);
	Vec4 oldIBasis(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
	Vec4 oldJBasis(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);

	m_values[Ix] = oldIBasis.x * c + oldJBasis.x * s;
	m_values[Iy] = oldIBasis.y * c + oldJBasis.y * s;
	m_values[Iz] = oldIBasis.z * c + oldJBasis.z * s;
	m_values[Iw] = oldIBasis.w * c + oldJBasis.w * s;

	m_values[Jx] = oldIBasis.x * (-s) + oldJBasis.x * c;
	m_values[Jy] = oldIBasis.y * (-s) + oldJBasis.y * c;
	m_values[Jz] = oldIBasis.z * (-s) + oldJBasis.z * c;
	m_values[Jw] = oldIBasis.w * (-s) + oldJBasis.w * c;
}

void Mat44::AppendYRotation(float degreesRotationAboutY)
{
	float c = CosDegrees(degreesRotationAboutY);
	float s = SinDegrees(degreesRotationAboutY);

	Vec4 oldIBasis(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
	Vec4 oldKBasis(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);

	m_values[Ix] = oldIBasis.x * c + oldKBasis.x * (-s);
	m_values[Iy] = oldIBasis.y * c + oldKBasis.y * (-s);
	m_values[Iz] = oldIBasis.z * c + oldKBasis.z * (-s);
	m_values[Iw] = oldIBasis.w * c + oldKBasis.w * (-s);

	m_values[Kx] = oldIBasis.x * s + oldKBasis.x * c;
	m_values[Ky] = oldIBasis.y * s + oldKBasis.y * c;
	m_values[Kz] = oldIBasis.z * s + oldKBasis.z * c;
	m_values[Kw] = oldIBasis.w * s + oldKBasis.w * c;
}

void Mat44::AppendXRotation(float degreesRotationAboutX)
{
	float c = CosDegrees(degreesRotationAboutX);
	float s = SinDegrees(degreesRotationAboutX);
	//c, -s
	//s,  c
	Vec4 oldJBasis(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
	Vec4 oldKBasis(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);

	m_values[Jx] = oldJBasis.x * c + oldKBasis.x * s;
	m_values[Jy] = oldJBasis.y * c + oldKBasis.y * s;
	m_values[Jz] = oldJBasis.z * c + oldKBasis.z * s;
	m_values[Jw] = oldJBasis.w * c + oldKBasis.w * s;

	m_values[Kx] = oldJBasis.x * (-s) + oldKBasis.x * c;
	m_values[Ky] = oldJBasis.y * (-s) + oldKBasis.y * c;
	m_values[Kz] = oldJBasis.z * (-s) + oldKBasis.z * c;
	m_values[Kw] = oldJBasis.w * (-s) + oldKBasis.w * c;
}

void Mat44::AppendTranslation2D(Vec2 const& translationXY)
{
	m_values[Tx] = translationXY.x * m_values[Ix] + translationXY.y * m_values[Jx] + m_values[Tx];
	m_values[Ty] = translationXY.x * m_values[Iy] + translationXY.y * m_values[Jy] + m_values[Ty];
	m_values[Tz] = translationXY.x * m_values[Iz] + translationXY.y * m_values[Jz] + m_values[Tz];
	m_values[Tw] = translationXY.x * m_values[Iw] + translationXY.y * m_values[Jw] + m_values[Tw];
}

void Mat44::AppendTranslation3D(Vec3 const& translationXYZ)
{
	m_values[Tx] = translationXYZ.x * m_values[Ix] + translationXYZ.y * m_values[Jx] + translationXYZ.z * m_values[Kx] + m_values[Tx];
	m_values[Ty] = translationXYZ.x * m_values[Iy] + translationXYZ.y * m_values[Jy] + translationXYZ.z * m_values[Ky] + m_values[Ty];
	m_values[Tz] = translationXYZ.x * m_values[Iz] + translationXYZ.y * m_values[Jz] + translationXYZ.z * m_values[Kz] + m_values[Tz];
	m_values[Tw] = translationXYZ.x * m_values[Iw] + translationXYZ.y * m_values[Jw] + translationXYZ.z * m_values[Kw] + m_values[Tw];
}

void Mat44::AppendScaleUniform2D(float uniformScaleXY)
{
	m_values[Ix] *= uniformScaleXY;
	m_values[Iy] *= uniformScaleXY;
	m_values[Iz] *= uniformScaleXY;
	m_values[Iw] *= uniformScaleXY;

	m_values[Jx] *= uniformScaleXY;
	m_values[Jy] *= uniformScaleXY;
	m_values[Jz] *= uniformScaleXY;
	m_values[Jw] *= uniformScaleXY;
}

void Mat44::AppendScaleUniform3D(float uniformScaleXYZ)
{
	m_values[Ix] = uniformScaleXYZ * m_values[Ix];
	m_values[Iy] = uniformScaleXYZ * m_values[Iy];
	m_values[Iz] = uniformScaleXYZ * m_values[Iz];
	m_values[Iw] = uniformScaleXYZ * m_values[Iw];
									 
	m_values[Jx] = uniformScaleXYZ * m_values[Jx];
	m_values[Jy] = uniformScaleXYZ * m_values[Jy];
	m_values[Jz] = uniformScaleXYZ * m_values[Jz];
	m_values[Jw] = uniformScaleXYZ * m_values[Jw];
									 
	m_values[Kx] = uniformScaleXYZ * m_values[Kx];
	m_values[Ky] = uniformScaleXYZ * m_values[Ky];
	m_values[Kz] = uniformScaleXYZ * m_values[Kz];
	m_values[Kw] = uniformScaleXYZ * m_values[Kw];
}

void Mat44::AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY)
{
	m_values[Ix] = nonUniformScaleXY.x * m_values[Ix];
	m_values[Iy] = nonUniformScaleXY.x * m_values[Iy];
	m_values[Iz] = nonUniformScaleXY.x * m_values[Iz];
	m_values[Iw] = nonUniformScaleXY.x * m_values[Iw];
													
	m_values[Jx] = nonUniformScaleXY.y * m_values[Jx];
	m_values[Jy] = nonUniformScaleXY.y * m_values[Jy];
	m_values[Jz] = nonUniformScaleXY.y * m_values[Jz];
	m_values[Jw] = nonUniformScaleXY.y * m_values[Jw];
}

void Mat44::AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ)
{
	m_values[Ix] = nonUniformScaleXYZ.x * m_values[Ix];
	m_values[Iy] = nonUniformScaleXYZ.x * m_values[Iy];
	m_values[Iz] = nonUniformScaleXYZ.x * m_values[Iz];
	m_values[Iw] = nonUniformScaleXYZ.x * m_values[Iw];
			   									  
	m_values[Jx] = nonUniformScaleXYZ.y * m_values[Jx];
	m_values[Jy] = nonUniformScaleXYZ.y * m_values[Jy];
	m_values[Jz] = nonUniformScaleXYZ.y * m_values[Jz];
	m_values[Jw] = nonUniformScaleXYZ.y * m_values[Jw];
			   									  
	m_values[Kx] = nonUniformScaleXYZ.z * m_values[Kx];
	m_values[Ky] = nonUniformScaleXYZ.z * m_values[Ky];
	m_values[Kz] = nonUniformScaleXYZ.z * m_values[Kz];
	m_values[Kw] = nonUniformScaleXYZ.z * m_values[Kw];
}

Vec2 const Mat44::TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const
{
	return Vec2(m_values[Ix] * vectorQuantityXY.x + m_values[Jx] * vectorQuantityXY.y,
				m_values[Iy] * vectorQuantityXY.x + m_values[Jy] * vectorQuantityXY.y);
}

Vec3 const Mat44::TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const
{
	return Vec3(m_values[Ix] * vectorQuantityXYZ.x + m_values[Jx] * vectorQuantityXYZ.y + m_values[Kx] * vectorQuantityXYZ.z,
				m_values[Iy] * vectorQuantityXYZ.x + m_values[Jy] * vectorQuantityXYZ.y + m_values[Ky] * vectorQuantityXYZ.z,
				m_values[Iz] * vectorQuantityXYZ.x + m_values[Jz] * vectorQuantityXYZ.y + m_values[Kz] * vectorQuantityXYZ.z);
}

Vec2 const Mat44::TransformPosition2D(Vec2 const& positionXY) const
{
	return Vec2(m_values[Ix] * positionXY.x + m_values[Jx] * positionXY.y + m_values[Tx],
				m_values[Iy] * positionXY.x + m_values[Jy] * positionXY.y + m_values[Ty]);
}

Vec3 const Mat44::TransformPosition3D(Vec3 const& position3D) const
{
	return Vec3(m_values[Ix] * position3D.x + m_values[Jx] * position3D.y + m_values[Kx] * position3D.z + m_values[Tx],
				m_values[Iy] * position3D.x + m_values[Jy] * position3D.y + m_values[Ky] * position3D.z + m_values[Ty],
				m_values[Iz] * position3D.x + m_values[Jz] * position3D.y + m_values[Kz] * position3D.z + m_values[Tz]);
}

Vec4 const Mat44::TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const
{
	return Vec4(m_values[Ix] * homogeneousPoint3D.x + m_values[Jx] * homogeneousPoint3D.y + m_values[Kx] * homogeneousPoint3D.z + m_values[Tx] * homogeneousPoint3D.w,
				m_values[Iy] * homogeneousPoint3D.x + m_values[Jy] * homogeneousPoint3D.y + m_values[Ky] * homogeneousPoint3D.z + m_values[Ty] * homogeneousPoint3D.w,
				m_values[Iz] * homogeneousPoint3D.x + m_values[Jz] * homogeneousPoint3D.y + m_values[Kz] * homogeneousPoint3D.z + m_values[Tz] * homogeneousPoint3D.w,
				m_values[Iw] * homogeneousPoint3D.x + m_values[Jw] * homogeneousPoint3D.y + m_values[Kw] * homogeneousPoint3D.z + m_values[Tw] * homogeneousPoint3D.w);
}

float* Mat44::GetAsFloatArray()
{
	return &m_values[0];
}

float const* Mat44::GetAsFloatArray() const
{
	return &m_values[0];
}

Vec2 const Mat44::GetIBasis2D() const
{
	Vec2 iBasis(m_values[Ix], m_values[Iy]);
	return iBasis;
}

Vec2 const Mat44::GetJBasis2D() const
{
	Vec2 jBasis(m_values[Jx], m_values[Jy]);
	return jBasis;
}

Vec2 const Mat44::GetTranslation2D() const
{
	Vec2 translated2D(m_values[Tx], m_values[Ty]);
	return translated2D;
}

Vec3 const Mat44::GetIBasis3D() const
{
	Vec3 iBasis(m_values[Ix], m_values[Iy], m_values[Iz]);
	return iBasis;
}

Vec3 const Mat44::GetJBasis3D() const
{
	Vec3 jBasis(m_values[Jx], m_values[Jy], m_values[Jz]);
	return jBasis;
}

Vec3 const Mat44::GetKBasis3D() const
{
	Vec3 kBasis(m_values[Kx], m_values[Ky], m_values[Kz]);
	return kBasis;
}

Vec3 const Mat44::GetTranslation3D() const
{
	Vec3 translated3D(m_values[Tx], m_values[Ty], m_values[Tz]);
	return translated3D;
}

Vec4 const Mat44::GetIBasis4D() const
{
	Vec4 iBasis(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
	return iBasis;
}

Vec4 const Mat44::GetJBasis4D() const
{
	Vec4 jBasis(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
	return jBasis;
}

Vec4 const Mat44::GetKBasis4D() const
{
	Vec4 kBasis(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);
	return kBasis;
}

Vec4 const Mat44::GetTranslation4D() const
{
	Vec4 translated4D(m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]);
	return translated4D;
}

Mat44 const Mat44::GetOrthonormalInverse() const
{
	Mat44 result;
	float mat[] = 
	{
		m_values[Ix],				m_values[Iy],				m_values[Iz],					m_values[Iw],
		m_values[Jx],				m_values[Jy],				m_values[Jz],					m_values[Jw],
		m_values[Kx],				m_values[Ky],				m_values[Kz],					m_values[Kw],
		0.f,						0.f,						0.f,							1.f,
	};
	result = Mat44(mat);
	result.Transpose();
	Mat44 translateMatt;

	translateMatt.m_values[Tx] = -m_values[Tx];
	translateMatt.m_values[Ty] = -m_values[Ty];
	translateMatt.m_values[Tz] = -m_values[Tz];
	result.Append(translateMatt);

	return result;
}
