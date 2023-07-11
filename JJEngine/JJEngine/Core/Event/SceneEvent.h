#pragma once
#include "Event.h"
#include <sstream>
class LoadSceneEvent : public Event
{
public:
	//add this always
	EVENT_CLASS_TYPE(LoadScene);
	LoadSceneEvent(const std::string& sceneName)
		:SceneName(sceneName)
	{
	}
	inline std::string GetSceneName() const { return SceneName; }

private:
	std::string SceneName;
};
