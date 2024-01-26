#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/Vec2.hpp"
#include "VertexUtils.hpp"


void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float scaleXY, float rotationDegreesAboutZ, Vec2 const& translationXY)
{
	for (int vertIndex = 0; vertIndex < numVerts; ++vertIndex)
	{
		Vec3& pos = verts[vertIndex].m_position;
		TransformPositionXY3D(pos, scaleXY, rotationDegreesAboutZ, translationXY);
	}
}



void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, Mat44 const& transform)
{
	Vec3 translation = transform.GetTranslation3D();
//  Vec3 iBasis = transform.GetIBasis3D();
// 	Vec3 jBasis = transform.GetJBasis3D();
// 	Vec3 kBasis = transform.GetKBasis3D();
	for (int vertIndex = 0; vertIndex < verts.size(); ++vertIndex)
	{
		Vec3& pos = verts[vertIndex].m_position;
		pos = transform.TransformVectorQuantity3D(pos);
		pos += translation;
		//TransformPosition3D(pos, iBasis, jBasis, kBasis, translation);
	}
}

void AddVertsForAABB2(std::vector<Vertex_PCU>& mainVertexList, AABB2 const& aabb2, Rgba8 const& color, Vec2 uvAtMins, Vec2 uvAtMaxs)
{
	mainVertexList.push_back(Vertex_PCU(Vec2(aabb2.m_mins.x, aabb2.m_mins.y), color, Vec2(uvAtMins.x, uvAtMins.y)));
	mainVertexList.push_back(Vertex_PCU(Vec2(aabb2.m_maxs.x, aabb2.m_mins.y), color, Vec2(uvAtMaxs.x, uvAtMins.y)));
	mainVertexList.push_back(Vertex_PCU(Vec2(aabb2.m_mins.x, aabb2.m_maxs.y), color, Vec2(uvAtMins.x, uvAtMaxs.y))); 

	mainVertexList.push_back(Vertex_PCU(Vec2(aabb2.m_maxs.x, aabb2.m_maxs.y), color, Vec2(uvAtMaxs.x, uvAtMaxs.y)));
	mainVertexList.push_back(Vertex_PCU(Vec2(aabb2.m_mins.x, aabb2.m_maxs.y), color, Vec2(uvAtMins.x, uvAtMaxs.y)));
	mainVertexList.push_back(Vertex_PCU(Vec2(aabb2.m_maxs.x, aabb2.m_mins.y), color, Vec2(uvAtMaxs.x, uvAtMins.y)));
}

void AddVertsForLine2(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& color)
{
	float halfThickness = 0.5f * thickness;

	Vec2 D = endPos - startPos;
	Vec2 F = D.GetNormalized();
	Vec2 FS = F * halfThickness;
	Vec2 LS = FS.GetRotated90Degrees();

	Vec2 EL = endPos + FS + LS;
	Vec2 ER = endPos + FS - LS;
	Vec2 SL = startPos - FS + LS;
	Vec2 SR = startPos - FS - LS;

	constexpr int NUM_VERTS = 3 * 2;
	Vertex_PCU vert[NUM_VERTS];

	vert[0].m_position = Vec3(SL.x, SL.y, 0.f);
	vert[1].m_position = Vec3(SR.x, SR.y, 0.f);
	vert[2].m_position = Vec3(ER.x, ER.y, 0.f);

	vert[3].m_position = Vec3(ER.x, ER.y, 0.f);
	vert[4].m_position = Vec3(EL.x, EL.y, 0.f);
	vert[5].m_position = Vec3(SL.x, SL.y, 0.f);

	for (int vertIndex = 0; vertIndex < NUM_VERTS; ++vertIndex)
	{
		vert[vertIndex].m_color = color;
	}

	AddVertsFormList(mainVertexList, vert, NUM_VERTS);
}
void AddVertsForRing2(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& center, float radius, float thickness, Rgba8 const& color)
{
	float halfThickness = 0.5f * thickness;
	float innerRadius = radius - halfThickness;
	float outerRadius = radius + halfThickness;

	constexpr int NUM_SIDES = 32;
	constexpr int NUM_TRIS = 2 * NUM_SIDES;
	constexpr int NUM_VERTS = 3 * NUM_TRIS;
	Vertex_PCU vert[NUM_VERTS];

	constexpr float DEGREES_PER_SIDE = 360.f / (float)NUM_SIDES;

	for (int sideNum = 0; sideNum < NUM_SIDES; ++sideNum)
	{
		float startDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum);
		float endDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum + 1);
		float cosStart = CosDegrees(startDegrees);
		float sinStart = SinDegrees(startDegrees);
		float cosEnd = CosDegrees(endDegrees);
		float sinEnd = SinDegrees(endDegrees);

		Vec3 innerStartPos = Vec3(center.x + innerRadius * cosStart, center.y + innerRadius * sinStart, 0.f);
		Vec3 outerStartPos = Vec3(center.x + outerRadius * cosStart, center.y + outerRadius * sinStart, 0.f);
		Vec3 outerEndPos = Vec3(center.x + outerRadius * cosEnd, center.y + outerRadius * sinEnd, 0.f);
		Vec3 innerEndPos = Vec3(center.x + innerRadius * cosEnd, center.y + innerRadius * sinEnd, 0.f);

		int vertIndexA = (6 * sideNum) + 0;
		int vertIndexB = (6 * sideNum) + 1;
		int vertIndexC = (6 * sideNum) + 2;
		int vertIndexD = (6 * sideNum) + 3;
		int vertIndexE = (6 * sideNum) + 4;
		int vertIndexF = (6 * sideNum) + 5;

		vert[vertIndexA].m_position = innerEndPos;
		vert[vertIndexB].m_position = innerStartPos;
		vert[vertIndexC].m_position = outerStartPos;
		vert[vertIndexA].m_color = color;
		vert[vertIndexB].m_color = color;
		vert[vertIndexC].m_color = color;

		vert[vertIndexD].m_position = innerEndPos;
		vert[vertIndexE].m_position = outerStartPos;
		vert[vertIndexF].m_position = outerEndPos;
		vert[vertIndexD].m_color = color;
		vert[vertIndexE].m_color = color;
		vert[vertIndexF].m_color = color;

		AddVertsFormList(mainVertexList, vert, NUM_VERTS);
	}
}

