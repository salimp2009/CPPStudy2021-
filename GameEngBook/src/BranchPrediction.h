#pragma once




inline int SafeIntegerDivide(int a, int b, int defaultVal)
{
	return (b != 0 ? a / b : defaultVal);
}

union Data { std::uint32_t m_datau;  float m_dataf = 0.0f; };


inline float SafeFloatDivide_pred(float a, float b,float d)
{
	/* convert bool value of b != 0.0f into either 1U or 0U (unsigned int)*/
	const std::uint32_t condition = (std::uint32_t)(b != 0.0f);

	/*create mask depending on condition*/
	/* convert 1U -> 0xFFFFFFFFU */
	/* convert 0U -> 0x00000000U */
	/* 0U - condition is uequa to 0xFFFFFFFF if condition is 0x1U*/
	const std::uint32_t mask = 0U - condition;

	/*do the divison as float; if  b==0 result will be QNaN; that is why we used float*/
	const float q = a / b;
	Data qq;
	qq.m_dataf = q;
	
	Data dd;
	dd.m_dataf = d;
	
	Data rr;
	rr.m_datau = (qq.m_datau & mask) | (dd.m_datau & ~mask);
	
	return rr.m_dataf;
}
