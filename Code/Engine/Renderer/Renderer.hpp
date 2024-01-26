#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
// ---------Macro---------------------------
#define DX_SAFE_RELEASE(dxObject)			\
{											\
	if ((dxObject) != nullptr)				\
	{										\
		(dxObject)->Release();				\
		(dxObject) = nullptr;				\
	}										\
}
//------------------------------------------
#include <vector>
#include <Engine/Core/Rgba8.hpp>
#include <Engine/Renderer/Camera.hpp>
#include <Engine/Core/Vertex_PCU.hpp>
#include "Engine/Window/Window.hpp"
#include "Game/EngineBuildPreferences.hpp"
#include "../Math/Vec4.hpp"
#include "Engine/Math/Vec3.hpp"
#include "IndexBuffer.hpp"
#include "../Core/Vertex_PNCU.hpp"

#if defined(OPAQUE)
#undef OPAQUE
#endif

class VertexBuffer;
class IndexBuffer;
class Shader;
class BitmapFont;
class Texture;
struct IntVec2;
class ConstantBuffer;
class ModelConstantBuffer;
class Image;


enum class SamplerMode
{
	POINT_CLAMP,
	BILINEAR_WRAP,
	COUNT
};

enum class RasterizerMode
{
	SOLID_CULL_NONE,
	SOLID_CULL_BACK,
	WIREFRAME_CULL_NONE,
	WIREFRAME_CULL_BACK,
	COUNT
};

enum class DepthMode
{
	DISABLE,
	ENABLED,
	COUNT
};

enum class BlendMode
{
	ALPHA,
	ADDITIVE, 
	OPAQUE,
	COUNT
};

struct ModelConstants
{
	Mat44 ModelMatrix;
	Vec4 ModelColor;
};


struct RendererConfig
{
	Window* m_window = nullptr;
};

struct LightConstants
{
	Vec3 SunDirection;
	float SunIntensity = 0.f;
	float AmbientIntensity = 0.f;
	Vec3 WorldEyePosition;
	int  NormalMode;
	int  SpecularMode;
	float SpecularIntensity;
	float SpecularPower;
	//Vec3 padding;
};

enum VertexType
{
	VERTEX_PCU,
	VERTEX_PNCU,
	VERTEX_PCUTBN,
};




//----------------------------------------------
class Renderer
{
public:
	Renderer(RendererConfig const& config);
	~Renderer();
	void Startup();
	void BeginFrame()   const;
	void EndFrame()		const;
	void Shutdown();

