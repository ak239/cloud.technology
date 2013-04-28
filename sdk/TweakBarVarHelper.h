#pragma once

#include <functional>

template<typename T>
class TweakBarVarHelper
{
private:
	std::unary_function<void,T> setter;
	std::unary_function<void,T> getter;
}