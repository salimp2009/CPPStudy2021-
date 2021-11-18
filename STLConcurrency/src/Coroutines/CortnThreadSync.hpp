#pragma once
#include "ConcurrencyPCH.h"


class Event
{
public:
	Event() = default;
	Event(const Event&) = delete;
	Event(Event&&) = delete;
	Event& operator=(const Event&) = delete;
	Event& operator=(Event&&) = delete;

	class Awaiter;
	Awaiter operator co_await() const noexcept;

	void notify() noexcept;

private:
	friend class Awaiter;
	mutable std::atomic<void*> suspendedWaiter{ nullptr };
	mutable std::atomic<bool> notified{ false };
};

class Event::Awaiter
{
public:
	Awaiter(const Event& eve) : event{ eve } {}
	bool await_ready() const;
	bool await_suspend(std::coroutine_handle<> corHandle) noexcept;
	void await_resume() noexcept {}

private:
	friend class Event;
	const Event& event;
	std::coroutine_handle<> coroutineHandle;
};


bool Event::Awaiter::await_ready() const
{
	// TODO: Check if we can use even.notified.load(std::memory_order_acquire)
	if (event.suspendedWaiter.load() != nullptr)
	{
		throw std::runtime_error("more than one waiter is not valid!!\n");
	}
	// event.notified==false; coroutine is suspended
	// event.notified==true; coroutine is executed like a normal function
	// TODO: Check if we can use even.notified.load(std::memory_order_release)
	std::printf("debug:await readY!\n");

	return event.notified;
}


void Event::notify() noexcept
{
	//notified.store(true, std::memory_order_release);
	notified = true;
	auto* waiter = static_cast<Awaiter*>(suspendedWaiter.load());

	// if a waiter is available then resume it
	if (waiter != nullptr)
	{
		std::printf("debug: Notifying!\n");

		// this will call await_resume
		waiter->coroutineHandle.resume();
	}
}

bool Event::Awaiter::await_suspend(std::coroutine_handle<> corHandle) noexcept
{
	coroutineHandle = corHandle;

	if (event.notified)
	{
		return false;
	}
	std::printf("debug: await_suspend!\n");
	// store the waiter to notify it !
	// TODO; check if this may have std::memory_order_acquire!!
	event.suspendedWaiter.store(this);
	return true;
}


Event::Awaiter Event::operator co_await() const noexcept
{
	std::printf("debug:co_await!\n");
	return Awaiter{ *this };
}

// this is generator and hold the promise_type in it
struct Task
{
	struct promise_type
	{
		Task get_return_object() { std::printf("debug: Promise: get_return object: !\n"); return {}; }
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void return_void() { }
		void unhandled_exception() { }
	};
};

Task receiver(Event& event)
{
	auto start = std::chrono::high_resolution_clock::now();
	co_await event;
	std::printf("got the notification\n");
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsedTime = std::chrono::duration<double>(end - start);
	//std::printf("Waited: %f seconds\n", elapsedTime.count());
	fmt::print("Waited: {} seconds\n", elapsedTime.count());
}

using namespace std::chrono_literals;

inline void CortnThreadSync_Example()
{
	std::printf("\n--CortnThreadSyn--\n");
	std::printf("Notification Before Waiting!\n");

	Event event1{};
	auto senderThread1   = std::thread([&event1] {event1.notify(); });  // notifiy the awaiter
	auto receiverThread1 = std::thread(receiver, std::ref(event1));		

	receiverThread1.join();
	senderThread1.join();
	
	std::printf("\n");

	std::printf("Notification Before Waiting!\n");

	Event event2{};
	auto receiverThread2 = std::thread(receiver, std::ref(event2));
	auto senderThread2 = std::thread([&event2] 
		{
			std::this_thread::sleep_for(1s);
			event2.notify(); 
		});  // notifiy the awaiter

	receiverThread2.join();
	senderThread2.join();


}