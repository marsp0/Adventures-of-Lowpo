#include "util.hpp"
#include <iostream>

void printVector(glm::vec3 vector, std::string name)
{
	std::cout << name << ": x = " << vector.x << " y = " << vector.y << " z = " << vector.z << std::endl;
}