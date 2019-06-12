#pragma once

#include <memory>

#include "Data.hpp"

enum EventType
{
	Move = 0,
	MouseMove,
	EventTypeEnd
};

class Event
{
	public:
		Event(int senderID, int receiverID, EventType type);

		int 						senderID;
		int 						receiverID; // broadcast if receiver is 0
		EventType 					type;
		std::shared_ptr<Data>		data;

	private:
};