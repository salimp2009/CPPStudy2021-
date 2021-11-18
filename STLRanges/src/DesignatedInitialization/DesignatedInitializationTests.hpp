#pragma once
#include "RangesPCH.hpp"
#include "AggregateInitiliazation.hpp"

inline void AggregateIinitialization()
{
	std::printf("\n-AggregateIinitialization-\n");

	Point2D point2dOne{ .x = 20, .y = 45 };

	Point3D point3dOne{ 30, 55,15 };

	fmt::print(" point2dOne: x:{}, y: {}\n", point2dOne.x, point2dOne.y);
	fmt::print(" point3dOne: x:{}, y: {}, z:{}\n", point3dOne.x, point3dOne.y, point3dOne.z);

	NamedPoint namedpoint1{ .name{"start"}, .pt{0, 12, 15} };
	NamedPoint namedpoint2{ .name{"start"}, .pt{.x = 20, .y = 15, .z = 45 } };

	fmt::print(" namedpoint1; name: {0}, Point: [{1}, {2}, {3}]\n", namedpoint1.name, namedpoint1.pt.x, namedpoint1.pt.y, namedpoint1.pt.z);
	fmt::print(" namedpoint2; name: {0}, Point: [{1}, {2}, {3}]\n", namedpoint2.name, namedpoint2.pt.x, namedpoint2.pt.y, namedpoint2.pt.z);

	// Brace initialization ensure members are default initialized; in this case all zero
	// Brace initiliazation also prevents narrowing
	// Brace initialization also prevent vertex parsing; e.g : const Point3 pt3(); is seen as a function that return const Point3 with no arguments
	Point3D pt3DTwo{};
	fmt::print(" pt3DTwo: x:{}, y: {}, z:{}\n", pt3DTwo.x, pt3DTwo.y, pt3DTwo.z);

	// testing RVO for NonCopy or Movable class

}

inline void ReturnValueOptimization()
{
	std::printf("\n- ReturnValueOptimization-\n");
	auto myVal = Use();
	myVal.x = 25;

	fmt::print("Test call site Use creates NonCopyableorMovable x: {}\n", myVal.x);

	auto resVec = GetVectorPoint3d();
	std::puts("vec3d: ");
	for (const auto& Point : resVec)
	{
		fmt::print("[{}, {}, {}]\n", Point.x, Point.y, Point.z);
	}

	FileAccess({ .open = true });
	
	AddToPoint({.X=20}, 10);
	AddToPoint({.x=20,}, 10);
	//AddToPoint({ .y = 15 }, 10); // will not compile; it is ambiguous both Point and Point2D has .y but .X vs .x makes the difference

}


inline void DirectInitializeAggregates()
{
	std::printf("\n- DirectInitializeAggregates-\n");
	// this works in c++20 but not in c++17;
	// in c++17 you have to make ; std::make_unique<Point>(Point2D{20, 40});
	// This works as if braced initialized except it allows narrawing
	auto ptrPoint = std::make_unique<Point>(20, 40);

	// this compiled but a smart compiler gives warning for conversion / loss of data!
	//auto ptrPoint2 = std::make_unique<Point>(20.5, 40);

	Nested mynested{ 20, {20, 15} };
	Nested mynested2{ .i = 25, .mpoint{.X = 20} };

	int bArray[]{ 20,30,40 };
	int bArray2[](20, 30, 40); // since c++20

	LifeTimeExtension bTemporary{ 55 };	 // Not sure why this is OK not Dangling !!!!
	LifeTimeExtension pTemporary(20);    // Dangling reference 
	fmt::print("bTemp: {}, pTemp: {}\n", bTemporary.r, pTemporary.r);

	const auto& val = LifeTimeExtension(20); // extended lifetime ; no more dangling
	fmt::print("val: {}\n", val.r);
}

inline void AggregatesWithDefaultConstrctor()
{
	std::printf("\n-AggregatesWithDefaultConstrctor-\n");
	// This compiles in C++17 but not in C++20

#if ! _HAS_CXX20
	NotDefaultConstructible nt1{};
#endif //  _HAS_CXX20

	 
}

inline void AggregatesAndCTAD()
{
	std::printf("\n-AggregatesAndCTAD-\n");

	auto [returntype, err] = OpenFile("mytext.txt");
	fmt::print("returntype: {}, err: {}\n", returntype, err);
}




