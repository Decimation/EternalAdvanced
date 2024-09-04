#include "ProcessModule.h"

ProcessModule::ProcessModule(std::string name, const HMODULE module)
	: m_name(std::move(name)), m_module(module)
{
}

const std::string& ProcessModule::Name() const
{
	return m_name;
}

HMODULE ProcessModule::Module() const
{
	return m_module;
}
