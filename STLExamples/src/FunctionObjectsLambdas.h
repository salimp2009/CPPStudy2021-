#pragma once

#include "TupplePairUtilities.hpp"
#include "FunctObjLambdaUtils.hpp"
#include <complex>

#define FUNCTWRAP(fn)\
[](auto&&... x)\
	noexcept(noexcept(fn(std::forward<decltype(x)>(x)...)))\
	->decltype(fn(std::forward<decltype(x)>(x)...))\
	{return fn(std::forward<decltype(x)>(x)...);}


inline void FunctionObjLamb_Test()
{
	fmt::print("\n---------------Function Objects Lambda --------------------------\n");

	std::vector<int> v1{ 1,2 };

	fmt::print("vector print {}", fmt::join(v1, ", "));

	const PrinterEx vis = std::for_each(v1.begin(), v1.end(), PrinterEx());

	fmt::print("numcalls: {}\n", vis.numCalls);

	std::list<int>coll;
	IntSequence seq(1);

	std::generate_n<std::back_insert_iterator<std::list<int>>, int, IntSequence&>(std::back_inserter(coll), 4, seq);
	printCont(coll);

	std::generate_n(std::back_inserter(coll), 4,std::ref(seq));
	printCont(coll);

	std::generate_n(std::back_inserter(coll), 4, seq);
	printCont(coll);

	std::generate_n(std::back_inserter(coll), 2, IntSequence(44));
	printCont(coll);

	auto plus10times2 = [](auto&& y) {return (y + 10) * 2; };
	std::cout << plus10times2(7) << '\n';

	std::mem_fn(&MyPerson::printname)(MyPerson("Salim"));
	std::mem_fn(&MyPerson::printname2)(MyPerson("Didem"), "is a ");

	int x1 = 5;
	auto l1 = [&x1]() mutable noexcept { fmt::print("x1 from lambda thru a : {} \n", ++(x1)); };
	x1 = 456;
	l1();

	auto lmb2 = [x1]() mutable noexcept
	{
		fmt::print("x1 from lmb2 copied by val (mutable) {}\n", ++x1);

	};

	lmb2();
	std::cout << "value of x1 after lmb2 mutable changed it: " << x1 << '\n';
	//++x1;
	lmb2();
	std::cout << "value of x1 after lmb2 mutable changed it: " << x1 << '\n';
	lmb2();
	++x1;
	lmb2();


	/* no need for capturing any gloabal or static variables/objects; if you do you get compiler error*/
	auto lmbd3 = []() noexcept
	{
		++gval; ++static_val;
	};

	lmbd3();
	fmt::print("global/static values of gval &static_int after captured/modified in lambda: {0}, {1}\n", gval, static_val);

	Baz bz{ "Salim" };
	auto myBazf1 = bz.foo();
	myBazf1();

	/* this will not print the value of struct member since we create a temp object and
		by the time we call it expired therefore the value destroyed
	*/
	auto myBazf2 = Baz("Salitos").foo();
	myBazf2();

	std::unique_ptr<int>uptr = std::make_unique<int>(10);

	std::printf("address of uptr: %p \n", uptr.get());

	auto ulmd4 = [ptr = std::move(uptr)]() noexcept { fmt::print("ptr in lambda adress {0}, and value {1}\n", fmt::ptr(ptr.get()), *ptr); };
	ulmd4();

	std::printf("address of uptr after moved : %p \n", uptr.get());

	std::unique_ptr<int>uptr2 = std::make_unique<int>(10);

	auto Overloadedlambda = Overloader([](auto f) {return f(); });
	Overloadedlambda(myBazf1);
	Overloadedlambda(bz.foo());

	const int cx = 10;
	/* you cannot modify a captured const variable*/
	auto myconstlmbda = [cx]()mutable {fmt::print("{}\n", std::is_const_v<decltype(cx)>); };
	myconstlmbda();

	/* C++17 version of implementation of variadic capture in lambdas*/
	capture(1, 2, 3, "Salim");

	/* C++20 version of implementation of variadic capture in lambdas*/
#ifdef _HAS_CXX20
	std::string name = "Didem";
	callArgs(1, 2, 3, name);

	std::printf("name after callArgs %s\n", name.c_str());
#endif

	callwith10([](int x) {fmt::print("lambda with 10+ : {}\n", x); });
	callwith10([](int x) {fmt::print("lambda with 10+ : {}\n", x); });

	Bazptr mybaz;
	callwith10(mybaz);

	auto funcptr = static_cast<void(*)()>([]()->decltype(auto) {});
	static_assert(std::is_same_v<decltype(funcptr), void(*)()>);

	/*Example for passing lamda into container stateless*/
	using TFunct = void(*)(int&);
	std::vector<TFunct> ptrFunctVec;
	
	ptrFunctVec.emplace_back([](int& x) {fmt::print("x: {}\n", x); });
	ptrFunctVec.emplace_back([](int& x) {x *= 2; });
	ptrFunctVec.emplace_back(ptrFunctVec[0]);

	int vx = 10;
	for (const auto& elem : ptrFunctVec)
	{
		elem(vx);
	}
	

}

