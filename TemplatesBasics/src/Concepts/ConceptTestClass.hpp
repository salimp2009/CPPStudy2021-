#pragma once
#include "Templpch.h"
#include "Concepts/ConceptsAddAllVers02.hpp"

template<typename... Args>
concept Testclass = requires
{
	AddAllValues4(std::declval<Args>()...);  // declval will create type with the given type and pass into our function to test
};

/* TODO ; add a type that can create different type so we can pass it above to test it during compile time*/
/* Conditions to test : NOEXCEPT, HASOPERATOR PLUS, HASTYPE, VALIDRETURNTYPE
	the type we will add will have template parameter for above tests 
	so we can pass different template parameter to test different scenarios
*/

namespace details
{
	struct WithType
	{
		using type = int;
	};

	struct WithoutType {};

} // end of namespace details

template<bool NOEXCEPT, bool hasOperatorPlus, bool hasType, bool validReturnType>
class ObjectStub: public std::conditional_t<hasType, details::WithType, details::WithoutType>
{
public:
	ObjectStub() = default;
	
	/* operator + noexcept depends on template parameter NOEXCEPT and also other conditions*/
	ObjectStub& operator+(const ObjectStub& rhs) noexcept(NOEXCEPT) requires (hasOperatorPlus && validReturnType)
	{
		return *this;
	}

	/*Operator + with invalid return type*/
	int operator+(const ObjectStub& rhs) noexcept(NOEXCEPT) requires (hasOperatorPlus && not validReturnType)
	{
		return 3;
	}

};

/* Moved these into ConcecptExamples into function; easier for using */
///* Creating test objects with differen conditions to test concept!*/
//using NoAdd				= ObjectStub<true, false, true, true>;
//using ValidClass			= ObjectStub<true, true, true, true>;
//using NotNoEcept			= ObjectStub<false, true, true, true>;
//using WithoutType			= ObjectStub<true, true, false, true>;
//using DifferentReturnType	= ObjectStub<true, true, false, false>;