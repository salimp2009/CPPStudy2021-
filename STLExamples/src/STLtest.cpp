#include "STLpch.h"

import TestModule;
import llvmmodule;			// this is a regular cpp file but it is compiled as c++ module interface from the properties c++ advanced
import LLVMTestModule;      // this .cppm file and vs studio sees this as a regular module like .ixx but when first imported the header under the llvmmodule 
							// it gave errors saying expecting ; then changed the order recompiled error is gone then put it back where it was No errors !
							// See MSVC blog on module https://devblogs.microsoft.com/cppblog/standard-c20-modules-support-with-msvc-in-visual-studio-2019-version-16-8/

#include "TupplePairUtilities.hpp"
#include "Person.h"
#include "MyTypeTraits.hpp"
#include "ChronoExamples.hpp"
#include "FunctionObjectsLambdas.h"
#include "STLAlgorithmTest.h"
#include "Allocators/CustomAllocatorTests.hpp"
#include "Allocators/PmrMemoryResources.h"
#include "cpp17features/Cpp17AddedComponents.h"
#include "cpp17features/FileSystemExamples.h"
#include "Experimental/ExperimentalTests.h"
#include "Experimental/IntrusiveSharedPtrImpl.hpp"
#include "cpp17features/VariantExamples.h"
#include "AttributesCPP20/AtttributesTests.hpp"




/*Use for Chrono examples but Does not work correctly due to Argument lookup problem; does not work in online compilers also */
template<typename V, typename R>
std::ostream& operator<<(std::ostream& os, const std::chrono::duration<V, R>& dur)
{
	os << "[" << dur.count() << "of " << R::num << ", "<< V::den << "] \n";
	return os;
}

typedef std::pair<int, float> IntFloatPair;

struct X
{
	int a = 1;
};

struct Test : public std::enable_shared_from_this<Test>
{

};

class Foo
{
public:
	Foo(std::tuple<int, float>) { std::cout << "Constructed Foo from a tuple\n"; };

	Foo(int a, float b) { std::cout << "Constructed Foo from an int and float \n"; };	

	void printFoo_nums(int a, int b) const { std::cout << "FOO prints; a: " << a << ", b: " << b << '\n'; }
};

void PairTest()
{
	std::tuple<int, float> t{ 42, 50.25f };
	std::pair<Foo, Foo> p4{ t,t };
	std::pair<Foo, Foo> p5{ std::piecewise_construct,  t, t };

	std::pair<int, float> p6{ 4, 20.3f };
	int c = 20;
	std::tie(c, std::ignore) = p6; // ties the first arg of p6 into c by reference so c changes; Changing c does not change P6
	std::cout << "c: " << c << '\n';



	auto& [u, v] = p6;
	std::cout << "u: " << u << ", v: " << v << '\n';

	auto pair1 = std::make_pair(20, "Salim");
	std::cout << pair1 << '\n';

	IntFloatPair p2(20, 40.5f);
	std::cout << p2 << '\n';

	std::cout << std::get<0>(p2) << '\n';

	std::cout << std::tuple_size<IntFloatPair>::value << '\n';
	std::cout << typeid(std::tuple_element<1, IntFloatPair>::type).name() << '\n';
	std::cout << std::boolalpha;
	std::cout << std::is_same_v<std::tuple_element<1, IntFloatPair>::type, float> << '\n'; // expected true
	static_assert(std::is_same_v<std::tuple_element<1, IntFloatPair>::type, float>, "Pair.second element must be a float");
	std::cout << std::is_same_v<decltype(p2.first), float> << '\n';   // expected false

}

void TupleTest()
{
	std::tuple<int, float, std::string> t1{ 2, 23.4f, "Salim" };

	std::cout << "\n----------------------Tuples Test ------------------------------------\n";
	std::cout << std::get<0>(t1) << '\n';
	std::cout << std::get<1>(t1) << '\n';
	std::cout << std::get<2>(t1) << '\n';

	std::cout <<"\t tuple print utility test: "<< t1 << '\n';


	std::tuple<int, float, std::string> t2 = std::make_tuple(2, 23.4f, "Didem");

	std::cout << std::boolalpha;
	std::cout << (t1 < t2) << '\n';

	std::cout << std::tuple_size<decltype(t2)>::value << '\n';
	std::cout << typeid(std::tuple_element<1, decltype(t2)>::type).name() << '\n';

	std::tuple<int, double> t3 = { 2, 45.6 };
	std::cout << t3 << '\n';

	std::cout << "\n----------------------Tuples Test ------------------------------------\n";
}

