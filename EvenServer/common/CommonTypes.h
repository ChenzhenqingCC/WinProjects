#ifndef __COMMONTYPES_H
#define __COMMONTYPES_H
#include <Zoic/include/Types.h>

//�������Ͷ���
#define TYPE_IN_MASK(mask,type) ( mask & (1 << type) )

template<typename HANDLE_T,typename SERIAL_T>
struct T_SERIALHANDLE
{//��ɫ���
	typedef HANDLE_T HandleType;
	typedef SERIAL_T SerialType;
	HANDLE_T handle;
	SERIAL_T serial;
	bool operator < ( const T_SERIALHANDLE &o) const;
	bool operator > ( const T_SERIALHANDLE &o) const;
	bool operator == ( const T_SERIALHANDLE &o) const;
	bool operator != ( const T_SERIALHANDLE &o) const;
};

template <class T>
struct T_POS2D
{//2Dλ��
	typedef T CoordType;
	T x;
	T y;
	bool operator < ( const T_POS2D &o) const;
	bool operator > ( const T_POS2D &o) const;
	bool operator == ( const T_POS2D &o) const;
	bool operator != ( const T_POS2D &o) const;
};

template <class T>
struct T_GRADS
{//�ݶȽṹ
	typedef T OffsetType;
	T	dx;	//xƫ��
	T	dy; //yƫ��
	bool operator < ( const T_GRADS &o) const;
	bool operator > ( const T_GRADS &o) const;
	bool operator == ( const T_GRADS &o) const;
	bool operator != ( const T_GRADS &o) const;
};

template <class T>
struct T_RECTANGLE
{
	typedef T CoordType;
	T xMin;
	T yMin;
	T xMax;
	T yMax;
	bool inRange(T x,T y) const;
	bool hasCover(const T_RECTANGLE& o) const;
};

typedef T_SERIALHANDLE<HandleType,SerialType>	SessionHandle;		//���Ӿ��
typedef T_POS2D<WORD>		POS2D_W;	//2Dλ��
typedef T_POS2D<short>		POS2D_S;	//2Dλ��(������)
typedef T_POS2D<long>		POS2D_L;	//2D��λ��(������)
typedef T_GRADS<char>		GRADS_C;	//2Dƫ��
typedef T_GRADS<short>		GRADS_S;	//2Dƫ��
typedef T_GRADS<long>		GRADS_L;	//2Dƫ��
typedef T_RECTANGLE<int>	RectRange;

//�����ṹ����������ʵ��
template<typename HANDLE_T,typename SERIAL_T>
inline bool T_SERIALHANDLE<HANDLE_T,SERIAL_T>::operator < ( const T_SERIALHANDLE<HANDLE_T,SERIAL_T> &o) const
{
	if(handle!=o.handle)
	{
		return handle < o.handle;
	}
	return serial < o.serial;
}
template<typename HANDLE_T,typename SERIAL_T>
inline bool T_SERIALHANDLE<HANDLE_T,SERIAL_T>::operator > ( const T_SERIALHANDLE<HANDLE_T,SERIAL_T> &o) const
{
	if(handle!=o.handle)
	{
		return handle > o.handle;
	}
	return serial > o.serial;
}
template<typename HANDLE_T,typename SERIAL_T>
inline bool T_SERIALHANDLE<HANDLE_T,SERIAL_T>::operator == ( const T_SERIALHANDLE<HANDLE_T,SERIAL_T> &o) const
{
	return handle == o.handle && serial == o.serial;
}
template<typename HANDLE_T,typename SERIAL_T>
inline bool T_SERIALHANDLE<HANDLE_T,SERIAL_T>::operator != ( const T_SERIALHANDLE<HANDLE_T,SERIAL_T> &o) const
{
	if(handle!=o.handle)
	{
		return true;
	}
	return serial != o.serial;
}

template <class T>
bool T_POS2D<T>::operator < ( const T_POS2D &o) const
{
	if(x!=o.x)
	{
		return x < o.x;
	}
	return y < o.y;
}
template <class T>
bool T_POS2D<T>::operator > ( const T_POS2D &o) const
{
	if(x!=o.x)
	{
		return x > o.x;
	}
	return y > o.y;
}
template <class T>
bool T_POS2D<T>::operator == ( const T_POS2D &o) const
{
	return x == o.x && y == o.y;
}
template <class T>
bool T_POS2D<T>::operator != ( const T_POS2D &o) const
{
	if(x!=o.x)
	{
		return true;
	}
	return y != o.y;
}

template <class T>
inline bool T_GRADS<T>::operator < ( const T_GRADS<T> &o) const
{
	if(dx!=o.dx)
	{
		return dx < o.dx;
	}
	return dy < o.dy;
}
template <class T>
inline bool T_GRADS<T>::operator > ( const T_GRADS<T> &o) const
{
	if(dx!=o.dx)
	{
		return dx > o.dx;
	}
	return dy > o.dy;
}
template <class T>
inline bool T_GRADS<T>::operator == ( const T_GRADS<T> &o) const
{
	return dx == o.dx && dy == o.dy;
}
template <class T>
inline bool T_GRADS<T>::operator != ( const T_GRADS<T> &o) const
{
	if(dx!=o.dx)
	{
		return true;
	}
	return dy != o.dy;
}

template <class T>
inline bool T_RECTANGLE<T>::inRange(T x,T y) const
{
	return !(x < xMin || y < yMin || x > xMax || y > yMax);
}

template <class T>
inline bool T_RECTANGLE<T>::hasCover(const T_RECTANGLE<T>& o) const
{
	return !(o.xMin > xMax || o.xMax < xMin || o.yMin > yMax || o.yMax < yMin);
}


namespace Common
{
	template <class T>
	inline T getInvlidValue()
	{
		T basic = 1;
		const int moveCount = ( sizeof(T) * 8 - 1 );
		return basic << moveCount;
	}

}

#endif