void AddVertsFormList(std::vector<Vertex_PCU>& mainVertexList, Vertex_PCU const* vertlist, int numVerts)
{
	for (int vertIndex = 0; vertIndex < numVerts; ++vertIndex, ++vertlist)
	{
		mainVertexList.push_back(*vertlist);
	}
}

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color)
{
	Vec2 endStartDisp = boneEnd - boneStart;
	Vec2 jNormal = endStartDisp.GetNormalized();
	Vec2 iNormal = jNormal.GetRotatedMinus90Degrees();

	float jLength = endStartDisp.GetLength();
	Vec2 halfDimensions = Vec2(radius, jLength * 0.5f);
	Vec2 obb2Center = boneStart + endStartDisp * 0.5f;

	OBB2 box = OBB2(obb2Center, iNormal, halfDimensions);
	AddVertsForOBB2D(mainVertexList, box, color, AABB2(0.f,0.f,1.f,1.f));
	AddVertsForSector2D(mainVertexList, boneStart, radius, -iNormal, 180.f, color);
	AddVertsForSector2D(mainVertexList, boneEnd, radius, iNormal, 180.f, color);

}

void AddVertsForDisc2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& center, float radius, Rgba8 const& color)
{
	float eachTriangleDegrees = 180.f / 64.f;

	for (float currentDegrees = 0.f; currentDegrees < 360.f; currentDegrees += eachTriangleDegrees)
	{
		Vec2 firstVertex = Vec2::MakeFromPolarDegrees(currentDegrees, radius) + center;
		Vec2 secVertex = Vec2::MakeFromPolarDegrees(currentDegrees + eachTriangleDegrees, radius) + center;

		mainVertexList.push_back(Vertex_PCU(Vec2(center), color, Vec2(0.f, 0.f)));
		mainVertexList.push_back(Vertex_PCU(Vec2(firstVertex), color, Vec2(0.f, 0.f)));
		mainVertexList.push_back(Vertex_PCU(Vec2(secVertex), color, Vec2(0.f, 0.f)));
	}
}

void AddVertsForSector2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& center, float radius, Vec2 const& startNormal, float sectorDegrees, Rgba8 const& color)
{
	float startDegrees = startNormal.GetOrientationDegrees();
	float eachTriangleDegrees = sectorDegrees / 64.f;

	for (float currentDegrees = startDegrees; currentDegrees < startDegrees + sectorDegrees; currentDegrees += eachTriangleDegrees)
	{
		Vec2 firstVertex = Vec2::MakeFromPolarDegrees(currentDegrees, radius) + center;
		Vec2 secVertex = Vec2::MakeFromPolarDegrees(currentDegrees + eachTriangleDegrees, radius) + center;

		mainVertexList.push_back(Vertex_PCU(Vec2(center), color, Vec2(0.f, 0.f)));
		mainVertexList.push_back(Vertex_PCU(Vec2(firstVertex), color, Vec2(0.f, 0.f)));
		mainVertexList.push_back(Vertex_PCU(Vec2(secVertex), color, Vec2(0.f, 0.f)));
	}
}

void AddVertsForOBB2D(std::vector<Vertex_PCU>& mainVertexList, OBB2 const& box, Rgba8 const& color, AABB2 const& uv)
{
	Vec2 const& center = box.m_center;
	Vec2 const& iBasis = box.m_iBasisNormal;
	Vec2 jBasis = box.GetJBasisNormal();

	Vec2 rightDisp = iBasis * box.m_halfDimensions.x;
	Vec2 upDisp = jBasis * box.m_halfDimensions.y;

	Vec2 TR = center + rightDisp + upDisp;
	Vec2 TL = center - rightDisp + upDisp;
	Vec2 BL = center - rightDisp - upDisp;
	Vec2 BR = center + rightDisp - upDisp;

	mainVertexList.push_back(Vertex_PCU(BL, color, Vec2(uv.m_mins.x, uv.m_mins.y)));
	mainVertexList.push_back(Vertex_PCU(BR, color, Vec2(uv.m_maxs.x, uv.m_mins.y)));
	mainVertexList.push_back(Vertex_PCU(TR, color, Vec2(uv.m_maxs.x, uv.m_maxs.y)));

	mainVertexList.push_back(Vertex_PCU(TR, color, Vec2(uv.m_maxs.x, uv.m_maxs.y)));
	mainVertexList.push_back(Vertex_PCU(TL, color, Vec2(uv.m_mins.x, uv.m_maxs.y)));
	mainVertexList.push_back(Vertex_PCU(BL, color, Vec2(uv.m_mins.x, uv.m_mins.y)));
}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color)
{
	AddVertsForLine2(mainVertexList, start, end, thickness, color);
}

