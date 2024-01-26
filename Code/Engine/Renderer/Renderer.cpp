#include "Engine/Core/Image.hpp"
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "BitmapFont.hpp"
#include "Shader.hpp"
#include "DefaultShader.hpp"
#include "Engine/Core/Image.hpp"

#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in very few places

//----------DirectX 11 headers------------


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#if defined(ENGINE_DEBUG_RENDER)
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "../Core/FileUtils.hpp"
#include "ConstantBuffer.hpp"
#include "ModelConstantBuffer.hpp"


//----------------------------------------

struct CameraConstants
{
	Mat44 ViewMatrix;
	Mat44 ProjectionMatrix;
};

static const int k_lightingConstantsSlot = 1;
static const int k_cameraConstantsSlot = 2;
static const int k_modelConstantsSlot = 3;


Renderer::Renderer(RendererConfig const& config)
	:m_config(config)
{

#if defined(ENGINE_DEBUG_RENDER)
	m_dxgiDebugModeule = (void*) ::LoadLibraryA("dxgidebug.dll");
	if (m_dxgiDebugModeule == nullptr)
	{
		ERROR_AND_DIE("Could not load dxgidebug.dll ");
	}
	typedef HRESULT(WINAPI* GetDebugModuleCB)(REFIID, void**);
	((GetDebugModuleCB) ::GetProcAddress((HMODULE)m_dxgiDebugModeule, "DXGIGetDebugInterface"))(__uuidof (IDXGIDebug), &m_dxgiDebug);

	if (m_dxgiDebug == nullptr)
	{
		ERROR_AND_DIE("Could not load debug module.");
	}

#endif
}

Renderer::~Renderer()
{

}

void Renderer::Startup()
{
	CreateRenderContext();
	DefaultShader defalutShader;
	m_defalutShader = CreateShader("Default", defalutShader.defaultShaderSource);
	//CreateShader("Data/Shader/Default");


	m_cameraCBO = CreateConstantBuffer(sizeof(CameraConstants));
	m_lightingCBO = CreateConstantBuffer(sizeof(LightConstants));
	m_immediateVBO = new VertexBuffer(sizeof(Vertex_PCU), sizeof(Vertex_PCU));
	m_immediateVBOForPNCU = new VertexBuffer(sizeof(Vertex_PCUTBN), sizeof(Vertex_PCUTBN));
	m_modelCBO = CreateModelConstantBuffer(sizeof(ModelConstants));

	InitializeRasterizerStates();
	

	SetBlendMode(BlendMode::ALPHA);
	m_defaultTexture = CreateTextureFromImage(Image(IntVec2(2, 2), Rgba8::WHITE));
	BindTexture(nullptr);

	InitializeSamplerStates();
	InitializeBlendStates();
	
	D3D11_TEXTURE2D_DESC texture2DDesc = {0};
	texture2DDesc.Width = m_config.m_window->GetClientDimensions().x;
	texture2DDesc.Height = m_config.m_window->GetClientDimensions().y;
	texture2DDesc.MipLevels = 1;
	texture2DDesc.ArraySize = 1;
	texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	texture2DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2DDesc.SampleDesc.Count = 1;

	HRESULT hr = m_device->CreateTexture2D(&texture2DDesc, nullptr, &m_depthStencilTexture);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Texture2D");
	}
	hr = m_device->CreateDepthStencilView(m_depthStencilTexture, nullptr, &m_depthStencilView);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Depth Stencil View");
	}

	InitializeDepthStencilState();

}

void Renderer::BeginFrame() const
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	ClearScreen(Rgba8::MAGENTA);
}

void Renderer::EndFrame() const
{
	//HWND windowHandle = ::GetActiveWindow();
	//HDC displayDeviceContext = ::GetDC(windowHandle);
	if (m_swapChain)
	{
		m_swapChain->Present(0, 0);
	}
}

