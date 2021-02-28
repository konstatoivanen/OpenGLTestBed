#pragma once
#include "PrecompiledHeader.h"

#define PK_BIND_FUNCTION(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define PK_BIND_MEMBER_FUNCTION(o, fn) [o](auto&&... args) -> decltype(auto) { return o->fn(std::forward<decltype(args)>(args)...); }

#define PK_STACK_ALLOC(Type, count) reinterpret_cast<Type*>(alloca(sizeof(Type) * count))

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
constexpr bool IsEqualRef(const Ref<T>& a, const Ref<T>& b)
{
	return a == b || (a && b && *a == *b);
}

template<typename T>
using Weak = std::weak_ptr<T>;