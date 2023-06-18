#include "UUIDHelper.h"

UUIDType UUIDHelper::MakeUUIDWithUINT64(uint64_t low, uint64_t high)
{
	UUIDType uuid;
	*((uint64_t*)&uuid) = low;
	*((uint64_t*)&uuid + 1) = high;

	return uuid;
}