void Renderer::Shutdown()
{
	delete m_immediateVBO;
	delete m_immediateVBOForPNCU;
	delete m_cameraCBO;
	delete m_lightingCBO;
	delete m_modelCBO;

	for (int shaderIndex = 0; shaderIndex < m_loadedShaders.size(); shaderIndex++)
	{
		if (m_loadedShaders[shaderIndex])
		{
			delete m_loadedShaders[shaderIndex];
			m_loadedShaders[shaderIndex] = nullptr;
		}
	}
	m_loadedShaders.clear();

	for (int texIndex = 0; texIndex < m_loadedTextures.size(); texIndex++)
	{
		if (m_loadedTextures[texIndex] != nullptr)
		{
			delete m_loadedTextures[texIndex];
			m_loadedTextures[texIndex] = nullptr;
		}
	}
	m_loadedTextures.clear();

	DX_SAFE_RELEASE(m_swapChain);
	DX_SAFE_RELEASE(m_deviceContext);
	DX_SAFE_RELEASE(m_device);
	DX_SAFE_RELEASE(m_renderTargetView);
	DX_SAFE_RELEASE(m_depthStencilView);
	DX_SAFE_RELEASE(m_depthStencilTexture);

	for (int modeIndex = 0; modeIndex < (int)BlendMode::COUNT; modeIndex++)
	{
		DX_SAFE_RELEASE(m_blendStates[modeIndex]);
	}
	for (int modeIndex = 0; modeIndex < (int)SamplerMode::COUNT; modeIndex++)
	{
		DX_SAFE_RELEASE(m_samplerStates[modeIndex]);
	}
	for (int modeIndex = 0; modeIndex < (int)RasterizerMode::COUNT; modeIndex++)
	{
		DX_SAFE_RELEASE(m_rasterizerStates[modeIndex]);
	}
	for (int modeIndex = 0; modeIndex < (int)DepthMode::COUNT; modeIndex++)
	{
		DX_SAFE_RELEASE(m_depthStencilStates[modeIndex]);
	}

	m_blendState = nullptr;
	m_rasterizerState = nullptr;
	m_samplerState = nullptr;
	m_depthStencilState = nullptr;
	
#if defined(ENGINE_DEBUG_RENDER)
	((IDXGIDebug*)m_dxgiDebug)->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));

	((IDXGIDebug*)m_dxgiDebug)->Release();
	m_dxgiDebug = nullptr;

	::FreeLibrary((HMODULE)m_dxgiDebugModeule);
	m_dxgiDebugModeule = nullptr;
#endif
}

void Renderer::ClearScreen(Rgba8 const& clearColor) const
{
	float colorFloats[4];
	clearColor.GetAsFloats(colorFloats);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, colorFloats);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void Renderer::BeginCamera(Camera& camera) 
{
	if (camera.m_viewport.m_mins == Vec2::ZERO && camera.m_viewport.m_maxs == Vec2::ZERO)
	{
		camera.m_viewport.m_maxs.x = (float)m_config.m_window->GetClientDimensions().x;
		camera.m_viewport.m_maxs.y = (float)m_config.m_window->GetClientDimensions().y ;
	}

	float viewportWidth = camera.m_viewport.m_maxs.x - camera.m_viewport.m_mins.x;
	float viewportHeight = camera.m_viewport.m_maxs.y - camera.m_viewport.m_mins.y;
	float aspect = viewportWidth / viewportHeight;
	float ratio = (float)m_config.m_window->GetClientDimensions().x / (float)m_config.m_window->GetClientDimensions().y;
	float scaler = ratio / aspect;

	CameraConstants localCC = {};
	localCC.ViewMatrix = camera.GetViewMatrix();
	localCC.ProjectionMatrix = camera.GetProjectionMaxtrix(); 
	localCC.ProjectionMatrix.AppendScaleNonUniform2D(Vec2(1.f, scaler));

	CopyCPUToGPU(&localCC, sizeof(CameraConstants) , m_cameraCBO);
	BindConstantBuffer(k_cameraConstantsSlot, m_cameraCBO);

	

	D3D11_VIEWPORT m_viewport = { 0 };
	m_viewport.TopLeftX = camera.m_viewport.m_mins.x;
	m_viewport.TopLeftY = camera.m_viewport.m_mins.y;
	m_viewport.Width = viewportWidth;
	m_viewport.Height = viewportHeight;
	m_viewport.MinDepth = 0;
	m_viewport.MaxDepth = 1;
	if (m_deviceContext)
	{
		m_deviceContext->RSSetViewports(1, &m_viewport);
	}
	SetModelConstants();
}

void Renderer::EndCamera(const Camera& camera) const
{
	UNUSED(camera);
}

void Renderer::DrawVertexArray(int numVertexes,  Vertex_PCU const* vertexes)
{
	SetStatesIfChanged();
	CopyCPUToGPU(vertexes, (size_t)numVertexes * m_immediateVBO->GetStride(), m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes);
}

void Renderer::DrawVertexArray(VertexBuffer* vbo, int numIndexes, IndexBuffer* ibo)
{
	SetStatesIfChanged();
	BindVertexBuffer(vbo);
	DrawIndexBuffer(ibo, numIndexes);
}

