#include "DebugRender.hpp"
#include "../Core/Vertex_PCU.hpp"
#include <vector>
#include "Engine/Renderer/Renderer.hpp"
#include "../Core/Stopwatch.hpp"
#include "../Math/MathUtils.hpp"
#include "../Core/VertexUtils.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
/*#include "Game/Player.hpp"*/
class DebugRender;
DebugRender* m_DebugRender = nullptr;
class Player;
class DebugRenderEntity
{
public:
	DebugRenderEntity(float duration);
	~DebugRenderEntity();

	void Render();
	void SetDebugRenderMode();
public:
	std::vector<Vertex_PCU> verts;
	Stopwatch* m_watch = nullptr;
	Rgba8 m_currentColor;
	Rgba8 m_startColor;
	Rgba8 m_endColor;
	Rgba8 m_xRayColor;
	DebugRenderMode m_mode = DebugRenderMode::USE_DEPTH;
	RasterizerMode rasterizeMode = RasterizerMode::SOLID_CULL_BACK;
	BlendMode blendMode = BlendMode::ALPHA;
	DepthMode depthMode = DepthMode::ENABLED;
	SamplerMode samplerMode = SamplerMode::POINT_CLAMP;
	Texture* m_texture = nullptr;
	//--------- For message -----------
	bool isMessage = false;
	int  slotIndex = 0;
	//--------- billboard--------------
	bool isBillBoard = false;
	Vec3 m_BillBoardPosition;
};

class DebugRender
{
public:
	DebugRender() {};
	~DebugRender() {};
	void ClearEntities();
public:
	DebugRenderConfig m_config;
	std::vector<DebugRenderEntity*> WorldEntities;
	std::vector<DebugRenderEntity*> screenTexts;
	bool m_isVisible = false;
	BitmapFont* m_font = nullptr;
	Player* m_currentPlayer = nullptr;
};

void DebugRender::ClearEntities()
{
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		delete  m_DebugRender->WorldEntities[entityIndex];
		m_DebugRender->WorldEntities[entityIndex] = nullptr;
	}
	for (int entityIndex = 0; entityIndex < m_DebugRender->screenTexts.size(); ++entityIndex)
	{
		delete  m_DebugRender->screenTexts[entityIndex];
		m_DebugRender->screenTexts[entityIndex] = nullptr;
	}
}


DebugRenderEntity::DebugRenderEntity(float duration)
{
	if (duration == -1.f)
	{
		m_watch = new Stopwatch(99999999999999999999999999999999.f);
	}
	else
	{
		m_watch = new Stopwatch(duration);
	}
	m_watch->Start();
}

DebugRenderEntity::~DebugRenderEntity()
{
	delete m_watch;
}

void DebugRenderEntity::Render()
{
	if (m_mode == DebugRenderMode::X_RAY)
	{
		for (int vertIndex = 0; vertIndex < verts.size(); ++vertIndex)
		{
			verts[vertIndex].m_color = m_xRayColor;
		}
	}
	m_DebugRender->m_config.m_renderer->SetBlendMode(blendMode);
	m_DebugRender->m_config.m_renderer->BindShader(nullptr);
	m_DebugRender->m_config.m_renderer->BindTexture(m_texture);
	m_DebugRender->m_config.m_renderer->SetRasterizerMode(rasterizeMode);
	m_DebugRender->m_config.m_renderer->SetDepthMode(depthMode);
	m_DebugRender->m_config.m_renderer->DrawVertexArray((int)verts.size(), verts.data());
	if (m_mode == DebugRenderMode::X_RAY)
	{
		for (int vertIndex = 0; vertIndex < verts.size(); ++vertIndex)
		{
			verts[vertIndex].m_color = m_currentColor;
		}
		m_DebugRender->m_config.m_renderer->SetBlendMode(BlendMode::OPAQUE);
		m_DebugRender->m_config.m_renderer->SetDepthMode(DepthMode::ENABLED);
		m_DebugRender->m_config.m_renderer->DrawVertexArray((int)verts.size(), verts.data());
	}
}


