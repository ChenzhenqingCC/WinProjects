#ifndef __MASKVALUE_H
#define __MASKVALUE_H


#define MASK_VALUE(TYPE) (1 << (TYPE) )
#define MASK_HAVE_TYPE(MASK,TYPE) ( MASK & MASK_VALUE(TYPE) )

#pragma pack(1)

template<typename MASK_TYPE>
class MASKED_VALUE
{
public:
	inline MASK_TYPE reset(MASK_TYPE mask = 0)
	{
		MASK_TYPE old = m_mask;
		m_mask = mask;
		return old;
	}
	inline MASK_TYPE getValue() const
	{
		return m_mask;
	}
	inline bool haveMask(MASK_TYPE mask = ~0) const
	{
		return ( m_mask & mask ) ? true : false;
	}
	inline void addMask(MASK_TYPE mask)
	{
		m_mask |= mask;
	}
	inline void removeMask(MASK_TYPE mask)
	{
		m_mask &= ~mask;
	}
	inline MASK_TYPE fullMask()
	{
		return reset(MASK_TYPE(~0));
	}
	inline static MASK_TYPE maskOf(unsigned char enumValue)
	{
		return MASK_VALUE(enumValue);
	}
private:
	MASK_TYPE m_mask;
};

#pragma pack()

#endif
