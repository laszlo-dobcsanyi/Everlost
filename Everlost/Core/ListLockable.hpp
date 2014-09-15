#ifndef CORE_LIST_LOCKABLE_HPP
#define CORE_LIST_LOCKABLE_HPP

#include <list>
#include <boost\thread.hpp>

template<class T> class ListLockable
{
public:
	int number;
	std::list<T> list;
	boost::shared_mutex mutex;

	inline ListLockable() : number(0) { };
	inline ~ListLockable() { };

	void Add(T& _element)
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex);
		list.push_back(_element);
		++number;
	}

	void Remove(T& _element)
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex);
		list.remove(_element);
		--number;
	}

	void Clear()
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex);
		list.clear();
		number = 0;
	}

private:
	ListLockable(const ListLockable<T>& _other);
	ListLockable& operator=(const ListLockable<T>* _other);
};

#endif