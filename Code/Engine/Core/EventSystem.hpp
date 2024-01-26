#pragma once
#include "NamedStrings.hpp"
#include <vector>
#include <string>
#include <map>

class NamedProperties;

typedef NamedStrings EventArgs; // should be NamedProperties later
typedef bool (*EventCallbackFunction)(EventArgs& args);
typedef std::vector<EventCallbackFunction> SubscriptionList;//EventSubscription

struct EventSystemConfig
{

};

class EventSystem
{
public:
	EventSystem(EventSystemConfig const& config);
	~EventSystem();
	void Startup(); 
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
    void UnsubscribeFromAllEvents(EventCallbackFunction functionPtr);
	bool FireEvent(std::string const& eventName, EventArgs& args);
	bool FireEvent(std::string const& eventName);
	void GetAllRegisteredCommandsName(std::vector<std::string>& out_listName);

protected:
	EventSystemConfig		m_config;
	std::map<std::string, SubscriptionList>		m_subscriptionListsByEventName;

};

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeFromAllEvents(EventCallbackFunction functionPtr);
bool FireEvent(std::string const& eventName, EventArgs& args);
bool FireEvent(std::string const& eventName);
void GetAllRegisteredCommandsName(std::vector<std::string>& out_listName);