// Author: Deci | Project: EternalAdvanced | Name: ProcessModule.h
// Date: 2024/09/04 @ 15:09:59

#pragma once
#include <string>
#include <Windows.h>

class ProcessModule
{
private:
	const std::string m_name;
	const HMODULE     m_module;

public:
	ProcessModule(std::string name, const HMODULE module);

	const std::string& Name() const;

	HMODULE Module() const;
};