inline void FunctionObjLamb_Test2()
{
	std::printf("\n---------------Function Objects Lambda Part2--------------------------\n");

	auto lam1 = [](int x = 10) {fmt::print("x: {}\n", x); };
	lam1();

	int x1 = 15;
	auto lam2 = [&z = x1]() {fmt::print("z: {}\n", z); };
	lam2();
	x1 = 35;
	lam2();

	std::unique_ptr<int>uptr(new int(10));

	std::function<void()>ufunct = [ptr = &uptr]{};

	using namespace std::string_literals;
	/* vector is used for example but prefer std::Array if const is used */
	const std::vector<std::string>vs = { "Salim"s, "Didem", "foobar"s, "Demir"s };

	const auto prefix = "foo"s;
	auto result = std::find_if(vs.cbegin(), vs.cend(), 
					[searchprefix = prefix + "bar"s](std::string_view elem) { return elem == searchprefix; });

	if (result != vs.cend())
	{
		fmt::print("result: {}", *result);
	}

	const auto myf2 = Baz{ "c++" }.foo2();
	myf2();

	const auto sumLambda = [](auto&&... args)
	{
		fmt::print("sum of {} numbers: ", sizeof... (args));
		return sumFold(std::forward<decltype(args)>(args)...);
	};

	fmt::print("{} \n", sumLambda(1, 2.1, 3.1f, 4.1));

	/** Example to check the when using auto && there is no extra copies by checking the values address in container
		and comparing in the std::for_each and passing into lambda
	*/
	const std::map<std::string, int>numbers = { {"one", 1}, {"two", 2}, {"three",3} };

	for (auto it = numbers.cbegin(); it != numbers.end(); ++it)
	{
		fmt::print("{0}, {1} \n", fmt::ptr(&(it->first)), fmt::ptr(&(it->second)));
	}

	std::for_each(numbers.cbegin(), numbers.cend(),
		[](auto&& elem) 
		{
			fmt::print("{0}, {1} \n", fmt::ptr(&(elem.first)), fmt::ptr(&(elem.second)));	
		});

	const std::array<int, 10> arr1={ 1,2,3,4,5,6,7,8,9 };

	/* Will not compile since fooFI overloads cannot be deduced */
	//std::for_each(arr1.begin(), arr1.end(), fooFI);

	std::for_each(arr1.begin(), arr1.end(), [](auto&& x) noexcept(noexcept(fooFI(std::forward<decltype(x)>(x)))) ->decltype(auto) { return fooFI(std::forward<decltype(x)>(x)); });
	std::printf("\n");
	
	/* More generic lambda version of the above within a Macro */
	std::for_each(arr1.begin(), arr1.end(), FUNCTWRAP(fooFI));
	std::printf("\n");

	constexpr auto factorialLambd = [](auto&& n) constexpr noexcept
	{
		constexpr auto factor_impl = [](auto&& n, const auto& impl) constexpr noexcept->int
		{return n > 1 ? n * impl(n - 1, impl) : 1; };
		
		return factor_impl(n, factor_impl);
	
	};

	const int n = 5;
	fmt::print("factorial of {1}: {0}\n",factorialLambd(n), n);

}



