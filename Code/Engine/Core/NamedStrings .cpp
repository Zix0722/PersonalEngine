#include "NamedStrings.hpp"

void NamedStrings::PopulateFromXmlElementAttributes(XmlElement const& element)
{
	for (XmlAttribute const* attribute = element.FirstAttribute(); attribute; attribute = attribute->Next())
	{
		std::string attributeName = attribute->Name();
		std::string attributeValue = attribute->Value();
		SetValue(attributeName, attributeValue);
	}
}

void NamedStrings::SetValue(std::string const& keyName, std::string const& newValue)
{
	m_keyValuePairs[keyName] = newValue;
}

std::string NamedStrings::GetValue(std::string const& keyName, std::string const& defaultValue) const
{
	std::string value = defaultValue;

	std::map<std::string, std::string>::const_iterator keyValueIterator = m_keyValuePairs.find(keyName);
	if (keyValueIterator != m_keyValuePairs.cend())
	{
		value = keyValueIterator->second;
	}
	return value;
}

bool NamedStrings::GetValue(std::string const& keyName, bool defaultValue) const
{
	bool value = defaultValue;

	std::map<std::string, std::string>::const_iterator keyValueIterator = m_keyValuePairs.find(keyName);
	if (keyValueIterator != m_keyValuePairs.cend())
	{
		std::string valueString = keyValueIterator->second;
		value = (bool)atoi(valueString.c_str());
	}
	return value;
}

int NamedStrings::GetValue(std::string const& keyName, int defaultValue) const
{
	int value = defaultValue;

	std::map<std::string, std::string>::const_iterator keyValueIterator = m_keyValuePairs.find(keyName);
	if (keyValueIterator != m_keyValuePairs.cend())
	{
		std::string valueString = keyValueIterator->second;
		value = atoi(valueString.c_str());
	}
	return value;
}

float NamedStrings::GetValue(std::string const& keyName, float defaultValue) const
{
	float value = defaultValue;

	std::map<std::string, std::string>::const_iterator keyValueIterator = m_keyValuePairs.find(keyName);
	if (keyValueIterator != m_keyValuePairs.cend())
	{
		std::string valueString = keyValueIterator->second;
		if (valueString == "")
		{
			return value;
		}
		value = (float)atof(valueString.c_str());
	}
	return value;
}

std::string NamedStrings::GetValue(std::string const& keyName, char const* defaultValue) const
{
	std::string value = defaultValue;

	std::map<std::string, std::string>::const_iterator keyValueIterator = m_keyValuePairs.find(keyName);
	if (keyValueIterator != m_keyValuePairs.cend())
	{
		value = keyValueIterator->second;
	}
	return value;
}

Rgba8 NamedStrings::GetValue(std::string const& keyName, Rgba8 const& defaultValue) const
{
	Rgba8 value = defaultValue;

	std::map<std::string, std::string>::const_iterator keyValueIterator = m_keyValuePairs.find(keyName);
	if (keyValueIterator != m_keyValuePairs.cend())
	{
		value.SetFromText(keyValueIterator->second.c_str());
	}
	return value;
}

Vec2 NamedStrings::GetValue(const std::string& keyName, const Vec2& defaultValue) const
{
	Vec2 value = defaultValue;

	std::map<std::string, std::string>::const_iterator keyValueIterator = m_keyValuePairs.find(keyName);
	if (keyValueIterator != m_keyValuePairs.cend())
	{
		value.SetFromText(keyValueIterator->second.c_str());
	}
	return value;
}

IntVec2 NamedStrings::GetValue(const std::string& keyName, const IntVec2& defaultValue) const
{
	IntVec2 value = defaultValue;

	std::map<std::string, std::string>::const_iterator keyValueIterator = m_keyValuePairs.find(keyName);
	if (keyValueIterator != m_keyValuePairs.cend())
	{
		value.SetFromText(keyValueIterator->second.c_str());
	}
	return value;
}
