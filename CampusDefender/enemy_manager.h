//���˹�����
//�ṩ���������������洢���е��˶���
//����������е��˵��������ڣ��������˵����ɡ����¡���Ⱦ�����ٵȲ���
//��������ص���ײ���

#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

//ע������˳�򣬷���������
#include "enemy.h"
#include "manager.h"
#include "config_manager.h"
#include "home_manager.h"
#include"enemy_type.h"
#include "enemies.h"
#include "coin_manager.h"
#include "bullet_manager.h"

#include <vector>
#include<SDL.h>

//���˹�������
class EnemyManager :public Manager<EnemyManager>
{
	friend class Manager<EnemyManager>;

public:
	typedef std::vector<Enemy*> EnemyList;

	void on_update(double delta)
	{
		//�������е���״̬����̬��ʵ��
		for (Enemy* enemy : enemy_list)
		{
			enemy->on_update(delta);
		}

		process_home_collision();
		process_bullet_collision();

		remove_invalid_enemy();
	}

	//��Ⱦ���ֵ��ˣ���̬��ʵ��
	void on_render(SDL_Renderer* renderer)
	{
		for (Enemy* enemy : enemy_list)
		{
			enemy->on_render(renderer);
		}
	}

	void spawn_enemy(EnemyType type, int idx_spawn_point)
	{
		static Vector2 position;
		static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		//��ȡ���˳���������
		static const Map::SpawnerRoutePool& spawner_route_pool
			= ConfigManager::instance()->map.get_idx_spawner_pool();

		//��������Ч�ĳ��������ɵ���
		const auto& itor = spawner_route_pool.find(idx_spawn_point); //������ iterator
		if (itor == spawner_route_pool.end())
		{
			return;
		}

		Enemy* enemy = nullptr;

		//����ģʽ�����ɲ�ͬ���͵���


		switch (type)
		{
		case EnemyType::Slim:
			enemy = new SlimEnemy();
			break;
		case EnemyType::KingSlim:
			enemy = new KingSlimeEnemy();
			break;
		case EnemyType::Skeleton:
			enemy = new SkeletonEnemy();
			break;
		case EnemyType::Goblin:
			enemy = new GoblinEnemy();
			break;
		case EnemyType::GoblinPriest:
			enemy = new GoblinPriestEnemy();
			break;
		default:
			enemy = new SlimEnemy();
			break;
		}

		//�����ͷż��ܣ��ظ���Χ��������ֵ
		enemy->set_on_skill_released
		(
			[&](Enemy* enemy_src)
			{
				double recover_radius = enemy_src->get_recover_radius();
				//�����޻ָ�����
				if (recover_radius < 0)
				{
					return;
				}

				const Vector2& pos_src = enemy_src->get_position();
				for (Enemy* enemy_dst : enemy_list)
				{
					const Vector2& pos_dst = enemy_dst->get_position();
					//�������֮��ľ���
					double distance = (pos_dst - pos_src).length();
					if (distance <= recover_radius)
					{
						//��������ڻָ��뾶�ڣ���������ֵ
						enemy_dst->increase_hp(enemy_src->get_recover_intensity());
					}
				}
			}
		);

		//Ѱ·ϵͳ
		const Route::IdxList& idx_list = itor->second.get_idx_list();
		position.x = rect_tile_map.x + idx_list[0].x * SIZE_TILE + SIZE_TILE / 2;
		position.y = rect_tile_map.y + idx_list[0].y * SIZE_TILE + SIZE_TILE / 2;

		enemy->set_position(position);
		enemy->set_route(&itor->second);

		enemy_list.push_back(enemy);
	}

	bool check_cleared()
	{
		return enemy_list.empty();
	}

	EnemyManager::EnemyList& get_enemy_list()
	{
		return enemy_list;
	}



protected:

	EnemyManager() = default;
	//��������
	~EnemyManager()
	{
		for (Enemy* enemy : enemy_list)
		{
			delete enemy;
		}
	}
private:

	EnemyList enemy_list;

private:

	//���˽������
	void process_home_collision()
	{
		static const SDL_Point& idx_home = ConfigManager::instance()->map.get_idx_home();
		static const SDL_Rect& rect_tile_map = ConfigManager::instance()->rect_tile_map;
		static const Vector2& position_home_tile
		{
			(double)rect_tile_map.x + idx_home.x * SIZE_TILE,
			(double)rect_tile_map.y + idx_home.y * SIZE_TILE
		};

		for (Enemy* enemy : enemy_list)
		{
			if(enemy->can_remove())
			{
				continue;
			}

			const Vector2& position = enemy->get_position();

			//�жϵ����������ײ
			if (position.x >= position_home_tile.x && position.y >= position_home_tile.y
				&& position.x <= position_home_tile.x + SIZE_TILE
				&& position.y <= position_home_tile.y + SIZE_TILE)
			{
				enemy->make_invalid();

				//���˽�����أ����ص�Ѫ
				HomeManager::instance()->decrease_hp(enemy->get_damage()); //��������˺�
			}

		}

	}

	void process_bullet_collision()
	{
		static BulletManager::BulletList& bullet_list
			= BulletManager::instance()->get_bullet_list();

		for (Enemy* enemy : enemy_list)
		{
			if (enemy->can_remove()) continue;

			const Vector2& size_enemy = enemy->get_size();
			const Vector2& pos_enemy = enemy->get_position();

			for (Bullet* bullet : bullet_list)
			{
				if (!bullet->can_collide()) continue;

				const Vector2& pos_bullet = bullet->get_position();

				if (pos_bullet.x >= pos_enemy.x - size_enemy.x / 2
					&& pos_bullet.y >= pos_enemy.y - size_enemy.y / 2
					&& pos_bullet.x <= pos_enemy.x + size_enemy.x / 2
					&& pos_bullet.y <= pos_enemy.y + size_enemy.y / 2)
				{
					double damage = bullet->get_damage();
					double damage_range = bullet->get_damage_range();
					if (damage_range < 0)
					{
						enemy->decrease_hp(damage);
						if (enemy->can_remove())
							try_spawn_coin_prop(pos_enemy, enemy->get_reward_ratio());
					}
					else
					{
						for (Enemy* target_enemy : enemy_list)
						{
							const Vector2& pos_target_enemy = target_enemy->get_position();
							if ((pos_target_enemy - pos_bullet).length() <= damage_range)
							{
								target_enemy->decrease_hp(damage);
								if (target_enemy->can_remove())
									try_spawn_coin_prop(pos_target_enemy, enemy->get_reward_ratio());
							}
						}
					}

					bullet->on_collide(enemy);
				}
			}
		}
	}

	void remove_invalid_enemy()
	{
		enemy_list.erase(std::remove_if(enemy_list.begin(), enemy_list.end(),
			[](const Enemy* enemy)
			{
				bool deletable = enemy->can_remove();
				if (deletable) delete enemy;
				return deletable;
			}), enemy_list.end());
	}

	void try_spawn_coin_prop(const Vector2& position, double ratio)
	{
		static CoinManager* instance = CoinManager::instance();

		if ((double)(rand() % 100) / 100 <= ratio)
			instance->spawn_coin_prop(position);
	}

};



#endif // !_ENEMY_MANAGER_H_