void DebugRenderEntity::SetDebugRenderMode()
{
	if (m_mode == DebugRenderMode::ALWAYS)
	{
		blendMode = BlendMode::ALPHA;
		depthMode = DepthMode::DISABLE;
		samplerMode = SamplerMode::POINT_CLAMP;
	}
	if (m_mode == DebugRenderMode::X_RAY)
	{
		blendMode = BlendMode::ALPHA;
		depthMode = DepthMode::DISABLE;
		samplerMode = SamplerMode::POINT_CLAMP;
	}
	if (m_mode == DebugRenderMode::USE_DEPTH)
	{
		blendMode = BlendMode::ALPHA;
		depthMode = DepthMode::ENABLED;
		samplerMode = SamplerMode::POINT_CLAMP;
	}
}

void DebugRenderSystemStartup(DebugRenderConfig const& config)
{
	m_DebugRender = new DebugRender();
	m_DebugRender->m_config = config;
	m_DebugRender->m_font = config.m_renderer->CreateOrGetBitmapFont("Data/Fonts/SquirrelFixedFont");
	DebugAddWorldArrow(Vec3(0.f, 0.f, 0.f), Vec3(1.5f, 0.f, 0.f), 0.1f, -1.f, Rgba8::RED, Rgba8::RED);
	DebugAddWorldArrow(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 1.5f, 0.f), 0.1f, -1.f, Rgba8::GREEN, Rgba8::GREEN);
	DebugAddWorldArrow(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 1.5f), 0.1f, -1.f, Rgba8::BLUE, Rgba8::BLUE);
	Mat44 xTransform = Mat44::CreateTranslation3D(Vec3(1.3f, 0.f, 0.5f));
	xTransform.AppendXRotation(90.f);
	DebugAddWorldText("x - forward", xTransform, 0.2f, Vec2(.5f, .5f), -1.f, Rgba8::RED,Rgba8::RED);
	Mat44 yTransform = Mat44::CreateTranslation3D(Vec3(0.0f, 1.f, 0.5f));
	yTransform.AppendZRotation(-90.f);
	yTransform.AppendXRotation(90.f);
	DebugAddWorldText("y - left", yTransform, 0.2f, Vec2(.5f, .5f), -1.f, Rgba8::GREEN, Rgba8::GREEN);
	Mat44 zTransform = Mat44::CreateTranslation3D(Vec3(0.f, -0.5f, 1.f));
	zTransform.AppendYRotation(-90.f);
	DebugAddWorldText("z - up", zTransform, 0.2f, Vec2(.5f, .5f), -1.f, Rgba8::BLUE, Rgba8::BLUE);

	m_DebugRender->m_isVisible = !config.m_startHidden;
	SubscribeEventCallbackFunction("debugrenderclear", Command_DebugRenderClear);
	SubscribeEventCallbackFunction("debugrendertoggle", Command_DebugRenderToggle);
}

void DebugRenderSystemShutdown()
{
	m_DebugRender->ClearEntities();
	delete m_DebugRender;
}

void DebugRenderSetVisible()
{
	m_DebugRender->m_isVisible = true;
}

void DebugRenderSetHidden()
{
	m_DebugRender->m_isVisible = false;
}

void DebugRenderClear()
{
	m_DebugRender->ClearEntities();
}

