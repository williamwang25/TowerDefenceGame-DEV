//子弹管理器类
//提供子弹序列容器，存储所有子弹对象
//提供子弹的创建、更新、渲染、删除等功能

#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_

#include"manager.h"
#include"bullet.h"
#include"bullets.h"
#include"bullet_type.h"

#include<vector>

class BulletManager : public Manager<BulletManager>
{
	friend class Manager<BulletManager>;

public:
	typedef std::vector<Bullet*> BulletList;

public:

	void on_update(double delta)
	{
		for (Bullet* bullet : bullet_list)
		{
			bullet->on_update(delta);
		}

		//删除无效子弹
		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(), [](const Bullet* bullet)  //lamada表达式
			{ 
				bool deletable = bullet->can_remove();
				if (deletable)
				{
					delete bullet;
				}
				return deletable;
			}), bullet_list.end());
	}

	//渲染子弹
	void on_render(SDL_Renderer* renderer)
	{
		for (Bullet* bullet : bullet_list)
		{
			bullet->on_render(renderer);
		}
	}

	BulletList& get_bullet_list()
	{
		return bullet_list;
	}

	//发射子弹
	void fire_bullet(BulletType type, const Vector2& position, const Vector2& velocity, double damage)
	{
		Bullet* bullet = nullptr;

		//根据子弹类型创建子弹对象（工厂模式）
		switch (type)
		{
		case Arrow:
			bullet = new ArrowBullet();
			break;
		case Axe:
			bullet = new AxeBullet();
			break;
		case Shell:
			bullet = new ShellBullet();
			break;
		default:
			bullet = new ArrowBullet();
			break;
		}

		bullet->set_position(position);
		bullet->set_velocity(velocity);
		bullet->set_damage(damage);

		bullet_list.push_back(bullet);
	} 

protected:
	BulletManager() = default;

	~BulletManager()
	{
		for (Bullet* bullet : bullet_list)
		{
			delete bullet;
		}
	}

private:
	//子弹序列容器
	BulletList bullet_list;
};


#endif // !_BULLET_MANAGER_H_
