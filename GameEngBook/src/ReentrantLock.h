#pragma once

class ReentrantLock32
{
private:
	std::atomic<std::size_t> m_atomic;
	std::int32_t			 m_refCount;
public:
	ReentrantLock32() noexcept : m_atomic{ 0 }, m_refCount{ 0 } {}

	void Acquire() noexcept
	{
		std::hash<std::thread::id> hasher;
		std::size_t tid = hasher(std::this_thread::get_id());

		/*if this thread does not hold the lock..*/
		if(m_atomic.load(std::memory_order_relaxed)!=tid)
		{ 
			/* wait until we hold the lock and check it!!*/
			std::size_t unlockValueExpected = 0;
			while (!m_atomic.compare_exchange_weak(unlockValueExpected, tid,
				    std::memory_order_relaxed,
				    std::memory_order_relaxed))  // relaxed used because there is a fence below protecting the loads
			{
				unlockValueExpected = 0;
				std::this_thread::yield();
			}
		}
		/*we get the lock; increase ref_counter; protected by the fence below!!*/
		/* m_refCount will be used by both Acquire and Release*/
		++m_refCount;
		/* all reads above will be protected and cannot be reordered; safe!!*/
		std::atomic_thread_fence(std::memory_order_acquire);
	}


	void Release()
	{
		std::atomic_thread_fence(std::memory_order_release);
		
		std::hash<std::thread::id> hasher;
		std::size_t tid = hasher(std::this_thread::get_id());

		/*Note: check if this load is safe; possibly safe since release and acquire synchronizes
			no other thread can get the lock if this one releases !!!!
		*/
		std::size_t actualThreadID = m_atomic.load(std::memory_order_relaxed);
		assert(actualThreadID == tid);

		--m_refCount;
		if (m_refCount == 0)
		{
			/* release lock safely since we own it*/
			m_atomic.store(0, std::memory_order_relaxed);
		}
	}

	bool TryAcquire() noexcept
	{
		std::hash<std::thread::id> hasher;
		std::size_t tid = hasher(std::this_thread::get_id());

		bool acquired = false;
		if (m_atomic.load(std::memory_order_relaxed) == tid)
		{
			acquired = true;
		}
		else
		{
			std::size_t unlockValueExpected = 0;
			acquired = m_atomic.compare_exchange_strong(unlockValueExpected, tid,
				std::memory_order_relaxed,
				std::memory_order_relaxed); // relaxed used because there is a fence below protecting the loads
		}

		if (acquired)
		{
			++m_refCount;
			std::atomic_thread_fence(std::memory_order_acquire);
		}

		return acquired;
	}

};  // end of ReentrantLock class
