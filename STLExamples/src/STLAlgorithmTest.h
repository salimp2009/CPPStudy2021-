#pragma once

inline void STLAlgorithms_Test()
{
	std::printf("\n---------------STL Algorithms Test--------------------------\n");

	long sum{ 0 };
	std::vector coll1{ 1,2,3,4,5,7,8 };
	auto MeanVal = std::for_each(coll1.begin(), coll1.end(), [&sum](auto&& val)
		{
			sum += val;
		});

	double meanValue = static_cast<double>(sum) / static_cast<double>(coll1.size());

	fmt::print("mean value: {0:.3f}\n", meanValue);

	std::list<int> coll2 = { 1,2,3,4,5,6,7,8,9,10 };

	int count{ 0 };
	/* if remove_if used to remove the 3rd element only then use counter by ref otherwise it removes the second 3rd element*/
	/* better solution to use find the value and only erase that one if the container has one and such funct is needed or this*/
	/* for a list if you want to erase 3rd element use erase and give the pos*/
	auto pos=std::remove_if(coll2.begin(), coll2.end(), [&count](auto&& elem)
	{
		/*return true if it is 3rd element*/
		return ++count == 3;
	
	});

	coll2.erase(pos, coll2.end());

	printCont(coll2);

	/* add the first value of coll2 to each element; for each return predicate function by using std::move*/
	auto pred = std::for_each(std::next(coll2.begin()), coll2.end(), [=](auto&& elem) { elem += *coll2.begin(); });
	printCont(coll2);

	const auto coll3 = coll2;

	/* std::count and count_if return the total number that matches the values given or predicate (returns bool)*/
	auto numofCount = std::count_if(coll3.cbegin(), coll3.cend(), [](auto&& elem) noexcept {return elem > 4; });
		
	fmt::print("number of Counts : {}\n", numofCount);
	printCont(coll3);

	std::list<int>coll4(10);
	std::iota(coll4.begin(), coll4.end(), 0);
	printCont(coll4);
	coll4.push_back(4);
	
#if __has_include(<ranges>) && _HAS_CXX20
	/* search not only the first found but beyond*/
	/* ranges only used for testing purposes; not any benefit here 
		unless you combine with other algorithms using c++20 features
	*/
	std::list<int>::iterator pos1;
	std::list<int>::iterator pos2;
	const int value = 4;
	/* find and find_if normally stop at the first match value or condition to make it continue you need an "if" or a "loop" to continue*/
	if (pos1 = std::ranges::find(coll4, value); pos1 != coll4.end())
	{
		pos2 = std::ranges::find(++pos1, coll4.end(), value);

	}

	fmt::print("first pos1: {}\n", *(--pos1));
	
	/* print only between to find value */
	if (pos1 != coll4.end() && pos2 != coll4.end())
	{
		std::ranges::copy(pos1, ++pos2, std::ostream_iterator<int>(std::cout," "));
	}
#endif

}


