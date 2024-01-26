#include "Vertex_PNCU.hpp"

Vertex_PCUTBN::Vertex_PCUTBN(Vec3 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords, Vec3 const& normal)
	: m_position(position)
	, m_color(tint)
	, m_uvTexCoords(uvTexCoords)
	, m_normal(normal)
{
	m_tangent = Vec3(0.f, 0.f, 0.f);
	m_binormal = Vec3(0.f, 0.f, 0.f);
}

Vertex_PCUTBN::Vertex_PCUTBN()
{

}

Vertex_PCUTBN::Vertex_PCUTBN(Vec3 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords, Vec3 const& tangent, Vec3 const& binormal, Vec3 const& normal)
	: m_position(position)
	, m_color(tint)
	, m_uvTexCoords(uvTexCoords)
	, m_tangent(tangent)
	, m_binormal(binormal)
	, m_normal(normal)
{

}

Vertex_PCUTBN::Vertex_PCUTBN(Vec3 const& position, Rgba8 const& tint, Vec2 const& uvTexCoords)
	: m_position(position)
	, m_color(tint)
	, m_uvTexCoords(uvTexCoords)
{
	m_tangent = Vec3(0.f, 0.f, 0.f);
	m_binormal = Vec3(0.f, 0.f, 0.f);
	m_normal = Vec3(0.f, 0.f, 0.f);
}

Vertex_PCUTBN::~Vertex_PCUTBN()
{


}
