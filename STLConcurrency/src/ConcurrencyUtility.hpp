#pragma once

#ifdef NDEBUG
	#define ASSERTM(exp, msg) assert ((void)0)
#else
	#define	ASSERTM(exp, msg) assert(((void)msg, exp))
#endif

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

class MySingleton
{
private:
	inline static std::once_flag initInstanceFlag{};
	inline static MySingleton* instance=nullptr;
	MySingleton() = default;
	~MySingleton() = default;

public:
	MySingleton(const MySingleton&) = delete;
	MySingleton operator=(const MySingleton&) = delete;

	[[nodiscard]] static MySingleton* getInstance()
	{
		std::call_once(initInstanceFlag, MySingleton::initSingleton);
		return instance;
	}

	/* instance of a Singleton will create only one time when it is first called */
	static void initSingleton()
	{
		instance = new MySingleton();
	}
};

/* Thread Safe with a know method by Richar Meirs Singleton
	NOTE: check if your compiler support static thread safe !!!!
	VS Studio blog indicates since c++11 static variable are thread safe but it might not be in another compiler
	see: https://docs.microsoft.com/en-us/cpp/cpp/storage-classes-cpp?view=msvc-160
	TODO: ADD a volatile dummy variable in getInstance() to prevent reordering ; see the Concurrency with Moderness book!!!
*/
class MySingletonRM
{
public:
	[[nodiscard]] static MySingletonRM& getInstance() 
	{
		static MySingletonRM instance;
		return instance;
	}

private:
	MySingletonRM() = default;
	~MySingletonRM() = default;
	MySingletonRM(const MySingletonRM&) = delete;
	MySingletonRM operator=(const MySingletonRM&) = delete;
};

struct MyData
{
	inline static std::string gName = "global";					// unique in program/struct/class
	inline static thread_local std::string tName = "tlocal";	// unique per thread
	std::string lName = "local";								// unique per object

	void printData(const std::string& msg)
	{
		std::cout << msg << "\n";
		std::cout << " - gName : " << gName << '\n';
		std::cout << " - tName : " << tName << '\n';
		std::cout << " - lName : " << lName << '\n';
	}

};

inline thread_local MyData myThreadData;  // one object per thread


template<typename TFunct>
constexpr void RunAndMeasure(std::string_view title, TFunct&& func) 
{
	auto start = std::chrono::high_resolution_clock::now();
	
	auto returnedValue = func();
	const auto end = std::chrono::high_resolution_clock::now();
	
	auto duration = std::chrono::duration<double, std::milli>(end - start).count();

	fmt::print("title: {0}, duration: {1}, returned Value: {2}\n", title, duration, returnedValue);

}