void Renderer::DrawVertexArray(VertexBuffer* vb, int numVerts)
{
	if (!vb)
	{
		return;
	}
	SetStatesIfChanged();
	BindVertexBuffer(vb);
	m_deviceContext->Draw(numVerts, 0);
}

void Renderer::DrawVertexArray(int numVertexes, Vertex_PCUTBN const* vertexes)
{
	SetStatesIfChanged();
	CopyCPUToGPU(vertexes, (size_t)numVertexes * m_immediateVBOForPNCU->GetStride(), m_immediateVBOForPNCU);
	DrawVertexBuffer(m_immediateVBOForPNCU, numVertexes);
}

void Renderer::DrawVertexArrayLines(int numVertexes, Vertex_PCU const* vertexes)
{
	SetStatesIfChanged();
	CopyCPUToGPU(vertexes, (size_t)numVertexes * m_immediateVBO->GetStride(), m_immediateVBO);
	DrawVertexBufferLinesList(m_immediateVBO, numVertexes);
}

BitmapFont* Renderer::CreateOrGetBitmapFont(char const* bitmapFontFilePathNoExtension)
{
	std::string str = bitmapFontFilePathNoExtension;
	str += ".png";
	for (int fontIndex = 0; fontIndex < m_loadedFonts.size(); fontIndex++)
	{
		if (m_loadedFonts[fontIndex]->m_fontFilePathNameWithNoExtension == bitmapFontFilePathNoExtension)
		{
			return m_loadedFonts[fontIndex];
		}
	}
	
	BitmapFont* newBitmapFont = CreateBitMapFontFromFile(str.c_str());
	m_loadedFonts.push_back(newBitmapFont);

	return newBitmapFont;
}

void Renderer::CreateRenderContext()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.Width = m_config.m_window->GetClientDimensions().x;
	swapChainDesc.BufferDesc.Height = m_config.m_window->GetClientDimensions().y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = static_cast<HWND>(m_config.m_window->GetHwnd());
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	unsigned int deviceFlags = 0;

#if defined(ENGINE_DEBUG_RENDER)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags, nullptr, 0,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		ERROR_AND_DIE("Could not initialize pointers group");
	}

	ID3D11Texture2D* backBuffer;
	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not get swap chain buffer.");
	}

	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could create render target view for swap chain buffer.");
	}

	backBuffer->Release();
}


void Renderer::InitializeRasterizerStates()
{
	D3D11_RASTERIZER_DESC m_rasterizerDesc = {};
	m_rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	m_rasterizerDesc.CullMode = D3D11_CULL_BACK;
	m_rasterizerDesc.FrontCounterClockwise = true;
	m_rasterizerDesc.DepthClipEnable = true;
	m_rasterizerDesc.AntialiasedLineEnable = true;

	HRESULT hr = m_device->CreateRasterizerState(&m_rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_BACK]);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Rasterizer State SOLID_CULL_BACK");
	}
	m_rasterizerDesc.CullMode = D3D11_CULL_NONE;


	hr = m_device->CreateRasterizerState(&m_rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_NONE]);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Rasterizer State SOLID_CULL_NONE");
	}
	m_rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

	hr = m_device->CreateRasterizerState(&m_rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_NONE]);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Rasterizer State WIREFRAME_CULL_NONE");
	}
	m_rasterizerDesc.CullMode = D3D11_CULL_BACK;
	hr = m_device->CreateRasterizerState(&m_rasterizerDesc, &m_rasterizerStates[(int)RasterizerMode::WIREFRAME_CULL_BACK]);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Rasterizer State WIREFRAME_CULL_BACK ");
	}
	m_rasterizerState = m_rasterizerStates[(int)RasterizerMode::SOLID_CULL_BACK];
	m_deviceContext->RSSetState(m_rasterizerState);
}

void Renderer::InitializeSamplerStates()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::POINT_CLAMP]);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Sampler State POINT_CLAMP");
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::BILINEAR_WRAP]);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Sampler State BILINEAR_WRAP");
	}
	m_samplerState = m_samplerStates[(int)SamplerMode::POINT_CLAMP];
	m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
}

void Renderer::InitializeBlendStates()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = true;

	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)BlendMode::ALPHA]);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)BlendMode::ADDITIVE]);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;

	m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)BlendMode::OPAQUE]);

	m_blendState = m_blendStates[(int)BlendMode::ALPHA];
	float zerosArray[] = { 0.f,0.f,0.f,0.f };
	m_deviceContext->OMSetBlendState(m_blendState, zerosArray, 0xffffffff);
}

