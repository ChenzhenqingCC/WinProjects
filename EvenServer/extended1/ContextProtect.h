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

//�������ڹ����ֵ
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

//ȡ��ֵ�����ã��������б��ݣ���Ӧֵ�ĸĶ�������
#define CONTEXT_TAKE(CLASS,VALUE) \
	_m_context_##VALUE.getValue(); \
	CLASS const& context_##VALUE = _m_context_##VALUE.getValue();

//ȡ��ֵ�����ò����б��ݣ������������������ʱ���Զ��ѱ��ݵ�ֵ��ԭ
#define CONTEXT_PROTECT(CLASS,VALUE) \
	CONTEXT_TAKE(CLASS,VALUE); \
	DataBackUp< CLASS > _BackUp_context_##VALUE(_m_context_##VALUE.getValue());

#endif