	void ClearScreen(Rgba8 const& clearColor)					const;
	void BeginCamera(Camera& camera)						;
	void EndCamera(const Camera& camera)						const;
	void DrawVertexArray(int numVertexes, Vertex_PCU const* vertexes);
	void DrawVertexArrayLines(int numVertexes, Vertex_PCU const* vertexes);
	void DrawVertexArray(int numVertexes, Vertex_PCUTBN const* vertexes);
	void DrawVertexArray(VertexBuffer* vbo, int numIndexes, IndexBuffer* ibo);
	void DrawVertexArray(VertexBuffer* vb, int numVerts);
	BitmapFont* CreateOrGetBitmapFont(char const* bitmapFontFilePathNoExtension);
	Texture* CreateOrGetTextureFromFile(char const* imageFilePath);
	Texture* CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, unsigned char* texelData);
	Texture* CreateTextureFromImage(Image const& image);
	Texture* GetTextureForFileName(char const* imageFilePath);
	void BindTexture(Texture* texture);
	void BindTextures(Texture const* texture0, Texture const* texture1, Texture const* texture2);

	RendererConfig const& GetConfig() const { return m_config; }

	void BindShader(Shader* shader);
	void BindDefaultShader();
	Shader* CreateShader(char const* shaderName, char const* shaderSource, VertexType vertexType = VERTEX_PCU);
	Shader* CreateShader(char const* shaderName, VertexType vertexType = VERTEX_PCU);
	bool CompileShaderToByteCode(std::vector<unsigned char>& outByteCode, char const* name, char const* source, char const* entryPoint, char const* target);

	VertexBuffer* CreateVertexBuffer(size_t const size, unsigned int stride);
	void CopyCPUToGPU(void const* data, size_t size, VertexBuffer*& vbo);
	void BindVertexBuffer(VertexBuffer*& vbo);
	void DrawVertexBuffer(VertexBuffer*& vbo, int vertexCount, int vertexOffset = 0);
	void BindVertexBufferLinesList(VertexBuffer*& vbo);
	void DrawVertexBufferLinesList(VertexBuffer*& vbo,int vertexCount, int vertexOffset = 0);

	IndexBuffer* CreateIndexBuffer(size_t const size);
	void CopyCPUToGPU(void const* data, size_t size, IndexBuffer*& ibo);
	void BindIndexBuffer(IndexBuffer*& ibo);
	void DrawIndexBuffer(IndexBuffer*& ibo, int indexCount);

	ConstantBuffer* CreateConstantBuffer(const size_t size);
	void CopyCPUToGPU(void const* data, size_t size, ConstantBuffer* cbo);
	void BindConstantBuffer(int slot, ConstantBuffer* cbo);

	ModelConstantBuffer* CreateModelConstantBuffer(const size_t size);
	void CopyCPUToGPU(void const* data, size_t size, ModelConstantBuffer* modelCBO);
	void BindConstantBuffer(int slot, ModelConstantBuffer* modelCBO);
	void SetModelConstants(Mat44 const& modelMatrix = Mat44(), Rgba8 const& modelColor = Rgba8::WHITE);

	void SetBlendMode(BlendMode blendMode);
	void SetStatesIfChanged();
	void SetSamplerMode(SamplerMode samplerMode);
	void SetRasterizerMode(RasterizerMode rasterizerMode);
	void SetDepthMode(DepthMode depthMode);

	Shader* GetDefalutShader() const;
	void UpdateLightConstantsBuffer(Vec3 direction, float sunIntensity, float ambientIntensity, int NormalMode = 0, int SpecularMode = 0, float SpecularIntensity = 0.1f, float SpecularPower = 32.f);

	ID3D11Device* GetD11Device() const;
	ID3D11DeviceContext* GetD11DeviceContext() const;
private:
	Texture* CreateTextureFromFile(char const* imageFilePath);
	BitmapFont* CreateBitMapFontFromFile(const char* filePath);
	void CreateRenderContext();/*const;*/
	void InitializeRasterizerStates();
	void InitializeSamplerStates();
	void InitializeBlendStates();

	void InitializeDepthStencilState();
protected:
	RendererConfig m_config;
	std::vector<Texture*> m_loadedTextures;
	std::vector<BitmapFont*> m_loadedFonts;

	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;

	void* m_dxgiDebug = nullptr;
	void* m_dxgiDebugModeule = nullptr;

	std::vector<Shader*> m_loadedShaders;
	Shader* m_defalutShader = nullptr;
	Shader* m_currentShader = nullptr;
	VertexBuffer* m_immediateVBO = nullptr;
	VertexBuffer* m_immediateVBOForPNCU = nullptr;
	ConstantBuffer* m_cameraCBO = nullptr;
	ConstantBuffer* m_lightingCBO = nullptr;
	ModelConstantBuffer* m_modelCBO = nullptr;

	Texture const* m_defaultTexture = nullptr;

	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D* m_depthStencilTexture = nullptr;

	ID3D11BlendState* m_blendState = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;
	ID3D11RasterizerState* m_rasterizerState = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;

	BlendMode m_desiredBlendMode = BlendMode::ALPHA;
	SamplerMode m_desiredSamplerMode = SamplerMode::POINT_CLAMP;
	RasterizerMode m_desiredRasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	DepthMode m_desiredDepthMode = DepthMode::ENABLED;

	ID3D11BlendState* m_blendStates[(int)(BlendMode::COUNT)] = {};
	ID3D11SamplerState* m_samplerStates[(int)(SamplerMode::COUNT)] = {};
	ID3D11RasterizerState* m_rasterizerStates[(int)(RasterizerMode::COUNT)] = {};
	ID3D11DepthStencilState* m_depthStencilStates[(int)(DepthMode::COUNT)] = {};


};