void Renderer::InitializeDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { 0 };
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int) DepthMode::ENABLED]);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Depth Stencil State ENABLED");
	}
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int)DepthMode::DISABLE]);
	if (FAILED(hr))
	{
		ERROR_AND_DIE("Could not Create Depth Stencil State DISABLE");
	}
	m_depthStencilState = m_depthStencilStates[(int)DepthMode::ENABLED];
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
}

Texture* Renderer::CreateOrGetTextureFromFile(char const* imageFilePath)
{
	// See if we already have this texture previously loaded
	Texture* existingTexture = GetTextureForFileName(imageFilePath);
	if (existingTexture)
	{
		return existingTexture;
	}

	// Never seen this texture before!  Let's load it.
	Texture* newTexture = CreateTextureFromFile(imageFilePath);
	return newTexture;
}

Texture* Renderer::CreateTextureFromFile(char const* imageFilePath)
{
	Image localImage = Image(imageFilePath);
	Texture* newTexture = CreateTextureFromImage(localImage);

	return newTexture;
}

BitmapFont* Renderer::CreateBitMapFontFromFile(const char* filePath)
{
	Texture* texture = CreateOrGetTextureFromFile(filePath);
	BitmapFont* font = new BitmapFont(filePath, *texture);
	return font;
}

