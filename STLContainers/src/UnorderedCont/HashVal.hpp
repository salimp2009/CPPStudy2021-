#pragma once

// TODO: try to convert this in folding expression with variadic args
template<typename T>
constexpr void hash_combine(std::size_t& seed, const T& val) noexcept
{
	seed ^= std::hash<T>()(val)+0x9e3779b9 + (seed << 6) + (seed >> 2);
}


template< typename... Types>
constexpr void hash_val(std::size_t& seed, Types&&... args) noexcept
{
	(..., hash_combine(seed, std::forward<Types>(args)));
}

template<typename... Types>
constexpr std::size_t hash_val(Types&&... args) noexcept
{
	std::size_t seed = 0;
	hash_val(seed, std::forward<Types>(args)...);
	return seed;
}
