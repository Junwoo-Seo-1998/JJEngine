#include "CommandQueue.h"
#include <cstring>

#include "Core/Utils/Log.h"

CommandQueue::CommandQueue()
{
	//for now 8mb
	constexpr int mbSize = 8;
	m_CommandBuffer = new uint8_t[mbSize * 1024 * 1024]; // 8mb buffer
	m_CommandBufferPtr = m_CommandBuffer;
	memset(m_CommandBuffer, 0, mbSize * 1024 * 1024);
}

CommandQueue::~CommandQueue()
{
	delete[] m_CommandBuffer;
}

void* CommandQueue::Allocate(CommandFunction func, uint32_t size)
{
	//todo 4 bytes align
	//EngineLog::Critical("Func Size {}", sizeof(CommandFunction));
	//EngineLog::Critical("Size {}", size);
	*(CommandFunction*)m_CommandBufferPtr = func;
	m_CommandBufferPtr += sizeof(CommandFunction);

	*(uint32_t*)m_CommandBufferPtr = size;
	m_CommandBufferPtr += sizeof(uint32_t);

	void* memory = m_CommandBufferPtr;
	m_CommandBufferPtr += size;

	m_CommandCount++;
	return memory;
}

void CommandQueue::Execute()
{
	uint8_t* buffer = m_CommandBuffer;

	for (uint32_t i = 0; i < m_CommandCount; i++)
	{
		CommandFunction function = *(CommandFunction*)buffer;
		buffer += sizeof(CommandFunction);

		uint32_t size = *(uint32_t*)buffer;
		buffer += sizeof(uint32_t);
		function(buffer);
		buffer += size;
	}

	m_CommandBufferPtr = m_CommandBuffer;
	m_CommandCount = 0;
}