inline void STLAlgorithms_Test2()
{
	std::printf("\n---------------STL Algorithms Test2--------------------------\n");

	std::deque<int> coll1 = {1,2,7,7,6,3,9,7,7,7,7,7,6,25,40 };
	printCont(coll1);
	const int value = 7;
	const int count = 3;
	/*search_n gives you the for given count of consecutive elements
	  returns pos of first one; e.g. search 3 consecutive of 7's;
	  the search stops at first match
	*/
	auto pos1=std::search_n(coll1.begin(), coll1.end(), count, value);
	if (pos1 != coll1.end())
	{
		auto dist = std::distance(coll1.begin(), pos1) + 1;
		fmt::print("the first three consecutive of sevens is at {}\n", dist);
	}

	/* if passed a predicate the predicate has two inputs ; first is the element in collection, second is a value 
		but the passed value does not neccessarily to be used; predicate needs to return a bool
		e.g; search for 3 consecutive odd elements; (stops at first match)
	*/
	auto pos2 = std::search_n(coll1.begin(), coll1.end(), count, 0,
		[](auto&& elem, auto&& value) { return elem % 2 == 1; });

	if (pos2 != coll1.end())
	{
		auto dist2 = std::distance(coll1.begin(), pos2) + 1;
		fmt::print("the first three consecutive of odd elems is at {}\n", dist2);
	}

	std::deque<int>coll2 = {4,5, 1,2,3,4,5,6,7,8, 4,5,6 };
	std::deque<int>subColl = { 4,5,6 };

	/*Search stop on the 1st match ; if not match return end() of container / range*/
	//auto pos3 = std::search(coll2.begin(), coll2.end(),		// the source container to search
	//						subColl.begin(), subColl.end());	// sub range to be searched for


	//if (pos3 != coll2.end())
	//{
	//	auto dist = std::distance(coll2.begin(), pos3) + 1;
	//	fmt::print("the subrange (4,5,6) found at pos: {}\n", dist);

	//}
	
	
	/* 1ST ALTERNATIVE to search all matching sub ranges to search for 
		for string there are different search methods that can be used 
		also std::string member find() member function is faster for strings
	*/
	for (auto pos3 = std::search(coll2.begin(), coll2.end(),			// the source container to search
		subColl.begin(), subColl.end()); pos3 != coll2.end();
		pos3 = std::search(pos3 + subColl.size(), coll2.end(), subColl.begin(), subColl.end()))
	{
		auto dist = std::distance(coll2.begin(), pos3) + 1;
		fmt::print("the subrange (4,5,6) found at pos: {}\n", dist);
	}

	/* 2ND ALTERNATIVE using boyer_moore_search algorithm; used for big text data searchs
		this is the fastest search for bigger search texts; NOTE: Uses to much memory!!!
	*/
	std::boyer_moore_searcher bmsearch{ subColl.begin(), subColl.end() };
	for (auto pos3 = std::search(coll2.begin(), coll2.end(),			
		bmsearch); pos3 != coll2.end(); pos3 = std::search(pos3 + subColl.size(), coll2.end(), bmsearch))
	{
		auto dist = std::distance(coll2.begin(), pos3) + 1;
		fmt::print("the subrange  BOYER_MOORE SEARCH (4,5,6) found at pos: {}\n", dist);
	}

	std::vector<int> vec1 = { 1,2,3,4,5,6,7,8,9 };
	bool checkEvenArgs[3] = { true, false, true };
	printCont(vec1);
	
	auto pos4 = std::search(vec1.begin(), vec1.end(), checkEvenArgs, checkEvenArgs + std::size(checkEvenArgs),
		[](auto&& elem, bool cond)
		{
			if (cond == true)
			{
				return elem % 2 == 0;
			}
			else
			{
				return elem % 2 == 1;
			}

		});

	if (pos4 != vec1.end())
	{

		fmt::print("even: {0}, odd: {1}, even{2}\n", *pos4, *(pos4+1), *(pos4+2));
	}

	auto searchPred = [](auto&& elem, bool cond) 
	{
		if (cond == true)
		{
			return elem % 2 == 0;
		}
		else
		{
			return elem % 2 == 1;
		}

	};

	for (auto pos5 = std::search(vec1.begin(), vec1.end(), checkEvenArgs, checkEvenArgs + std::size(checkEvenArgs), searchPred); 
		pos5!= vec1.end();
		pos5= std::search(++pos5, vec1.end(), checkEvenArgs, checkEvenArgs + std::size(checkEvenArgs), searchPred))
	{
		auto dist = std::distance(vec1.begin(), pos5) + 1;
		fmt::print("found match (even, odd, even) at pos: {}\n", dist);

	}
}

/*TODO : Does not work ; LEarn How to do it!!!*/
template<typename Cont1, typename Cont2>
concept ContofEqualSize = requires(Cont1 c1, Cont2 c2) {
	(c1.size() == c2.size()); 

};

