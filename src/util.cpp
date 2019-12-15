#include "util.hpp"
#include <iostream>

void printVector(glm::vec3 vector, std::string name)
{
	std::cout << name << ": x = " << vector.x << " y = " << vector.y << " z = " << vector.z << std::endl;
}

void print2DVector(std::vector<glm::vec3>& vector, std::string name)
{
	std::cout << "------- 2D " << name << "-------" << std::endl;
	for (int i = 0; i < vector.size(); ++i)
	{
		printVector(vector[i]);
	}
	std::cout << "---------------------" << std::endl;
}