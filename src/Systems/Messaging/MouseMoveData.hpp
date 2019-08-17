#pragma once
#include "Data.hpp"

class MouseMoveData : public Data
{
	public:
		MouseMoveData(float deltaX, float deltaY);

		float deltaX;
		float deltaY;
	private:
};