Texture* Renderer::CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, unsigned char* texelData)
{
	// Check if the load was successful
	GUARANTEE_OR_DIE(texelData, Stringf("CreateTextureFromData failed for \"%s\" - texelData was null!", name));
	GUARANTEE_OR_DIE(bytesPerTexel >= 3 && bytesPerTexel <= 4, Stringf("CreateTextureFromData failed for \"%s\" - unsupported BPP=%i (must be 3 or 4)", name, bytesPerTexel));
	GUARANTEE_OR_DIE(dimensions.x > 0 && dimensions.y > 0, Stringf("CreateTextureFromData failed for \"%s\" - illegal texture dimensions (%i x %i)", name, dimensions.x, dimensions.y));

	Texture* newTexture = new Texture();
	newTexture->m_name = name; // NOTE: m_name must be a std::string, otherwise it may point to temporary data!
	newTexture->m_dimensions = dimensions;

	// Enable OpenGL texturing


	// Tell OpenGL that our pixel data is single-byte aligned


	// Ask OpenGL for an unused texName (ID number) to use for this texture


	// Tell OpenGL to bind (set) this as the currently active texture

	// Set texture clamp vs. wrap (repeat) default settings


	// Set magnification (texel > pixel) and minification (texel < pixel) filters

	// Pick the appropriate OpenGL format (RGB or RGBA) for this texel data
	

	// Upload the image texel data (raw pixels bytes) to OpenGL under this textureID


	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

Texture* Renderer::CreateTextureFromImage(Image const& image)
{
	D3D11_TEXTURE2D_DESC texture2DDesc = {};
	texture2DDesc.Width = image.GetDimensions().x;
	texture2DDesc.Height = image.GetDimensions().y;
	texture2DDesc.MipLevels = 1;
	texture2DDesc.ArraySize = 1;
	texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2DDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture2DDesc.SampleDesc.Count = 1;

	D3D11_SUBRESOURCE_DATA subresData = {};
	subresData.pSysMem = image.GetRawData();
	subresData.SysMemPitch = sizeof(Rgba8) * image.GetDimensions().x;

	Texture* newTexture = new Texture();

	HRESULT hr = m_device->CreateTexture2D(&texture2DDesc, &subresData, &newTexture->m_texture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create Texture2D");
	}
	if (newTexture->m_texture)
	{
		hr = m_device->CreateShaderResourceView(newTexture->m_texture, NULL, &newTexture->m_shaderResourceView);
		if (!SUCCEEDED(hr))
		{
			ERROR_AND_DIE("Could not create ShaderResourceView");
		}
	}
	newTexture->m_name = image.GetImageFilePath();
	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

Texture* Renderer::GetTextureForFileName(char const* imageFilePath)
{
	for (int textureIndex = 0; textureIndex < m_loadedTextures.size(); ++textureIndex)
	{
		if (m_loadedTextures[textureIndex]->GetImageFilePath() == imageFilePath)
		{
			return m_loadedTextures[textureIndex];
		}
	}

	return nullptr;
}


void Renderer::BindTexture(Texture* texture)
{
	if (texture)
	{
		m_deviceContext->PSSetShaderResources(0, 1, &texture->m_shaderResourceView);
	}
	else
	{
		m_deviceContext->PSSetShaderResources(0, 1, &m_defaultTexture->m_shaderResourceView);
	}
}

void Renderer::BindTextures(Texture const* texture0, Texture const* texture1, Texture const* texture2)
{
	if (texture0)
	{
		m_deviceContext->PSSetShaderResources(0, 1, &texture0->m_shaderResourceView);
	}
	else
	{
		m_deviceContext->PSSetShaderResources(0, 1, &m_defaultTexture->m_shaderResourceView);
	}
	if (texture1)
	{
		m_deviceContext->PSSetShaderResources(1, 1, &texture1->m_shaderResourceView);
	}
	else
	{
		m_deviceContext->PSSetShaderResources(1, 1, &m_defaultTexture->m_shaderResourceView);
	}
	if (texture2)
	{
		m_deviceContext->PSSetShaderResources(2, 1, &texture2->m_shaderResourceView);
	}
	else
	{
		m_deviceContext->PSSetShaderResources(2, 1, &m_defaultTexture->m_shaderResourceView);
	}
}

void Renderer::SetBlendMode(BlendMode blendMode)
{
	if (blendMode == BlendMode::ALPHA)
	{
		m_desiredBlendMode = BlendMode::ALPHA;
	}
	else if (blendMode == BlendMode::ADDITIVE)
	{
		m_desiredBlendMode = BlendMode::ADDITIVE;
	}
	else if (blendMode == BlendMode::OPAQUE)
	{
		m_desiredBlendMode = BlendMode::OPAQUE;
	}
	else
	{
		ERROR_AND_DIE(Stringf("Unknown / unsupported blend mode #%i", blendMode));
	}
	
}

void Renderer::SetStatesIfChanged()
{
	for (int modeIndex = 0; modeIndex < (int)BlendMode::COUNT; modeIndex++)
	{
		if (m_blendState == m_blendStates[modeIndex])
		{
			if ((int)m_desiredBlendMode != modeIndex)
			{
				m_blendState = m_blendStates[(int)m_desiredBlendMode];
				
			}
		}
	}
	for (int modeIndex = 0; modeIndex < (int)SamplerMode::COUNT; modeIndex++)
	{
		if (m_samplerState == m_samplerStates[modeIndex])
		{
			if ((int)m_desiredSamplerMode != modeIndex)
			{
				m_samplerState = m_samplerStates[(int)m_desiredSamplerMode];
			}
		}
	}
	for (int modeIndex = 0; modeIndex < (int)RasterizerMode::COUNT; modeIndex++)
	{
		if (m_rasterizerState == m_rasterizerStates[modeIndex])
		{
			if ((int)m_desiredRasterizerMode != modeIndex)
			{
				m_rasterizerState = m_rasterizerStates[(int)m_desiredRasterizerMode];
			}
		}
	}
	for (int modeIndex = 0; modeIndex < (int)DepthMode::COUNT; modeIndex++)
	{
		if (m_depthStencilState == m_depthStencilStates[modeIndex])
		{
			if ((int)m_desiredDepthMode != modeIndex)
			{
				m_depthStencilState = m_depthStencilStates[(int)m_desiredDepthMode];
				
			}
		}
	}
	float zerosArray[] = { 0.f,0.f,0.f,0.f };
	m_deviceContext->OMSetBlendState(m_blendState, zerosArray, 0xffffffff);
	m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	m_deviceContext->RSSetState(m_rasterizerState);
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 0);
}

void Renderer::SetSamplerMode(SamplerMode samplerMode)
{
	switch (samplerMode)
	{
	case SamplerMode::POINT_CLAMP:
		m_desiredSamplerMode = SamplerMode::POINT_CLAMP;
		break;
	case SamplerMode::BILINEAR_WRAP:
		m_desiredSamplerMode = SamplerMode::BILINEAR_WRAP;
		break;
	case SamplerMode::COUNT:
		break;
	default:
		break;
	}
}

void Renderer::SetRasterizerMode(RasterizerMode rasterizerMode)
{
	switch (rasterizerMode)
	{
	case RasterizerMode::SOLID_CULL_NONE:
		m_desiredRasterizerMode = RasterizerMode::SOLID_CULL_NONE;
		break;
	case RasterizerMode::SOLID_CULL_BACK:
		m_desiredRasterizerMode = RasterizerMode::SOLID_CULL_BACK;
		break;
	case RasterizerMode::WIREFRAME_CULL_NONE:
		m_desiredRasterizerMode = RasterizerMode::WIREFRAME_CULL_NONE;
		break;
	case RasterizerMode::WIREFRAME_CULL_BACK:
		m_desiredRasterizerMode = RasterizerMode::WIREFRAME_CULL_BACK;
		break;
	case RasterizerMode::COUNT:
		break;
	default:
		break;
	}
}

void Renderer::SetDepthMode(DepthMode depthMode)
{
	switch (depthMode)
	{
	case DepthMode::DISABLE:
		m_desiredDepthMode = DepthMode::DISABLE;
		break;
	case DepthMode::ENABLED:
		m_desiredDepthMode = DepthMode::ENABLED;
		break;
	case DepthMode::COUNT:
		break;
	default:
		break;
	}
}

Shader* Renderer::GetDefalutShader() const
{
	return m_defalutShader;
}


void Renderer::UpdateLightConstantsBuffer(Vec3 direction, float sunIntensity, float ambientIntensity, int NormalMode /*= 0*/, int SpecularMode /*= 0*/, float SpecularIntensity /*= 0.1f*/, float SpecularPower /*= 32.f*/)
{
	LightConstants localLC = {};
	localLC.SunDirection = direction.GetNormalized();
	localLC.SunIntensity = sunIntensity;
	localLC.AmbientIntensity = ambientIntensity;
	localLC.NormalMode = NormalMode;
	localLC.SpecularMode = SpecularMode;
	localLC.SpecularIntensity = SpecularIntensity;
	localLC.SpecularPower = SpecularPower;

	CopyCPUToGPU(&localLC, sizeof(LightConstants), m_lightingCBO);
	BindConstantBuffer(k_lightingConstantsSlot, m_lightingCBO);
}

ID3D11Device* Renderer::GetD11Device() const
{
	return m_device;
}

ID3D11DeviceContext* Renderer::GetD11DeviceContext() const
{
	return m_deviceContext;
}

void Renderer::BindShader(Shader* shader)
{
	m_currentShader = shader;
	if (shader == nullptr)
	{
		m_deviceContext->VSSetShader(m_defalutShader->m_vertexShader, nullptr, 0);
		m_deviceContext->PSSetShader(m_defalutShader->m_pixelShader, nullptr, 0);
	}
	else
	{
		m_deviceContext->VSSetShader(shader->m_vertexShader, nullptr, 0);
		m_deviceContext->PSSetShader(shader->m_pixelShader, nullptr, 0);
	}
}

void Renderer::BindDefaultShader()
{
	m_currentShader = m_defalutShader;
	
	m_deviceContext->VSSetShader(m_defalutShader->m_vertexShader, nullptr, 0);
	m_deviceContext->PSSetShader(m_defalutShader->m_pixelShader, nullptr, 0);
}

Shader* Renderer::CreateShader(char const* shaderName, char const* shaderSource, VertexType vertexType)
{
	UNUSED(vertexType);
	for (int shaderIndex = 0; shaderIndex < m_loadedShaders.size(); ++shaderIndex)
	{
		if (m_loadedShaders[shaderIndex]->GetName() == shaderName)
		{
			return m_loadedShaders[shaderIndex];
		}
	}
	ShaderConfig newConfig;
	newConfig.m_name = shaderName;
	Shader* newShader = new Shader(newConfig);
	m_currentShader = newShader;
	
	std::vector<uint8_t> m_vertexShaderByteCode;
	std::vector<uint8_t> m_pixelShaderByteCode;
	CompileShaderToByteCode(m_vertexShaderByteCode, shaderName, shaderSource, "VertexMain", "vs_5_0");
	CompileShaderToByteCode(m_pixelShaderByteCode, shaderName, shaderSource, "PixelMain", "ps_5_0");
	m_device->CreateVertexShader(m_vertexShaderByteCode.data(), m_vertexShaderByteCode.size(), nullptr, &m_currentShader->m_vertexShader);
	m_device->CreatePixelShader(m_pixelShaderByteCode.data(), m_pixelShaderByteCode.size(), nullptr, &m_currentShader->m_pixelShader);

	size_t pos = newShader->GetName().find("Lit");
	size_t pos1 = newShader->GetName().find("Sprite");
	if (pos != std::string::npos || pos1 != std::string::npos)
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
 			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		unsigned int numElement = sizeof(inputElementDesc) / sizeof(inputElementDesc[0]);
		HRESULT hr = m_device->CreateInputLayout(inputElementDesc, numElement, m_vertexShaderByteCode.data(), m_vertexShaderByteCode.size(), &(m_currentShader->m_inputLayout));
		if (!SUCCEEDED(hr))
		{
			ERROR_AND_DIE("Could not create input layout for Lit shader");
		}
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"POSITION2D", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		unsigned int numElement = sizeof(inputElementDesc) / sizeof(inputElementDesc[0]);
		HRESULT hr = m_device->CreateInputLayout(inputElementDesc, numElement, m_vertexShaderByteCode.data(), m_vertexShaderByteCode.size(), &(m_currentShader->m_inputLayout));
		if (!SUCCEEDED(hr))
		{
			ERROR_AND_DIE("Could not create input layout");
		}
	}


	m_loadedShaders.push_back(newShader);
	return newShader;
}

