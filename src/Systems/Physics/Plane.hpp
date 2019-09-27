#pragma once

#include "Collider.hpp"

class Plane: public Collider
{
	public:
		
		Plane(  int entityID,
				glm::vec3 center,
				ColliderType colliderType,
				DynamicType dynamicType,
				std::vector<glm::vec3> points);
		~Plane();

		virtual void ComputeDerivedData();
		virtual void Update(glm::vec3 translation);

	private:

};