//������

#ifndef _ENEMY_H_
#define _ENEMY_H_

//ע������˳��

#include"timer.h"
#include"route.h"
#include"vector2.h"
#include"animation.h"
#include"config_manager.h"


#include<functional>


class Enemy
{
public:
	typedef std::function<void(Enemy* enemy)> SkillCallback; //�ͷż��ܻص�����

public:
	Enemy()
	{
		timer_skill.set_one_shot(false);
		timer_skill.set_on_timeout([&]() {on_skill_released(this); });

		timer_sketch.set_one_shot(true);
		//�ܻ�Ч������˸������ʱ�� 0.075
		timer_sketch.set_wait_time(0.075);
		timer_sketch.set_on_timeout([&]() {is_show_sketch = false; });

		timer_restore_speed.set_one_shot(true);
		//�ָ��ƶ��ٶ�
		timer_restore_speed.set_on_timeout([&]() {speed = max_speed; });
	}
	~Enemy() = default;

	//֡���º���
	void on_update(double delta)
	{
		timer_skill.on_update(delta);
		timer_sketch.on_update(delta);
		timer_restore_speed.on_update(delta);

		Vector2 move_distance = velocity * delta; //�ƶ�����
		Vector2 target_distance = position_target - position; //Ŀ�����
		position += move_distance < target_distance ? move_distance : target_distance; //ȡ��Сֵ

		if (target_distance.approx_zero())
		{
			idx_target++; //Ŀ������+1
			refresh_position_target(); //����Ŀ���λ��
			direction = (position_target - position).normalize(); //���·���
		}

		//�ƶ��ٶ�
		velocity.x = direction.x * speed * SIZE_TILE;
		velocity.y = direction.y * speed * SIZE_TILE;

		bool is_show_x_anim = abs(velocity.x) > abs(velocity.y); //ˮƽ�����ƶ�

		//��ʾ��ͬ���򶯻�
		if (is_show_sketch)
		//��ʾ�ܻ�Ч��
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
		//����ʾ�ܻ�Ч��
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

	//��Ⱦ����
	void on_render(SDL_Renderer* renderer)
	{
		static SDL_Point point;

		static SDL_Rect rect;
		static const Vector2 size_hp_bar = {40,8}; //Ѫ�����С
		static const SDL_Color color_border = { 116,185,124,255 }; //Ѫ���߿���ɫ
		static const SDL_Color color_content = { 226,255,194,255 }; //Ѫ���ڲ���ɫ��
		static const int offset_y = 2;
		
		point.x = (int)(position.x - size.x / 2);
		point.y = (int)(position.y - size.y / 2);
		anim_current->on_render(renderer, point);

		//��ȾѪ��

		if (hp < max_hp)
		{
			rect.x = (int)(position.x - size_hp_bar.x / 2);
			rect.y = (int)(position.y - size.y / 2 - size_hp_bar.y-offset_y);
			rect.w = (int)(size_hp_bar.x * hp / max_hp);
			rect.h = (int)size_hp_bar.y;

			//��ȾѪ���ڲ�
			SDL_SetRenderDrawColor(renderer,color_content.r,color_content.g,color_content.b,color_content.a);
			SDL_RenderFillRect(renderer, &rect); 

			//��ȾѪ���߿�
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

	//��Ѫ����
	void increase_hp(double val)
	{
		hp += val;
		if (hp > max_hp)
		{
			hp = max_hp;
		}

		//��ʾ�ܻ�Ч��
		is_show_sketch = true;
		timer_sketch.restart();
	}
	
	//�ܻ���Ѫ
	void decrease_hp(double val)
	{
		hp -= val;
		if (hp <= 0)
		{
			hp = 0;
			is_alive = false;
		}
	}

	//�ܻ�����
	void slow_down()
	{
		speed = max_speed - 0.5;
		timer_restore_speed.set_wait_time(1.0);
		timer_restore_speed.restart();
	}

	//����λ��
	void set_position(const Vector2& position)
	{
		this->position = position;
	}

	//����·��
	void set_route(const Route* route)
	{
		this->route = route;

		refresh_position_target();
	}

	void make_invalid()
	{
		is_alive = false;
	}

	//�ṩһϵ���ⲿ�ӿ�

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

	//�Ƿ�����Ƴ�
	bool can_remove() const
	{
		return !is_alive; //���������������Ƴ�
	}

	//����·������
	//���ڷ���������
	double get_route_process() const
	{
		if (route->get_idx_list().size() == 1)
		{
			return 1;
		}
		return (double)idx_target / (route->get_idx_list().size() - 1); //·������/·�����ȵİٷֱ�
	}

protected:
	Vector2 size;
	Timer timer_skill; //������ȴ��ʱ��

	//�ƶ�����
	Animation anim_up;
	Animation anim_down;
	Animation anim_left;
	Animation anim_right;
	//�ܻ�����
	Animation anim_up_sketch;
	Animation anim_down_sketch;
	Animation anim_left_sketch;
	Animation anim_right_sketch;

	//���˲���
	double max_hp = 0; //�������ֵ
	double hp = 0; //��ǰ����ֵ
	double max_speed=0; //����ٶ�
	double speed = 0; //��ǰ�ٶ�
	double damage = 0; 
	double reward_ratio = 0;//����ϵ��

	//�ָ�����˲���
	double recover_interval = 0; //�ָ����
	double recover_range = 0; //�ָ���Χ
	double recover_intensity = 0; //�ָ�ǿ��


private:
	Vector2 position;
	Vector2 velocity;
	Vector2 direction;
	
	bool is_alive = true;

	Timer timer_sketch; //�ܻ�Ч����ʱ��
	bool is_show_sketch = false;

	Animation* anim_current = nullptr;

	SkillCallback on_skill_released; //�ͷż��ܻص�����

	Timer timer_restore_speed; //�ָ��ٶȼ�ʱ��

	//·��
	const Route* route = nullptr;
	int idx_target = 0;

	Vector2 position_target;

private:
	//������װ
	//����Ŀ���λ��
	void refresh_position_target()
	{
		const Route::IdxList& idx_list = route->get_idx_list();

		//���Ŀ������С��·�������б��С
		if (idx_target < idx_list.size())
		{
			const SDL_Point& point = idx_list[idx_target]; 
			static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;

			//Ŀ��λ�ã�ȡ���ĵ�
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