void DebugRenderBeginFrame()
{
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		DebugRenderEntity* entity = m_DebugRender->WorldEntities[entityIndex];
		if (entity == nullptr)
		{
			continue;
		}
		if (entity->m_startColor == entity->m_endColor && entity->m_mode != DebugRenderMode::X_RAY)
		{
			continue;
		}
		float colorFraction = entity->m_watch->GetElapsedFration();
		entity->m_currentColor.r = (unsigned char)Interpolate((float)entity->m_startColor.r, (float)entity->m_endColor.r, colorFraction);
		entity->m_currentColor.g = (unsigned char)Interpolate((float)entity->m_startColor.g, (float)entity->m_endColor.g, colorFraction);
		entity->m_currentColor.b = (unsigned char)Interpolate((float)entity->m_startColor.b, (float)entity->m_endColor.b, colorFraction);
		entity->m_currentColor.a = (unsigned char)Interpolate((float)entity->m_startColor.a, (float)entity->m_endColor.a, colorFraction);
		entity->m_xRayColor.r = (unsigned char)GetClamped((float)(entity->m_currentColor.r + 100.f), 0.f, 255.f);
		entity->m_xRayColor.g = (unsigned char)GetClamped((float)(entity->m_currentColor.g + 100.f), 0.f, 255.f);
		entity->m_xRayColor.b = (unsigned char)GetClamped((float)(entity->m_currentColor.b + 100.f), 0.f, 255.f);
		entity->m_xRayColor.a = (unsigned char)GetClamped((float)(entity->m_currentColor.a - 70.f), 50.f, 150.f);
		for (int vertIndex = 0; vertIndex < entity->verts.size(); ++vertIndex)
		{
			entity->verts[vertIndex].m_color = entity->m_currentColor;
		}
	}

	for (int entityIndex = 0; entityIndex < m_DebugRender->screenTexts.size(); ++entityIndex)
	{
		DebugRenderEntity* entity = m_DebugRender->screenTexts[entityIndex];
		if (entity == nullptr)
		{
			continue;
		}
		if (entity->m_startColor == entity->m_endColor && entity->m_mode != DebugRenderMode::X_RAY)
		{
			continue;
		}
		float colorFraction = entity->m_watch->GetElapsedFration();
		entity->m_currentColor.r = (unsigned char)Interpolate((float)entity->m_startColor.r, (float)entity->m_endColor.r, colorFraction);
		entity->m_currentColor.g = (unsigned char)Interpolate((float)entity->m_startColor.g, (float)entity->m_endColor.g, colorFraction);
		entity->m_currentColor.b = (unsigned char)Interpolate((float)entity->m_startColor.b, (float)entity->m_endColor.b, colorFraction);
		entity->m_currentColor.a = (unsigned char)Interpolate((float)entity->m_startColor.a, (float)entity->m_endColor.a, colorFraction);
		entity->m_xRayColor.r = (unsigned char)GetClamped((float)(entity->m_currentColor.r + 100.f), 0.f, 255.f);
		entity->m_xRayColor.g = (unsigned char)GetClamped((float)(entity->m_currentColor.g + 100.f), 0.f, 255.f);
		entity->m_xRayColor.b = (unsigned char)GetClamped((float)(entity->m_currentColor.b + 100.f), 0.f, 255.f);
		entity->m_xRayColor.a = (unsigned char)GetClamped((float)(entity->m_currentColor.a - 70.f), 100.f, 255.f);
		for (int vertIndex = 0; vertIndex < entity->verts.size(); ++vertIndex)
		{
			entity->verts[vertIndex].m_color = entity->m_currentColor;
		}
	}
}

void DebugRenderWorld(Camera& camera)
{
	if (m_DebugRender->m_isVisible)
	{
		m_DebugRender->m_config.m_renderer->BeginCamera(camera);
		for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
		{
			if (m_DebugRender->WorldEntities[entityIndex] == nullptr)
			{
				continue;
			}
			if (m_DebugRender->WorldEntities[entityIndex]->isBillBoard)
			{
				Vec3 position = m_DebugRender->WorldEntities[entityIndex]->m_BillBoardPosition;
				m_DebugRender->m_config.m_renderer->SetModelConstants(GetBillboardMatrix(BillboardType::FULL_CAMERA_OPPOSING, camera.m_orientation.GetAsMatrix_XFwd_YLeft_ZUp(), position));
				m_DebugRender->WorldEntities[entityIndex]->Render();
			}
			else
			{
				m_DebugRender->m_config.m_renderer->SetModelConstants();
				m_DebugRender->WorldEntities[entityIndex]->Render();
			}
		}
		m_DebugRender->m_config.m_renderer->EndCamera(camera);
	}
}

void DebugRenderScreen(Camera& camera)
{
	if (m_DebugRender->m_isVisible)
	{
		m_DebugRender->m_config.m_renderer->BeginCamera(camera);
		for (int entityIndex = 0; entityIndex < m_DebugRender->screenTexts.size(); ++entityIndex)
		{
			if (m_DebugRender->screenTexts[entityIndex] == nullptr)
			{
				continue;
			}
			m_DebugRender->m_config.m_renderer->SetModelConstants();
			m_DebugRender->screenTexts[entityIndex]->Render();
		}
		m_DebugRender->m_config.m_renderer->EndCamera(camera);
	}
}