void AddVertsForArrow2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& startPos, Vec2 const& endPos, float arrowSize, float lineThickness, Rgba8 const& color)
{
	Vertex_PCU tipVert[3];
	
	Vec2 negativeFwdNormal = (startPos - endPos).GetNormalized();

	Vec2 leftPointOfTheTip = negativeFwdNormal.GetRotatedDegrees(20.f);
	Vec2 rightPointOfTheTip = negativeFwdNormal.GetRotatedDegrees(-20.f);

	float leftPointOrientationDegrees = leftPointOfTheTip.GetOrientationDegrees();
	float rightPointOrientationDegrees = rightPointOfTheTip.GetOrientationDegrees();

	Vec2 leftPos = endPos + Vec2::MakeFromPolarDegrees(leftPointOrientationDegrees, 1.0f * arrowSize);
	Vec2 rightPos = endPos + Vec2::MakeFromPolarDegrees(rightPointOrientationDegrees, 1.0f * arrowSize);

	tipVert[0].m_position = Vec3(endPos.x, endPos.y, 0.f);
	tipVert[1].m_position = Vec3(rightPos.x, rightPos.y, 0.f);
	tipVert[2].m_position = Vec3(leftPos.x, leftPos.y, 0.f); 

	tipVert[0].m_color = color;
	tipVert[1].m_color = color;
	tipVert[2].m_color = color;

	AddVertsForLineSegment2D(mainVertexList, startPos, endPos, lineThickness, color);
	AddVertsFormList(mainVertexList, tipVert, 3);
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color /*= Rgba8::WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	mainVertexList.emplace_back(Vertex_PCU(bottomLeft, color, Vec2(UVs.m_mins.x, UVs.m_mins.y)));
	mainVertexList.emplace_back(Vertex_PCU(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y)));
	mainVertexList.emplace_back(Vertex_PCU(topRight, color, Vec2(UVs.m_maxs.x, UVs.m_maxs.y)));

	mainVertexList.emplace_back(Vertex_PCU(topRight, color, Vec2(UVs.m_maxs.x, UVs.m_maxs.y)));
	mainVertexList.emplace_back(Vertex_PCU(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y)));
	mainVertexList.emplace_back(Vertex_PCU(bottomLeft, color, Vec2(UVs.m_mins.x, UVs.m_mins.y)));
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& vertexes, std::vector<unsigned int>& indexes, Vec3 const& topLeft, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Rgba8 const& color /*= Rgba8::WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	unsigned int startIndex = (unsigned int)vertexes.size();
	vertexes.push_back(Vertex_PCU(bottomLeft, color, Vec2(UVs.m_mins.x, UVs.m_mins.y)));
	vertexes.push_back(Vertex_PCU(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y)));
	vertexes.push_back(Vertex_PCU(topRight, color, Vec2(UVs.m_maxs.x, UVs.m_maxs.y)));
	vertexes.push_back(Vertex_PCU(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y)));

	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 2);

	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 3);
}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	unsigned int startIndex = (unsigned int)vertexes.size();
	Vec3 uDirection = (bottomRight - bottomLeft).GetNormalized();
	Vec3 vDirection = (topLeft - bottomLeft).GetNormalized();
	Vec3 normal = CrossProduct3D(uDirection, vDirection).GetNormalized();

	vertexes.push_back(Vertex_PCUTBN(bottomLeft, color, Vec2(UVs.m_mins.x, UVs.m_mins.y), normal));
	vertexes.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), normal));
	vertexes.push_back(Vertex_PCUTBN(topRight, color, Vec2(UVs.m_maxs.x, UVs.m_maxs.y), normal));
	vertexes.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), normal));

	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 2);

	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 3);
}

void AddVertsForCube3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, AABB3 box, Rgba8 const& color /*= Rgba8::WHITE*/)
{
	Vec3 frontTopLeft = Vec3(box.m_mins.x, box.m_maxs.y, box.m_maxs.z);
	Vec3 frontTopRight = Vec3(box.m_mins.x, box.m_mins.y, box.m_maxs.z);
	Vec3 frontBottomLeft = Vec3(box.m_mins.x, box.m_maxs.y, box.m_mins.z);
	Vec3 frontBottomRight = Vec3(box.m_mins.x, box.m_mins.y, box.m_mins.z);
	Vec3 BackTopLeft = Vec3(box.m_maxs.x, box.m_maxs.y, box.m_maxs.z);
	Vec3 BackTopRight = Vec3(box.m_maxs.x, box.m_mins.y, box.m_maxs.z);
	Vec3 BackBottomLeft = Vec3(box.m_maxs.x, box.m_maxs.y, box.m_mins.z);
	Vec3 BackBottomRight = Vec3(box.m_maxs.x, box.m_mins.y, box.m_mins.z);

	AddVertsForQuad3D(verts, indexes, BackBottomRight, BackBottomLeft, BackTopLeft, BackTopRight, color);
	AddVertsForQuad3D(verts, indexes, frontBottomLeft, frontBottomRight, frontTopRight, frontTopLeft, color);
	AddVertsForQuad3D(verts, indexes, BackBottomLeft, frontBottomLeft, frontTopLeft, BackTopLeft, color);
	AddVertsForQuad3D(verts, indexes, frontBottomRight, BackBottomRight, BackTopRight, frontTopRight, color);
	AddVertsForQuad3D(verts, indexes, frontTopLeft, frontTopRight, BackTopRight, BackTopLeft, color);
	AddVertsForQuad3D(verts, indexes, BackBottomLeft, BackBottomRight, frontBottomRight, frontBottomLeft, color);
	CalculateTangents(verts, indexes);

}