void SmartPointers_Test()
{
	std::printf ("\n----------------------Smart Pointers------------------------------------\n\n");
	
	std::shared_ptr<std::string> ps1{ new std::string("Salim") };
	/* faster one allocation instead of 2 ; shared_ptr uses two object; one counter and the pointer/value */
	std::shared_ptr<std::string> ps2 = std::make_shared<std::string>("Didem"); 

	std::cout <<"ps1: "<< *ps1 << '\n';
	std::printf("ps2: %s \n", (*ps2).c_str());

	(*ps1)[0] = 'D';
	ps1->replace(1, 1, "A");
	std::cout <<"ps1 modified: "<< *ps1 << '\n';

	using ShrdPtr = std::shared_ptr<std::string>;

	std::vector<ShrdPtr> NameList;
	NameList.push_back(ps1);
	NameList.emplace_back(std::move(ps2));


	std::cout << "NameList: ";
	for (auto&& names : NameList)
	{
		std::cout << *names << " ";
		std::cout << '\n';
	}

	std::cout << "Use count of ps1: " << *NameList[0] <<", use count: "<< NameList[0].use_count() << '\n';

	std::cout << "Use count of ps2: " << *NameList[1] << ", use count: " << NameList[1].use_count() << '\n';

#ifdef _HAS_CXX17 
	/* Compiles as of c++17; does not works work before c++17*/
	/* custom deleters can be used ; if not used since c++17 default deleter also call delete[]p */
	std::shared_ptr<int[]> ps3(new int[10], [](int* p) { std::cout << "custom deleter\n"; delete[]p; });
	std::shared_ptr<int[]> ps4{ new int[10] };
#endif // __HAS_CXX17

/* Does not work with C++17; works with C++20*/
#if _HAS_CXX20 
	std::shared_ptr<int[]> ps555 = std::make_shared<int[10]>();
#endif // CXX20

	

	ps3[0] = 1;
	ps3[1] = 2;
	std::cout << ps3[1] << '\n';


	/* DONT THIS ANYMORE; Before C++17; older version*/
	std::shared_ptr<int>ps5(new int[10]{ 1,2,3,4,5 }, [](int* p) { std::cout << "Old style shared ptr w/ array\n"; delete[] p; });
	*ps5 = 10;
	(ps5.get())[1] = 20;
	std::cout << *ps5 << (ps5.get())[4]<< '\n';

	std::printf("\n----------------------Smart Pointers------------------------------------\n");
}

