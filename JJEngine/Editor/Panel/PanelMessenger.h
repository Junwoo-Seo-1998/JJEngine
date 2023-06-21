#pragma once
#include <queue>
#include <memory>
#define ENTITY_SELECTED "EntitySelected"
#define FILE_OPEN "FileOpen"

template<typename t>
struct ContentType{
public: 
	t content{};
};

struct MessageType {
public:
	std::string title{};
	std::shared_ptr<void> attach{};
};

class PanelMessenger {
	std::queue<MessageType> message{};
public:
	bool HasMessage();
	void LeaveMessage(MessageType text);
	MessageType ReadMessage();
	void DeleteMessage();
};