void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& vertexes, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color /*= Rgba8::WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 midPointOfTop = (topLeft + topRight) * 0.5f;
	Vec3 midPointOfBott = (bottomLeft + bottomRight) * 0.5f;
	Vec2 UVTop =  Vec2((UVs.m_maxs.x - UVs.m_mins.x) * 0.5f + UVs.m_mins.x, UVs.m_maxs.y);
	Vec2 UVBott = Vec2((UVs.m_maxs.x - UVs.m_mins.x) * 0.5f + UVs.m_mins.x, UVs.m_mins.y);
	Vec3 uDirection = (bottomRight - bottomLeft).GetNormalized();
	Vec3 vDirection = (topLeft - bottomLeft).GetNormalized();
	Vec3 normal = CrossProduct3D(uDirection, vDirection).GetNormalized();

	vertexes.push_back(Vertex_PCUTBN(bottomLeft, color, Vec2(UVs.m_mins.x, UVs.m_mins.y), -uDirection));
	vertexes.push_back(Vertex_PCUTBN(midPointOfBott, color, UVBott, normal));
	vertexes.push_back(Vertex_PCUTBN(midPointOfTop, color, UVTop, normal));

	vertexes.push_back(Vertex_PCUTBN(midPointOfTop, color, UVTop, normal));
	vertexes.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), -uDirection));
	vertexes.push_back(Vertex_PCUTBN(bottomLeft, color, Vec2(UVs.m_mins.x, UVs.m_mins.y), -uDirection));

	vertexes.push_back(Vertex_PCUTBN(midPointOfBott, color, UVBott, normal));
	vertexes.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), uDirection));
	vertexes.push_back(Vertex_PCUTBN(topRight, color, Vec2(UVs.m_maxs.x, UVs.m_maxs.y), uDirection));


	vertexes.push_back(Vertex_PCUTBN(topRight, color, Vec2(UVs.m_maxs.x, UVs.m_maxs.y), uDirection));
	vertexes.push_back(Vertex_PCUTBN(midPointOfTop, color, UVTop, normal));
	vertexes.push_back(Vertex_PCUTBN(midPointOfBott, color, UVBott, normal));
}



void CalculateTangents(std::vector<Vertex_PCUTBN>& vertsArray, std::vector<unsigned int>& indexes)
{
	//size_t numOfVerts = vertsArray.size();
	int size = (int)indexes.size();
	for (int i = 0; i < size; i++)
	{
		if (i % 3 == 0) //for each triangle
		{
			int indexP0 = indexes[i];
			int indexP1 = indexes[i + 1];
			int indexP2 = indexes[i + 2];
			Vec2 w0 = vertsArray[indexP0].m_uvTexCoords;
			Vec2 w1 = vertsArray[indexP1].m_uvTexCoords;
			Vec2 w2 = vertsArray[indexP2].m_uvTexCoords;
			Vec3 e1 = vertsArray[indexP1].m_position - vertsArray[indexP0].m_position;//p1-p0
			Vec3 e2 = vertsArray[indexP2].m_position - vertsArray[indexP0].m_position;//p2-p0
			float x1 = w1.x - w0.x, x2 = w2.x - w0.x;
			float y1 = w1.y - w0.y, y2 = w2.y - w0.y;
			float r = 1.0f / (x1 * y2 - x2 * y1);
			Vec3 t = (e1 * y2 - e2 * y1) * r;
			Vec3 b = (e2 * x1 - e1 * x2) * r;
			vertsArray[indexP0].m_tangent += t;
			vertsArray[indexP1].m_tangent += t;
			vertsArray[indexP2].m_tangent += t;
			vertsArray[indexP0].m_binormal += b;
			vertsArray[indexP1].m_binormal += b;
			vertsArray[indexP2].m_binormal += b;
		}
	}
	for (int vertIndex = 0; vertIndex < vertsArray.size(); vertIndex++)
	{
		Mat44 tangentSpaceMatrix;
		tangentSpaceMatrix.SetIJK3D(vertsArray[vertIndex].m_tangent, vertsArray[vertIndex].m_binormal, vertsArray[vertIndex].m_normal);
		tangentSpaceMatrix.Orthonormalize_XFwd_YLeft_ZUp();
		vertsArray[vertIndex].m_tangent = tangentSpaceMatrix.GetIBasis3D();
		vertsArray[vertIndex].m_binormal = tangentSpaceMatrix.GetJBasis3D();
		vertsArray[vertIndex].m_normal = tangentSpaceMatrix.GetKBasis3D();
	}
}

void AddVertsForDebugDrawOnTangents(std::vector<Vertex_PCU>& verts, std::vector<Vertex_PCUTBN> const& originals, float length)
{
	for (int i = 0; i < originals.size(); ++i)
	{
		Vec3 pos = originals[i].m_position;
		Vec3 normalEnd = pos + originals[i].m_normal * length;
		Vec3 biEnd = pos + originals[i].m_binormal * length;
		Vec3 tangEnd = pos + originals[i].m_tangent * length;
		verts.push_back(Vertex_PCU(pos, Rgba8::BLUE, Vec2::ZERO));
		verts.push_back(Vertex_PCU(normalEnd, Rgba8::BLUE, Vec2::ZERO));
		verts.push_back(Vertex_PCU(pos, Rgba8::GREEN, Vec2::ZERO));
		verts.push_back(Vertex_PCU(biEnd, Rgba8::GREEN, Vec2::ZERO));
		verts.push_back(Vertex_PCU(pos, Rgba8::RED, Vec2::ZERO));
		verts.push_back(Vertex_PCU(tangEnd, Rgba8::RED, Vec2::ZERO));
	}
}

void AddVertsForAABB3(std::vector<Vertex_PCU>& mainVertexList, AABB3 const& aabb3, Rgba8 const& color /*= Rgba8::WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	float Back = aabb3.m_mins.x;
	float Front = aabb3.m_maxs.x;
	float Top = aabb3.m_maxs.z;
	float Bottom = aabb3.m_mins.z;
	float Left = aabb3.m_mins.y;
	float Right = aabb3.m_maxs.y;

	Vec3 frontTopLeft = Vec3(Front, Left, Top);
	Vec3 frontTopRight = Vec3(Front, Right, Top);
	Vec3 frontBottomLeft = Vec3(Front, Left, Bottom);
	Vec3 frontBottomRight = Vec3(Front, Right, Bottom);
	Vec3 BackTopLeft = Vec3(Back, Left, Top);
	Vec3 BackTopRight = Vec3(Back, Right, Top);
	Vec3 BackBottomLeft = Vec3(Back, Left, Bottom);
	Vec3 BackBottomRight = Vec3(Back, Right, Bottom);

	AddVertsForQuad3D(mainVertexList, BackBottomRight, BackBottomLeft, BackTopLeft, BackTopRight, color, UVs);
	AddVertsForQuad3D(mainVertexList, frontBottomLeft, frontBottomRight, frontTopRight, frontTopLeft, color, UVs);
	AddVertsForQuad3D(mainVertexList, BackBottomLeft, frontBottomLeft, frontTopLeft, BackTopLeft, color, UVs);
	AddVertsForQuad3D(mainVertexList, frontBottomRight, BackBottomRight, BackTopRight, frontTopRight, color, UVs);
	AddVertsForQuad3D(mainVertexList, BackTopRight, BackTopLeft, frontTopLeft, frontTopRight, color, UVs);
	AddVertsForQuad3D(mainVertexList, frontBottomRight, frontBottomLeft, BackBottomLeft, BackBottomRight, color, UVs);
}

void AddVertsForSphere3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& center, float radius, Rgba8 const& color /*= Rgba8::WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/, int numLatitudeSlices /*= 8*/)
{
	float deltaYaw = 360.f / numLatitudeSlices;
	float numLongtitudeStacks = 2.f * numLatitudeSlices;
	float deltaPitch = 180.f / numLongtitudeStacks;

	for (int stackIndex = 0; stackIndex < numLatitudeSlices; stackIndex++)
	{
		float yaw = deltaYaw * stackIndex;
		for (int sliceIndex = 0; sliceIndex < numLongtitudeStacks; sliceIndex++)
		{
			float pitch = -90.f + deltaPitch * sliceIndex;
			float minU = RangeMap(yaw, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
			float maxU = RangeMap(yaw + deltaYaw, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
			float minV = RangeMap(pitch, 90.f, -90.f, UVs.m_mins.y, UVs.m_maxs.y);
			float maxV = RangeMap(pitch + deltaPitch, 90.f, -90.f, UVs.m_mins.y, UVs.m_maxs.y);

			Vec3 BL = Vec3::MakeFromPolarDegrees(pitch + deltaPitch, yaw, radius);
			Vec3 BR = Vec3::MakeFromPolarDegrees(pitch + deltaPitch, yaw + deltaYaw, radius);
			Vec3 TL = Vec3::MakeFromPolarDegrees(pitch, yaw, radius);
			Vec3 TR = Vec3::MakeFromPolarDegrees(pitch, yaw + deltaYaw, radius);

			mainVertexList.push_back(Vertex_PCU(BL + center, color, Vec2(minU, maxV)));
			mainVertexList.push_back(Vertex_PCU(BR + center, color, Vec2(maxU, maxV)));
			mainVertexList.push_back(Vertex_PCU(TR + center, color, Vec2(maxU, minV)));

			mainVertexList.push_back(Vertex_PCU(TR + center, color, Vec2(maxU, minV)));
			mainVertexList.push_back(Vertex_PCU(TL + center, color, Vec2(minU, minV)));
			mainVertexList.push_back(Vertex_PCU(BL + center, color, Vec2(minU, maxV)));
		}
	}
	
}

void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& center, float radius, float numSlices, float numStacks, Rgba8 const& color /*= Rgba8::WHITE*/, AABB2 const& UVs)
{
	int numofSlices = RoundDownToInt(numSlices);
	int numofStacks = RoundDownToInt(numStacks);
	if ((float)numofSlices != numSlices)
		numofSlices++;
	if ((float)numofStacks != numStacks)
		numofStacks++;
	int NUM_VERTS = numofSlices * numofSlices * 2 * 3;
	verts.reserve(verts.size() + NUM_VERTS);
	float degreePerSlice = 360.f / numSlices;
	float degreePerStack = 180.f / numStacks;
	float uPerSlice = UVs.GetDimensions().x / numSlices;
	float vPerStack = UVs.GetDimensions().y / numStacks;
	int numofVerts = 2 + (numofStacks - 1) * numofSlices;
	int numofIndexes = 3 * ((numofStacks - 2) * numofSlices * 2 + 2 * numofSlices);
	verts.reserve(verts.size() + numofVerts);
	indexes.reserve(indexes.size() + numofIndexes);
	int base = (int)verts.size();
	Vertex_PCUTBN bottom;
	bottom.m_position = center - radius * Vec3(0.f, 0.f, 1.f);
	bottom.m_color = color;
	bottom.m_uvTexCoords = Vec2(0.f, 0.f);
	verts.push_back(bottom);
	for (int stackIndex = 0; stackIndex < numofStacks; stackIndex++) {
		if (stackIndex == 0)
			continue;
		for (int sliceIndex = 0; sliceIndex < numofSlices; sliceIndex++) {
			float startSliceDegree = GetClamped((float)sliceIndex * degreePerSlice, 0.f, 360.f);
			float startStackDegree = GetClamped((float)stackIndex * degreePerStack - 90.f, -90.f, 90.f);
			float startSliceU = GetClamped((float)sliceIndex * uPerSlice + UVs.m_mins.x, UVs.m_mins.x, UVs.m_maxs.x);
			float startStackV = GetClamped((float)stackIndex * vPerStack + UVs.m_mins.y, UVs.m_mins.y, UVs.m_maxs.y);
			Vec3 BL = Vec3::MakeFromPolarDegrees(startStackDegree, startSliceDegree, radius) + center;
			Vec2 uvAtMins = Vec2(startSliceU, startStackV);
			verts.emplace_back(Vertex_PCUTBN(BL, color, uvAtMins));
		}
	}
	Vertex_PCUTBN top;
	top.m_position = center + radius * Vec3(0.f, 0.f, 1.f);
	top.m_color = color;
	top.m_uvTexCoords = Vec2(1.f, 1.f);
	verts.push_back(top);
	int full = (int)verts.size() - 1;
	for (int stackIndex = 0; stackIndex < numofStacks; stackIndex++) {
		for (int sliceIndex = 0; sliceIndex < numofSlices; sliceIndex++) {
			if (stackIndex == 0) {
				indexes.push_back(base);
				if (sliceIndex != numofSlices - 1)
					indexes.push_back(base + sliceIndex + 2);
				else indexes.push_back(base + 1);
				indexes.push_back(base + sliceIndex + 1);
			}
			else if (stackIndex == numofStacks - 1) {
				indexes.push_back(full);
				//if (sliceIndex != numofSlices - 1)
				indexes.push_back(base + (stackIndex - 1) * numofSlices + sliceIndex + 1);
				//else indexes.push_back(base + (stackIndex - 1) * numofSlices + 1);
				if (sliceIndex == numofSlices - 1)
					indexes.push_back(base + (stackIndex - 1) * numofSlices + 1);
				else indexes.push_back(base + (stackIndex - 1) * numofSlices + sliceIndex + 2);
			}
			else {
				int BL = base + (stackIndex - 1) * numofSlices + sliceIndex + 1;
				int BR = base + (stackIndex - 1) * numofSlices + sliceIndex + 2;
				int TL = base + (stackIndex)*numofSlices + sliceIndex + 1;
				int TR = base + (stackIndex)*numofSlices + sliceIndex + 2;
				if (sliceIndex == numofSlices - 1)
				{
					BR = base + (stackIndex - 1) * numofSlices + 1;
					TR = base + (stackIndex)*numofSlices + 1;
				}
				Vec3 x = verts[BR].m_position - verts[BL].m_position;
				Vec3 y = verts[TR].m_position - verts[BR].m_position;
				Vec3 normal = CrossProduct3D(x, y).GetNormalized();
				verts[BL].m_normal = normal;
				verts[BR].m_normal = normal;
				verts[TL].m_normal = normal;
				verts[TR].m_normal = normal;
				indexes.push_back(BL);
				indexes.push_back(BR);
				indexes.push_back(TR);
				indexes.push_back(BL);
				indexes.push_back(TR);
				indexes.push_back(TL);
			}
		}
	}
	CalculateTangents(verts, indexes);
}

void AddVertsForLineSegment3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& start, Vec3 const& end, float thickness, Rgba8 const& color)
{
	float halfThickness = 0.5f * thickness;
	Vec3 idirection = (end - start).GetNormalized();
	Vec3 jdirection = CrossProduct3D(idirection, Vec3(0.0f, 0.0f, 1.0f));
	if (jdirection.GetLengthSquared() < 0.01f)
	{
		jdirection = CrossProduct3D(idirection, Vec3(0.0f, 1.0f, 0.0f));
	}
	jdirection = jdirection.GetNormalized();
	Vec3 kdirection = CrossProduct3D(idirection, jdirection);
	kdirection = kdirection.GetNormalized();
	
	Vec3 frontTopLeft	    = start - halfThickness * idirection - halfThickness * jdirection + halfThickness * kdirection;
	Vec3 frontTopRight		= start - halfThickness * idirection + halfThickness * jdirection + halfThickness * kdirection;
	Vec3 frontBottomLeft	= end + halfThickness * idirection - halfThickness * jdirection + halfThickness * kdirection;
	Vec3 frontBottomRight	= end + halfThickness * idirection + halfThickness * jdirection + halfThickness * kdirection;
	Vec3 BackTopLeft		= start - halfThickness * idirection - halfThickness * jdirection - halfThickness * kdirection;
	Vec3 BackTopRight		= start - halfThickness * idirection + halfThickness * jdirection - halfThickness * kdirection;
	Vec3 BackBottomLeft		= end + halfThickness * idirection - halfThickness * jdirection - halfThickness * kdirection;
	Vec3 BackBottomRight	= end + halfThickness * idirection + halfThickness * jdirection - halfThickness * kdirection;

	AddVertsForQuad3D(mainVertexList, BackBottomRight, BackBottomLeft, BackTopLeft, BackTopRight, color, AABB2::ZERO_TO_ONE);
	AddVertsForQuad3D(mainVertexList, frontBottomLeft, frontBottomRight, frontTopRight, frontTopLeft, color, AABB2::ZERO_TO_ONE);
	AddVertsForQuad3D(mainVertexList, BackBottomLeft, frontBottomLeft, frontTopLeft, BackTopLeft, color, AABB2::ZERO_TO_ONE);

	AddVertsForQuad3D(mainVertexList, frontBottomRight, BackBottomRight, BackTopRight, frontTopRight, color, AABB2::ZERO_TO_ONE);
	AddVertsForQuad3D(mainVertexList, BackTopRight, BackTopLeft, frontTopLeft, frontTopRight, color, AABB2::ZERO_TO_ONE);
	AddVertsForQuad3D(mainVertexList, frontBottomRight, frontBottomLeft, BackBottomLeft, BackBottomRight, color, AABB2::ZERO_TO_ONE);
}

