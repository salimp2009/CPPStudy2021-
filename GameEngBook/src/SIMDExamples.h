#pragma once

#include "GameEnginePCH.h"
#include <xmmintrin.h>


void AddArrays_sse(std::size_t count, float* results, const float* dataA, const float* dataB)
{

	/*size of all 3 arrays needs to match and a multiple of four*/
	assert(count % 4 == 0);

	for (auto i = 0; i < count; i += 4)
	{
		/* load the first 4 in each array and sums them up and loads the next 4 if any*/
		__m128 a = _mm_load_ps(&dataA[i]);
		__m128 b = _mm_load_ps(&dataB[i]);
		__m128 r = _mm_add_ps(a, b);
		/* every add of 4 values will be stored in array contigously ; 
			each element of the result array will result of added 4 values in given arrays
		*/
		_mm_store_ps(&results[i], r);
	}
}


inline void TestAddSSE()
{
	std::printf("\n---------------SIMExamples TestAddSSE------------------------\n");

	alignas(16) float A[4];
	alignas(16) float B[4] = { 2.0f, 4.0f, 6.0f, 8.0f };

	/* this is similar to seting a[] ={1,2,3,4}
		but SIMD _mm_set_ps() reads from reverse order
	*/
	__m128 a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
	__m128 b = _mm_load_ps(&B[0]);

	/* add vectors*/
	__m128 r=  _mm_add_ps(a, b);

	/* store the _mm128 a in an array in the  empty array*/
	_mm_store_ps(&A[0], a);

	alignas(16) float R[4];
	_mm_store_ps(&R[0], r);

	fmt::print("array A: {0}\n", fmt::join(A, ", "));
	fmt::print("array B: {0}\n", fmt::join(B, ", "));
	fmt::print("A+B (SIMD ops): {0}\n", fmt::join(R, ", "));

	alignas(16) float results[std::size(A)];
	AddArrays_sse(std::size(A), results, A, B);
	fmt::print("array results: {0}\n", fmt::join(results, ", "));
}


/* NON SIMD DOT ARRAY*/
void DotArrays_ref(std::size_t count, float result[], const float a[], const float b[])
{
	for (auto i = 0; i < count; ++i)
	{
		const int j = i * 4;
		result[i] = a[j + 0] * b[j + 0]		  // ax*bx
			+ a[j + 1] * b[j + 1]  // ay*by
			+ a[j + 2] * b[j + 2]  // az*bz
			+ a[j + 3] * b[j + 3];  // aw*bw
	}
}

/* Slow version SSE Horizontal add version*/
void DotArrays_sse_horizontal(std::size_t count, float result[], const float a[], const float b[])
{
	for (auto i = 0; i < count; ++i)
	{
		const int j = i * 4;
		/* each four block of element as a single sse elements*/
		__m128 va = _mm_load_ps(&a[j]); // ax, ay, az, aw 
		__m128 vb = _mm_load_ps(&b[j]); // bx, by, bz, bw 

		__m128 v0 = _mm_mul_ps(va, vb);

		/*add across the registers twice which makes it slower than a regular for loop*/
		__m128 v1 = _mm_hadd_ps(v0, v0); // adds vw + vz and vx + vy in two pair into 4 slots; z+w, x+y, z+w, x+y
		__m128 vr = _mm_hadd_ps(v1, v1); // doing twice gets us sum of all x, y,z,w but in the 4 slots and it has to do 2 operations

		/* extract only the vr.x components into first ith element in result*/
		_mm_store_ss(&result[i], vr);
	}
}

/* Better SIMD DotArray version*/
void DotArrays_sse(std::size_t count, float finalresult[], const float a[], const float b[])
{
	for (auto i = 0; i < count; i += 4)
	{
		__m128 vaX = _mm_load_ps(&a[(i+0) * 4]); //a[0,4, 8, 12]
		__m128 vaY = _mm_load_ps(&a[(i+1) * 4]); //a[1,5, 9, 12]
		__m128 vaZ = _mm_load_ps(&a[(i+2) * 4]); //a[2,6, 8, 12]
		__m128 vaW = _mm_load_ps(&a[(i+3) * 4]); //a[3,7, 8, 12]

		__m128 vbX = _mm_load_ps(&b[(i + 0) * 4]); //b[0,4, 8, 12]
		__m128 vbY = _mm_load_ps(&b[(i + 1) * 4]); //b[1,5, 9, 12]
		__m128 vbZ = _mm_load_ps(&b[(i + 2) * 4]); //b[2,6, 8, 12]
		__m128 vbW = _mm_load_ps(&b[(i + 3) * 4]); //b[3,7, 8, 12]

		__m128 result;
		result = _mm_mul_ps(vaX, vbX);
		result = _mm_add_ps(result, _mm_mul_ps(vaY, vbY));
		result = _mm_add_ps(result, _mm_mul_ps(vaZ, vbZ));
		result = _mm_add_ps(result, _mm_mul_ps(vaW, vbW));

		_mm_store_ps(&finalresult[i], result);
	}
}

