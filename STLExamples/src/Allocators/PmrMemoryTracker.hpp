#pragma once

#include "STLpch.h"

class Tracker : public std::pmr::memory_resource
{
private:
	std::pmr::memory_resource* upstream;	
	std::string prefix{};

public:
	/*wrapper for the upstream resource object*/
	explicit Tracker(std::pmr::memory_resource* us=std::pmr::get_default_resource()): upstream{us} 
	{
	}

	explicit Tracker(std::string p, std::pmr::memory_resource* us = std::pmr::get_default_resource()) : prefix{ std::move(p) }, upstream{ us }
	{
	}

private:
	virtual void* do_allocate(size_t bytes, size_t alignment) override
	{
		std::printf("%s allocate %zu Bytes\n", prefix.c_str(), bytes);
		void* up =upstream->allocate(bytes, alignment);
		return up;
	}

	virtual void do_deallocate(void* ptr, size_t bytes, size_t alignment)override
	{
		std::printf("%s de-allocate %zu Bytes\n", prefix.c_str(), bytes);
		upstream->deallocate(ptr, bytes, alignment);
	}
	
	virtual bool do_is_equal(const memory_resource& other) const noexcept override
	{
		if (this == &other) return true;

		auto op = dynamic_cast<const Tracker*>(&other);

		return op != nullptr && op->prefix == prefix && upstream->is_equal(other);
	}

};