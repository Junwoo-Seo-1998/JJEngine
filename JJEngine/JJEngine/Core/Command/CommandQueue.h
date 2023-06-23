#pragma once
#include <cstdint>
class CommandQueue
{
public:
	typedef void(*CommandFunction)(void*);

	CommandQueue();
	~CommandQueue();

	void* Allocate(CommandFunction func, uint32_t size);

	void Execute();
private:
	uint8_t* m_CommandBuffer;
	uint8_t* m_CommandBufferPtr;
	uint32_t m_CommandCount = 0;
};