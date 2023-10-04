#include <iostream>
#include <string>
#include <omp.h> 
#include <stdio.h> 
#include <vector>

int main(int argc, char** argv) //argv[0] - количество чисел, argv[1] - количество потоков
{
	#pragma serial
	{
		omp_set_num_threads(std::stoi(argv[1]));
	}

	std::vector<int> vec;
	int n = std::stoi(argv[0]);
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
			printf("%d ", vec[i]);
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
			printf("%d ", vec[i]);
	}
	
	return 0;
}