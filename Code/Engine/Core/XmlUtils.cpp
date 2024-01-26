#include "XmlUtils.hpp"
#include "Rgba8.hpp"
#include "../Math/Vec2.hpp"
#include "../Math/IntVec2.hpp"
#include "ErrorWarningAssert.hpp"
#include "../Math/Vec3.hpp"

const XmlElement& GetElementFromXMLFile(XmlDocument& document, const char* xmlFilePath)
{
	XmlError errorFlag = document.LoadFile(xmlFilePath);
	std::string errorMessage = "Xml Not Found: ";
	errorMessage.append(xmlFilePath);

	GUARANTEE_OR_DIE(errorFlag == XmlError::XML_SUCCESS, errorMessage.c_str());

	const XmlElement* element = document.RootElement();
	return *element;
}

int ParseXMLAttribute(XmlElement const& element, char const* attributeName, int defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	int value = defaultValue;
	if (attribute)
	{
		value = atoi(attribute);
	}
	return value;
}

char ParseXMLAttribute(XmlElement const& element, char const* attributeName, char defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	char value = defaultValue;
	if (attribute)
	{
		value = attribute[0];
	}
	return value;
}

bool ParseXMLAttribute(XmlElement const& element, char const* attributeName, bool defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	bool value = defaultValue;
	if (attribute)
	{
		value = GetBoolFromText(attribute);
	}
	return value;
}

float ParseXMLAttribute(XmlElement const& element, char const* attributeName, float defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	float value = defaultValue;
	if (attribute)
	{
		value = (float)atof(attribute);
	}
	return value;
}

Rgba8 ParseXMLAttribute(XmlElement const& element, char const* attributeName, Rgba8 const& defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	Rgba8 value = defaultValue;
	if (attribute)
	{
		value.SetFromText(attribute);
	}
	return value;
}

Vec2 ParseXMLAttribute(XmlElement const& element, char const* attributeName, Vec2 const& defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	Vec2 value = defaultValue;
	if (attribute)
	{
		value.SetFromText(attribute);
	}
	return value;
}

IntVec2 ParseXMLAttribute(XmlElement const& element, char const* attributeName, IntVec2 const& defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	IntVec2 value = defaultValue;
	if (attribute)
	{
		value.SetFromText(attribute);
	}
	return value;
}

std::string ParseXMLAttribute(XmlElement const& element, char const* attributeName, std::string const& defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	std::string value = defaultValue;
	if (attribute)
	{
		value = std::string(attribute);
	}
	return value;
}

Vec3 ParseXMLAttribute(XmlElement const& element, char const* attributeName, Vec3 const& defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	Vec3 value = defaultValue;
	if (attribute)
	{
		value.SetFromText(attribute);
	}
	return value;
}

Strings ParseXmlAttribute(XmlElement const& element, char const* attributeName, Strings const& defaultValues, const unsigned char delimeter)
{
	const char* attribute = element.Attribute(attributeName);
	Strings value = defaultValues;
	if (attribute)
	{
		value = SplitStringOnDelimiter(attribute, delimeter);
	}
	return value;
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, char const* defaultValue)
{
	const char* attribute = element.Attribute(attributeName);
	std::string value = defaultValue;
	if (attribute)
	{
		value = std::string(attribute);
	}
	return value;
}

FloatRange ParseXMlAttribute(XmlElement const& element, char const* attributeName, FloatRange const& defaultValue, char delimeter)
{
	const char* attribute = element.Attribute(attributeName);
	FloatRange value = defaultValue;
	if (attribute)
	{
		Strings values = SplitStringOnDelimiter(attribute, delimeter);
		if (values.size() == 2)
		{
			float min = (float)atof(values[0].c_str());
			float max = (float)atof(values[1].c_str());
			value = FloatRange(min, max);
		}
	}
	return value;
}

