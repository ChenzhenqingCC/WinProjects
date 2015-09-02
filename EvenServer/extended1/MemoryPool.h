#ifndef __MEMORYPOOL_H
#define __MEMORYPOOL_H

#include <vector>
#include <stack>

template <class T>
class MemoryPool
{
public:
	MemoryPool();
	~MemoryPool();
	void init(size_t init_mem_size,size_t inc_mem_size);
	void release();
	void * alloc();
	void dealloc(T *);
	void setIncMemSize(size_t newSize);
	size_t allocedCount() const;
private:
	bool createMemory(size_t size);
	struct SingleBlock
	{
		char block[sizeof(T)];
	};
	std::vector<SingleBlock *> m_pages;
	std::stack<void *> m_unuse;
	size_t m_incMemSize;
	size_t m_fullsize;
};

template <class T>
MemoryPool<T>::MemoryPool()
	:m_incMemSize(0)
	,m_fullsize(0)
{
}

template <class T>
MemoryPool<T>::~MemoryPool()
{
	release();
}

template <class T>
inline void MemoryPool<T>::release()
{
	ASSERT(m_fullsize==m_unuse.size());
	for (size_t i = 0; i < m_pages.size(); ++i)
	{
		delete[] m_pages[i];
	}

	m_unuse=std::stack<void *>();
	m_pages.clear();
	m_fullsize = 0;
}

template <class T>
inline void MemoryPool<T>::init(size_t init_mem_size,size_t inc_mem_size)
{
	m_incMemSize = inc_mem_size;
	createMemory(init_mem_size);
}

template <class T>
inline bool MemoryPool<T>::createMemory(size_t size)
{
	if (size==0)
	{
		return true;
	}
	SingleBlock *p = new SingleBlock[size];
	m_pages.push_back(p);
	for (size_t i = 0; i < size; ++i)
	{
		m_unuse.push(&p[i]);
	}
	m_fullsize+=size;
	return true;
}

template <class T>
inline void * MemoryPool<T>::alloc()
{
	if(m_unuse.empty())
	{
		createMemory(m_incMemSize);
	}
	void* pValue = m_unuse.top();
	m_unuse.pop();
	return pValue;
}

template <class T>
inline size_t MemoryPool<T>::allocedCount() const
{
	return  m_fullsize - m_unuse.size();
}

template <class T>
inline void MemoryPool<T>::dealloc(T * pValue)
{
	if (!pValue)
	{
		return;
	}
	m_unuse.push(pValue);
}

#endif


