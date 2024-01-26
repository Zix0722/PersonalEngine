#pragma once
#include <string>
#include "ThirdParty/TinyXML2/tinyxml2.h"
#include "Engine/Core/StringUtils.hpp"
#include "../Math/FloatRange.hpp"
//#include "Engine/Core/ErrorWarningAssert.hpp"
struct Rgba8;
struct Vec2;
struct Vec3;
struct IntVec2;


typedef tinyxml2::XMLElement XmlElement;
typedef tinyxml2::XMLAttribute XmlAttribute;
typedef tinyxml2::XMLDocument XmlDocument;
typedef tinyxml2::XMLError XmlError;

const XmlElement& GetElementFromXMLFile(XmlDocument& document, const char* xmlFilePath);

int			ParseXMLAttribute(XmlElement const& element, char const* attributeName, int defaultValue);
char 		ParseXMLAttribute(XmlElement const& element, char const* attributeName, char defaultValue);
bool		ParseXMLAttribute(XmlElement const& element, char const* attributeName, bool defaultValue);
float		ParseXMLAttribute(XmlElement const& element, char const* attributeName, float defaultValue);
Rgba8		ParseXMLAttribute(XmlElement const& element, char const* attributeName,  Rgba8 const& defaultValue);
Vec2		ParseXMLAttribute(XmlElement const& element, char const* attributeName,  Vec2 const& defaultValue);
Vec3		ParseXMLAttribute(XmlElement const& element, char const* attributeName, Vec3 const& defaultValue);
IntVec2		ParseXMLAttribute(XmlElement const& element, char const* attributeName,  IntVec2 const& defaultValue);
std::string ParseXMLAttribute(XmlElement const& element, char const* attributeName,  std::string const& defaultValue);
Strings		ParseXmlAttribute(XmlElement const& element, char const* attributeName, Strings const& defaultValues, const unsigned char delimeter);
FloatRange  ParseXMlAttribute(XmlElement const& element, char const* attributeName, FloatRange const& defaultValue, char delimeter);

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, char const* defaultValue);