Shader* Renderer::CreateShader(char const* shaderName, VertexType vertexType /*= VERTEX_PCU*/)
{
	std::string shaderNameStr(shaderName);
	shaderNameStr += ".hlsl";
	std::string shaderSource;
 	int flag = FileReadToString(shaderSource, shaderNameStr);
	if (flag == -1)
	{
		DebuggerPrintf("Shader read fail!");
	}

	Shader* result = CreateShader(shaderNameStr.c_str(), shaderSource.c_str(), vertexType);
	return result;
}


bool Renderer::CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target)
{
	DWORD flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined(ENGINE_DEBUG_RENDER)
	flags = D3DCOMPILE_DEBUG;
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	flags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif

	ID3D10Blob* Blob = nullptr;
	ID3D10Blob* pError = nullptr;

	HRESULT hr = D3DCompile(source, strlen(source), name, nullptr, nullptr, entryPoint, target, flags, 0, &Blob, &pError);
	if (!SUCCEEDED(hr))
	{
		if (pError)
		{
			const char* pErrorMessage = (const char*)pError->GetBufferPointer();
			DebuggerPrintf(pErrorMessage);
			pError->Release();
		}
		ERROR_AND_DIE("Could not compile shader");
	}
	else
	{
		outByteCode.assign((unsigned char*)Blob->GetBufferPointer(), (unsigned char*)Blob->GetBufferPointer() + Blob->GetBufferSize());
		DX_SAFE_RELEASE(Blob);
		return true;
	}

}

