#pragma once

// typical aggregate initilazation; 
// no user defined constructor or inherited 
// no virtual members functions
// no private or protected non-static data members
// no private virtual, private or protected base classes; only public base classes
#include <cerrno>


struct Point2D
{
	int x;
	int y;
};

class Point3D
{
public:
	int x;
	int y;
	int z;
};

struct NamedPoint
{
	std::string name;
	Point3D pt;
};

struct NonCopyableorMovable
{
	NonCopyableorMovable() = default;
	NonCopyableorMovable(const NonCopyableorMovable&) = delete;
	NonCopyableorMovable(NonCopyableorMovable&&) = delete;

	NonCopyableorMovable& operator==(const NonCopyableorMovable&) = delete;
	NonCopyableorMovable& operator==(NonCopyableorMovable&&) = delete;
	~NonCopyableorMovable() = default;

	int x = 2;
};

// testing Return Value Optimization for NonCopyorMovable class
NonCopyableorMovable RVOfunc()
{
	return {};
}

inline auto Use()
{
	// Since RVOfunc returns a nonmovable or copyable type normally it should fails
	// but compiler creates this not on the stack ; right at the call stack so it does not move or copy it and compiles
	auto myVal = RVOfunc();
	fmt::print("Use creates NonCopyableorMovable x: {}\n", myVal.x);
	return RVOfunc();
}

inline auto GetVectorPoint3d()
{
	std::vector<Point3D> vec3d;

	vec3d.emplace_back(Point3D{ .x = 10, .y = 20, .z = 30 });
	vec3d.emplace_back(Point3D{ .y = 20, .z = 30 });
	vec3d.emplace_back(15, 25, 35);

	return vec3d;
}


struct FileAccessParameters
{
	bool open;
	bool close;
	bool readonly;
};

inline void FileAccess(const FileAccessParameters& params)
{
	fmt::print("open: {}, close: {}, readonly: {}\n", params.open, params.close, params.readonly);
}

struct  Point
{
	int X;
	int y;
};

inline void AddToPoint(Point2D point, int value)
{

	fmt::print("Add Point2D: result: x: {}, y:{}\n", point.x + value, point.y + value);
}

inline void AddToPoint(Point point, int value)
{
	fmt::print("Add Point; result: x: {}, y:{}\n", point.X + value, point.y + value);
}

struct Nested
{
	int i;
	Point mpoint;
};


struct LifeTimeExtension
{
	int&& r;
};

struct NotDefaultConstructible
{
	int x;
private:
	NotDefaultConstructible() = delete;
};


// Used for an POSIX file open system 
// errno is a preprocessor macro used for error indication. see https://en.cppreference.com/w/cpp/error/errno
int open(const char*)
{
	return 1;
}

template<typename T>
struct ReturnCode
{
	T returnCode;
	int err;
};

// Deduction guide needed in C++17
// But C++20 can deduce type with CTAD for Aggregates as well so no need
#if !  _HAS_CXX20
	template<typename T>
	ReturnCode(T, int)->ReturnCode<int>;
#endif 


auto OpenFile(const char* fileName)
{
	return ReturnCode{ open(fileName), errno};
}