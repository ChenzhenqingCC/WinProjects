#ifndef __FAKEOBJECT_H
#define __FAKEOBJECT_H

template <class T>
class FakeObject
{
public:
	T* getFakePtr();
private:
	char m_buffer[sizeof(T)];
};

template <class T>
inline T* FakeObject<T>::getFakePtr()
{
	return reinterpret_cast<T*>(m_buffer);
}

#endif