VertexBuffer* Renderer::CreateVertexBuffer(size_t const size, unsigned int stride)
{
	ID3D11Buffer* m_buffer = nullptr;
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create buffer");
	}
	
	VertexBuffer* result = new VertexBuffer(size, stride);
	result->m_buffer = m_buffer;
	return result;
}

void Renderer::CopyCPUToGPU(void const* data, size_t size, VertexBuffer*& vbo)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	if (vbo->m_size < size)
	{
		unsigned int stride = vbo->GetStride();
		delete vbo;
		vbo = CreateVertexBuffer(size, stride);
	}
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = { 0 };

	HRESULT hr = m_deviceContext->Map(vbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not Map");
	}
	memcpy(mappedSubresource.pData, data, size);

	m_deviceContext->Unmap(vbo->m_buffer, NULL);
}

void Renderer::CopyCPUToGPU(void const* data, size_t size, ConstantBuffer* cbo)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = { 0 };

	HRESULT hr = m_deviceContext->Map(cbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not Map");
	}
	memcpy(mappedSubresource.pData, data, size);

	m_deviceContext->Unmap(cbo->m_buffer, NULL);
}

void Renderer::CopyCPUToGPU(void const* data, size_t size, ModelConstantBuffer* modelCBO)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = { 0 };

	HRESULT hr = m_deviceContext->Map(modelCBO->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not Map");
	}
	memcpy(mappedSubresource.pData, data, size);

	m_deviceContext->Unmap(modelCBO->m_buffer, NULL);
}

void Renderer::BindConstantBuffer(int slot, ConstantBuffer* cbo)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	m_deviceContext->VSSetConstantBuffers(slot, 1, &cbo->m_buffer);
	m_deviceContext->PSSetConstantBuffers(slot, 1, &cbo->m_buffer);

	if (m_currentShader)
	{
		m_deviceContext->IASetInputLayout(m_currentShader->m_inputLayout);
	}
	else
	{
		m_deviceContext->IASetInputLayout(m_defalutShader->m_inputLayout);
	}
}

