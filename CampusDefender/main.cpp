//SDL宏定义，防止main命名冲突（需在引入SDL库前定义）
#define SDL_MAIN_HANDLED

//头文件
#include"game_manager.h"


int main(int argc,char **argv)
{
	return GameManager::instance()->run(argc, argv); //运行游戏
}