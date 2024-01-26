#pragma once
#include "Renderer.hpp"

class ModelConstantBuffer
{
	friend class Renderer;
public:
	ModelConstantBuffer(size_t size);
	ModelConstantBuffer(ModelConstantBuffer const& copy) = delete;
	virtual ~ModelConstantBuffer();

	ID3D11Buffer* m_buffer = nullptr;
	size_t m_size = 0;
}; 
