#pragma once




/*---std::cout overloaded for Pair---*/
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p)
{
	return os << "[" << p.first << ", " << p.second << "] \n";
}

/*---std::cout overloaded for Tupple---*/
template<typename... TArgs>
std::ostream& operator<<(std::ostream& os, const std::tuple<TArgs...>& tupleObj)
{
	std::apply([&os](const TArgs...tupleArgs)

		{
			os << '[';
			std::size_t n{ 0 };
			((os << tupleArgs << (++n < sizeof ...(TArgs) ? ", " : "")), ...);
			os << ']' << '\n';
		}, tupleObj);
	return os;
}


/*---BOOST LIBRARY IMPLEMENTATION OF Tuple Print Interface; less expensive---*/

//template<int IDX, int MAX, typename... Args> 
//struct PRINT_TUPLE
//{
//	static void print(std::ostream& os, const std::tuple<Args...>& t)
//	{
//
//		os<< std::get<IDX>(t) << (IDX+1 < MAX ? ", " : "");
//		PRINT_TUPLE<IDX+1, MAX, Args...>::print(os, t);
//	}
//
//};
//
//template<int MAX, typename... Args>
//struct PRINT_TUPLE<MAX, MAX, Args...>
//{
//	static void print(std::ostream& os, const std::tuple<Args...>& t)
//	{ }
//};
//
//template<typename...Args>
//std::ostream& operator<<(std::ostream& os, const std::tuple <Args...>& t)
//{
//	os << '[';
//	PRINT_TUPLE<0, sizeof...(Args), Args...>::print(os, t);
//	os << "]\n";
//	return os;
//}

template<typename Cont>
constexpr void printCont(Cont& cont)
{
	if (cont.empty()) return;

	for (auto&& elem : cont)
	{
		fmt::print("{} ", elem);
	}
	fmt::print("\n");
}
