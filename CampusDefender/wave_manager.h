//攻击波次管理器

#ifndef _WAVE_MANAGER_H_
#define _WAVE_MANAGER_H_


#include "timer.h"
#include "manager.h"
#include "config_manager.h"
#include "enemy_manager.h"
#include "coin_manager.h"

#include<vector>


class WaveManager :public Manager<WaveManager>
{
	friend class Manager<WaveManager>;

public:

	void on_update(double delta)
	{
		static ConfigManager* instance = ConfigManager::instance();

		if (instance->is_game_over)
		{
			return;
		}
		if (!is_wave_started)
		{
			timer_start_wave.on_update(delta);
		}
		else
		{
			timer_spawn_enemy.on_update(delta);
		}

		//当前波次结束，跳转到下一波
		if (is_spawned_last_enemy && EnemyManager::instance()->check_cleared())
		{
			//波次结束奖励
			CoinManager::instance()->increase_coin(instance->wave_list[idx_wave].rewards);

			idx_wave++; //下一波

			//全部波次结束，游戏胜利
			if (idx_wave >= instance->wave_list.size())
			{
				instance->is_game_win = true;
				instance->is_game_over = true;
				return;
			}

			//波次未结束，继续下一波
			else
			{
				idx_spawn_event = 0;
				is_wave_started = true;
				is_spawned_last_enemy = false;

				const Wave& wave = instance->wave_list[idx_wave];
				timer_start_wave.set_wait_time(wave.interval);
				timer_start_wave.restart();
			}
		}
	}

protected:
	WaveManager()
	{
		static const std::vector<Wave>& wave_list = ConfigManager::instance()->wave_list;

		timer_start_wave.set_one_shot(true);
		timer_start_wave.set_wait_time(wave_list[0].interval);
		timer_start_wave.set_on_timeout
		(
			[&]()
			{
				is_wave_started = true;
				timer_spawn_enemy.set_wait_time(wave_list[idx_wave].spawn_event_list[0].interval);
				timer_spawn_enemy.restart();
			}
		);

		timer_spawn_enemy.set_one_shot(true);
		timer_spawn_enemy.set_on_timeout
		(
			[&]()
			{
				const std::vector<Wave::SpawnEvent>& spawn_event_list = wave_list[idx_wave].spawn_event_list;
				const Wave::SpawnEvent& spawn_event = spawn_event_list[idx_spawn_event];

				//生成敌人 敌人类型/出生点
				EnemyManager::instance()->spawn_enemy(spawn_event.enemy_type, spawn_event.spawn_point);

				idx_spawn_event++;

				if (idx_spawn_event >= spawn_event_list.size())
				{
					is_spawned_last_enemy = true;
					return;
				}

				timer_spawn_enemy.set_wait_time(spawn_event_list[idx_spawn_event].interval);
				timer_spawn_enemy.restart();
			}
		);
	}

	~WaveManager() = default;

private:
	int idx_wave = 0;
	int idx_spawn_event = 0;
	Timer timer_start_wave;
	Timer timer_spawn_enemy;
	bool is_wave_started = false;
	bool is_spawned_last_enemy = false;

};
#endif // !_WAVE_MANAGER_H_