void WeakPointers_Test()
{
	std::printf("\n----------------------Weak Pointers------------------------------------\n\n");
	
	std::shared_ptr<Person> p1 = InitFamily("salim");
	std::cout << "salim's family exist!!!!\n";
	std::cout << "- salim is shared " << p1.use_count() << " times\n";

	/* Make sure the the shared pointer that weak_ptr is pointing still exist otherwise we will get a nullptr*/
	/*Alternative is to check if p1->mother->kids[0].lock() */
	if (!p1->mother->kids[0].expired())
	{
		std::cout << "- name of 1st kid of salim's mom: " << p1->mother->kids[0].lock()->name << '\n';
	}

	p1 = InitFamily("didem");
	std::cout << "Didem's family exist!!!!\n";
	std::cout << "p1 use count: " << p1.use_count() << '\n';

	/**In order to increase life time of object we first create an shared_ptr
	 then we create a ptr to one if it is members so that when the object goes out of scope 
	 the first shared_ptr will hold it ; known as "Aliasing Constructor" 
	 */
	std::shared_ptr<X> xp(new X);
	std::shared_ptr<int> pa(xp, &xp->a);

	std::cout <<"a of Struct X by pa: "<< *pa << '\n';

	int* pint = new int(10);

	std::shared_ptr<int> p1int{ pint };
	std::shared_ptr<int> p2int{ p1int };

	std::cout <<"p1 int use count: "<< p1int.use_count() << '\n';

	/** Since Test has enable_shared_from this; the below code 
		Creating shared ptr from the the same raw pointer is OK
		Before C++17 it gives runtime error; the two shared pointer dont share ownership
	*/
	Test* ptest = new Test();

	std::shared_ptr<Test>p1test(ptest);
	
	/** this cause an error exit since both shared pointers own the the same raw pointer then once one of them is deleting the other becomes invalid
		and destructor still tries to delete and gives error exit code; program runs but dont use it this way
	*/
	//std::shared_ptr<Test>p2test(ptest); 
	// use count is still one since p1 and p2 dont share ownership; use with caution 
	// if one of the pointer deletes the resource then it will cause runtime errors or undefine behav.
	std::cout << "p1test use count: " << p1test.use_count() << '\n';

	std::weak_ptr<Test>p3weak{ ptest->weak_from_this() };
	/*you can create a shared ptr from original raw ptr that shares owner ship p1test*/
	std::shared_ptr<Test>p4test{ ptest->shared_from_this() };

	/* p1test use count increase since we create a shared_ptr using shared from this*/
	/* no need to delete the raw pointer here since wrapped up into shared_ptr*/
	std::cout << "p1test use count: " << p1test.use_count() << '\n';

	std::unique_ptr<std::string>uptr{ new std::string("Salim") };
	std::cout << *uptr << '\n';

	std::unique_ptr<std::string[]>pstr{ new std::string[]{"Didem", "Demir"} };
	std::cout << "pstr string array  pstr[0]: " << pstr[0] << '\n';
	std::cout << "pstr string array  pstr[1]: " << pstr[1] << '\n';
	std::cout << "second element of array, first character of string;  pstr[1][0]: " << pstr[1][0] << '\n';

	std::printf("\n----------------------Weak Pointers------------------------------------\n\n");
}

void NumericLimits_Test()
{
	std::printf("\n---------------------Numeric Limits------------------------------------\n\n");
	/* Returns the base of exponent for float*/
	std::cout << std::numeric_limits<float>::radix << '\n';
	std::cout <<std::numeric_limits<float>::epsilon() << '\n';
	std::cout<<std::boolalpha;
	std::cout << std::numeric_limits<char>::is_signed << '\n';
	std::printf("\n---------------------Numeric Limits------------------------------------\n\n");
}

void TypeTraits_Test()
{
	std::printf("\n---------------------Type Traits------------------------------------\n\n");

	std::unique_ptr<int>Intptr = std::make_unique<int>(5);

	int* ptr = new int(12);
	/*calls pointer version*/
	TypeFoo(ptr);				
	TypeFoo(Intptr.get());
	/* call value version*/
	TypeFoo(*Intptr);
	TypeFoo(Intptr);

	std::cout << std::boolalpha;
	std::cout << std::is_pointer_v<decltype(ptr)> << '\n';

	TypeFoo2(ptr);
	/* gets the address of pointer is_pointer does not recognizes unique_ptr as pointer type */
	TypeFoo2(Intptr); 
	
	std::cout << std::is_integral_v<float> << '\n';
	std::cout << std::is_integral_v<nullptr_t> << '\n';
	std::cout << std::is_integral_v<bool> << '\n';

	std::cout << (5 != 4) << '\n';

	TypeFoo3(ptr);
	TypeFoo3(*ptr);

	/* std::is_pointer does not recognizes unique_ptr as pointer type therefore you need to use get()*/
	TypeFoo3(Intptr.get());
	TypeFoo3(*Intptr);
	

	delete ptr;
	std::printf("\n---------------------Type Traits------------------------------------\n\n");
}

void FunctionWrappers_Test()
{
	X x;
	/* Reference_wrapper allows us to use ref to any object to pass in containers that does not allow directed access*/
	std::vector<std::reference_wrapper<X>> myXvec;
	myXvec.push_back(x);
	std::cout << myXvec[0].get().a << '\n';
	myXvec[0].get().a = 12;

	std::cout << x.a << '\n';

}

