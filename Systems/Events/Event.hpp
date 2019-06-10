#pragma once

#include <memory>

enum EventType
{
	Move,
	MouseMove,
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