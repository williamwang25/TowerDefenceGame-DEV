//敌人类

#ifndef _ENEMY_H_
#define _ENEMY_H_

//注意声明顺序

#include"timer.h"
#include"route.h"
#include"vector2.h"
#include"animation.h"
#include"config_manager.h"


#include<functional>


class Enemy
{
public:
	typedef std::function<void(Enemy* enemy)> SkillCallback; //释放技能回调函数

public:
	Enemy()
	{
		timer_skill.set_one_shot(false);
		timer_skill.set_on_timeout([&]() {on_skill_released(this); });

		timer_sketch.set_one_shot(true);
		//受击效果（闪烁）持续时间 0.075
		timer_sketch.set_wait_time(0.075);
		timer_sketch.set_on_timeout([&]() {is_show_sketch = false; });

		timer_restore_speed.set_one_shot(true);
		//恢复移动速度
		timer_restore_speed.set_on_timeout([&]() {speed = max_speed; });
	}
	~Enemy() = default;

	//帧更新函数
	void on_update(double delta)
	{
		timer_skill.on_update(delta);
		timer_sketch.on_update(delta);
		timer_restore_speed.on_update(delta);

		Vector2 move_distance = velocity * delta; //移动距离
		Vector2 target_distance = position_target - position; //目标距离
		position += move_distance < target_distance ? move_distance : target_distance; //取最小值

		if (target_distance.approx_zero())
		{
			idx_target++; //目标索引+1
			refresh_position_target(); //更新目标点位置
			direction = (position_target - position).normalize(); //更新方向
		}

		//移动速度
		velocity.x = direction.x * speed * SIZE_TILE;
		velocity.y = direction.y * speed * SIZE_TILE;

		bool is_show_x_anim = abs(velocity.x) > abs(velocity.y); //水平方向移动

		//显示不同方向动画
		if (is_show_sketch)
		//显示受击效果
		{
			if (is_show_x_anim)
			{
				anim_current = velocity.x > 0 ? &anim_right_sketch : &anim_left_sketch;
			}
			else
			{
				anim_current = velocity.y > 0 ? &anim_down_sketch : &anim_up_sketch;
			}
		}
		//不显示受击效果
		else
		{
			if (is_show_x_anim)
			{
				anim_current = velocity.x > 0 ? &anim_right : &anim_left;
			}
			else
			{
				anim_current = velocity.y > 0 ? &anim_down : &anim_up;
			}
		}

		anim_current->on_update(delta);
	}

	//渲染动画
	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Point point;

		static SDL_Rect rect;
		static const Vector2 size_hp_bar = {40,8}; //血条框大小
		static const SDL_Color color_border = { 116,185,124,255 }; //血条边框颜色
		static const SDL_Color color_content = { 226,255,194,255 }; //血条内部颜色：
		static const int offset_y = 2;
		
		point.x = (int)(position.x - size.x / 2);
		point.y = (int)(position.y - size.y / 2);
		anim_current->on_render(renderer, point);

		//渲染血条

		if (hp < max_hp)
		{
			rect.x = (int)(position.x - size_hp_bar.x / 2);
			rect.y = (int)(position.y - size.y / 2 - size_hp_bar.y-offset_y);
			rect.w = (int)(size_hp_bar.x * hp / max_hp);
			rect.h = (int)size_hp_bar.y;

			//渲染血条内部
			SDL_SetRenderDrawColor(renderer,color_content.r,color_content.g,color_content.b,color_content.a);
			SDL_RenderFillRect(renderer, &rect); 

			//渲染血条边框
			rect.w = (int)size_hp_bar.x;
			SDL_SetRenderDrawColor(renderer, color_border.r, color_border.g, color_border.b, color_border.a);
			SDL_RenderDrawRect(renderer, &rect);
		}
	}

	 //
	void set_on_skill_released(const SkillCallback& on_skill_released)
	{
		this->on_skill_released = on_skill_released;
	}

	//回血函数
	void increase_hp(double val)
	{
		hp += val;
		if (hp > max_hp)
		{
			hp = max_hp;
		}

		//显示受击效果
		is_show_sketch = true;
		timer_sketch.restart();
	}
	
	//受击掉血
	void decrease_hp(double val)
	{
		hp -= val;
		if (hp <= 0)
		{
			hp = 0;
			is_alive = false;
		}
	}

	//受击减速
	void slow_down()
	{
		speed = max_speed - 0.5;
		timer_restore_speed.set_wait_time(1.0);
		timer_restore_speed.restart();
	}

	//设置位置
	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	//设置路径
	void set_route(const Route* route)
	{
		this->route = route;

		refresh_position_target();
	}

	void make_invalid()
	{
		is_alive = false;
	}

	//提供一系列外部接口

	double get_hp()
	{
		return hp;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const Vector2& get_velocity() const
	{
		return velocity;
	}

	double get_damage() const
	{
		return damage;
	}

	double get_reward_ratio() const
	{
		return reward_ratio;
	}

	double get_recover_radius() const
	{
		return SIZE_TILE * recover_range;
	}
	
	double get_recover_intensity() const
	{
		return recover_intensity;
	}

	//是否可以移除
	bool can_remove() const
	{
		return !is_alive; //敌人死亡，可以移除
	}

	//计算路径进度
	//用于防御塔索敌
	double get_route_process() const
	{
		if (route->get_idx_list().size() == 1)
		{
			return 1;
		}
		return (double)idx_target / (route->get_idx_list().size() - 1); //路径索引/路径长度的百分比
	}

protected:
	Vector2 size;
	Timer timer_skill; //技能冷却计时器

	//移动动画
	Animation anim_up;
	Animation anim_down;
	Animation anim_left;
	Animation anim_right;
	//受击动画
	Animation anim_up_sketch;
	Animation anim_down_sketch;
	Animation anim_left_sketch;
	Animation anim_right_sketch;

	//敌人参数
	double max_hp = 0; //最大生命值
	double hp = 0; //当前生命值
	double max_speed=0; //最大速度
	double speed = 0; //当前速度
	double damage = 0; 
	double reward_ratio = 0;//奖励系数

	//恢复类敌人参数
	double recover_interval = 0; //恢复间隔
	double recover_range = 0; //恢复范围
	double recover_intensity = 0; //恢复强度


private:
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;
	
	bool is_alive = true;

	Timer timer_sketch; //受击效果计时器
	bool is_show_sketch = false;

	Animation* anim_current = nullptr;

	SkillCallback on_skill_released; //释放技能回调函数

	Timer timer_restore_speed; //恢复速度计时器

	//路径
	const Route* route = nullptr;
	int idx_target = 0;

	Vector2 position_target;

private:
	//函数封装
	//更新目标点位置
	void refresh_position_target()
	{
		const Route::IdxList& idx_list = route->get_idx_list();

		//如果目标索引小于路径索引列表大小
		if (idx_target < idx_list.size())
		{
			const SDL_Point& point = idx_list[idx_target]; 
			static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;

			//目标位置，取中心点
			position_target.x = rect_tile_map.x + point.x * SIZE_TILE + SIZE_TILE / 2;
			position_target.y = rect_tile_map.y + point.y * SIZE_TILE + SIZE_TILE / 2;

		}
		else
		{
			is_alive = false;
		}
	}

};



#endif // !_ENEMY_H_
