#ifndef __CONTEXTPROTECT_H
#define __CONTEXTPROTECT_H

template<class T>
class DataBackUp
{
public:
	DataBackUp(T& data);
	~DataBackUp();
	T& getCurrentValue();
	const T& getBackupValue() const;
private:
	T& m_data;
	T m_backup;
};

template<class T> inline DataBackUp<T>::DataBackUp(T& data)
	:m_data(data)
	,m_backup(data)
{
}

template<class T> inline DataBackUp<T>::~DataBackUp()
{
	m_data = m_backup;
}

template<class T> inline T& DataBackUp<T>::getCurrentValue()
{
	return m_data;
}

template<class T> inline const T& DataBackUp<T>::getBackupValue() const
{
	return m_backup;
}

//Macros
#define DATA_BACKUP(CLASS,VALUE) DataBackUp< CLASS > _BackUp_##VALUE(VALUE);

//定义用于管理的值
#define CONTEXT_REGISTER(CLASS,VALUE) \
	class _CCR_##VALUE\
	{ \
	public: \
		inline CLASS & getValue() \
		{ \
			return m_value; \
		} \
		inline CLASS const& getValue() const \
		{ \
			return m_value; \
		} \
	private: \
		CLASS m_value; \
	} _m_context_##VALUE;

//取得值的引用，并不进行备份，相应值的改动将持续
#define CONTEXT_TAKE(CLASS,VALUE) \
	_m_context_##VALUE.getValue(); \
	CLASS const& context_##VALUE = _m_context_##VALUE.getValue();

//取得值的引用并进行备份，将在它的作用域结束时将自动把备份的值还原
#define CONTEXT_PROTECT(CLASS,VALUE) \
	CONTEXT_TAKE(CLASS,VALUE); \
	DataBackUp< CLASS > _BackUp_context_##VALUE(_m_context_##VALUE.getValue());

#endif
