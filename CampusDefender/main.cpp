//SDL�궨�壬��ֹmain������ͻ����������SDL��ǰ���壩
#define SDL_MAIN_HANDLED

//ͷ�ļ�
#include"game_manager.h"


int main(int argc,char **argv)
{
	return GameManager::instance()->run(argc, argv); //������Ϸ
}