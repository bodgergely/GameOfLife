#pragma once

template<class T>
class Observer
{
public:
	Observer(){}
	virtual ~Observer() {}
	virtual void notify(const T&) = 0;
};