void AddVertsForLineSegment3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, Vec3 const& start, Vec3 const& end, float thickness, Rgba8 const& color)
{
	float halfThickness = 0.5f * thickness;
	Vec3 idirection = (end - start).GetNormalized();
	Vec3 jdirection = CrossProduct3D(idirection, Vec3(0.0f, 0.0f, 1.0f));
	if (jdirection.GetLengthSquared() < 0.01f)
	{
		jdirection = CrossProduct3D(idirection, Vec3(0.0f, 1.0f, 0.0f));
	}
	jdirection = jdirection.GetNormalized();
	Vec3 kdirection = CrossProduct3D(idirection, jdirection);
	kdirection = kdirection.GetNormalized();

	Vec3 frontTopLeft = start - halfThickness * idirection - halfThickness * jdirection + halfThickness * kdirection;
	Vec3 frontTopRight = start - halfThickness * idirection + halfThickness * jdirection + halfThickness * kdirection;
	Vec3 frontBottomLeft = end + halfThickness * idirection - halfThickness * jdirection + halfThickness * kdirection;
	Vec3 frontBottomRight = end + halfThickness * idirection + halfThickness * jdirection + halfThickness * kdirection;
	Vec3 BackTopLeft = start - halfThickness * idirection - halfThickness * jdirection - halfThickness * kdirection;
	Vec3 BackTopRight = start - halfThickness * idirection + halfThickness * jdirection - halfThickness * kdirection;
	Vec3 BackBottomLeft = end + halfThickness * idirection - halfThickness * jdirection - halfThickness * kdirection;
	Vec3 BackBottomRight = end + halfThickness * idirection + halfThickness * jdirection - halfThickness * kdirection;

	AddVertsForQuad3D(vertexes, indexes, BackBottomRight, BackBottomLeft, BackTopLeft, BackTopRight, color, AABB2::ZERO_TO_ONE);
	AddVertsForQuad3D(vertexes, indexes, frontBottomLeft, frontBottomRight, frontTopRight, frontTopLeft, color, AABB2::ZERO_TO_ONE);
	AddVertsForQuad3D(vertexes, indexes, BackBottomLeft, frontBottomLeft, frontTopLeft, BackTopLeft, color, AABB2::ZERO_TO_ONE);

	AddVertsForQuad3D(vertexes, indexes, frontBottomRight, BackBottomRight, BackTopRight, frontTopRight, color, AABB2::ZERO_TO_ONE);
	AddVertsForQuad3D(vertexes, indexes, BackTopRight, BackTopLeft, frontTopLeft, frontTopRight, color, AABB2::ZERO_TO_ONE);
	AddVertsForQuad3D(vertexes, indexes, frontBottomRight, frontBottomLeft, BackBottomLeft, BackBottomRight, color, AABB2::ZERO_TO_ONE);
}

