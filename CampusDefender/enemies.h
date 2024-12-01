//注意声明顺序
#include"enemy.h"
#include"config_manager.h" 
#include"resources_manager.h"

//五种敌人类

//Slim类
class SlimEnemy :public Enemy
{
public:
	//Slim敌人构造函数
	SlimEnemy()
	{
		//加载贴图
		static const ResourcesManager::TexturePool& texture_pool =
			ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture* tex_slim = texture_pool.find(ResID::Tex_Slime)->second;
		static SDL_Texture* tex_slim_sketch = texture_pool.find(ResID::Tex_SlimeSketch)->second;
		//加载数据
		static ConfigManager::EnemyTemplate& slim_template =
			ConfigManager::instance()->slim_template;

		//动画索引：由资源文件中的动画顺序决定
		static const std::vector<int> idx_list_up = { 6,7,8,9,10,11 };
		static const std::vector<int> idx_list_down = { 0,1,2,3,4,5 };
		static const std::vector<int> idx_list_left = { 18,19,20,21,22,23 };
		static const std::vector<int> idx_list_right = { 12,13,14,15,16,17 };

		//初始化动画
		anim_up.set_loop(true);
		anim_up.set_interval(0.1);
		anim_up.set_frame_data(tex_slim, 6, 4, idx_list_up);
		anim_down.set_loop(true);
		anim_down.set_interval(0.1);
		anim_down.set_frame_data(tex_slim, 6, 4, idx_list_down);
		anim_left.set_loop(true);
		anim_left.set_interval(0.1);
		anim_left.set_frame_data(tex_slim, 6, 4, idx_list_left);
		anim_right.set_loop(true);
		anim_right.set_interval(0.1);
		anim_right.set_frame_data(tex_slim, 6, 4, idx_list_right);

		//初始化受击动画
		anim_up_sketch.set_loop(true);
		anim_up_sketch.set_interval(0.1);
		anim_up_sketch.set_frame_data(tex_slim_sketch, 6, 4, idx_list_up);
		anim_down_sketch.set_loop(true);
		anim_down_sketch.set_interval(0.1);
		anim_down_sketch.set_frame_data(tex_slim_sketch, 6, 4, idx_list_down);
		anim_left_sketch.set_loop(true);
		anim_left_sketch.set_interval(0.1);
		anim_left_sketch.set_frame_data(tex_slim_sketch, 6, 4, idx_list_left);
		anim_right_sketch.set_loop(true);
		anim_right_sketch.set_interval(0.1);
		anim_right_sketch.set_frame_data(tex_slim_sketch, 6, 4, idx_list_right);

		//初始化Slim数据
		max_hp = slim_template.hp;
		max_speed = slim_template.speed;
		damage = slim_template.damage;
		reward_ratio = slim_template.reward_ratio;
		recover_interval = slim_template.recover_interval;
		recover_range = slim_template.recover_range;
		recover_intensity = slim_template.recover_intensity;

		size.x = 48;
		size.y = 48;

		hp = max_hp;
		speed = max_speed;
	}

	~SlimEnemy() = default;
};

//KingSlim类
class KingSlimeEnemy :public Enemy
{
public:
	KingSlimeEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture* tex_king_slime = texture_pool.find(ResID::Tex_KingSlime)->second;
		static SDL_Texture* tex_king_slime_sketch = texture_pool.find(ResID::Tex_KingSlimeSketch)->second;
		static ConfigManager::EnemyTemplate& king_slim_template = ConfigManager::instance()->king_slim_template;

		static const std::vector<int> idx_list_up = { 18, 19, 20, 21, 22, 23 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4, 5 };
		static const std::vector<int> idx_list_left = { 6, 7, 8, 9, 10, 11 };
		static const std::vector<int> idx_list_right = { 12, 13, 14, 15, 16, 17 };

		anim_up.set_loop(true); anim_up.set_interval(0.1);
		anim_up.set_frame_data(tex_king_slime, 6, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.1);
		anim_down.set_frame_data(tex_king_slime, 6, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.1);
		anim_left.set_frame_data(tex_king_slime, 6, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.1);
		anim_right.set_frame_data(tex_king_slime, 6, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.1);
		anim_up_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.1);
		anim_down_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.1);
		anim_left_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.1);
		anim_right_sketch.set_frame_data(tex_king_slime_sketch, 6, 4, idx_list_right);

		max_hp = king_slim_template.hp;
		max_speed = king_slim_template.speed;
		damage = king_slim_template.damage;
		reward_ratio = king_slim_template.reward_ratio;
		recover_interval = king_slim_template.recover_interval;
		recover_range = king_slim_template.recover_range;
		recover_intensity = king_slim_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~KingSlimeEnemy() = default;
};

//Skeleton类
class SkeletonEnemy : public Enemy
{
public:
	SkeletonEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture* tex_skeleton = texture_pool.find(ResID::Tex_Skeleton)->second;
		static SDL_Texture* tex_skeleton_sketch = texture_pool.find(ResID::Tex_SkeletonSketch)->second;
		static ConfigManager::EnemyTemplate& skeleton_template = ConfigManager::instance()->skeleton_template;

		static const std::vector<int> idx_list_up = { 5, 6, 7, 8, 9 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4 };
		static const std::vector<int> idx_list_left = { 15, 16, 17, 18, 19 };
		static const std::vector<int> idx_list_right = { 10, 11, 12, 13, 14 };