void Renderer::BindConstantBuffer(int slot, ModelConstantBuffer* modelCBO)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	m_deviceContext->VSSetConstantBuffers(slot, 1, &modelCBO->m_buffer);
	m_deviceContext->PSSetConstantBuffers(slot, 1, &modelCBO->m_buffer);
	if (m_currentShader)
	{
		m_deviceContext->IASetInputLayout(m_currentShader->m_inputLayout);
	}
	else
	{
		m_deviceContext->IASetInputLayout(m_defalutShader->m_inputLayout);
	}
}

ModelConstantBuffer* Renderer::CreateModelConstantBuffer(const size_t size)
{
	ID3D11Buffer* m_buffer = nullptr;
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create constant buffer");
	}

	ModelConstantBuffer* result = new ModelConstantBuffer(size);
	result->m_buffer = m_buffer;
	return result;
}

void Renderer::SetModelConstants(Mat44 const& modelMatrix /*= Mat44()*/, Rgba8 const& modelColor /*= Rgba8::WHITE*/)
{
	ModelConstants modelBuffer = {};
	modelBuffer.ModelMatrix = modelMatrix;
	float colorFloat[4];
	modelColor.GetAsFloats(colorFloat);
	modelBuffer.ModelColor = Vec4(colorFloat[0], colorFloat[1], colorFloat[2], colorFloat[3]);

	CopyCPUToGPU(&modelBuffer, sizeof(ModelConstants), m_modelCBO);
	BindConstantBuffer(k_modelConstantsSlot, m_modelCBO);
}

void Renderer::BindVertexBuffer(VertexBuffer*& vbo)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	unsigned int stride;
	unsigned int offset;
	stride = vbo->GetStride();
	offset = 0;

	
	m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &offset);
	if (m_currentShader)
	{
		m_deviceContext->IASetInputLayout(m_currentShader->m_inputLayout);
	}
	else
	{
		m_deviceContext->IASetInputLayout(m_defalutShader->m_inputLayout);
	}
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}


void Renderer::DrawVertexBuffer(VertexBuffer*& vbo, int vertexCount, int vertexOffset /*= 0*/)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	BindVertexBuffer(vbo);
	m_deviceContext->Draw(vertexCount, vertexOffset);
}

void Renderer::BindVertexBufferLinesList(VertexBuffer*& vbo)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	unsigned int stride;
	unsigned int offset;
	stride = vbo->GetStride();
	offset = 0;


	m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &offset);
	if (m_currentShader)
	{
		m_deviceContext->IASetInputLayout(m_currentShader->m_inputLayout);
	}
	else
	{
		m_deviceContext->IASetInputLayout(m_defalutShader->m_inputLayout);
	}
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Renderer::DrawVertexBufferLinesList(VertexBuffer*& vbo, int vertexCount, int vertexOffset /*= 0*/)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	BindVertexBufferLinesList(vbo);
	m_deviceContext->Draw(vertexCount, vertexOffset);
}

IndexBuffer* Renderer::CreateIndexBuffer(size_t const size)
{
	ID3D11Buffer* m_buffer = nullptr;
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create buffer");
	}

	IndexBuffer* result = new IndexBuffer(size);
	result->m_buffer = m_buffer;
	return result;
}

void Renderer::CopyCPUToGPU(void const* data, size_t size, IndexBuffer*& ibo)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	if (ibo->m_size < size)
	{
		delete ibo;

		ibo = CreateIndexBuffer(size);
	}
	D3D11_MAPPED_SUBRESOURCE mappedSubresource = { 0 };

	HRESULT hr = m_deviceContext->Map(ibo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not Map index buffer");
	}
	memcpy(mappedSubresource.pData, data, size);

	m_deviceContext->Unmap(ibo->m_buffer, NULL);
}

void Renderer::BindIndexBuffer(IndexBuffer*& ibo)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	m_deviceContext->IASetIndexBuffer(ibo->m_buffer, DXGI_FORMAT_R32_UINT, 0);
	if (m_currentShader)
	{
		m_deviceContext->IASetInputLayout(m_currentShader->m_inputLayout);
	}
	else
	{
		m_deviceContext->IASetInputLayout(m_defalutShader->m_inputLayout);
	}
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::DrawIndexBuffer(IndexBuffer*& ibo, int indexCount)
{
	if (m_deviceContext == nullptr)
	{
		return;
	}
	BindIndexBuffer(ibo);
	m_deviceContext->DrawIndexed(indexCount, 0,0);
}

ConstantBuffer* Renderer::CreateConstantBuffer(const size_t size)
{
	ID3D11Buffer* m_buffer = nullptr;
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = m_device->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create constant buffer");
	}

	ConstantBuffer* result = new ConstantBuffer(size);
	result->m_buffer = m_buffer;
	return result;
}