inline void FunctionObjLamb_TestC17()
{
	std::printf("\n---------------Function Objects Lambda Part3 / C++17--------------------------\n");

	int x1{ 10 };

	const auto lam1 = [&x1](auto&& y)noexcept { x1 += y; };
	CallWithOptimAndNormal(lam1);

	const auto lam2 = [&x1](auto&& y) {x1 += y; };
	CallWithOptimAndNormal(lam2);

	fmt::print("x1 after the calls above: {}\n", x1);

	/** Example for compile calculation using constexpt in the function def;
		it will also work at run time if the passed value are not known compile time
	*/
	constexpr std::array arr1{ 1,2,3 };
	constexpr auto sum = SimpleAccumulate(arr1, [](auto&& x) { return x * x; }, 0);
	static_assert(sum == 14);

	const std::string in = "Hello Man";
	const std::string out = std::invoke([copy = in]() mutable
		{
			return copy.replace(copy.find("Man"), 3, "C++");
			

		});

	fmt::print("string out after replace: {}\n", out);

	/* Fold expressions in C++17 basics*/
	const auto sumLamda = [](auto&&... args)
	{
		fmt::print("sum of {} numbers \n", sizeof...(args));
		return (args + ...);
	};

	fmt::print("{}\n", sumLamda(1.2, 1.3, 1.4));

	const auto printer = [](auto&& first, auto&&... args)
	{
		/* Alternative to end the comma operator*/
		//auto currentsize = sizeof...(args);
		//((std::cout << args << (currentsize-- >1 ? ", " : " ")), ...)<<'\n';
		std::cout << first; 
		((std::cout << ',' << args), ...) << '\n';
		
		fmt::print("{0}, {1}\n", first, fmt::join(std::make_tuple(args...) , ","));
	};

	printer(1, 2, "Salim");
	
	const auto test = Overloader{
		[](const int& i) {fmt::print("int overloaded: {}\n", i); },
		[](const float& f) { fmt::print("float overloaded: {}\n", f); },
		[](std::string_view sv) { fmt::print("string overloaded: {}\n", sv); }

	};

	test("10.0f");
	test(10.0f);

	const auto PrintVisitor = [](const auto& t) {fmt::print("PrintVisitor: {}\n", t); };

	std::variant<int, float, std::string> intFloatString{ "My Variant is great" };

	std::visit( PrintVisitor, intFloatString );

	std::visit(test, intFloatString);
}

inline void LambdaIIFE_C17()
{
	std::printf("\n---------------Lambda IIFE / C++17--------------------------\n");
	using Student = std::pair<std::vector<double>, std::string>;

	const std::vector<Student> db = { { {5.0, 5.0, 5.0, 4.0}, "John"},
									  { {5.0, 5.0, 5.0, 4.0}, "Mark"},
									  { {5.0, 5.0, 5.0, 4.0}, "Jane"},
									};

	/* Converting the calculation of averages above into a lambda that is invoked immediately*/
	auto averages =[](const std::vector<Student>& Indb)
	{
		std::vector<std::pair<double, std::string>> out;
		for (auto&& [marks, name] : Indb)
		{
			double avg = std::accumulate(marks.begin(), marks.end(), 0.0) / marks.size();
			out.push_back({ avg, name });
		}

		
		/*Alternative to for loop; but for loops looks better*/
		//std::transform(Indb.begin(), Indb.end(), std::back_inserter(out),
		//	[&](auto&& student)
		//	{
		//		/*to be able to structured bindings work added capture by & into lambda in the transform*/
		//		auto&& [marks, name] = student;
		//		double avg = std::accumulate(marks.begin(), marks.end(), 0.0) / marks.size();
		//		return std::make_pair(avg, name);
		//	});

		return out;
	}(db);

	printCont(averages);

}

