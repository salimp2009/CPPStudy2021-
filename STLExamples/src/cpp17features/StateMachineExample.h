#pragma once
#include <variant>

struct DoorOpened {};
struct DoorClosed {};
struct DoorLocked {};

using State = std::variant<DoorOpened, DoorClosed, DoorLocked>;

struct mOpenEvent
{
	State operator()(const DoorOpened&) { return DoorOpened(); }
	State operator()(const DoorClosed&) { return DoorOpened(); }
	/* cannot open locked door*/
	State operator()(const DoorLocked&) { return DoorLocked(); }
};

struct mCloseEvent
{
	State operator()(const DoorOpened&) { return DoorClosed(); }
	State operator()(const DoorClosed&) { return DoorClosed(); }
	State operator()(const DoorLocked&) { return DoorLocked(); }
};

struct mLockEvent
{
	/* cannot lock open doors!*/
	State operator()(const DoorOpened&) { return DoorOpened(); }
	State operator()(const DoorClosed&) { return DoorLocked(); }
	State operator()(const DoorLocked&) { return DoorLocked(); }
};

struct mUnLockEvent
{
	/* cannot unlock open doors!*/
	State operator()(const DoorOpened&) { return DoorOpened(); }
	/* already unnlocked!*/
	State operator()(const DoorClosed&) { return DoorClosed(); }
	/* unlock but the door is closed!*/
	State operator()(const DoorLocked&) { return DoorClosed(); }
};


	struct DoorState
	{
		
		void open()
		{
			m_state = std::visit(mOpenEvent{}, m_state);
		}

		void close()
		{
			m_state = std::visit(mCloseEvent{}, m_state);
		}

		void lock()
		{
			m_state = std::visit(mLockEvent{}, m_state);
		}

		void unlock()
		{
			m_state = std::visit(mUnLockEvent{}, m_state);
		}

		State m_state;
	};