void AddVertsForAABBWireframe3D(std::vector<Vertex_PCU>& mainVertexList, AABB3 const& bounds, float lineThickness, Rgba8 const& tint /*= Rgba8::WHITE*/)
{
	float Back = bounds.m_mins.x;
	float Front = bounds.m_maxs.x;
	float Top = bounds.m_maxs.z;
	float Bottom = bounds.m_mins.z;
	float Left = bounds.m_mins.y;
	float Right = bounds.m_maxs.y;

	Vec3 frontTopLeft = Vec3(Front, Left, Top);
	Vec3 frontTopRight = Vec3(Front, Right, Top);
	Vec3 frontBottomLeft = Vec3(Front, Left, Bottom);
	Vec3 frontBottomRight = Vec3(Front, Right, Bottom);
	Vec3 BackTopLeft = Vec3(Back, Left, Top);
	Vec3 BackTopRight = Vec3(Back, Right, Top);
	Vec3 BackBottomLeft = Vec3(Back, Left, Bottom);
	Vec3 BackBottomRight = Vec3(Back, Right, Bottom);

	AddVertsForLineSegment3D(mainVertexList, frontTopLeft, frontTopRight, lineThickness, tint);
	AddVertsForLineSegment3D(mainVertexList, frontTopRight, BackTopRight, lineThickness, tint);
	AddVertsForLineSegment3D(mainVertexList, BackTopRight, BackTopLeft, lineThickness, tint);
	AddVertsForLineSegment3D(mainVertexList, BackTopLeft, frontTopLeft, lineThickness, tint);

	AddVertsForLineSegment3D(mainVertexList, frontTopLeft, frontBottomLeft, lineThickness, tint);
	AddVertsForLineSegment3D(mainVertexList, frontTopRight, frontBottomRight, lineThickness, tint);
	AddVertsForLineSegment3D(mainVertexList, BackTopRight, BackBottomRight, lineThickness, tint);
	AddVertsForLineSegment3D(mainVertexList, BackTopLeft, BackBottomLeft, lineThickness, tint);

	AddVertsForLineSegment3D(mainVertexList, frontBottomLeft, frontBottomRight, lineThickness, tint);
	AddVertsForLineSegment3D(mainVertexList, frontBottomRight, BackBottomRight, lineThickness, tint);
	AddVertsForLineSegment3D(mainVertexList, BackBottomRight, BackBottomLeft, lineThickness, tint);
	AddVertsForLineSegment3D(mainVertexList, BackBottomLeft, frontBottomLeft, lineThickness, tint);
}

