#pragma once
#include <string>
#include "Engine/Core/NamedStrings.hpp"
//-----------------------------------------------------------------------------------------------
// #SD1ToDo: Move this useful macro to a more central place, e.g. Engine/Core/EngineCommon.hpp
//
#define UNUSED(x) (void)(x);
#define STATIC 
#pragma warning(disable : 26812) // prefer enum class warning
#pragma warning(disable : 4127) // conditional expression is constant 

extern NamedStrings* g_gameConfigBlackboard;