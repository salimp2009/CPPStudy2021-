#pragma once
#include "STLpch.h"
#include "StateMachineExample.h"


template<class... Bases>
struct Overload : Bases...
{
	using Bases::operator()...;
};

/* this is not needed for C++20*/
template<class...T> 
Overload(T...)->Overload<T...>;

template<typename Variant, typename... Handlers>
auto match(Variant&& v, Handlers&&... handlers)
{
	return std::visit(Overload{ std::forward<Handlers>(handlers)...}, std::forward<Variant>(v));
}

template<typename... Variant, typename... Handlers>
auto match2(Variant&&... v, Handlers&&... handlers)
{
	//(match(v, handlers...), ...);
	return (std::visit(Overload{ std::forward<Handlers>(handlers)... }, std::forward<Variant>(v)), ...);
}

struct PrintVisitor
{
	void operator()(int i) { fmt::print("int: {}\n", i); }
	void operator()(float f) { fmt::print("float: {}\n", f); }
	void operator()(const std::string& s) { fmt::print("string: {}\n", s); }
};


inline void PrintVisitor_Variants()
{
	std::printf("\n---PrintVisitor_Variants---\n");
	std::variant<int, float, std::string> intFloatString;

	static_assert(std::variant_size_v<decltype(intFloatString)> == 3);

	std::visit(PrintVisitor{}, intFloatString);

	fmt::print("index = {}\n", intFloatString.index());
	
	intFloatString = 55.0f;
	fmt::print("index = {}\n", intFloatString.index());
	
	intFloatString = "Salim";
	fmt::print("index = {}\n", intFloatString.index());

	if (const auto intPtr = std::get_if<int>(&intFloatString))
	{
		fmt::print("intPtr: {0}", *intPtr);
	}
	else if (const auto floatPtr = std::get_if<float>(&intFloatString))
	{
		fmt::print("floatPtr: {0}", *floatPtr);
	}


	if (std::holds_alternative<int>(intFloatString))
	{
		fmt::print("the variant holds an int!\n");
	}
	else if (std::holds_alternative<float>(intFloatString))
	{
		fmt::print("the variant holds a float!\n");
	}
	else if (std::holds_alternative<std::string>(intFloatString))
	{
		fmt::print("the variant holds a string: {}!\n", std::get<std::string>(intFloatString));
	}
	 
	std::variant<std::vector<int>> vecInt{ std::in_place_index<0>, {1,2,3,4} };

	for (const auto& elem : *std::get_if<std::vector<int>>(&vecInt))
	{
		fmt::print("elem: {0} ", elem);
	}

	/* const char* is converted to bool in the below example!*/
	std::variant<std::string, int, bool> typeconversionVar = "Salim";
	fmt::print("\nindex of typeconversion var: {}\n", typeconversionVar.index());

	/*Ambigious variant; compiler cannot decide the type since it can be any of the given type below!!; GCC has a fix*/
	//std::variant<long, float, double> narrowingVar = 0;
}

inline void variantMultipleTest()
{
	struct mOpengl { };
	struct mDirectx { };
	struct mVulkan { };
	struct mMetal { };

	std::variant<mOpengl, mDirectx, mVulkan, mMetal> windowsRenderer{ mDirectx{} };
	std::variant<mOpengl, mDirectx, mVulkan, mMetal> linuxRenderer{ mVulkan{} };
	std::variant<mOpengl, mDirectx, mVulkan, mMetal> macOSRenderer{ mMetal{} };

	/* if you pass multiple variants then there have to be more overloads and it creates unneccessary code so prefer to use one*/
	std::visit(Overload{
		[](const mDirectx& dx) { std::cout<<"DirectX Renderer API !\n"; },
		[](const mMetal& met) {  std::cout << "Metal Renderer API !\n"; },
		[](const mOpengl& ogl) {  std::cout << "OpenGL Renderer API !\n"; },
		[](const mVulkan& vul) {  std::cout << "Vulkan Renderer API !\n"; },
		[](const auto& b) {std::cout << "invalid composition...\n"; },
		}, linuxRenderer);

	match(linuxRenderer, 
		[](const mDirectx& dx) { std::cout << "DirectX Renderer API !\n"; },
		[](const mMetal& met) {  std::cout << "Metal Renderer API !\n"; },
		[](const mOpengl& ogl) {  std::cout << "OpenGL Renderer API !\n"; },
		[](const mVulkan& vul) {  std::cout << "Vulkan Renderer API !\n"; },
		[](const auto& b) {std::cout << "invalid composition...\n"; }
		);

	/* TODO ; try to implement a match function  that takes */
	//match2(linuxRenderer,
	//	[](const mDirectx& dx) { std::cout << "DirectX Renderer API !\n"; },
	//	[](const mMetal& met) {  std::cout << "Metal Renderer API !\n"; },
	//	[](const mOpengl& ogl) {  std::cout << "OpenGL Renderer API !\n"; },
	//	[](const mVulkan& vul) {  std::cout << "Vulkan Renderer API !\n"; },
	//	[](const auto& b) {std::cout << "invalid composition...\n"; }
	//	);

}

class ThrowingClass
{
public:
	explicit ThrowingClass(int i) { if (i == 0) throw int(10); }
	operator int() { throw int(10); }
};


inline void variantExecptionTest()
{
	std::printf("\n---variant Execption Test---\n");

	std::variant<int, ThrowingClass> var;

	try
	{
		var = ThrowingClass(0);
	}
	catch (...)
	{
		/* the old state is kept since the exception is thrown after variant created
		   therefore var.valueless_by_exception() = false, 
		   we keep the old state which is by default the first type of the variant
		*/
		std::printf("exception: catch(...)\n");
		fmt::print("valueless by exception: {0}, value of variant after exception: {1}\n", var.valueless_by_exception(), std::get<int>(var));
	}

	try
	{
		var.emplace<0>(ThrowingClass(10));
	}
	catch (...)
	{
		/* the old state is destroyed the variant is in invalid state*/
		std::printf("exception by emplace: catch(...)\n");
		fmt::print("valueless by exception: {0}\n", var.valueless_by_exception());
	}

}

inline void StateMachineTest()
{
	DoorState state;
	assert(std::holds_alternative<DoorOpened>(state.m_state));
	state.lock();
	assert(std::holds_alternative<DoorOpened>(state.m_state));
	state.close();
	assert(std::holds_alternative<DoorClosed>(state.m_state));
	state.lock();
	assert(std::holds_alternative<DoorLocked>(state.m_state));
	state.unlock();
	assert(std::holds_alternative<DoorClosed>(state.m_state));
	state.open();
	assert(std::holds_alternative<DoorOpened>(state.m_state));
}

