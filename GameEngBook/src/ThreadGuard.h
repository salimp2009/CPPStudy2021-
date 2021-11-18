#pragma once


class threadguard
{
private:
	std::thread& m_thread;

public:
	explicit threadguard(std::thread& th) :m_thread{ th } {}
	threadguard(const threadguard& ) = delete;
	threadguard& operator=(const threadguard&) = delete;
	

	~threadguard()
	{
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

};
