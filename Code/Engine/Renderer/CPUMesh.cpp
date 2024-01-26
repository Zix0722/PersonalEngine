#include "CPUMesh.hpp"
#include "Engine/Renderer/GPUMesh.hpp"
#include "../Core/Vertex_PNCU.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "../Core/VertexUtils.hpp"
CPUMesh::CPUMesh()
{

}

CPUMesh::~CPUMesh()
{

}

void CPUMesh::Render(Renderer* Render) const
{
	Render->DrawVertexArray((int)m_vertexes.size(), m_vertexes.data());
}

void CPUMesh::CreateBuffers(GPUMesh& out_gpuMesh, Renderer* renderer)
{
	CalculateTangents(m_vertexes, m_indexes);
	out_gpuMesh.m_indexes = renderer->CreateIndexBuffer(m_indexes.size() * sizeof(unsigned int)); 
	out_gpuMesh.m_vertexes = renderer->CreateVertexBuffer(m_vertexes.size() * sizeof(Vertex_PCUTBN), sizeof(Vertex_PCUTBN));

	renderer->CopyCPUToGPU(m_vertexes.data(), (size_t)m_vertexes.size() * (out_gpuMesh.m_vertexes)->GetStride(), out_gpuMesh.m_vertexes);
	renderer->CopyCPUToGPU(m_indexes.data(), (size_t)m_indexes.size() * (out_gpuMesh.m_indexes)->GetStride(), out_gpuMesh.m_indexes);
}

