
#include <xmmintrin.h>

#include "test_rig.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <omp.h>
#include <random>
#include <vector>

#define SSE_WIDTH		4

#define ARR_SZ 1024

struct test_mem {
	float sum_final;
	float *a;
	float *b;
	int len;
	bool initialized;
} ;


void SimdMulSum(std::shared_ptr<void> mem)
{
	test_mem *data = static_cast<test_mem *>(mem.get());

	int len = data->len;

	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = data->a;
	register float *pb = data->b;

	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; i++ )
	{
		sum[0] += data->a[i] * data->b[i];
	}

	float final_sum = 0;
	final_sum += sum[0] + sum[1] + sum[2] + sum[3];

	data->sum_final = final_sum;
}

void mul_sum(std::shared_ptr<void> mem)
{
	test_mem *data = static_cast<test_mem *>(mem.get());

	int len = data->len;

	float sum = 0.0;

	for( int i = 0; i < data->len; i++ )
	{
		sum += data->a[i] * data->b[i];
	}

	data->sum_final = sum;
}

void v_init(std::shared_ptr<void> mem) {
	test_mem *data = static_cast<test_mem *>(mem.get());

	if(!data->initialized) {
		data->a = new float[data->len];
		data->b = new float[data->len];
		data->initialized = true;
	}

  	// construct a trivial random generator engine from a time-based seed:
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator (seed);

	std::uniform_real_distribution<float> a_dist(-1000, 1000);
	std::uniform_real_distribution<float> b_dist(-1000, 1000);

	for (int n = 0; n < data->len; n++) {
		data->a[n] = a_dist(generator);
		data->b[n] = b_dist(generator);
	}
};

int main(int argc, char *argv[]) {
	std::shared_ptr<test_mem> mem = std::make_shared<test_mem>();

	int arr_sz = ARR_SZ;

	if(argc >= 2) {
		arr_sz = std::stof(std::string(argv[1]));
	}

	mem->len = arr_sz;
	mem->initialized = false;
	  
	TestRig vsse(mem, SimdMulSum, v_init);
	TestRig mult(mem, mul_sum, v_init);

	vsse.Init(0);

	float simd_sum = 0;

	// Run 10 trials.
	for (int i = 0; i < 10; i++) {
		vsse.Run(static_cast<double>(mem->len));
		float simd_sum = mem->sum_final;
		mult.Run(static_cast<double>(mem->len));
	}

	// std::cout << "FINAL SUM: " << simd_sum << std::endl;
	// std::cout << "MegaMults/s: " << vsse.MaxPerf() << std::endl;

	// std::cout << "FINAL SUM: " << mem->sum_final << std::endl;
	// std::cout << "MegaMults/s: " << mult.MaxPerf() << std::endl;

	float speedup = vsse.MaxPerf() / mult.MaxPerf();

	if (argc >= 3 && std::string(argv[2]) == "-s") {
		std::cout << "Speedup = " << speedup << std::endl;

		std::cout << "Writing to records.csv..." << std::endl;

		std::ofstream outfile;
		outfile.open("records.csv", std::ios_base::app);

		// Setting the precision for output.
		outfile << std::fixed;
		outfile << std::setprecision(3);
		outfile << arr_sz << '\t' << speedup << std::endl;
		outfile.close();
	}

	delete [] mem->a;
	delete [] mem->b;
	
}