void DebugRenderEndFrame()
{
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		DebugRenderEntity* entity = m_DebugRender->WorldEntities[entityIndex];
		if (entity == nullptr)
		{
			continue;
		}
		if (entity->m_watch->GetElapsedFration() >= 1.f || entity->m_watch->m_duration == 0.f)
		{
			delete m_DebugRender->WorldEntities[entityIndex];
			m_DebugRender->WorldEntities[entityIndex] = nullptr;
		}
	}

	for (int entityIndex = 0; entityIndex < m_DebugRender->screenTexts.size(); ++entityIndex)
	{
		DebugRenderEntity* entity = m_DebugRender->screenTexts[entityIndex];
		if (entity == nullptr)
		{
			continue;
		}
		if (entity->m_watch->GetElapsedFration() >= 1.f || entity->m_watch->m_duration == 0.f)
		{
			if (entity->isMessage)
			{
				for (int innerIndex = 0; innerIndex < m_DebugRender->screenTexts.size(); ++innerIndex)
				{
					DebugRenderEntity*& innerEntity = m_DebugRender->screenTexts[innerIndex];
					if (innerEntity == nullptr)
					{
						continue;
					}
					if (innerEntity->isMessage)
					{
						if (m_DebugRender->screenTexts[entityIndex]->slotIndex < innerEntity->slotIndex)
						{
							innerEntity->slotIndex--;
							TransformVertexArrayXY3D((int)innerEntity->verts.size(), innerEntity->verts.data(), 1.f, 0.f, Vec2(0.f, 20.f));
						}
					}
				}
			}
			delete m_DebugRender->screenTexts[entityIndex];
			m_DebugRender->screenTexts[entityIndex] = nullptr;
		}
	}
}

