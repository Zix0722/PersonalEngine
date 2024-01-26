#include "EventSystem.hpp"
#include "DevConsole.hpp"


EventSystemConfig emptyConfig;
EventSystem g_theEventSystem(emptyConfig);

EventSystem::EventSystem(EventSystemConfig const& config)
	:m_config(config)
{

}

EventSystem::~EventSystem()
{

}



void EventSystem::Startup()
{

}


void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	SubscriptionList& subscriptionList = m_subscriptionListsByEventName[eventName];
	subscriptionList.push_back(functionPtr);
}

void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	SubscriptionList& subscriptionList = m_subscriptionListsByEventName[eventName];
	for (int funcIndex = 0; funcIndex < (int)subscriptionList.size(); ++funcIndex)
	{
		if (subscriptionList[funcIndex] == functionPtr)
		{
			subscriptionList[funcIndex] = nullptr;
		}
	}
}

void EventSystem::UnsubscribeFromAllEvents(EventCallbackFunction functionPtr)
{
	for (auto eventIter = m_subscriptionListsByEventName.begin(); eventIter != m_subscriptionListsByEventName.end(); ++eventIter)
	{
		std::string const& eventName = eventIter->first;
		UnsubscribeEventCallbackFunction(eventName, functionPtr);

	}
}

bool EventSystem::FireEvent(std::string const& eventName)
{
	for (auto var = m_subscriptionListsByEventName.begin(); var != m_subscriptionListsByEventName.end(); ++var)
	{
		if (var->first == eventName)
		{
			SubscriptionList& subscriptionList = m_subscriptionListsByEventName[eventName];
			for (int funcIndex = 0; funcIndex < (int)subscriptionList.size(); ++funcIndex)
			{
				EventCallbackFunction callbackFuncPtr = subscriptionList[funcIndex];
				if (callbackFuncPtr != nullptr)
				{
					EventArgs emptyArgs;
					bool wasConsumed = callbackFuncPtr(emptyArgs);
					if (wasConsumed)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
{
	for (auto var = m_subscriptionListsByEventName.begin(); var != m_subscriptionListsByEventName.end(); ++var)
	{
		if (var->first == eventName)
		{
			SubscriptionList& subscriptionList = m_subscriptionListsByEventName[eventName];
			for (int funcIndex = 0; funcIndex < (int)subscriptionList.size(); ++funcIndex)
			{
				EventCallbackFunction callbackFuncPtr = subscriptionList[funcIndex];
				if (callbackFuncPtr != nullptr)
				{
					bool wasConsumed = callbackFuncPtr(args);
					if (wasConsumed)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void EventSystem::GetAllRegisteredCommandsName(std::vector<std::string>& out_listName)
{
	for (auto var = m_subscriptionListsByEventName.begin(); var != m_subscriptionListsByEventName.end(); ++var) 
	{
		out_listName.push_back(var->first);
	}
}

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	g_theEventSystem.SubscribeEventCallbackFunction(eventName, functionPtr);
}

void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	g_theEventSystem.UnsubscribeEventCallbackFunction(eventName, functionPtr);
}

void UnsubscribeFromAllEvents(EventCallbackFunction functionPtr)
{
	g_theEventSystem.UnsubscribeFromAllEvents(functionPtr);
}

bool FireEvent(std::string const& eventName, EventArgs& args)
{
	return g_theEventSystem.FireEvent(eventName, args);
}

bool FireEvent(std::string const& eventName)
{
	return g_theEventSystem.FireEvent(eventName);
}

void GetAllRegisteredCommandsName(std::vector<std::string>& out_listName)
{
	g_theEventSystem.GetAllRegisteredCommandsName(out_listName);
}
