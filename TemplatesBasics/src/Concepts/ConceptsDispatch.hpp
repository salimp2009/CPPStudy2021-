#pragma once
#include "Templpch.h"
#include <span>
/* Example Usage of Concepts*/

/* e.g; data packet/buffer for small size data packs over TCP/IP*/
/* ack = acknowledgement; limit 10 ack packs (requirements for the example);
	will be queued to 10 packets 
*/
//using AckType = std::array<char, 16>;

/* e.g; data packet/buffer for bigger data packs over TCP/IP
	will be send immediately
*/

//std::array<char, 100> payLoadData;

template<typename T>
concept ByteLikeType = std::is_same_v<char, T> || std::is_same_v<unsigned char, T>
|| std::is_same_v<const char, T> || std::is_same_v<const unsigned char, T>;

/* Array has a value_type alias, since we will pass a container for the buffer 
  we want to access the underlying type e.g.; array<int..> or vector<int> ...; int is the underlying value_type
  */
template<typename T>
using value_type_t = typename std::remove_reference_t<T>::value_type;

template<typename T>
constexpr auto ExtractSize(T t = {})
{
	return t.size();
}

/* we can use t.size() function in concept or a requires-expression directly since the size is evaluted when the object is created
   therefore we write a wrapper that evaluates the size of an array since it is constexpr and we can use that in requires or concepts
   the only requirement for the type passed in wrapper has to have  default constructor T()
*/
template<typename T>
concept SmallBuffer = ExtractSize<std::remove_reference_t<T>>() <= 16 && ByteLikeType<value_type_t<T>>;

template<typename T>
concept LargeBuffer = not SmallBuffer<T> && ByteLikeType<value_type_t<T>>;

using AckType = std::array<char, 16>;

/*TODO: Check if this is OK to use!*/
//using AckType = std::span<char, 16>;

template<typename T>
using TimedQueue = std::queue<T>;

static TimedQueue<AckType> gAckQueue;

/* Actual send thru the network; the Send() is a constraint buffer to either sendTCP or put it in a queue
	based on the size of the buffer !
*/
void sendTCP(const char* data, const std::size_t len) { std::printf("Sending the buffer!\n"); }

template<typename T>
void PushToAckQueue(T& buffer)
{
	gAckQueue.push(buffer);
	std::printf("Adding buffer to AckQueue!\n");
}

void FlushAckQueue()
{
	while (gAckQueue.size())
	{
		const auto buf = gAckQueue.front();
		sendTCP(buf.data(), buf.size());
	}
}


template<typename T>
requires SmallBuffer<T>
void Send(T&& buffer)
{
	if (gAckQueue.size() > 10)
	{
		FlushAckQueue();
	}
	else
	{
		PushToAckQueue(buffer);
	}
}

template<typename T>
requires LargeBuffer<T>
void Send(T&& buffer)
{
	sendTCP(buffer.data(), buffer.size());
}