void DebugAddWorldPoint(Vec3 const& pos, float radius, float duration, Rgba8 const& startColor /*= Rgba8::WHITE*/, Rgba8 const& endColor /*= Rgba8::WHITE*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->WorldEntities[entityIndex];
		if (m_DebugRender->WorldEntities[entityIndex] == nullptr)
		{
			entity = new DebugRenderEntity(duration);
			entity->m_startColor = startColor;
			entity->m_currentColor = startColor;
			entity->m_endColor = endColor;
			entity->m_mode = mode;
			entity->SetDebugRenderMode();
			AddVertsForUVSphereZ3D(entity->verts, pos, radius, 8, 16, startColor);
			return;
		}
	}
	DebugRenderEntity* newEntity = new DebugRenderEntity(duration);
	m_DebugRender->WorldEntities.push_back(newEntity);
	newEntity->m_startColor = startColor;
	newEntity->m_currentColor = startColor;
	newEntity->m_endColor = endColor;
	newEntity->m_mode = mode;
	newEntity->SetDebugRenderMode();
	AddVertsForUVSphereZ3D(newEntity->verts, pos, radius, 8, 16, startColor);
	return;
}

void DebugAddWorldLine(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor /*= Rgba8::WHITE*/, Rgba8 const& endColor /*= Rgba8::WHITE*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->WorldEntities[entityIndex];
		if (m_DebugRender->WorldEntities[entityIndex] == nullptr)
		{
			entity = new DebugRenderEntity(duration);
			entity->m_startColor = startColor;
			entity->m_currentColor = startColor;
			entity->m_endColor = endColor;
			entity->m_mode = mode;
			entity->SetDebugRenderMode();
			AddVertsForCylinder3D(entity->verts, start, end, radius, entity->m_currentColor);
			return;
		}
	}
	DebugRenderEntity* newEntity = new DebugRenderEntity(duration);
	m_DebugRender->WorldEntities.push_back(newEntity);
	newEntity->m_startColor = startColor;
	newEntity->m_currentColor = startColor;
	newEntity->m_endColor = endColor;
	newEntity->m_mode = mode;
	newEntity->SetDebugRenderMode();
	AddVertsForCylinder3D(newEntity->verts, start, end, radius, newEntity->m_currentColor);
	return;
}

void DebugAddWorldWireCylinder(Vec3 const& base, Vec3 const& top, float radius, float duration, Rgba8 const& startColor /*= Rgba8::WHITE*/, Rgba8 const& endColor /*= Rgba8::WHITE*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->WorldEntities[entityIndex];
		if (m_DebugRender->WorldEntities[entityIndex] == nullptr)
		{
			entity = new DebugRenderEntity(duration);
			entity->m_startColor = startColor;
			entity->m_currentColor = startColor;
			entity->m_endColor = endColor;
			entity->m_mode = mode;
			entity->rasterizeMode = RasterizerMode::WIREFRAME_CULL_BACK;
			entity->SetDebugRenderMode();
			AddVertsForCylinder3D(entity->verts, base, top, radius, entity->m_currentColor, AABB2::ZERO_TO_ONE, 16);
			return;
		}
	}
	DebugRenderEntity* newEntity = new DebugRenderEntity(duration);
	m_DebugRender->WorldEntities.push_back(newEntity);
	newEntity->m_startColor = startColor;
	newEntity->m_currentColor = startColor;
	newEntity->m_endColor = endColor;
	newEntity->m_mode = mode;
	newEntity->rasterizeMode = RasterizerMode::WIREFRAME_CULL_BACK;
	newEntity->SetDebugRenderMode();
	AddVertsForCylinder3D(newEntity->verts, base, top, radius, newEntity->m_currentColor, AABB2::ZERO_TO_ONE, 16);
	return;
}

void DebugAddWorldWireSphere(Vec3 const& center, float radius, float duration, Rgba8 const& startColor /*= Rgba8::WHITE*/, Rgba8 const& endColor /*= Rgba8::WHITE*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->WorldEntities[entityIndex];
		if (m_DebugRender->WorldEntities[entityIndex] == nullptr)
		{
			entity = new DebugRenderEntity(duration);
			entity->m_startColor = startColor;
			entity->m_currentColor = startColor;
			entity->m_endColor = endColor;
			entity->m_mode = mode;
			entity->rasterizeMode = RasterizerMode::WIREFRAME_CULL_BACK;
			entity->SetDebugRenderMode();
			AddVertsForUVSphereZ3D(entity->verts, center, radius, 12.f, 24.f, entity->m_currentColor, AABB2::ZERO_TO_ONE);
			return;
		}
	}
	DebugRenderEntity* newEntity = new DebugRenderEntity(duration);
	m_DebugRender->WorldEntities.push_back(newEntity);
	newEntity->m_startColor = startColor;
	newEntity->m_currentColor = startColor;
	newEntity->m_endColor = endColor;
	newEntity->m_mode = mode;
	newEntity->rasterizeMode = RasterizerMode::WIREFRAME_CULL_BACK;
	newEntity->SetDebugRenderMode();
	AddVertsForUVSphereZ3D(newEntity->verts, center, radius, 12.f, 24.f, newEntity->m_currentColor, AABB2::ZERO_TO_ONE);
	return;
}

void DebugAddWorldArrow(Vec3 const& start, Vec3 const& end, float radius, float duration, Rgba8 const& startColor /*= Rgba8::WHITE*/, Rgba8 const& endColor /*= Rgba8::WHITE*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	Vec3 dispment = end - start;
	Vec3 coneBottomPoint = dispment * 0.7f + start;
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->WorldEntities[entityIndex];
		if (m_DebugRender->WorldEntities[entityIndex] == nullptr)
		{
			entity = new DebugRenderEntity(duration);
			entity->m_startColor = startColor;
			entity->m_currentColor = startColor;
			entity->m_endColor = endColor;
			entity->m_mode = mode;
			entity->rasterizeMode = RasterizerMode::SOLID_CULL_BACK;
			entity->SetDebugRenderMode();
			AddVertsForCone3D(entity->verts, coneBottomPoint, end, radius + radius, startColor, AABB2::ZERO_TO_ONE, 12);
			AddVertsForCylinder3D(entity->verts, start, coneBottomPoint, radius, startColor, AABB2::ZERO_TO_ONE, 12);
			return;
		}
	}
	DebugRenderEntity* newEntity = new DebugRenderEntity(duration);
	m_DebugRender->WorldEntities.push_back(newEntity);
	newEntity->m_startColor = startColor;
	newEntity->m_currentColor = startColor;
	newEntity->m_endColor = endColor;
	newEntity->m_mode = mode;
	newEntity->rasterizeMode = RasterizerMode::SOLID_CULL_BACK;
	newEntity->SetDebugRenderMode();
	AddVertsForCone3D(newEntity->verts, coneBottomPoint, end, radius + radius, startColor, AABB2::ZERO_TO_ONE, 12);
	AddVertsForCylinder3D(newEntity->verts, start, coneBottomPoint, radius, startColor, AABB2::ZERO_TO_ONE, 12);
	return;

}

void DebugAddWorldText(std::string const& text, Mat44 const& transform, float textHeight, Vec2 const& aligment, float duration, Rgba8 const& startColor /*= Rgba8::WHITE*/, Rgba8 const& endColor /*= Rgba8::WHITE*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->WorldEntities[entityIndex];
		if (m_DebugRender->WorldEntities[entityIndex] == nullptr)
		{
			entity = new DebugRenderEntity(duration);
			entity->m_startColor = startColor;
			entity->m_currentColor = startColor;
			entity->m_endColor = endColor;
			entity->m_mode = mode;
			entity->rasterizeMode = RasterizerMode::SOLID_CULL_NONE;
			entity->SetDebugRenderMode();
			entity->m_texture = &m_DebugRender->m_font->GetTexture();
			m_DebugRender->m_font->AddVertsForText3D(entity->verts, Vec2(0.f, 0.f), textHeight, text, startColor, 1.f, aligment);
			TransformVertexArray3D(entity->verts, transform);
			return;
		}
	}
	DebugRenderEntity* newEntity = new DebugRenderEntity(duration);
	m_DebugRender->WorldEntities.push_back(newEntity);
	newEntity->m_startColor = startColor;
	newEntity->m_currentColor = startColor;
	newEntity->m_endColor = endColor;
	newEntity->m_mode = mode;
	newEntity->rasterizeMode = RasterizerMode::SOLID_CULL_NONE;
	newEntity->SetDebugRenderMode();
	newEntity->m_texture = &m_DebugRender->m_font->GetTexture();
	m_DebugRender->m_font->AddVertsForText3D(newEntity->verts, Vec2(0.f, 0.f), textHeight, text, startColor, 1.f, aligment);
	TransformVertexArray3D(newEntity->verts, transform);
	return;
}

void DebugAddWorldBillBoardText(std::string const& text, Vec3 const& origin, float textHeight, Vec2 const& alignment, float duration, Rgba8 const& startColor /*= Rgba8::WHITE*/, Rgba8 const& endColor /*= Rgba8::WHITE*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	for (int entityIndex = 0; entityIndex < m_DebugRender->WorldEntities.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->WorldEntities[entityIndex];
		if (m_DebugRender->WorldEntities[entityIndex] == nullptr)
		{
			entity = new DebugRenderEntity(duration);
			entity->m_startColor = startColor;
			entity->m_currentColor = startColor;
			entity->m_endColor = endColor;
			entity->m_mode = mode;
			entity->rasterizeMode = RasterizerMode::SOLID_CULL_NONE;
			entity->SetDebugRenderMode();
			entity->m_texture = &m_DebugRender->m_font->GetTexture();
			entity->isBillBoard = true;
			Mat44 transform = Mat44::CreateXRotationDegrees(90.f);
			transform.AppendYRotation(90.f);
			m_DebugRender->m_font->AddVertsForText3D(entity->verts, Vec2(0.f, 0.f), textHeight, text, startColor, 1.f, alignment);
			TransformVertexArray3D(entity->verts, transform);
			entity->m_BillBoardPosition = origin;
			return;
		}
	}
	DebugRenderEntity* newEntity = new DebugRenderEntity(duration);
	m_DebugRender->WorldEntities.push_back(newEntity);
	newEntity->m_startColor = startColor;
	newEntity->m_currentColor = startColor;
	newEntity->m_endColor = endColor;
	newEntity->m_mode = mode;
	newEntity->rasterizeMode = RasterizerMode::SOLID_CULL_NONE;
	newEntity->SetDebugRenderMode();
	newEntity->m_texture = &m_DebugRender->m_font->GetTexture();
	newEntity->isBillBoard = true;
	Mat44 transform = Mat44::CreateXRotationDegrees(90.f);
	transform.AppendYRotation(90.f);
	m_DebugRender->m_font->AddVertsForText3D(newEntity->verts, Vec2(0.f, 0.f), textHeight, text, startColor, 1.f, alignment);
	TransformVertexArray3D(newEntity->verts, transform);
	newEntity->m_BillBoardPosition = origin;
	return;
}

void DebugAddScreenText(std::string const& text, Vec2 const& position, float size, Vec2 const& alignment, float duration, Rgba8 const& startColor /*= Rgba8::WHITE*/, Rgba8 const& endColor /*= Rgba8::WHITE*/, DebugRenderMode mode /*= DebugRenderMode::USE_DEPTH*/)
{
	AABB2 bounds = AABB2(Vec2(0.f, 0.f), Vec2(1600.f, 800.f));
	bounds.m_maxs += position;
	bounds.m_mins += position;
	for (int entityIndex = 0; entityIndex < m_DebugRender->screenTexts.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->screenTexts[entityIndex];
		if (m_DebugRender->screenTexts[entityIndex] == nullptr)
		{
			entity = new DebugRenderEntity(duration);
			entity->m_startColor = startColor;
			entity->m_currentColor = startColor;
			entity->m_endColor = endColor;
			entity->m_mode = mode;
			entity->rasterizeMode = RasterizerMode::SOLID_CULL_NONE;
			entity->SetDebugRenderMode();
			entity->m_texture = &m_DebugRender->m_font->GetTexture();
			m_DebugRender->m_font->AddVertsForTextInBox2D(entity->verts, bounds, size * 1.f, text, startColor, 1.f, alignment);
			return;
		}
	}
	DebugRenderEntity* newEntity = new DebugRenderEntity(duration);
	m_DebugRender->screenTexts.push_back(newEntity);
	newEntity->m_startColor = startColor;
	newEntity->m_currentColor = startColor;
	newEntity->m_endColor = endColor;
	newEntity->m_mode = mode;
	newEntity->rasterizeMode = RasterizerMode::SOLID_CULL_NONE;
	newEntity->SetDebugRenderMode();
	newEntity->m_texture = &m_DebugRender->m_font->GetTexture();
	m_DebugRender->m_font->AddVertsForTextInBox2D(newEntity->verts, bounds, size * 1.f, text, startColor, 1.f, alignment);
	return;
}

