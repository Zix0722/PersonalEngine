#include "Shader.hpp"
#include "Engine/Renderer/Renderer.hpp"

Shader::Shader(ShaderConfig const& config)
	:m_config(config)
{

}



Shader::~Shader()
{
	DX_SAFE_RELEASE(m_inputLayout);
	DX_SAFE_RELEASE(m_pixelShader);
	DX_SAFE_RELEASE(m_vertexShader);
}

std::string const& Shader::GetName() const
{
	return m_config.m_name;
}

