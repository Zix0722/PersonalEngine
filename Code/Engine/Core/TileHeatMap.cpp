#include "TileHeatMap.hpp"

TileHeatmap::TileHeatmap(IntVec2 const& dimensions)
{
	m_dimensions = dimensions;
	m_heatValues = std::vector<float>((size_t)m_dimensions.x * m_dimensions.y, 0.f);
}

TileHeatmap::TileHeatmap(IntVec2 const& dimensions, float initialValue)
{
	m_dimensions = dimensions;
	m_heatValues = std::vector<float>((size_t)m_dimensions.x * m_dimensions.y, initialValue);
}

void TileHeatmap::SetAllValues(float const& newVal /*= 0.f*/)
{
	m_heatValues = std::vector<float>((size_t)m_dimensions.x * m_dimensions.y, newVal);
}

void TileHeatmap::SetHeat(IntVec2 const& cellCoords, float heatVal)
{
	int valIndex = cellCoords.x + m_dimensions.x * cellCoords.y;
	m_heatValues[valIndex] = heatVal;
}

void TileHeatmap::SetHeat(int const& index, float heatVal)
{
	m_heatValues[index] = heatVal;
}

void TileHeatmap::AddHeat(IntVec2 const& cellCoords, float heatVal)
{
	int valIndex = cellCoords.x + m_dimensions.x * cellCoords.y;
	m_heatValues[valIndex] += heatVal;
}

void TileHeatmap::AddHeat(int const& index, float heatVal)
{
	m_heatValues[index] += heatVal;
}

float TileHeatmap::GetHeat(int x, int y) const
{
	return GetHeat(IntVec2(x, y));
}

float TileHeatmap::GetHeat(IntVec2 const& cellCoords) const
{
	int valIndex = cellCoords.x + m_dimensions.x * cellCoords.y;
	return m_heatValues[valIndex];
}

float TileHeatmap::GetHeat(int index) const
{
	return m_heatValues[index];
}

int TileHeatmap::GetIndexOfMinNeighbor(int startIndex) const
{
	UNUSED(startIndex);
	return -1;
}

void TileHeatmap::SetNeighborsValuesIfNotEqualMax(float value, float maxValue)
{
	UNUSED(value);
	UNUSED(maxValue);
}

IntVec2 TileHeatmap::GetDimensions() const
{
	return m_dimensions;
}

bool TileHeatmap::CheckNeigborsValIfEqualTo(float value, int index)
{
	int coordsX = index % m_dimensions.x;
	int coordsY = index / m_dimensions.x;
	IntVec2 currentCoords = IntVec2(coordsX, coordsY);

	IntVec2 northCoords = currentCoords + IntVec2(0, 1);
	IntVec2 southCoords = currentCoords + IntVec2(0, -1);
	IntVec2 eastCoords = currentCoords + IntVec2(1, 0);
	IntVec2 westCoords = currentCoords + IntVec2(-1, 0);

	if (northCoords.x > 0 && northCoords.x < m_dimensions.x && northCoords.y > 0 && northCoords.y < m_dimensions.y)
	{
		if (GetHeat(northCoords) != value)
		{
			return false;
		}
	}
	if (southCoords.x > 0 && southCoords.x < m_dimensions.x && southCoords.y > 0 && southCoords.y < m_dimensions.y)
	{
		if (GetHeat(southCoords) != value)
		{
			return false;
		}
	}
	if (eastCoords.x > 0 && eastCoords.x < m_dimensions.x && eastCoords.y > 0 && eastCoords.y < m_dimensions.y)
	{
		if (GetHeat(eastCoords) != value)
		{
			return false;
		}
	}
	if (westCoords.x > 0 && westCoords.x < m_dimensions.x && westCoords.y > 0 && westCoords.y < m_dimensions.y)
	{
		if (GetHeat(westCoords) != value)
		{
			return false;
		}
	}
	return true;
	//todo check tile at board, for now I ignore this kind of cases
	
}

