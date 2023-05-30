#pragma once
#include <string>

struct NameComponent
{
public:
	std::string Name;

	NameComponent() = default;

	NameComponent(const NameComponent& r_other) = default;

	NameComponent(const std::string& name)
		: Name(name) {}

	operator std::string& () { return Name; }
	operator const std::string& () const { return Name; }
};
