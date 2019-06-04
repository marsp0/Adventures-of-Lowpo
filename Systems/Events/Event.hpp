#pragma once

#include <memory>

enum EventType
{
	PLAYER_MOVED,
};

class Event
{
	public:
		Event();

		int 						senderID;
		int 						receiverID; // broadcast if receiver is 0
		EventType 					type;
		std::shared_ptr<Data>		data;

	private:
};