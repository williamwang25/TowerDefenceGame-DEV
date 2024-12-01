///管理器类，作为各种管理器的基类，如配置文件管理器；资源管理器；游戏管理器等

//作用1：作为游戏对象的容器
//作用2：控制游戏对象的生命周期，生成，交互等行为
//作用3：实现游戏逻辑的控制

#ifndef _MANAGER_H_
#define _MANAGER_H_

//可继承单例模板类：Manager
template<typename T> 


//可继承单例：Manager
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
