// Author: Deci | Project: EternalAdvanced | Name: Fnc.h
// Date: 2024/09/04 @ 15:09:46

#pragma once
#include <type_traits>


// General template
template <typename Ret, bool Void = std::is_void_v<Ret>, typename... Args>
class Fnc
{
	Ret (&fnc_)(Args...);

public:
	Fnc(Ret (&fnc)(Args...)) : fnc_(fnc)
	{
	}

	Ret operator()(Args... args) { return fnc_(args...); }
};

// Void return specialized template
template <typename Ret, typename... Args>
class Fnc<Ret, true, Args...>
{
	void (&fnc_)(Args...);

public:
	Fnc(void (&fnc)(Args...)) : fnc_(fnc)
	{
	}

	void operator()(Args... args) { (fnc_)(args...); }
};

// Fnc helper function
template <class Ret, bool Void = std::is_void<Ret>::value, class... Args>
const Fnc<Ret, Void, Args...> make_fnc(Ret (&fnc)(Args...))
{
	return Fnc<Ret, Void, Args...>(fnc);
}