inline void LambdasInConcurency()
{
	std::printf("\n---------------Lambdas in Concurrency - Part1 / C++17--------------------------\n");

	const auto PrintThreadID = [](std::string_view sv) {
		fmt::print("message: {0}, thread id: {1}\n", sv, std::this_thread::get_id()); 
	};

	std::vector<int>nums(100);

	/* this example is subject to data race if multiple threads used since std::vector is passed by ref*/
	std::thread iotaThread([&nums, &PrintThreadID](int StartArg) 
		{
			std::iota(nums.begin(), nums.end(), StartArg);
			PrintThreadID("iota in");
		}, 10 /* value the lambda expects is passed when creating the thread*/
		);

	iotaThread.join();

	printCont(nums);

	/* if counter is not atomic it will cause data race as different thread might write/modify the value*/
	std::atomic<int> counter{ 0 };
	const auto maxThreads = std::thread::hardware_concurrency();
	fmt::print("maxThread: {}\n", maxThreads);
	std::vector<std::thread>threads;
	threads.reserve(maxThreads);

	for (std::size_t tCounter{ 0 }; tCounter < maxThreads; ++tCounter)
	{
		threads.emplace_back([&counter]() noexcept
			{
				for (int i{ 0 }; i < 1000; ++i)
				{
					counter.fetch_add(1);
					counter.fetch_sub(1);
					counter.fetch_add(1);
					counter.fetch_sub(1);
				}
			});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	fmt::print("counter: {}\n", counter.load());

	/*Basic std::future example*/
	/** the return type of the function passes in arguments of std::future has to match the type of std::future
		if the passed function expects arguments then those has to be passed arguments to std::future
	*/
	std::future<std::vector<int>> iotaFuture = std::async(std::launch::async, 
			[startArgs=15]() 
			{
				std::vector<int>num2(100);
				std::iota(num2.begin(), num2.end(), startArgs);
				fmt::print("calling iota from thread : {}\n", std::this_thread::get_id());
				return num2;
			});

	auto result= iotaFuture.get();
	printCont(result);

	/* A naive example of parallel algorithms in C++17;*/
	std::vector<int>nums3(1000);
	std::iota(nums3.begin(), nums3.end(), 0);
	std::mutex m;
	std::vector<int>out;

	std::for_each(std::execution::par, nums3.begin(), nums3.end(),
		[&m, &out](auto&& elem) 
		{
			if (elem % 2 == 0)
			{
				std::lock_guard guard(m);
				out.push_back(elem);
			}
		});

	printCont(out);


}

#ifdef _HAS_CXX20
inline void Lambdas_C20_Test()
{

	std::printf("\n---------------Lambdas in C++20 - Part1 --------------------------\n");
	auto [x, y] = std::tuple{ std::string{"Hello c++20"}, 42 };

	const auto ParamLength = [&x, &y]() { return x.length() + y; };
	fmt::print("Param Length: {}\n", ParamLength());

	/* this example does not MSVC compiler; works in GCC and Clang only*/
	const int x1{ 10 };
	/* consteval works only compile time whereas constexpr can work both compile or runtime*/
	auto lam1 = [](int n) consteval -> int {return n + n; };
	lam1(x1);

	std::unique_ptr<int> uptr{ new int(10) };

	captureTest(std::move(uptr), 1, 2, "Salim");
	captureTest(std::move(uptr), 5.4F, 25., "ZORT");

	/* in C++20 template types can be variadic too; below is example is unnecessary complicated just for test purposes; especially return type*/
	auto lam2 = []<typename T, typename... TArgs>(const std::vector<T>&vec, TArgs... args)->std::vector<std::enable_if_t<(std::is_convertible_v<T, TArgs> && ...), T>>
	{

		fmt::print("size: {0}, vec capacity: {1}\n", std::size(vec), vec.capacity());
		return { std::forward<TArgs>(args)... };
	};

	std::vector<int> vec1 = { 1, 2, 3 };
	auto vec3 = lam2(vec1, 5, 6, 7);
	printCont(vec3);

	SignedIntsOnly(1);
	MyTemplateFunction(4);

	FloatsOnlyFunction(5.8f);

	auto GenLambda1 = [](SignedIntegral auto param) { return param * param + 1; };
	fmt::print("Generic lambda with Concepts: {}\n", GenLambda1(5));

	const auto RenderCall = [](IRenderable auto&& obj) { obj.render(); };

	Circle c1;
	Square sq1;
	RenderCall(c1);
	RenderCall(sq1);

	const auto nameComp = [](auto&& a, auto&& b) { return b.name < a.name; };

	/* since C++20 the types stateless lambdas can be deduced */
	std::set< Product, decltype(nameComp)> prodSet = { {"Cup", 10, 100.0}, {"TV", 12, 1500.0}, {"Book", 2, 150.0}, {"Comp", 25, 2000.0} };

	for (auto&& product : prodSet)
	{
		fmt::print("name {0}, ID: {1}, Price: {2}\n", product.name, product.id, product.price);
	}

	/* Example for C++20 to store a stateless lambda in custom type and it is even copyable*/
	const auto idComp = [](auto&& a) { return a.ID != 0; };

	/* decltype is not needed but VS intellisense thinks there is no constructor but it works regardless of the warning*/
	Product2<decltype(idComp)> p1{ "Laptop", 10, 100.0, idComp };

	Product2 p2{p1};

	fmt::print("name {0}, ID: {1}, Price: {2}\n", p1.name, p1.id, p1.price);
	fmt::print("name {0}, ID: {1}, Price: {2}\n", p2.name, p2.id, p2.price);

	/* Example showing STL algorithms are marked as constexpr and 
	   constexpr lambdas can be used with for compile time operations
	*/
	constexpr std::array arr1{ 1,2,3,4,5,6,7,8,9,10 };
	static_assert(std::accumulate(arr1.begin(), arr1.end(), 0, [](auto&& a, auto&& b) noexcept { return a + b; })==55);

	/* C++20; compile algorithms with stateless lambdas passed in function params and function uses auto type specifier in params*/
	constexpr auto minVal = CountValues(std::array{ -10, 6,8,-5,2,4,6}, [](auto&& a) { return a <= 0; });
	/* above calculation has done in compile time*/
	static_assert(minVal == 2);
	fmt::print("minVal: {}\n", minVal);

	Triple tr1{ 1.0f, 2, std::string{ "Salim" } };

	

}
#endif
