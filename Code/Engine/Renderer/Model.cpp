#include "Model.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Game/GameCommon.hpp"
#include "../Core/FileUtils.hpp"
#include "../Core/ErrorWarningAssert.hpp"
#include "../Math/MathUtils.hpp"
#include "../Math/Vec3.hpp"
#include "../../ModelViewer/Code/Game/Material.hpp"

Model::Model()
{

}

Model::~Model()
{

}

bool Model::LoadFromXmlElement(const char* filename)
{
	XmlDocument Document;
	XmlError error = Document.LoadFile(filename);
	if (!error == 0)
	{
		return false;
	}
	XmlElement* RootElement = Document.RootElement();
	m_name = ParseXMLAttribute(*RootElement, "name", m_name);
	m_path = ParseXMLAttribute(*RootElement, "path", m_path);
	std::string shaderPath;
	shaderPath = ParseXMLAttribute(*RootElement, "shader", shaderPath);
	if (shaderPath != "")
	{
		m_shader = g_theRenderer->CreateShader(shaderPath.c_str());
	}
	std::string material;
	material = ParseXMLAttribute(*RootElement, "material", material);
	if (material != "")
	{
		m_material = Material::GetMaterialByPath(material.c_str());
	}

	XmlElement* transformElement = RootElement->FirstChildElement();
	m_transform.x = ParseXMLAttribute(*transformElement, "x", m_transform.x);
	m_transform.y = ParseXMLAttribute(*transformElement, "y", m_transform.y);
	m_transform.z = ParseXMLAttribute(*transformElement, "z", m_transform.z);
	m_transform.t = ParseXMLAttribute(*transformElement, "t", m_transform.t);
	m_transform.scale = ParseXMLAttribute(*transformElement, "scale", m_transform.scale);

	return true;
}