void AddVertsForCylindarZ3D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, float numSlices, Rgba8 const& tint /*= Rgba8::WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	float deltaDegrees = 360.f / numSlices;
	Vec3 topCenter = Vec3(centerXY.x, centerXY.y, minMaxZ.m_max);
	Vec3 bottomCenter = Vec3(centerXY.x, centerXY.y, minMaxZ.m_min);
	for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
	{
		float degrees = sliceIndex * deltaDegrees;
		Vec3 currentPoint = Vec3::MakeFromPolarDegrees(0.f, degrees, radius);
		Vec3 nextPoint = Vec3::MakeFromPolarDegrees(0.f, degrees + deltaDegrees, radius);
		float currentU = RangeMapClamped(degrees, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
		float nextU = RangeMapClamped(degrees + deltaDegrees, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
		// top
		mainVertexList.push_back(Vertex_PCU(currentPoint + topCenter, tint, Vec2(0.5f + CosDegrees(degrees) * 0.5f, 0.5f + SinDegrees(degrees) * 0.5f)));
		mainVertexList.push_back(Vertex_PCU(nextPoint + topCenter, tint, Vec2(0.5f + CosDegrees(degrees + deltaDegrees) * 0.5f, 0.5f + SinDegrees(degrees + deltaDegrees) * 0.5f)));
		mainVertexList.push_back(Vertex_PCU(topCenter, tint, Vec2(0.5f,0.5f)));
		// body

		AddVertsForQuad3D(mainVertexList, currentPoint + bottomCenter, nextPoint + bottomCenter, nextPoint + topCenter, currentPoint + topCenter, tint, AABB2(Vec2(currentU, UVs.m_mins.y), Vec2(nextU, UVs.m_maxs.y)));


		// bottom
		mainVertexList.push_back(Vertex_PCU(nextPoint + bottomCenter, tint, Vec2(0.5f + CosDegrees(degrees + deltaDegrees) * 0.5f, 0.5f + SinDegrees(degrees + deltaDegrees) * 0.5f)));
		mainVertexList.push_back(Vertex_PCU(currentPoint + bottomCenter, tint, Vec2(0.5f + CosDegrees(degrees) * 0.5f, 0.5f + SinDegrees(degrees) * 0.5f)));
		mainVertexList.push_back(Vertex_PCU(bottomCenter, tint, Vec2(0.5f, 0.5f)));
	}
}

void AddVertsForCylindarZWireframe3D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, float numSlices, float lineThickness, Rgba8 const& tint /*= Rgba8::WHITE*/)
{
	float deltaDegrees = 360.f / numSlices;
	Vec3 topCenter = Vec3(centerXY.x, centerXY.y, minMaxZ.m_max);
	Vec3 bottomCenter = Vec3(centerXY.x, centerXY.y, minMaxZ.m_min);
	for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
	{
		float degrees = sliceIndex * deltaDegrees;
		Vec3 currentPoint = Vec3::MakeFromPolarDegrees(0.f, degrees, radius);
		Vec3 nextPoint = Vec3::MakeFromPolarDegrees(0.f, degrees + deltaDegrees, radius);
		
		// top
		AddVertsForLineSegment3D(mainVertexList, currentPoint + topCenter, nextPoint + topCenter, lineThickness, tint);
		// body
		AddVertsForLineSegment3D(mainVertexList, currentPoint + topCenter, currentPoint + bottomCenter, lineThickness, tint);

		// bottom
		AddVertsForLineSegment3D(mainVertexList, currentPoint + bottomCenter, nextPoint + bottomCenter, lineThickness, tint);
	}
}

void AddVertsForUVSphereZ3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& center, float radius, float numSlices, float numStacks, Rgba8 const& tint /*= Rgba8::WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{

	float deltaYaw = 360.f / numStacks;
	float deltaPitch = 180.f / numSlices;

	for (int stackIndex = 0; stackIndex < numStacks; stackIndex++)
	{
		float yaw = deltaYaw * stackIndex;
		for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
		{
			float pitch = -90.f + deltaPitch * sliceIndex;
			float minU = RangeMap(yaw, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
			float maxU = RangeMap(yaw + deltaYaw, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
			float minV = RangeMap(pitch, 90.f, -90.f, UVs.m_mins.y, UVs.m_maxs.y);
			float maxV = RangeMap(pitch + deltaPitch, 90.f, -90.f, UVs.m_mins.y, UVs.m_maxs.y);

			Vec3 BL = Vec3::MakeFromPolarDegrees(pitch + deltaPitch, yaw, radius);
			Vec3 BR = Vec3::MakeFromPolarDegrees(pitch + deltaPitch, yaw + deltaYaw, radius);
			Vec3 TL = Vec3::MakeFromPolarDegrees(pitch, yaw, radius);
			Vec3 TR = Vec3::MakeFromPolarDegrees(pitch, yaw + deltaYaw, radius);

			mainVertexList.push_back(Vertex_PCU(BL + center, tint, Vec2(minU, maxV)));
			mainVertexList.push_back(Vertex_PCU(BR + center, tint, Vec2(maxU, maxV)));
			mainVertexList.push_back(Vertex_PCU(TR + center, tint, Vec2(maxU, minV)));

			mainVertexList.push_back(Vertex_PCU(TR + center, tint, Vec2(maxU, minV)));
			mainVertexList.push_back(Vertex_PCU(TL + center, tint, Vec2(minU, minV)));
			mainVertexList.push_back(Vertex_PCU(BL + center, tint, Vec2(minU, maxV)));
		}
	}
}

void AddVertsForUVSphereZWireframe3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& center, float radius, float numSlices, float numStacks, float lineThickness, Rgba8 const& tint /*= Rgba8::WHITE*/)
{
	float deltaYaw = 360.f / numStacks;
	float deltaPitch = 180.f / numSlices;

	for (int stackIndex = 0; stackIndex < numStacks; stackIndex++)
	{
		float yaw = deltaYaw * stackIndex;
		for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
		{
			float pitch = -90.f + deltaPitch * sliceIndex;
			
			Vec3 BL = Vec3::MakeFromPolarDegrees(pitch + deltaPitch, yaw, radius)  + center;
			Vec3 BR = Vec3::MakeFromPolarDegrees(pitch + deltaPitch, yaw + deltaYaw, radius) + center;
			Vec3 TL = Vec3::MakeFromPolarDegrees(pitch, yaw, radius) + center;
			Vec3 TR = Vec3::MakeFromPolarDegrees(pitch, yaw + deltaYaw, radius) + center;

			AddVertsForLineSegment3D(mainVertexList, TL, TR, lineThickness, tint);
			AddVertsForLineSegment3D(mainVertexList, TL, BL, lineThickness, tint);

		}
	}
}

