#include "EulerAngles.hpp"
#include "Engine/Math/Vec3.hpp"

EulerAngles::EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees)
	:m_rollDegrees(rollDegrees)
	,m_yawDegrees(yawDegrees)
	,m_pitchDegrees(pitchDegrees)
{

}

EulerAngles::EulerAngles(Vec3 normalizedDirection)
{
	m_yawDegrees = normalizedDirection.GetAngleAboutZDegrees();
	m_pitchDegrees = 0.f;
	m_rollDegrees = 0.f;
}

void EulerAngles::GetAsVectors_XFwd_YLeft_ZUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis) const
{
	Mat44 newMat;
	newMat.AppendZRotation(m_yawDegrees);
	newMat.AppendYRotation(m_pitchDegrees);
	newMat.AppendXRotation(m_rollDegrees);

	out_forwardIBasis = newMat.GetIBasis3D();
	out_leftJBasis = newMat.GetJBasis3D();
	out_upKBasis = newMat.GetKBasis3D();
}

Mat44 EulerAngles::GetAsMatrix_XFwd_YLeft_ZUp() const
{
	Mat44 newMat;
	newMat.AppendZRotation(m_yawDegrees);
	newMat.AppendYRotation(m_pitchDegrees);
	newMat.AppendXRotation(m_rollDegrees);

	return newMat;
}