void DebugAddMessage(std::string const& text, float duration, Rgba8 const& startColor /*= Rgba8::WHITE*/, Rgba8 const& endColor /*= Rgba8::WHITE*/)
{
	int localSlotIndex = -1;
	for (int entityIndex = 0; entityIndex < m_DebugRender->screenTexts.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->screenTexts[entityIndex];
		if (entity == nullptr)
		{
			continue;
		}
		if (entity->isMessage)
		{
			localSlotIndex++;
		}
	}


	for (int entityIndex = 0; entityIndex < m_DebugRender->screenTexts.size(); ++entityIndex)
	{
		DebugRenderEntity*& entity = m_DebugRender->screenTexts[entityIndex];
		if (m_DebugRender->screenTexts[entityIndex] == nullptr)
		{
			entity = new DebugRenderEntity(duration);
			entity->m_startColor = startColor;
			entity->m_currentColor = startColor;
			entity->m_endColor = endColor;
			entity->rasterizeMode = RasterizerMode::SOLID_CULL_NONE;
			entity->isMessage = true;
			entity->slotIndex = localSlotIndex++;
			entity->SetDebugRenderMode();
			entity->m_texture = &m_DebugRender->m_font->GetTexture();
			Vec2 slotStartPos = Vec2(0.f, 250.f - 20.f * entity->slotIndex);
			AABB2 bounds = AABB2(Vec2(0.f, 0.f), Vec2(100.f * 5, 100.f * 5));
			bounds.m_maxs += slotStartPos;
			bounds.m_mins += slotStartPos;
			m_DebugRender->m_font->AddVertsForTextInBox2D(entity->verts, bounds, 20.f, text, startColor, 1.f, Vec2(0.f, 1.f));
			return;
		}
	}
	DebugRenderEntity* newEntity = new DebugRenderEntity(duration);
	m_DebugRender->screenTexts.push_back(newEntity);
	newEntity->m_startColor = startColor;
	newEntity->m_currentColor = startColor;
	newEntity->m_endColor = endColor;
	newEntity->isMessage = true;
	newEntity->slotIndex = localSlotIndex++;
	newEntity->rasterizeMode = RasterizerMode::SOLID_CULL_NONE;
	newEntity->SetDebugRenderMode();
	newEntity->m_texture = &m_DebugRender->m_font->GetTexture();
	Vec2 slotStartPos = Vec2(0.f, 250.f - 20.f * newEntity->slotIndex);
	AABB2 bounds = AABB2(Vec2(0.f, 0.f), Vec2(100.f * 5, 100.f * 5));
	bounds.m_maxs += slotStartPos;
	bounds.m_mins += slotStartPos;
	m_DebugRender->m_font->AddVertsForTextInBox2D(newEntity->verts, bounds, 20.f, text, startColor, 1.f, Vec2(0.f, 1.f));
	return;
}

bool Command_DebugRenderClear(EventArgs& args)
{
	UNUSED(args);
	m_DebugRender->ClearEntities();
	return true;
}

bool Command_DebugRenderToggle(EventArgs& args)
{
	UNUSED(args);
	m_DebugRender->m_isVisible = !m_DebugRender->m_isVisible;
	return true;
}