bool Model::LoadObjFile()
{
	std::string objFullData;

	int readResult = FileReadToString(objFullData, m_path.c_str());
	if (readResult == -1)
	{
		GUARANTEE_OR_DIE(readResult != -1, "File Reading failed!");
		return false;
	}

	Strings lines;
	Strings GeometricVerts;
	Strings TexutureCoords;
	Strings VertsNormals;
	Strings PolygonalFaceElements;

	std::vector<Vertex_PCUTBN> verts;
	std::vector<unsigned int> indexes;

	std::vector<Vec3> vertPositionList;
	std::vector<Vec2> vertTextureUVList;
	std::vector<Vec3> vertNormalsList;


	lines = SplitStringOnDelimiter(objFullData, '\n');

	for (int lineIndex = 0; lineIndex < lines.size(); ++lineIndex)
	{
		std::string& line = lines[lineIndex];
		if (line[0] == 'v' && isspace(line[1]))
		{
			GeometricVerts.push_back(line);
		}
		else if (line[0] == 'v' && line[1] == 't' && isspace(line[2]))
		{
			TexutureCoords.push_back(line);
		}
		else if (line[0] == 'v' && line[1] == 'n' && isspace(line[2]))
		{
			VertsNormals.push_back(line);
		}
		else if (line[0] == 'f' && isspace(line[1]))
		{
			PolygonalFaceElements.push_back(line);
		}
		else
		{

		}
	}

	// GeometricVerts lines, feeding into vertPositionList
	for (int GeoIndex = 0; GeoIndex < GeometricVerts.size(); ++GeoIndex)
	{
		Strings lineElement;
		std::string& line = GeometricVerts[GeoIndex];
		lineElement = SplitStringOnDelimiter(line, ' ', true);
		
		vertPositionList.emplace_back(std::stof(lineElement[1]), std::stof(lineElement[2]), std::stof(lineElement[3]));

	}
	// TexutureCoords lines, feeding into vertTextureUVList
	for (int uvIndex = 0; uvIndex < TexutureCoords.size(); ++uvIndex)
	{
		Strings lineElement;
		std::string& line = TexutureCoords[uvIndex];
		lineElement = SplitStringOnDelimiter(line, ' ');
		vertTextureUVList.emplace_back(std::stof(lineElement[1]), std::stof(lineElement[2]));
	}
	// feeding vertNormalsList
	for (int normalIndex = 0; normalIndex < VertsNormals.size(); ++normalIndex)
	{
		Strings lineElement;
		std::string& line = VertsNormals[normalIndex];
		lineElement = SplitStringOnDelimiter(line, ' ');
		Vec3 normal = Vec3(std::stof(lineElement[1]), std::stof(lineElement[2]), std::stof(lineElement[3]));
		normal = normal.GetNormalized();
		vertNormalsList.emplace_back(normal);
	}

	unsigned int currentIndex = 0;

	if (PolygonalFaceElements.size() == 0)
	{
		
		for (int i = 0; i < vertPositionList.size() - 2; i = i + 3)
		{
			Vec3 pos1 = vertPositionList[i];
			Vec3 pos2 = vertPositionList[i + 1];
			Vec3 pos3 = vertPositionList[i + 2];

			Vec3 uDirection = (pos1 - pos2).GetNormalized();
			Vec3 vDirection = (pos2 - pos3).GetNormalized();
			Vec3 normal = CrossProduct3D(uDirection, vDirection).GetNormalized();

			Vertex_PCUTBN pncu1 = Vertex_PCUTBN(pos1, Rgba8::WHITE, Vec2(0.f,0.f), normal);
			Vertex_PCUTBN pncu2 = Vertex_PCUTBN(pos2, Rgba8::WHITE, Vec2(0.f, 0.f), normal);
			Vertex_PCUTBN pncu3 = Vertex_PCUTBN(pos3, Rgba8::WHITE, Vec2(0.f, 0.f), normal);
			m_cpuMesh.m_vertexes.push_back(pncu1);
			m_cpuMesh.m_vertexes.push_back(pncu2);
			m_cpuMesh.m_vertexes.push_back(pncu3);

			m_cpuMesh.m_indexes.push_back(currentIndex);
			m_cpuMesh.m_indexes.push_back(currentIndex + 1);
			m_cpuMesh.m_indexes.push_back(currentIndex + 2);
			currentIndex += 3;
		}
		m_cpuMesh.CreateBuffers(m_gpuMesh, g_theRenderer);
	}
	else
	{
		//Creating Face 
		for (int faceIndex = 0; faceIndex < PolygonalFaceElements.size(); ++faceIndex)
		{
			Strings lineElement;
			std::string& line = PolygonalFaceElements[faceIndex];

			lineElement = SplitStringOnDelimiter(line, ' ', true);

			std::vector<Vec3> oneLinePos;
			std::vector<Vec2> oneLineUV;
			std::vector<Vec3> oneLineNormal;
			std::vector<unsigned int> oneLineIndex;
			bool hasNormal = false;

			
			Strings firstElement = SplitStringOnDelimiter(lineElement[1], '/');
			if (firstElement.size() == 1)
			{
				for (int i = 1; i < lineElement.size(); ++i)
				{
					Strings vertElement; // x//x or x/x/x or x
					Strings vertElement2;
					Strings vertElement3;
					std::string& vert = lineElement[i];

					vert.erase(std::remove(vert.begin(), vert.end(), '\r'), vert.end());
					vertElement = SplitStringOnDelimiter(vert, '/');

					if (vertElement.size() == 1)
					{
						int posIndex = std::stoi(vertElement[0]) - 1;
						Vec3 pos = vertPositionList[posIndex];
						oneLinePos.push_back(pos);
						oneLineUV.emplace_back(0.f, 0.f);
						oneLineIndex.push_back(currentIndex);  
					}
					else
					{

						int posIndex = std::stoi(vertElement[0]) - 1;

						Vec3 pos = vertPositionList[posIndex];


						//oneLinePos.push_back(pos);

						Vec2 uv = Vec2(0.f, 0.f);

						if (!vertElement[1].empty())
						{
							int uvIndex = std::stoi(vertElement[1]) - 1;
							uv = vertTextureUVList[uvIndex];
						}

						int normalIndex = std::stoi(vertElement[2]) - 1;

						Vec3 normal = vertNormalsList[normalIndex];

						Vertex_PCUTBN pncu = Vertex_PCUTBN(pos, Rgba8::WHITE, uv, normal);

						m_cpuMesh.m_vertexes.push_back(pncu);

						m_cpuMesh.m_indexes.push_back(currentIndex);

						currentIndex++;
						hasNormal = true;
					}

				}

				if (oneLineNormal.size() == 0 && hasNormal == false)
				{
					Vec3 uDirection = (oneLinePos[0] - oneLinePos[1]).GetNormalized();
					Vec3 vDirection = (oneLinePos[1] - oneLinePos[2]).GetNormalized();
					Vec3 normal = CrossProduct3D(uDirection, vDirection).GetNormalized();

					for (int i = 1; i < oneLinePos.size() - 1; i++)
					{
						Vertex_PCUTBN pncu = Vertex_PCUTBN(oneLinePos[0], Rgba8::WHITE, oneLineUV[0], normal);
						Vertex_PCUTBN pncu2 = Vertex_PCUTBN(oneLinePos[i], Rgba8::WHITE, oneLineUV[i], normal);
						Vertex_PCUTBN pncu3 = Vertex_PCUTBN(oneLinePos[i+1], Rgba8::WHITE, oneLineUV[i+1], normal);
						m_cpuMesh.m_vertexes.push_back(pncu);
						m_cpuMesh.m_vertexes.push_back(pncu2);
						m_cpuMesh.m_vertexes.push_back(pncu3);
						m_cpuMesh.m_indexes.push_back(currentIndex);
						currentIndex++;
						m_cpuMesh.m_indexes.push_back(currentIndex);
						currentIndex++;
						m_cpuMesh.m_indexes.push_back(currentIndex);
						currentIndex++;
					}
				}

			}
			else
			{
				for (int i = 1; i < lineElement.size() - 1; i ++)
				{
					Strings vertElement; // x//x or x/x/x or x
					Strings vertElement2;
					Strings vertElement3;

		

					std::string& vert = lineElement[1];
					std::string& vert2 = lineElement[i];
					std::string& vert3 = lineElement[i+1];

					vertElement = SplitStringOnDelimiter(vert, '/');
					vertElement2 = SplitStringOnDelimiter(vert2, '/');
					vertElement3 = SplitStringOnDelimiter(vert3, '/');

					int posIndex = std::stoi(vertElement[0]) - 1;
					int posIndex2 = std::stoi(vertElement2[0]) - 1;
					int posIndex3 = std::stoi(vertElement3[0]) - 1;

					Vec3 pos = vertPositionList[posIndex];
					Vec3 pos2 = vertPositionList[posIndex2];
					Vec3 pos3 = vertPositionList[posIndex3];


					Vec2 uv = Vec2(0.f, 0.f);
					Vec2 uv2 = Vec2(0.f, 0.f);
					Vec2 uv3 = Vec2(0.f, 0.f);

					if (!vertElement[1].empty())
					{
						int uvIndex = std::stoi(vertElement[1]) - 1;
						uv = vertTextureUVList[uvIndex];
					}
					if (!vertElement2[1].empty())
					{
						int uvIndex2 = std::stoi(vertElement2[1]) - 1;
						uv2 = vertTextureUVList[uvIndex2];
					}
					if (!vertElement3[1].empty())
					{
						int uvIndex3 = std::stoi(vertElement3[1]) - 1;
						uv3 = vertTextureUVList[uvIndex3];
					}

					int normalIndex = std::stoi(vertElement[2]) - 1;
					int normalIndex2 = std::stoi(vertElement2[2]) - 1;
					int normalIndex3 = std::stoi(vertElement3[2]) - 1;

					Vec3 normal = vertNormalsList[normalIndex];
					Vec3 normal2 = vertNormalsList[normalIndex2];
					Vec3 normal3 = vertNormalsList[normalIndex3];

					Vertex_PCUTBN pncu = Vertex_PCUTBN(pos, Rgba8::WHITE, uv, normal);
					Vertex_PCUTBN pncu2 = Vertex_PCUTBN(pos2, Rgba8::WHITE, uv2, normal2);
					Vertex_PCUTBN pncu3 = Vertex_PCUTBN(pos3, Rgba8::WHITE, uv3, normal3);

					m_cpuMesh.m_vertexes.push_back(pncu);
					m_cpuMesh.m_vertexes.push_back(pncu2);
					m_cpuMesh.m_vertexes.push_back(pncu3);

					m_cpuMesh.m_indexes.push_back(currentIndex);
					currentIndex++;
					m_cpuMesh.m_indexes.push_back(currentIndex);
					currentIndex++;
					m_cpuMesh.m_indexes.push_back(currentIndex);
					currentIndex++;

				}
			}
			
		}
		
		m_cpuMesh.CreateBuffers(m_gpuMesh, g_theRenderer);
	}

	DebuggerPrintf("Loaded. obj file %s\n", m_path.c_str());
	DebuggerPrintf("[File Data] verts: %d texture: %d normals %d faces %d triangles: %d\n", vertPositionList.size(), vertTextureUVList.size(), vertNormalsList.size(), PolygonalFaceElements.size(), PolygonalFaceElements.size());
	DebuggerPrintf("[Loaded mesh] verts: %d indexes: %d \n", m_cpuMesh.m_vertexes.size(), m_cpuMesh.m_indexes.size());

	return true;
}

