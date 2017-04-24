#pragma once

#include <thread>

class Thread
{
public:
	Thread() {}
	virtual ~Thread() {}
	virtual void startThread() = 0;
	virtual void join()
	{
		if(_thread.joinable())
			_thread.join();
	}
protected:
	std::thread _thread;
};
