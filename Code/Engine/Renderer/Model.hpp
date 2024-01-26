#pragma once
#include "GPUMesh.hpp"
#include "CPUMesh.hpp"
#include "Shader.hpp"

struct modelTransform
{
public:
	Vec3 x;
	Vec3 y;
	Vec3 z;
	Vec3 t;
	float scale = 1.f;
};

class Material;

class Model
{
public:
	Model();
	~Model();

	bool LoadFromXmlElement(const char* filename);
	bool LoadObjFile();
	Mat44 GetModelMatrix() const;
	void Update(float deltaSeconds);
	void Render() const;
public:
	GPUMesh m_gpuMesh;
	CPUMesh m_cpuMesh;
	Shader* m_shader = nullptr;
	std::string m_name;
	std::string m_path;
	modelTransform m_transform;
	Material* m_material = nullptr;
	Texture* m_texture = nullptr;
	Rgba8 m_tintColor = Rgba8::WHITE;
	EulerAngles m_orientation;
	EulerAngles m_angularVelocity;
};