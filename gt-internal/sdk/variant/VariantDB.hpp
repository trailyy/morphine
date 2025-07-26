#pragma once
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include "../proton/Boost.hpp"
#include "Variant.hpp"

#include <hash_map>
#include <string>

#pragma pack(push, 1)
class FunctionObject
{
public:
	BoostSignal m_sig;
};

class VariantDB
{
public:
	void* m_vtable;
	stdext::hash_map<std::string, Variant*> m_data;
	stdext::hash_map<std::string, FunctionObject*> m_function_data;
	stdext::hash_map<std::string, Variant*>::iterator m_iterator;

	Variant* GetVariant(const std::string& key_name) noexcept
	{
		for (const auto& [name, variant] : m_data)
		{
			if (name == key_name)
				return variant;
		}

		return nullptr;
	}

	FunctionObject* GetFunction(const std::string& key_name) noexcept
	{
		for (const auto& [name, function] : m_function_data)
		{
			if (name == key_name)
				return function;
		}

		return nullptr;
	}
};
#pragma pack(pop)