		anim_up.set_loop(true); anim_up.set_interval(0.15);
		anim_up.set_frame_data(tex_skeleton, 5, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.15);
		anim_down.set_frame_data(tex_skeleton, 5, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.15);
		anim_left.set_frame_data(tex_skeleton, 5, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.15);
		anim_right.set_frame_data(tex_skeleton, 5, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.15);
		anim_up_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.15);
		anim_down_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.15);
		anim_left_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.15);
		anim_right_sketch.set_frame_data(tex_skeleton_sketch, 5, 4, idx_list_right);

		max_hp = skeleton_template.hp;
		max_speed = skeleton_template.speed;
		damage = skeleton_template.damage;
		reward_ratio = skeleton_template.reward_ratio;
		recover_interval = skeleton_template.recover_interval;
		recover_range = skeleton_template.recover_range;
		recover_intensity = skeleton_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~SkeletonEnemy() = default;

};

//Goblin类
class GoblinEnemy : public Enemy
{
public:
	GoblinEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture* tex_goblin = texture_pool.find(ResID::Tex_Goblin)->second;
		static SDL_Texture* tex_goblin_sketch = texture_pool.find(ResID::Tex_GoblinSketch)->second;
		static ConfigManager::EnemyTemplate& goblin_template = ConfigManager::instance()->goblin_template;

		static const std::vector<int> idx_list_up = { 5, 6, 7, 8, 9 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4 };
		static const std::vector<int> idx_list_left = { 15, 16, 17, 18, 19 };
		static const std::vector<int> idx_list_right = { 10, 11, 12, 13, 14 };

		anim_up.set_loop(true); anim_up.set_interval(0.15);
		anim_up.set_frame_data(tex_goblin, 5, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.15);
		anim_down.set_frame_data(tex_goblin, 5, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.15);
		anim_left.set_frame_data(tex_goblin, 5, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.15);
		anim_right.set_frame_data(tex_goblin, 5, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.15);
		anim_up_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.15);
		anim_down_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.15);
		anim_left_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.15);
		anim_right_sketch.set_frame_data(tex_goblin_sketch, 5, 4, idx_list_right);

		max_hp = goblin_template.hp;
		max_speed = goblin_template.speed;
		damage = goblin_template.damage;
		reward_ratio = goblin_template.reward_ratio;
		recover_interval = goblin_template.recover_interval;
		recover_range = goblin_template.recover_range;
		recover_intensity = goblin_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;
	}

	~GoblinEnemy() = default;

};

//GoblinPriest类
class GoblinPriestEnemy : public Enemy
{
public:
	GoblinPriestEnemy()
	{
		static const ResourcesManager::TexturePool& texture_pool
			= ResourcesManager::instance()->get_texture_pool();
		static SDL_Texture* tex_goblin_priest = texture_pool.find(ResID::Tex_GoblinPriest)->second;
		static SDL_Texture* tex_goblin_priest_sketch = texture_pool.find(ResID::Tex_GoblinPriestSketch)->second;
		static ConfigManager::EnemyTemplate& goblin_priest_template = ConfigManager::instance()->goblin_priest_template;

		static const std::vector<int> idx_list_up = { 5, 6, 7, 8, 9 };
		static const std::vector<int> idx_list_down = { 0, 1, 2, 3, 4 };
		static const std::vector<int> idx_list_left = { 15, 16, 17, 18, 19 };
		static const std::vector<int> idx_list_right = { 10, 11, 12, 13, 14 };

		anim_up.set_loop(true); anim_up.set_interval(0.15);
		anim_up.set_frame_data(tex_goblin_priest, 5, 4, idx_list_up);
		anim_down.set_loop(true); anim_down.set_interval(0.15);
		anim_down.set_frame_data(tex_goblin_priest, 5, 4, idx_list_down);
		anim_left.set_loop(true); anim_left.set_interval(0.15);
		anim_left.set_frame_data(tex_goblin_priest, 5, 4, idx_list_left);
		anim_right.set_loop(true); anim_right.set_interval(0.15);
		anim_right.set_frame_data(tex_goblin_priest, 5, 4, idx_list_right);

		anim_up_sketch.set_loop(true); anim_up_sketch.set_interval(0.15);
		anim_up_sketch.set_frame_data(tex_goblin_priest_sketch, 5, 4, idx_list_up);
		anim_down_sketch.set_loop(true); anim_down_sketch.set_interval(0.15);
		anim_down_sketch.set_frame_data(tex_goblin_priest_sketch, 5, 4, idx_list_down);
		anim_left_sketch.set_loop(true); anim_left_sketch.set_interval(0.15);
		anim_left_sketch.set_frame_data(tex_goblin_priest_sketch, 5, 4, idx_list_left);
		anim_right_sketch.set_loop(true); anim_right_sketch.set_interval(0.15);
		anim_right_sketch.set_frame_data(tex_goblin_priest_sketch, 5, 4, idx_list_right);

		max_hp = goblin_priest_template.hp;
		max_speed = goblin_priest_template.speed;
		damage = goblin_priest_template.damage;
		reward_ratio = goblin_priest_template.reward_ratio;
		recover_interval = goblin_priest_template.recover_interval;
		recover_range = goblin_priest_template.recover_range;
		recover_intensity = goblin_priest_template.recover_intensity;

		size.x = 48, size.y = 48;
		hp = max_hp, speed = max_speed;

		timer_skill.set_wait_time(recover_interval);
	}

	~GoblinPriestEnemy() = default;

};