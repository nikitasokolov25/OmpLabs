#include <iostream>
#include <string>
#include <omp.h> 
#include <vector>

int main()
{
	int countThreads;
	std::cout << "Count of threads: ";
	std::cin >> countThreads;
	
	#pragma serial
	{
		try
		{
			if (countThreads < 1)
			{
				std::cerr << "Number of threads should be a positive number\n";
				return 2;
			}
			omp_set_num_threads(countThreads);
		}
		catch (...)
		{
			std::cerr << "Number or threads should be a number\n";
			return 3;
		}
	}

	std::vector<int> vec;
	int n;
	std::cout << "Count of numbers: ";
	std::cin >> n;
	int x;
	for (int i = 0; i < n; i++)
	{
		std::cin >> x;
		vec.push_back(x);
	}
	#pragma omp parallel for
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == 1 || vec[i] == 2 || vec[i] == 3)
		{
			std::cout << vec[i] << " ";
			continue;
		}
		bool f = true;
		for (int j = 2; j < std::ceil(sqrt(vec[i])) + 1; j++)
		{
			if (vec[i] % j == 0)
			{
				f = false;
				break;
			}
		}
		if (f == true)
			std::cout << vec[i] << " ";
	}
	
	return 0;
}
