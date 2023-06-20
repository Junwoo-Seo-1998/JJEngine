#include "PanelMessenger.h"

bool PanelMessenger::HasMessage()
{
	return message.empty() == false;
}

void PanelMessenger::LeaveMessage(std::string text)
{
	message.push(text);
}

std::string PanelMessenger::ReadMessage()
{
	return message.front();
}

void PanelMessenger::DeleteMessage()
{
	message.pop();
}
