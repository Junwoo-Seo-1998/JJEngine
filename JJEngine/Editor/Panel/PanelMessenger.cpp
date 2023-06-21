#include "PanelMessenger.h"

bool PanelMessenger::HasMessage()
{
	return message.empty() == false;
}

void PanelMessenger::LeaveMessage(MessageType text)
{
	message.push(text);
}

MessageType PanelMessenger::ReadMessage()
{
	return message.front();
}

void PanelMessenger::DeleteMessage()
{
	message.pop();
}
