#pragma once
#include "Engine/Core/EngineCommon.hpp"

class TileHeatmap {
public:
	TileHeatmap() {};
	TileHeatmap(IntVec2 const& dimensions);
	TileHeatmap(IntVec2 const& dimensions, float initialValue);
	~TileHeatmap() {};

	void SetAllValues( float const& newVal = 0.f);
	void SetHeat( IntVec2 const& cellCoords, float heatVal);
	void SetHeat( int const& index, float heatVal);
	void AddHeat( IntVec2 const& cellCoords, float heatVal);
	void AddHeat( int const& index, float heatVal);
	float GetHeat( int x, int y) const;
	float GetHeat( IntVec2 const& cellCoords) const;
	float GetHeat( int index) const;
	int GetIndexOfMinNeighbor(int startIndex) const;
	IntVec2 GetCoordsOfMinNeighbor(IntVec2 const& startCoords) const;
	void SetNeighborsValuesIfNotEqualMax(float value, float maxValue);
	IntVec2 GetDimensions() const;
	bool CheckNeigborsValIfEqualTo(float value, int index);

protected:
	std::vector<float> m_heatValues = std::vector<float>();
	IntVec2 m_dimensions = IntVec2(0, 0);
};