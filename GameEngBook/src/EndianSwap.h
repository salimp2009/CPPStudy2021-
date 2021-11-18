#pragma once



struct Example
{
	std::uint32_t m_a=0;
	std::uint16_t m_b=0;
	std::uint8_t m_c=0;
};

inline constexpr std::uint16_t swapInt16(std::uint16_t value)
{
	return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

inline constexpr std::uint32_t SwapInt32(std::uint32_t value)
{
	return ((value & 0x000000FF) << 24) 
		 | ((value & 0x0000FF00) << 8)
		 | ((value & 0x00FF0000) >> 8)
		 | ((value & 0xFF000000) >> 24);
}

/* Hold the value both in float and int32_t to do Endian Swapping*/
union U32F32
{
	std::uint32_t	m_asU32;
	float			m_asF32;
};

inline constexpr float swapF32(float value)
{
	U32F32 data;
	data.m_asF32 = value;

	/* Endian swap as integer*/
	data.m_asU32 = SwapInt32(data.m_asU32);

	/* return value as swap float*/
	return data.m_asF32;
}
