#pragma once
#include "Engine/Core/Vertex_PNCU.hpp"
#include <vector>
#include "Renderer.hpp"

class GPUMesh;

class CPUMesh
{
	friend class Renderer;
public:
	CPUMesh();
	~CPUMesh();

	void Render(Renderer* Render) const; // render immediately
	void CreateBuffers(GPUMesh& out_gpuMesh, Renderer* renderer);

public:
	std::vector<Vertex_PCUTBN> m_vertexes;
	std::vector<unsigned int> m_indexes;
};