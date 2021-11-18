#pragma once

// [=] the implicit capture of this is depreciated in c++20 and will probably be removed in c++23;
// so use this to pass the object as pointer or *this as a copy in lambdas inside a class that needs to access member variables
class SomeClass
{
private:
	int value{5};
public:
	int getTwiceValue()
	{
		auto twice = [*this]() { return value * 2; };
		return twice();
	}
};


struct Book
{
	std::string title;
	std::string isbn;
};

struct Price
{
	double amount;
};

auto compare = [](const Book& a, const Book& b) { return a.isbn > b.isbn; };

template<typename VALUE>
using MapBookSortedbyISBN = std::map<Book, VALUE, decltype(compare)>;

struct Magazine
{
	std::string name;
	std::string isbn;
};

template<typename T>
concept HasISBN = requires(T a)
{
	{a.isbn};
};

struct BookNoISBN
{
	std::string title;
	//std::string isbn;
};

auto compareISBN = []<HasISBN T>(T&& a, T&& b) { return a.isbn > b.isbn; };
template<typename KEY, typename VALUE>
using MapSortedbyISBN = std::map<KEY, VALUE, decltype(compareISBN)>;


template<typename... Origins, typename... Ts>
void printLog(Origins&&... origins, Ts&&... args)
{
	(fmt::print("origin: {}, args: {}", origins, args), ...);
}

// Does not compile with MSVC; MSVC Bug When there 2 variadic arguments in Lambda Capture ; it does not work; compİLES WİTH gcc / clang
template<typename... Origins>
auto getNamedLogger(Origins&&... origins)
{
	// lambda with variadic capture
	return [..._origins = std::forward<Origins>(origins)]<typename... Ts>(Ts... args)
	{
		printLog(_origins... , std::forward<Ts>(args)...);
	};

}


template<typename... Origins>
auto getNamedLogger2(Origins&&... origins)
{
	// lambda with requires ; // Example scenario; not to accept Origins as pointers and also the args not floating points
	return [..._origins = std::forward<Origins>(origins)]<typename... Ts>requires(not std::disjunction_v<std::is_floating_point<Ts>...>)
	(Ts... args) requires(not std::disjunction_v<std::is_pointer<Origins>...>)
	{
		printLog(_origins..., std::forward<Ts>(args)...);
	};
}

template<typename T>
concept NotFloatingPoint = not std::is_floating_point_v<T>;

template<typename... Origins>
auto getNamedLogger2A(Origins&&... origins)
{
	// added the  concept NotFloatingPoint in the template typename in lambda
	return[..._origins = std::forward<Origins>(origins)]<NotFloatingPoint... Ts> (Ts... args) requires(not std::disjunction_v<std::is_pointer<Origins>...>)
	{
		printLog(_origins..., std::forward<Ts>(args)...);
	};
}

// this may cause undefined behaviour due to lifetime of the struct ; it goes out out of scope and lambda calls a member variable of a class that goes out of scope
struct LambdaFactory
{
	auto foo()
	{
		return [=]() { std::printf("%s \n", s.c_str()); };
	}

	std::string s = "Lamda Factory";
	~LambdaFactory() { std::puts("Goodbye from Lmbda Factory"); }
};


inline auto makeLambda()
{
	LambdaFactory lambdafactory;
	return lambdafactory.foo();
}

struct LambdaFactory2
{
	auto foo()
	{
		return [*this]() { std::printf("%s \n", s.c_str()); };
	}

	std::string s = "Lambda Factory2";
	~LambdaFactory2() { std::puts("Goodbye from Lambda Factory2"); }
};

inline auto makeLambda2()
{
	LambdaFactory2 lambdafactory2;
	return lambdafactory2.foo();
}

