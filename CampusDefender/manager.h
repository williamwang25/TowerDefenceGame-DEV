///�������࣬��Ϊ���ֹ������Ļ��࣬�������ļ�����������Դ����������Ϸ��������

//����1����Ϊ��Ϸ���������
//����2��������Ϸ������������ڣ����ɣ���������Ϊ
//����3��ʵ����Ϸ�߼��Ŀ���

#ifndef _MANAGER_H_
#define _MANAGER_H_

//�ɼ̳е���ģ���ࣺManager
template<typename T> 


//�ɼ̳е�����Manager
class Manager
{
public:
	static T* instance()
	{
		if (!manager)
		{
			manager = new T();
		}
		return manager;
	}

private:
	static T* manager;

protected:
	Manager() = default;
	~Manager() = default;
	Manager(const Manager&) = delete;
	Manager& operator = (const Manager&) = delete;
};

template <typename T>
T* Manager<T>::manager = nullptr;

#endif // !_MANAGER_H_
