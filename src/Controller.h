#pragma once

#include "Point.h"
#include "Observer.h"
#include <vector>
#include <cstddef>
#include <mutex>
#include <condition_variable>

using std::vector;

class Controller
{
public:
	Controller(int col, int row) : _col(col), _row(row), _observer(NULL)
	{
	}

	/*
	 *needs locking
	 */
	void update(const vector<Point>& update)
	{
		std::lock_guard<std::mutex> lock(_queueMutex);
		_updates = update;
		_cv.notify_one();

	}

	/*
	 * needs locking
	 */
	vector<Point> get()
	{
		std::unique_lock<std::mutex> lock(_queueMutex);
		while(_updates.empty())
			_cv.wait(lock);
		return _updates;
	}

private:
	int			_col;
	int			_row;
	std::mutex		_queueMutex;
	std::condition_variable _cv;
	vector<Point> _updates;		// SHARED RESOURCE - needs locking
	Observer<std::vector<Point>>*	_observer;
};
