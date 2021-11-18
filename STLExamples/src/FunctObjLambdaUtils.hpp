#pragma once

/* Global and Static variables/objects to test Lambda Captures*/
int gval = 10;
static int static_val = 15;

template<class... Bases>
struct Overloader : Bases...
{
	using Bases::operator()... ;
};

template<class... Bases>Overloader(Bases...)->Overloader<Bases...>;


class IntSequence
{
private:
	int value;
public:
	constexpr IntSequence(int val) noexcept :value{ val } {}

	constexpr int operator()() noexcept
	{
		return value++;
	}

};

class MyPerson
{
private:
	std::string name;
public:
	MyPerson(std::string n) noexcept : name{ std::move(n) } {}

	void printname() const noexcept
	{
		fmt::print("MyPerson: {}\n", name);
	}
	void printname2(std::string&& prefix) const noexcept
	{
		fmt::print("MyPerson: {0} {1} \n", std::move(prefix), name);
	}

};

struct PrinterEx
{
	int numCalls;
	PrinterEx() noexcept :numCalls{ 0 } {}

	void operator()(int x) noexcept
	{
		fmt::print("{} ", x);
		fmt::print("\n");
		++numCalls;
	}
};


struct Baz
{
	decltype(auto) foo() noexcept
	{
		return [this]() noexcept{ fmt::print("value of member s: {0}\n", s); };
	}

	decltype(auto) foo2() noexcept
	{
		return [s=s]() noexcept { fmt::print("value of member s: {0}\n", s); };
	}

	std::string s;
};

/*C17 version of capturing */
template<typename... Args>
constexpr void capture(Args&&... args) noexcept
{
	auto captlambda = [args...]()noexcept
	{
		const auto tup = std::make_tuple(args...);
		fmt::print("tupple size: {0}, sizeof args: {1}, sizeof(Args): {2}\n", std::tuple_size_v<decltype(tup)>, sizeof... (args), sizeof... (Args));
		(fmt::print("{} ", args), ...);
		/*Alternative way of printing; might work containers in fmt*/
		fmt::print("\n{} \n", tup);
		
	};
	captlambda();
}

/*C20 version of above; capture variadics in lambdas*/
#ifdef _HAS_CXX20
	template<typename...Args>
	constexpr void callArgs(Args&&... args) noexcept
	{
		auto captC20vers = [...captargs = std::forward<Args>(args)]() noexcept
		{
			(fmt::print("{} ", captargs), ...);
			std::printf("\n");
		};
		captC20vers();
	}
#endif

	inline void callwith10(void(*func)(int)) noexcept { static int n= 10; func(++n); };

	struct Bazptr
	{
		using f_ptr = void(*)(int);

		void operator()(int s) const noexcept { return call(s); };

		/*conver to function pointer that takes an int return void and return the address of call function*/
		operator f_ptr() const noexcept { return &call; }
	private:
		static void call(int n)  { fmt::print("Bazptr calls: {}\n", n); }
	};

	template<typename... TArgs>
	constexpr decltype(auto) sumFold(TArgs&&... args) noexcept
	{
		return (args + ...);
	}

	/* same above but checks if types are convertible during and will get an error/warning before even compile time*/
	/* in this case it is redundant but usefull if it is an array that expects same types*/
	template<typename T, typename... TArgs, typename=std::enable_if_t<(std::is_convertible_v<T, TArgs> && ...)>>
	constexpr decltype(auto) sumFold(T val, TArgs&&... args) noexcept
	{
		return (val + (args + ...));
	}

	/* Used for overload resolution example by using auto */
	inline void fooFI(int x) { std::printf("Foo int");  }

	inline void fooFI(float x) { std::printf("Foo float"); }

	template<typename Callable>
	constexpr void CallWithOptimAndNormal(Callable&& fn)
	{
		/* Checks if the given function with the arguments is noexcept and invokable*/
		if constexpr (std::is_nothrow_invocable_v<Callable, int>)
		{
			fmt::print("calling fn(10) with optimization\n");
			fn(10);
		}
		else
		{
			fmt::print("calling fn(10) normally\n");
			fn(10);
		}
	}

	template<typename Range, typename Func, typename T>
	constexpr T SimpleAccumulate(Range&& range, Func func, T init)
	{
		for (auto&& elem : range)
		{
			init += func(std::forward<decltype(elem)>(elem));
		}

		return init;
	}

#ifdef _HAS_CXX20
	template<typename First, typename... Args>
	constexpr void captureTest(First&& first, Args&&... args) 
	{
		const auto printer = [first = std::move(first), ...captArgs = std::move(args)]()
		{
			fmt::print("{}", first);
			(fmt::print(", {}", captArgs), ...);
			std::printf("\n");
		};

		printer();
	}
	
	/* Basic Concept example from C++20*/
	template<class T>
	concept SignedIntegral = std::is_integral_v<T> && std::is_signed_v<T>;

	/* example usage of concepts in a template function; no need for typename in template parameters*/
	template<SignedIntegral T>
	void SignedIntsOnly(T val) { fmt::print("Concepts: signed integral: {}\n", val); }

	/* no need for templated auto can be used in function paramaters with or without concepts or other constraints*/
	void MyTemplateFunction(SignedIntegral auto val) { fmt::print("Concepts in funct parameters with auto: signed integral: {}\n", val); };
	
	void FloatsOnlyFunction(std::floating_point auto val) { fmt::print("Concepts and auto in funct: floating point only : {}\n", val); };

	/* Concept with required specifier; C++20*; it defines that a passed argument has certain functionality 
		that matches the certain constraints defined in a concept
	*/
	template<typename T>
	concept IRenderable = requires(T v) {
		{v.render()} ->std::same_as<void>;
		{v.getVertCount()}->std::convertible_to<std::size_t>;
	};
	
	struct Circle
	{
		void render() { fmt::print("render: Drawing Circle\n"); }
		std::size_t getVertCount() { return 12; }
	};

	struct Square
	{
		void render() { fmt::print("render: Drawing Square\n"); }
		std::size_t getVertCount() { return 4; }
	};

	template<typename F>
	struct Product2
	{
		std::string name;
		int id{ 0 };
		double price{ 0.0 };
		F pred;
	};

	/* C++20 constxpr functions/algorithms and auto specifier in function paramaters*/
	constexpr auto CountValues(auto container, auto cmp)
	{
		return std::count_if(std::begin(container), std::end(container), cmp);
	}

	template<typename T, typename U, typename V>
	struct Triple 
	{ 
		T t; U u; V v; 
	};

#endif

	
	struct Product
	{
		std::string name;
		int id{ 0 };
		double price{ 0.0 };
	};


	/* alternative to Concept with c++17 when c++20 is not available*/
	template<class T>
	constexpr bool ASignedIntegral = std::is_integral_v<T>&& std::is_signed_v<T>;

	/* alternative using alias to Concepts with c++17 when c++20 is not available*/
	template<class T>
	using SignedIntegralType = std::enable_if_t<(std::is_integral_v<T> && std::is_signed_v<T>)>;


	