Mat44 Model::GetModelMatrix() const
{
	Mat44 result = m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
	Mat44 matrix;
	matrix.SetIJK3D(m_transform.x, m_transform.y, m_transform.z);
	matrix.SetTranslation3D(m_transform.t);
	matrix.AppendScaleUniform3D(m_transform.scale);
	matrix.Append(result);
	return matrix;
}

void Model::Update(float deltaSeconds)
{
	UNUSED(deltaSeconds);
	m_orientation.m_yawDegrees += deltaSeconds * m_angularVelocity.m_yawDegrees;
	m_orientation.m_pitchDegrees += deltaSeconds * m_angularVelocity.m_pitchDegrees;
	m_orientation.m_rollDegrees += deltaSeconds * m_angularVelocity.m_rollDegrees;
}

void Model::Render() const
{
	g_theRenderer->SetBlendMode(BlendMode::OPAQUE);

	if (m_material)
	{
		g_theRenderer->BindShader(m_material->m_shader);
		g_theRenderer->BindTextures(m_material->m_diffuseTexture, m_material->m_normalTexture, m_material->m_specGlossEmitTexture);
	}
	else
	{
		g_theRenderer->BindShader(m_shader);
		g_theRenderer->BindTexture(m_texture);
	}
	

	

	g_theRenderer->SetModelConstants(GetModelMatrix(), m_tintColor);
	g_theRenderer->SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
	g_theRenderer->SetDepthMode(DepthMode::ENABLED);

	if (m_gpuMesh.m_indexes != nullptr)
	{
		m_gpuMesh.Render(g_theRenderer, (int)m_cpuMesh.m_indexes.size());
	}
	else
	{
		m_cpuMesh.Render(g_theRenderer);
	}

}