void DotArrays_sse_transpose(std::size_t count, float finalresult[], const float a[], const float b[])
{
	for (auto i = 0; i < count; i += 4)
	{
		__m128 vaX = _mm_load_ps(&a[(i + 0) * 4]); 
		__m128 vaY = _mm_load_ps(&a[(i + 1) * 4]); 
		__m128 vaZ = _mm_load_ps(&a[(i + 2) * 4]); 
		__m128 vaW = _mm_load_ps(&a[(i + 3) * 4]); 


		__m128 vbX = _mm_load_ps(&b[(i + 0) * 4]); 
		__m128 vbY = _mm_load_ps(&b[(i + 1) * 4]); 
		__m128 vbZ = _mm_load_ps(&b[(i + 2) * 4]); 
		__m128 vbW = _mm_load_ps(&b[(i + 3) * 4]); 

		_MM_TRANSPOSE4_PS(vaX, vaY, vaZ, vaW);
		_MM_TRANSPOSE4_PS(vbX, vbY, vbZ, vbW);

		__m128 result;
		result = _mm_mul_ps(vaX, vbX);
		result = _mm_add_ps(result, _mm_mul_ps(vaY, vbY));
		result = _mm_add_ps(result, _mm_mul_ps(vaZ, vbZ));
		result = _mm_add_ps(result, _mm_mul_ps(vaW, vbW));

		_mm_store_ps(&finalresult[i], result);
	}
}


inline void TestDotArraySSE()
{
	std::printf("\n---SIMD Dot Array Test---\n");

	alignas(16) float A[4]=  { 1.0f, 2.0f, 3.0f, 4.0f };
	alignas(16) float B[4] = {  2.0f, 4.0f, 6.0f, 8.0f};
	alignas(16) float results[1];
	DotArrays_ref(1, results, A, B);
	fmt::print("DotArray for-loop results: {0}\n", fmt::join(results, ", "));
	DotArrays_sse_horizontal(1, results, A, B);
	fmt::print("DotArray SIMD slow: {0}\n", fmt::join(results, ", "));

	/* store the values as transposed but no need since there is Sse function to transpose*/
	alignas(16) float AA[4*4] = { 
		0.0f, 4.0f, 8.0f, 12.0f,
		1.0f, 5.0f, 9.0f, 13.0f,
		2.0f, 6.0f, 10.0f, 14.0f,
		3.0f, 7.0f, 11.0f, 15.0f };

	alignas(16) float BB[4 * 4] = {
		0.0f, 4.0f, 8.0f, 12.0f,
		1.0f, 5.0f, 9.0f, 13.0f,
		2.0f, 6.0f, 10.0f, 14.0f,
		3.0f, 7.0f, 11.0f, 15.0f };

	alignas(16) float finalresults[std::extent_v<decltype(AA)> / 4];
	const std::size_t count = std::size(finalresults) / 4;

	DotArrays_sse(count, finalresults, AA, BB);
	fmt::print("DotArray SIMD faster (stored transposed): {0}\n", fmt::join(finalresults, ", "));

	alignas(16) float AAA[4 * 4] = {
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		12.0f, 13.0f, 14.0f, 15.0f };

	alignas(16) float BBB[4 * 4] = {
		0.0f, 1.0f, 2.0f, 3.0f,
		4.0f, 5.0f, 6.0f, 7.0f,
		8.0f, 9.0f, 10.0f, 11.0f,
		12.0f, 13.0f, 14.0f, 15.0f };

	DotArrays_sse_transpose(count, finalresults, AAA, BBB);
	fmt::print("DotArray SIMD transpose: {0}\n", fmt::join(finalresults, ", "));

}
