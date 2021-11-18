#pragma once
#include "Templpch.h"

void printVar() {}

template<typename TFirst, typename... Types>
void printVar(TFirst firstArg, Types... args)
{
	auto size = sizeof...(args);
	fmt::print("{0}{1}", firstArg, (size-->0 ? ", ": "\n"));
	
	if constexpr (sizeof...(args)>0)
	{
		printVar(args...);
	}
}

template<typename T>
void printVar2(T&& arg) 
{
	fmt::print("{0} ", arg);
}

template<typename TFirst, typename... Types>
void printVar2(TFirst&& firstArg, Types&&... args)
{
	printVar2(std::forward<TFirst>(firstArg));
	printVar2(std::forward<Types>(args)...);
}

template<typename... TArgs>
void printVar3(TArgs&&... args)
{
	auto size = sizeof...(args);
	(fmt::print("{0}{1}", args, (--size>0 ? ", ": "n")), ...);
}

template<typename... T>
decltype(auto) foldSum(T... args)
{
	return (... + args); // Equal to ((s1+s2)+s3)  // left fold;
}

template<typename... T>
decltype(auto) foldSumWithInit(T... args)
{
	return (sizeof...(args) + ... + args); // Equal to ((s1+s2)+s3)  // left fold;
}

void inline VariadicTemplate_Basics()
{
	printVar(7.5, "Hello variadics", "c++", 200);
	printVar2(7.5, "Hello variadics", "c++", 200);
	printVar3(455, "Hello newtricksy", "ccpCon2021", 2021);
	std::printf("\n");
	printVar2(35, 45, "jaws", 200);
	std::cout << "what the ff....";
	fmt::print("\nfold sum: {}", foldSum(1, 2, 3, 4, 5.5f));
	fmt::print("\nfold sum: {}\n", foldSumWithInit(1, 2, 3, 4));
}

struct Node
{
	/* prefer to use shared pointers for left */
	int value;
	Node* left;
	Node* right;
	Node(int i = 0) :value{ i }, left{ nullptr }, right{ nullptr } {}
};

auto left = &Node::left;
auto right = &Node::right;


/* traverse tree using fold expressions; 
   could not make this work with smart pointers

*/
template<typename T, typename... TP>
Node* traverse(T np, TP... paths)
{
	return (np ->* ... ->*paths); // np ->*paths1->*paths2 ... ; dereference begining Node's left or right node and it continues to derefence each node left or right 
};

inline void FoldingBinaryTree()
{
	Node* root = new Node(0);
	root->left = new Node(1);
	root->left->right = new Node(2);
	root->left->right->left = new Node(3);
	root->left->right->right = new Node(4);

	Node* node = traverse(root, left, right, right);

	fmt::print("Node result: {}\n",node->value);
	delete root->left->right->right;
	delete root->left->right->left;
	delete root->left->right;
	delete root->left;
	delete root;
}

template<std::size_t... Idx, typename Coll>
void printElems(Coll&& coll)
{
	int count{ 0 };
	(fmt::print("{0}{1}", coll[Idx],(++count<sizeof...(Idx) ? ", ":"\n")), ...);
}

template<std::size_t...>
struct Indices {};

template<typename Coll, std::size_t... Idx>
void printByIdx(Coll& coll, Indices<Idx...>)
{
	int count{ 0 };
	(fmt::print("{0}{1}", std::get<Idx>(coll), (++count < sizeof...(Idx) ? ", " : "\n")), ...);

}

inline void VariadicIndices()
{
	std::vector<std::string>coll1 = { "good", "times", "will", "come" };
	printElems<2,0,3,1>(coll1);

	std::array<std::string, 5> arr1= { "good", "times", "will", "come" };
	printByIdx(arr1, Indices<2, 0, 3, 1>{});
}


struct Baz
{
	decltype(auto) foo() noexcept
	{
		return [this]() noexcept { fmt::print("value of member s: {0}\n", s); };
	}

	decltype(auto) foo2() noexcept
	{
		return [s = s]() noexcept { fmt::print("value of member s: {0}\n", s); };
	}

	std::string s;
};

template<class... Bases>
struct Overloader :Bases...
{
	using Bases::operator()...;
};

template<class... Bases>
Overloader(Bases...)->Overloader<Bases...>;

template<typename Fn, typename... Args>
decltype(auto) FunctRef(Fn&& fn, Args&&... args)
{
	return std::invoke(fn, std::forward<Args>(args)...);
}

inline void VariadicBases_Overloader()
{
	int count = 25;
	using FuncOverloader = Overloader<void(*)(int)>;
	auto myfunc = [count](auto&& n)
	{
		fmt::print("func ref: {}", n+count);
		return n+count;
	};

	FunctRef(myfunc, 5);

	Baz mybaz{ "salim" };
	FunctRef(mybaz.foo());

	
}