void FunctionPointer_Test()
{

	std::vector<std::function<void(int, int)> > fvec;
	auto sum = [](int a, int b) {std::printf("sum of %d and %d = %d \n", a, b, (a + b)); };
	sum(2, 4);

	fvec.push_back(sum);
	fvec.emplace_back([](int a, int b) {std::printf("sum of %d and %d = %d \n", a, b, (a + b)); });
	fvec[0](2, 4);
	fvec[1](5, 3);

	std::function<void(const Foo&, int, int)> f_printfoo = &Foo::printFoo_nums;

	const Foo mfoo1(32, 45);
	f_printfoo(mfoo1, 3, 4);

	/*-----DONT USE std::bind in modern c++ since lamda or oter options more efficient-----*/
	/* if need to USE ; prefer std::bind_front if C++20 is available*/
#if  _DEBUG
		std::function<void(int, int)> f_printfoo2 = std::bind(&Foo::printFoo_nums, mfoo1, std::placeholders::_1, std::placeholders::_2);
		f_printfoo2(5, 8);
	#if _MSC_VER && _HAS_CXX20
		auto f_printfoo3 = std::bind_front(&Foo::printFoo_nums, mfoo1);
		f_printfoo3(11, 12);
	#endif 
#endif //  DEBUG

}

void Ratio_Test()
{
	[[maybe_unused]] std::ratio<5, 3> fivethirds;

	std::cout << "ratio num: " << fivethirds.num << ", den: " << fivethirds.den << '\n';

	using ratiotype = std::ratio_add<std::ratio<2, 4>, std::ratio<5, 3>>;

	std::cout << ratiotype::num << ", " << ratiotype::den << '\n';

	std::cout << std::boolalpha;
	std::cout <<"ratio equal: "<< std::ratio_equal_v<std::ratio<3, 5>, std::ratio<15, 25>> << '\n';

	std::cout << "predefined ratios milli num: " << std::milli::num <<", mill den :"<<std::milli::den<< 
		",\n nano num:" << std::nano::num <<", nano den: "<<std::nano::den<<'\n';
}



int main(int argc, char* argv[])
{
	//PairTest();
	//TupleTest();
	//SmartPointers_Test();
	//WeakPointers_Test();
	//NumericLimits_Test();
	//TypeTraits_Test();
	//FunctionWrappers_Test();
	//FunctionPointer_Test();
	//Ratio_Test();
	//Chrono_Test();
	
	FunctionObjLamb_Test();
	//FunctionObjLamb_Test2();
	//FunctionObjLamb_TestC17();
	//LambdaIIFE_C17();
	//LambdasInConcurency();
#ifdef  _HAS_CXX20
	Lambdas_C20_Test();
#endif //  _HAS_CXX20
	
	//STLAlgorithms_Test();
	//STLAlgorithms_Test2();
	//STLAlgorithms_Test3();
	//STLAlgorithms_Test4();
	
	//CustomAllocator_Basics();
	//StackAllocator_MemoryPool();
	
	//PmrMemoryResource_Basics();
	//PmrMonotonicBuffer_Basics();
	//PmrSynchronizedPool_Reason();
	//PmrSynchronizedPool_Basics();
	//PmrMonotonicBuff_SynchPool();
	//PmrMonotonicBuff_NoHeap();
	//PmrMonotonicSynch_Tracker();
	//PmrForCustomTypes();
	//PmrString_RegularStrings();
	
	//Optional_Basics();
	//Variant_Basics();
	//StdAny_Basics();
	//StdByte_Basics();
	fromChars_Example();
	toChars_Example();
	fromChars_Example2();
	fromChars_floatingPoint();
	toChars_Integers();

	PrintVisitor_Variants();
	variantMultipleTest();
	variantExecptionTest();
	StateMachineTest();

	//FileSystem_Basics();
	//CheckPath_SwitchFile(/*argc, argv[]*/);
	//CreateDirectory_Files();
	//ReadFileContents_FromPath();
	//FilterFilesUsingRegex();

	functionref_Tests(/*argc, argv[]*/);
	
	/* module function works as expected but red squigles dont go away!!!*/
	MyModuleFunc(255);

	/* Module function using a cpp file not .ixx file!*/
	llvmPrint();

	/* Module function using a cppn file not .ixx file!*/
	llvmPrint2();

	AttributesTest1();

	return 0;

}



