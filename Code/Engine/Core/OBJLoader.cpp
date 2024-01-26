#include "OBJLoader.hpp"
#include "Engine/Renderer/Model.hpp"
#include "FileUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "StringUtils.hpp"
#include "Time.hpp"

OBJLoader::OBJLoader()
{

}

OBJLoader::~OBJLoader()
{
	m_loadedModels.clear();
}

Model* OBJLoader::LoadOrGetModelFromPath(const char* filenameXML)
{
	Model* result = GetFromLoadedModelsList(filenameXML);
	if (result == nullptr)
	{
		result = LoadFromPath(filenameXML);
		return result;
	}

	return result;
}

Model* OBJLoader::LoadFromPath(const char* filenameXML)
{
	double startTime = GetCurrentTimeSeconds();
	DebuggerPrintf("-------------------------------------------------------------------------------------------------------------\n");
	Model* result = new Model();
	bool successedForReadXML = result->LoadFromXmlElement(filenameXML);

	if (!successedForReadXML)
	{
		return nullptr;
	}

	if (result->LoadObjFile())
	{
		m_loadedModels.push_back(result);
		double endTime = GetCurrentTimeSeconds();
		float timeSpend = (float)(endTime - startTime);
		DebuggerPrintf("[Time]	parse: %f ms\n", timeSpend);
		DebuggerPrintf("-------------------------------------------------------------------------------------------------------------\n");
		return result;
	}
	return nullptr;
}

Model* OBJLoader::GetFromLoadedModelsList(const char* filenameXML)
{
	Model tempModel = Model();
	if (!tempModel.LoadFromXmlElement(filenameXML))
	{
		return nullptr;
	}

	for (int modelIndex = 0; modelIndex < m_loadedModels.size(); ++modelIndex)
	{
		Model*& model = m_loadedModels[modelIndex];
		if (model->m_path == tempModel.m_path.c_str())
		{
			return model;
		}
	}

	return nullptr;
}