AABB2 GetVertexBounds2D(std::vector<Vertex_PCU> const& verts)
{
	float minX = 999999.f, minY = 999999.f, maxX = -1.f, maxY = -1.f;
	for (int vertIndex = 0; vertIndex < verts.size(); ++vertIndex)
	{
		if (verts[vertIndex].m_position.x < minX)
		{
			minX = verts[vertIndex].m_position.x;
		}
		if (verts[vertIndex].m_position.x > maxX)
		{
			maxX = verts[vertIndex].m_position.x;
		}
		if (verts[vertIndex].m_position.y < minY)
		{
			minY = verts[vertIndex].m_position.y;
		}
		if (verts[vertIndex].m_position.y > maxY)
		{
			maxY = verts[vertIndex].m_position.y;
		}
	}
	AABB2 result = AABB2(minX, minY, maxX, maxY);
	return result;
}

void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color /*= Rgba8::WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	Vec3 idirection = (end - start).GetNormalized();
	Vec3 jdirection = CrossProduct3D(idirection, Vec3(0.0f, 0.0f, 1.0f));
	if (jdirection.GetLengthSquared() < 0.01f)
	{
		jdirection = CrossProduct3D(idirection, Vec3(0.0f, 1.0f, 0.0f));
	}
	jdirection = jdirection.GetNormalized();
	Vec3 kdirection = CrossProduct3D(idirection, jdirection);
	kdirection = kdirection.GetNormalized();
	
	float deltaDegrees = 360.f / numSlices;
	Vec3 topCenter = Vec3(end.x, end.y, end.z);
	Vec3 bottomCenter = Vec3(start.x, start.y, start.z);
	for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
	{
		float degrees = sliceIndex * deltaDegrees;
		Vec3 currentPoint = (CosDegrees(degrees) * jdirection + SinDegrees(degrees) * kdirection) * radius;
		Vec3 nextPoint = (CosDegrees(degrees + deltaDegrees) * jdirection + SinDegrees(degrees + deltaDegrees) * kdirection) * radius;
		float currentU = RangeMapClamped(degrees, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
		float nextU = RangeMapClamped(degrees + deltaDegrees, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
		// top
		verts.push_back(Vertex_PCU(currentPoint + topCenter, color, Vec2(0.5f + CosDegrees(degrees) * 0.5f, 0.5f + SinDegrees(degrees) * 0.5f)));
		verts.push_back(Vertex_PCU(nextPoint + topCenter, color, Vec2(0.5f + CosDegrees(degrees + deltaDegrees) * 0.5f, 0.5f + SinDegrees(degrees + deltaDegrees) * 0.5f)));
		verts.push_back(Vertex_PCU(topCenter, color, Vec2(0.5f, 0.5f)));
		// body

		AddVertsForQuad3D(verts, currentPoint + bottomCenter, nextPoint + bottomCenter, nextPoint + topCenter, currentPoint + topCenter, color, AABB2(Vec2(currentU, UVs.m_mins.y), Vec2(nextU, UVs.m_maxs.y)));


		// bottom
		verts.push_back(Vertex_PCU(nextPoint + bottomCenter, color, Vec2(0.5f + CosDegrees(degrees + deltaDegrees) * 0.5f, 0.5f + SinDegrees(degrees + deltaDegrees) * 0.5f)));
		verts.push_back(Vertex_PCU(currentPoint + bottomCenter, color, Vec2(0.5f + CosDegrees(degrees) * 0.5f, 0.5f + SinDegrees(degrees) * 0.5f)));
		verts.push_back(Vertex_PCU(bottomCenter, color, Vec2(0.5f, 0.5f)));
	}
}

void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color /*= Rgba8::WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	Vec3 idirection = (end - start).GetNormalized();
	Vec3 jdirection = CrossProduct3D(idirection, Vec3(0.0f, 0.0f, 1.0f));
	if (jdirection.GetLengthSquared() < 0.01f)
	{
		jdirection = CrossProduct3D(idirection, Vec3(0.0f, 1.0f, 0.0f));
	}
	jdirection = jdirection.GetNormalized();
	Vec3 kdirection = CrossProduct3D(idirection, jdirection);
	kdirection = kdirection.GetNormalized();

	float deltaDegrees = 360.f / numSlices;
	Vec3 topCenter = Vec3(end.x, end.y, end.z);
	Vec3 bottomCenter = Vec3(start.x, start.y, start.z);
	for (int sliceIndex = 0; sliceIndex < numSlices; sliceIndex++)
	{
		float degrees = sliceIndex * deltaDegrees;
		Vec3 currentPoint = (CosDegrees(degrees) * jdirection + SinDegrees(degrees) * kdirection) * radius;
		Vec3 nextPoint = (CosDegrees(degrees + deltaDegrees) * jdirection + SinDegrees(degrees + deltaDegrees) * kdirection) * radius;
		float currentU = RangeMapClamped(degrees, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
		float nextU = RangeMapClamped(degrees + deltaDegrees, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
		
		// body

		AddVertsForQuad3D(verts, currentPoint + bottomCenter, nextPoint + bottomCenter, topCenter, topCenter, color, AABB2(Vec2(currentU, UVs.m_mins.y), Vec2(nextU, UVs.m_maxs.y)));


		// bottom
		verts.push_back(Vertex_PCU(nextPoint + bottomCenter, color, Vec2(0.5f + CosDegrees(degrees + deltaDegrees) * 0.5f, 0.5f + SinDegrees(degrees + deltaDegrees) * 0.5f)));
		verts.push_back(Vertex_PCU(currentPoint + bottomCenter, color, Vec2(0.5f + CosDegrees(degrees) * 0.5f, 0.5f + SinDegrees(degrees) * 0.5f)));
		verts.push_back(Vertex_PCU(bottomCenter, color, Vec2(0.5f, 0.5f)));
	}
}