template<typename Cont1, typename Cont2>
requires requires(Cont1 c1, Cont2 c2)
{
	c1.size() == c2.size();
}
auto CheckCont(Cont1 coll1, Cont2 comp)
{
	return std::equal(coll1.begin(), coll1.end(), comp.begin());
}



inline void STLAlgorithms_Test3()
{

	std::printf("\n---------------STL Algorithms Test3--------------------------\n");

	std::vector<int> coll1 = { 1,2,3,4,5 };
	std::list<int>comp1 = { 1,2,3,4,5,6,7,8 };
	std::list<int>comp2 = { 1,2,3};


	/* equal check if all the elements match; the 2nd comparaision container must have at least same amount
		of elements otherwise it Undefined behavior might happen; need to put std::is_permutation concept 
		before using it !!!
		NOTE: the std::ranges::equal has a check if both container elements match min criteria
	*/
	auto result = std::equal(coll1.begin(), coll1.end(),comp1.begin());

	/* Concept trial but somehow not work; want to check if 2 containers have exact equal size*/
	auto rr = CheckCont(coll1, comp1);

	fmt::print("rr: {}\n", rr);
	
	fmt::print("result: {}\n", result);

#if _HAS_CXX20 || __has_include(<ranges>)
	/*std::Ranges::equal has a constraint to check if both containers comprable if 2nd container has less elements
		the function is not invoked and returns false immediately without causing undefined behaviour
	*/
	auto result2= std::ranges::equal(coll1, comp1);
	fmt::print("result2: {}\n", result2);


#endif	

	std::vector<char>source(10, '.');

	for (int c = 'a'; c <= 'f'; ++c)
	{
		source.push_back(c);
	}

	source.insert(source.end(), 10, '.');
	printCont(source);

	std::vector<char>c1(source.begin(), source.end());
	printCont(c1);

	/* copy all letters into same container but move them to front to index 7*/
	std::copy(c1.cbegin() + 10, c1.cbegin() + 16,  // source
		c1.begin() + 7);						   // dest
	printCont(c1);

	std::vector<char>c2(source.cbegin(), source.cend());

	/* copy the letter from source and copy into dest start from the back and dest.end() is passed*/
	std::copy_backward(source.cbegin() + 10, source.cbegin() + 16, c2.begin() + 19);
	printCont(c2);

	std::vector<std::string>coll2 = { "Hello", "this", "is", "an", "example" };
	std::list<std::string>coll3;

	std::copy(coll2.crbegin(), coll2.crend(),std::back_inserter(coll3));
	printCont(coll2);
	printCont(coll3);

	std::move_backward(coll2.begin(), coll2.end(), coll3.end());
	printCont(coll2); // no more elements will be displayed; moved
	printCont(coll3);
}


inline void STLAlgorithms_Test4()
{
	std::printf("\n---------------STL Algorithms Test 4--------------------------\n");

	std::vector<int> vec1 = { 1,2,3,4,5,6,7,8,9 };
	std::list<int> list1;

	printCont(vec1);

	std::transform(vec1.begin(), vec1.end(),
		vec1.begin(),
		[](auto&& elem) {return elem *= -1; });

	printCont(vec1);

	std::transform(vec1.cbegin(), vec1.cend(),
		std::back_inserter(list1), [](auto&& elem) {return elem * 10; });

	printCont(list1);

	/* remove && remove_if dont erase ; only removes to the back of container and gives the iterator pos of the
		removed elements start pos and need to use erase to delete elements from that pos to the end
		when using lambda/predicates prefer stateless lambdas (no internal mutabla member variable)
		or use by ref to increase a counter in case needed!!
	*/
	vec1.erase(std::remove_if(vec1.begin(), vec1.end(),
		[](auto&& elem) { return elem >= -4; }),
		vec1.end());

	printCont(vec1);

	std::vector<int> vec2(10);
	std::iota(vec2.begin(), vec2.end(), 1);
	printCont(vec2);

	std::rotate(vec2.begin(), vec2.begin()+1, vec2.end()-3);
	printCont(vec2);
}

