//子弹类型
// 继承自Bullet类，实现不同的子弹效果

#ifndef _BULLETS_H_
#define _BULLETS_H_

#include"bullet.h"
#include"resources_manager.h"

//箭矢类
class ArrowBullet : public Bullet
{
public:
	ArrowBullet()
	{
		static SDL_Texture* tex_arrow = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_BulletArrow)->second;

		//动画帧序列
		static const std::vector<int> idx_list = { 0,1 };

		animation.set_loop(true);
		animation.set_interval(0.1);
		animation.set_frame_data(tex_arrow, 2, 1, idx_list);

		can_rotated = true;

		//子弹帧图像大小
		size.x = 48;
		size.y = 48;
	}

	~ArrowBullet() = default;

	void on_collide(Enemy* enemy) override
	{
		static const ResourcesManager::SoundPool& sound_pool
			= ResourcesManager::instance()->get_sound_pool();

		//随机播放三种击中音效
		switch (rand() % 3)
		{
		case 0:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_1)->second, 0);
			break;
		case 1:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_2)->second, 0);
			break;
		case 2:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ArrowHit_3)->second, 0);
			break;
		}

		Bullet::on_collide(enemy);
	}
};

//斧头类
class AxeBullet : public Bullet
{
public:
	AxeBullet()
	{
		static SDL_Texture* tex_axe = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_BulletAxe)->second;

		static const std::vector<int> idx_list = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

		animation.set_loop(true);
		animation.set_interval(0.1);
		animation.set_frame_data(tex_axe, 4, 2, idx_list);

		size.x = 48, size.y = 48;
	}

	~AxeBullet() = default;

	void on_collide(Enemy* enemy) override
	{
		static const ResourcesManager::SoundPool& sound_pool
			= ResourcesManager::instance()->get_sound_pool();

		switch (rand() % 3)
		{
		case 0:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_1)->second, 0);
			break;
		case 1:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_2)->second, 0);
			break;
		case 2:
			Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_AxeHit_3)->second, 0);
			break;
		}

		enemy->slow_down();

		Bullet::on_collide(enemy);
	}

};

//炮弹类
class ShellBullet : public Bullet
{
public:
	ShellBullet()
	{
		static SDL_Texture* tex_shell = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_BulletShell)->second;
		static SDL_Texture* tex_explode = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_EffectExplode)->second;

		static const std::vector<int> idx_list = { 0, 1 };
		static const std::vector<int> idx_explode_list = { 0, 1, 2, 3, 4 };

		animation.set_loop(true);
		animation.set_interval(0.1);
		animation.set_frame_data(tex_shell, 2, 1, idx_list);

		animation_explode.set_loop(false);
		animation_explode.set_interval(0.1);
		animation_explode.set_frame_data(tex_explode, 5, 1, idx_explode_list);
		animation_explode.set_on_finished(
			[&]()
			{
				make_invalid();
			});

		damage_range = 96;
		size.x = 48, size.y = 48;
	}

	~ShellBullet() = default;

	void on_update(double delta) override
	{
		if (can_collide())
		{
			Bullet::on_update(delta);
			return;
		}

		animation_explode.on_update(delta);
	}

	void on_render(SDL_Renderer* renderer) override
	{
		if (can_collide())
		{
			Bullet::on_render(renderer);
			return;
		}

		static SDL_Point point;

		point.x = (int)(position.x - 96 / 2);
		point.y = (int)(position.y - 96 / 2);

		animation_explode.on_render(renderer, point);
	}

	void on_collide(Enemy* enemy) override
	{
		static const ResourcesManager::SoundPool& sound_pool
			= ResourcesManager::instance()->get_sound_pool();

		Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_ShellHit)->second, 0);

		disable_collide();
	}

private:
	Animation animation_explode;

};



#endif // !_BULLETS_H_
