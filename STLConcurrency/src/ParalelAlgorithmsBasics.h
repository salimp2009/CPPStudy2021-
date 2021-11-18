#pragma once

inline void SortParallel_Basics()
{
	std::printf("\n-----SortParallel-----\n");

	std::vector<int> vec1 = { 1,2,3,4,5,6,7,8,9 };

	std::sort(vec1.begin(), vec1.end());

	std::sort(std::execution::par, vec1.begin(), vec1.end());

	/* VS implentation of std::execution::par_unseq is same as std::execution::par*/
	std::sort(std::execution::par_unseq, vec1.begin(), vec1.end());
	printCont(vec1);

	
	std::cout << std::is_execution_policy_v<float> << '\n';

}

inline void ForEachParallel_Basics()
{
	std::printf("\n-----ForEachParallel-----\n");

	std::vector<int> vec1 = { 1,2,3,4,5,6,7,8,9 };
	std::atomic<int> sum{ 0 };
	
//	std::for_each(std::execution::par, vec1.begin(), vec1.end(), [&](auto&& elem) {sum += elem+elem; });
	std::for_each(std::execution::par_unseq, vec1.begin(), vec1.end(), [&](auto&& elem) 
		{
			sum += elem + elem; 
			std::cout<<std::this_thread::get_id()<<'\n';
		
		});

	printCont(vec1);

	fmt::print("sum: {}\n", sum);

}

inline void TransformParallel_Basics(std::size_t vectorSize= 4'000'000)
{
	std::printf("\n-----Transform Parallel-----\n");

	const std::size_t vecSize = vectorSize;
	fmt::print("vector size: {}\n", vecSize);
	std::vector<double>vec(vecSize, 0.5);
	std::vector out(vec);

	RunAndMeasure("std::transform seq", [&vec, &out]()
		{
			std::transform(std::execution::seq, vec.begin(), vec.end(), out.begin(), 
				[](auto&& elem) 
				{
					return std::sin(elem) * std::cos(elem); 
				});

			return out.size();
		});

	RunAndMeasure("std::transform par", [&vec, &out]()
		{
			std::transform(std::execution::par, vec.begin(), vec.end(), out.begin(),
				[](auto&& elem)
				{
					return std::sin(elem) * std::cos(elem);
				});

			return out.size();
		});


}

inline void AccumulateReduce_Basics(std::size_t vectorSize = 4'000'000)
{
	std::printf("\n----- Accumulate Seq vs Reduce Parallel----\n");

	const std::size_t vecSize = vectorSize;
	fmt::print("vector size: {}\n", vecSize);
	std::vector<double>vec(vecSize, 0.5);
	
	RunAndMeasure("std::accumulate seq", [&vec]()
		{
			 return std::accumulate(vec.begin(), vec.end(), 0.0);
		});

	RunAndMeasure("std::reduce seq", [&vec]()
		{
			return std::reduce(std::execution::seq, vec.begin(), vec.end(), 0.0);
		});

	RunAndMeasure("std::reduce par", [&vec]()
		{
			return std::reduce(std::execution::par, vec.begin(), vec.end(), 0.0);
		});


}






