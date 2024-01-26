#include "ModelConstantBuffer.hpp"
#include "../Core/ErrorWarningAssert.hpp"


ModelConstantBuffer::ModelConstantBuffer(size_t size)
	:m_size(size)
{

}

ModelConstantBuffer::~ModelConstantBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}