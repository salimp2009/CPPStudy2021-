#pragma once
#include "STLContpch.h"
#include "StringStackSpecialize.hpp"

template<typename T>
class SPStack2
{
private:
	std::vector<T> cont;
public:
	SPStack2() = default;

	SPStack2(T elem) : cont({ std::move(elem) }) {}

	/* just implemented for learning purposes otherwise the below constructor with variadic is enough*/
	//SPStack2(std::initializer_list<T> ls) :cont{ ls } {}

	template<typename... U, typename = std::enable_if_t<(std::is_convertible_v<U, T> && ...)>>
	SPStack2(U&&... args) noexcept(noexcept(U())) : cont{ std::forward<U>(args)... } { }

	template<typename... U, typename = std::enable_if_t<(std::is_convertible_v<U, T> && ...)>>
	constexpr decltype(auto) emplace(U&&... elems) noexcept
	{
		return (cont.emplace_back(std::forward<U>(elems)), ...);
	}

	constexpr decltype(auto) emplace(T&& elems) noexcept
	{
		return cont.emplace_back(std::forward<T>(elems));
	}

	constexpr void push(T&& elem) noexcept;

	const T& top()  const&;

	T pop();

	std::vector<T>::iterator begin() { return cont.begin(); }
	std::vector<T>::iterator end() { return cont.end(); }
	std::vector<T>::iterator rbegin() { return cont.rbegin(); }
	std::vector<T>::iterator rend() { return cont.rend(); }

	std::vector<T>::const_iterator cbegin() const { return cont.begin(); }
	std::vector<T>::const_iterator cend() const { return cont.end(); }
	std::vector<T>::const_reverse_iterator rbegin()const { return cont.rbegin(); }
	std::vector<T>::const_reverse_iterator rend() const { return cont.rend(); }

};


template<typename T>
const T& SPStack2<T>::top() const&
{
	assert(!cont.empty());
	return cont.back();
}


template<typename T>
T SPStack2<T>::pop()
{
	assert(!cont.empty());
	T temp = cont.back();
	cont.pop_back();
	return temp;
}


template<typename T>
constexpr void SPStack2<T>::push(T&& elem) noexcept
{
	cont.push_back(std::forward<T>(elem));
}
