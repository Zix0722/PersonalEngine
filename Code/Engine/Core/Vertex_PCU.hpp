#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"

//------------------------
struct Vertex_PCU
{
public:
	Vec3 m_position;
	Rgba8 m_color;
	Vec2 m_uvTexCoords;
	Vec2 m_position2D;
public:
	Vertex_PCU(Vec3 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords);
	Vertex_PCU();

	//Vertex_PCU(Rgba8 color, Vec2 pos);
	Vertex_PCU(Vec2 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords); 
	~Vertex_PCU();
};