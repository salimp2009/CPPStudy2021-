#pragma once
#include "Templpch.h"

template<typename Cont>
concept RandomIteratorCont = std::random_access_iterator<typename Cont::iterator>;

template<typename Cont>
concept BiDirectIteratorCont = std::bidirectional_iterator<typename Cont::iterator>;

template<typename Cont>
concept ContainerWithSort = requires(Cont cont)
{
	//requires std::bidirectional_iterator<typename Cont::iterator>;
	cont.sort();
};


template<RandomIteratorCont T>
void PrintSort(T cont)
{
	std::sort(cont.begin(), cont.end());
	fmt::print("{}\n", fmt::join(cont, ", "));
}

template<ContainerWithSort  T>
void PrintSort(T cont)
{
	cont.sort();
	fmt::print("{}\n", fmt::join(cont, ", "));
}