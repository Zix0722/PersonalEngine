#pragma once

class Renderer;
class VertexBuffer;
class IndexBuffer;


class GPUMesh
{
public:
	GPUMesh();
	~GPUMesh();

	void Render(Renderer* Render, int indexesSize) const;  //Renders the buffer but does not set any state 

public:
	VertexBuffer* m_vertexes = nullptr;
	IndexBuffer* m_indexes = nullptr;
};