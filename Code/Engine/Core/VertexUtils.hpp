#pragma once
#include <vector>
#include "Vertex_PCU.hpp"
#include "Engine/Math/AABB2.hpp"
#include "../Math/EulerAngles.hpp"
#include "../Math/AABB3.hpp"
#include "../Math/FloatRange.hpp"
#include "Vertex_PNCU.hpp"
struct OBB2;

void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float scaleXY, float rotationDegreesAboutZ, Vec2 const& translationXY);
void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, Mat44 const& transform);
void AddVertsForAABB2(std::vector<Vertex_PCU>& mainVertexList, AABB2 const& aabb2, Rgba8 const& color, Vec2 uvAtMins = Vec2::ZERO, Vec2 uvAtMaxs = Vec2::ONE);
void AddVertsForLine2(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& color);
void AddVertsForRing2(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& center, float radius, float thickness, Rgba8 const& color);
void AddVertsFormList(std::vector<Vertex_PCU>& mainVertexList, Vertex_PCU const* vertlist, int numVerts);
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color);
void AddVertsForDisc2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& center, float radius, Rgba8 const& color);
void AddVertsForSector2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& center, float radius, Vec2 const& startNormal, float sectorDegrees, Rgba8 const& color);
void AddVertsForOBB2D(std::vector<Vertex_PCU>& mainVertexList, OBB2 const& box, Rgba8 const& color, AABB2 const& uv);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);
void AddVertsForArrow2D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& startPos, Vec2 const& endPos, float arrowSize, float lineThickness, Rgba8 const& color);
void AddVertsForQuad3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft
					 , Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForAABB3(std::vector<Vertex_PCU>& mainVertexList, AABB3 const& aabb3, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForSphere3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& center, float radius, 
						 Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE, int numLatitudeSlices = 8);
void AddVertsForLineSegment3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& start, Vec3 const& end, float thickness, Rgba8 const& color);
void AddVertsForLineSegment3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, Vec3 const& start, Vec3 const& end, float thickness, Rgba8 const& color);

void AddVertsForAABBWireframe3D(std::vector<Vertex_PCU>& mainVertexList, AABB3 const& bounds, float lineThickness, Rgba8 const& tint = Rgba8::WHITE);
void AddVertsForCylindarZ3D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, float numSlices, Rgba8 const& tint = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForCylindarZWireframe3D(std::vector<Vertex_PCU>& mainVertexList, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, float numSlices, float lineThickness, Rgba8 const& tint = Rgba8::WHITE);
void AddVertsForUVSphereZ3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& center, float radius, float numSlices, float numStacks, Rgba8 const& tint = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForUVSphereZWireframe3D(std::vector<Vertex_PCU>& mainVertexList, Vec3 const& center, float radius, float numSlices, float numStacks, float lineThickness, Rgba8 const& tint = Rgba8::WHITE);
AABB2 GetVertexBounds2D(std::vector<Vertex_PCU> const& verts);
void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts,
	Vec3 const& start, Vec3 const& end, float radius,
	Rgba8 const& color = Rgba8::WHITE,
	AABB2 const& UVs = AABB2::ZERO_TO_ONE,
	int numSlices = 8);
void AddVertsForCone3D(std::vector<Vertex_PCU>& verts,
	Vec3 const& start, Vec3 const& end, float radius,
	Rgba8 const& color = Rgba8::WHITE,
	AABB2 const& UVs = AABB2::ZERO_TO_ONE,
	int numSlices = 8);
void AddVertsForQuad3D(std::vector<Vertex_PCU>& vertexes,
	std::vector<unsigned int>& indexes,
	Vec3 const& topLeft, Vec3 const& bottomLeft,
	Vec3 const& bottomRight, Vec3 const& topRight,
	Rgba8 const& color = Rgba8::WHITE,
	AABB2 const& UVs = AABB2::ZERO_TO_ONE);

void AddVertsForCube3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, AABB3 box, Rgba8 const& color = Rgba8::WHITE);
void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& center, float radius, float numSlices, float numStacks, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, const Vec3& topLeft, const Vec3& bottomLeft,
	const Vec3& bottomRight, const Vec3& topRight, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& vertexes, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight,
	const Vec3& topRight, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);

void CalculateTangents(std::vector<Vertex_PCUTBN>& vertsArray, std::vector<unsigned int>& indexes);
void AddVertsForDebugDrawOnTangents(std::vector<Vertex_PCU>& verts, std::vector<Vertex_PCUTBN> const& originals, float length = 0.25f);

