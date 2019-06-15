#pragma once

#include "Data.hpp"

class MoveData : public Data
{
	public:

		MoveData(bool forward, bool backward, bool left, bool right);

		bool forward;
		bool backward;
		bool left;
		bool right;
};