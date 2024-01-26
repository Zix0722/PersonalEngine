#pragma once
#include "Renderer.hpp"

class ConstantBuffer
{
	friend class Renderer;
public:
	ConstantBuffer(size_t size);
	ConstantBuffer(ConstantBuffer const& copy) = delete;
	virtual ~ConstantBuffer();

	ID3D11Buffer* m_buffer = nullptr;
	size_t m_size = 0;

};