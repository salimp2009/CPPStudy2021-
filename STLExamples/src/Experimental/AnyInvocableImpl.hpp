#pragma once
#include "STLpch.h"

/* NOTE; in progress from C++Now Conference*/

template<typename> class any_invocable;

template<typename Ret, typename... Args>
class  any_invocable<Ret(Args...)>
{
	struct base
	{
		virtual Ret operator()(Args&&...args) = 0;
		virtual~base() = default;
	};

	template typename T>
	struct derived : base
	{
		T object;

		template<typename... CArgs>
		derived(CArgs&&... cargs) : object{ std::forward<CArgs>(cargs)... } {}

		virtual Ret operator()(Args&&...args) override { return std::invoke(object, std::forward<Args>(args)...); }
	};

	std::unique_ptr<base> m_ptr;

public:
	any_invocable() noexcept = default;
	any_invocable(any_invocable&&) noexcept = default;
	any_invocable& operator(any_invocable&&) noexcept = default;
	~any_invocable() = default;

	template<typename T>
	any_invocable(T&& object) 
	requires std::is_invocable_r_v<Ret, std::decay_t<T>&, Args&&...>
		: m_ptr{ std::make_unique < derived<std::decay_t<T>>(std::forward<T>(object)) } {}
	
	Ret operator()(Args&&... args) { return (*ptr)(std::forward<Args>(args)...); }
private:

};

void test(std::string);
//void test(any_invocable);

inline void calltest()
{
	test("hii");
}
