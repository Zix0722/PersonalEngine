#include "ConstantBuffer.hpp"
#include "../Core/ErrorWarningAssert.hpp"


ConstantBuffer::ConstantBuffer(size_t size)
	:m_size(size)
{

}

ConstantBuffer::~ConstantBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}


