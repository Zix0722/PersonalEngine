#include "Renderer.hpp"
#include "GPUMesh.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
GPUMesh::GPUMesh()
{

}

GPUMesh::~GPUMesh()
{
	delete m_indexes;
	delete m_vertexes;
}

void GPUMesh::Render(Renderer* Render, int indexesSize) const
{
	Render->DrawVertexArray(m_vertexes, indexesSize, m_indexes);
}

