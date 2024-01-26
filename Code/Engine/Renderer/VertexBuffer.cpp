#include "VertexBuffer.hpp"
#include "../Core/Vertex_PCU.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "../Core/Vertex_PNCU.hpp"

VertexBuffer::VertexBuffer(size_t size, unsigned int stride)
	:m_size(size)
	,m_stride(stride)
{

}

VertexBuffer::~VertexBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}

unsigned int VertexBuffer::GetStride() const
{
	return m_stride;
}



