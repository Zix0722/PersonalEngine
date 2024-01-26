#include "Vertex_PCU.hpp"

Vertex_PCU::Vertex_PCU(Vec3 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords)
	: m_position(position)
	, m_color (tint)
	, m_uvTexCoords (uvTexCoords)
{

}

Vertex_PCU::Vertex_PCU()
{
	
}





Vertex_PCU::Vertex_PCU(Vec2 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords)
	: m_position(position.x,position.y,0.f)
	, m_color(tint)
	, m_uvTexCoords(uvTexCoords)
{
}

Vertex_PCU::~Vertex_PCU()
{


}
