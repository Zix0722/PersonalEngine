#pragma once
#include <vector>

class Model;

class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	Model* LoadOrGetModelFromPath(const char* filenameXML);

private:
	Model* LoadFromPath(const char* filenameXML);
	Model* GetFromLoadedModelsList(const char* filenameXML);

private:
	std::vector<Model*> m_loadedModels;

};