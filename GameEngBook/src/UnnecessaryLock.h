#pragma once

/* this is class to test if a lock is neccessary for any given critical operation*/

class UnnecessaryLock
{
private:
	volatile bool	m_Locked;
public:
	UnnecessaryLock() :m_Locked{ false } {}

	void Acquire()
	{
		/*check no one has the lock!*/
		assert(!m_Locked);
		/* lock it so we can detect overlapping occurs; if another thread tries to acquire 
		 the assert above will notify us
		 */
		m_Locked = true;
	}

	void Release()
	{
		/*if any thread to access release without locking then assert will fire*/
		assert(m_Locked);

		/*unlock*/
		m_Locked = false;
	}

};

class UnnecessaryLockJanitor
{
private:
	UnnecessaryLock* m_pLock;
public:
	UnnecessaryLockJanitor(UnnecessaryLock& lock) : m_pLock{ &lock }
	{
		m_pLock->Acquire();
	}
	~UnnecessaryLockJanitor() { m_pLock->Release(); }

};

#if _DEBUG
	#define BEGIN_ASSERT_LOCK_NOT_NECESSARY(L) (L).Acquire()
	#define END_ASSERT_LOCK_NOT_NECESSARY(L) (L).Release()
#else
	#define BEGIN_ASSERT_LOCK_NOT_NECESSARY(L)
	#define END_ASSERT_LOCK_NOT_NECESSARY(L)
#endif

#if _DEBUG
	#define ASSERT_LOCK_NOT_NECESSARY(J,L)\
		UnnecessaryLockJanitor J(L)
#else
	#define ASSERT_LOCK_NOT_NECESSARY(J,L)
#endif