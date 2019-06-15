#pragma once

#include <memory>

#include "Data.hpp"

enum MessageType
{
	Move = 0,
	MouseMove,
	MessageTypeEnd
};

class Message
{
	public:
		Message(int senderID, int receiverID, MessageType type);

		int 						senderID;
		int 						receiverID; // broadcast if receiver is 0
		MessageType					type;
		std::shared_ptr<Data>		data;

	private:
};