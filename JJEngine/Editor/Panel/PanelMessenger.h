#pragma once
#include <queue>

#define ENTITY_SELECTED "EntitySelected"
#define FILE_OPEN "FileOpen"

class PanelMessenger {
	std::queue<std::string> message{};
public:
	bool HasMessage();
	void LeaveMessage(std::string text);
	std::string ReadMessage();
	void DeleteMessage();
};