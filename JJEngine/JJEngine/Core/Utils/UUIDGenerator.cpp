#include "UUIDGenerator.h"

namespace internal
{
	static std::random_device randomDevice;
	
	static std::mt19937 eng(randomDevice());

	uuids::uuid_random_generator uuid_random_generator{ eng };

	UUID stringNameSpace = uuid_random_generator();

	uuids::uuid_name_generator uuid_name_generator{ stringNameSpace };
}

UUID UUIDGenerator::Generate()
{
	return internal::uuid_random_generator();
}

UUID UUIDGenerator::Generate(const std::string& str)
{
	return internal::uuid_name_generator(str);
}
