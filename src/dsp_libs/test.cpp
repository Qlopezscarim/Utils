#include "time_filters.hpp"
#include <iostream>
int main()
{
	std::vector<uint32_t> test_vector_1 = {1,2,3,4,5};
	std::vector<double> test_vector_2 = {0.25,0.5,0.25};
	
	//Test 1: verify naive result
	auto result = Qnaive_time_impl(test_vector_1, test_vector_2);

	for (auto value: result)
	{
		std::cout << value << std::endl;
	}
	
	
	std::cout << "Test run finished" << std::endl;
}
