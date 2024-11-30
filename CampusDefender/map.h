//地图类

#ifndef _MAP_H_
#define _MAP_H_

#include"tile.h"
#include"route.h"

#include<SDL.h>
#include<string>
#include<fstream>
#include<sstream>

#include<unordered_map> //哈希表

class Map
{
public:
	typedef std::unordered_map<int, Route> SpawnerRoutePool; //刷怪路线池

public:
	Map() = default;
	~Map() = default;

	//加载地图文件函数
	bool load(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.good())
		{
			return false;
		}

		TileMap tile_map_temp;
		int idx_x = -1, idx_y = -1;

		//从上到下，从左到右读取地图数据 map.csv

		std::string str_line;
		while (std::getline(file, str_line))
		{
			//去除空格
			str_line = trim_str(str_line);
			//跳过空行
			if (str_line.empty())
			{
				continue;
			}
			idx_x = -1, idx_y++;
			tile_map_temp.emplace_back();
			std::string str_tile;
			std::stringstream str_stream(str_line);

			//读取地图格子数据，以","分隔
			while (std::getline(str_stream, str_tile, ','))
			{
				idx_x++;
				tile_map_temp[idx_y].emplace_back();
				Tile& tile = tile_map_temp[idx_y].back();
				load_tile_from_string(tile, str_tile);
			}

		}
		file.close();

		//地图数据为空
		if (tile_map_temp.empty()||tile_map_temp[0].empty())
		{
			return false;
		}

		tile_map = tile_map_temp; 

		generate_map_cache();

		return true;

	}

	//外部接口

	//获取数据文件行/列
	size_t get_width() const
	{
		if (tile_map.empty())
		{
			return 0;
		}
		return tile_map[0].size();
	}
	size_t get_height() const
	{
		return tile_map.size();
	}

	//获取tilemap
	const TileMap& get_tile_map() const
	{
		return tile_map;
	}

	//获取home坐标
	const SDL_Point& get_idx_home() const
	{
		return idx_home;
	}
	//获取怪物移动路径
	const SpawnerRoutePool& get_idx_spawner_pool() const
	{
		return spawner_route_pool;
	}

	//放置防御塔函数
	void place_tower(const SDL_Point& idx_tile)
	{
		tile_map[idx_tile.y][idx_tile.x].has_tower = true;
	}

private:
	TileMap tile_map;
	SDL_Point idx_home = { 0 };
	SpawnerRoutePool spawner_route_pool;
private:

	//截取数据，去除首尾空白字符
	std::string trim_str(const std::string& str)
	{
		size_t begin_idx = str.find_first_not_of(" \t");
		if (begin_idx == std::string::npos)
		{
			return "";
		}
		size_t end_idx = str.find_last_not_of(" \t");
		size_t idx_range = end_idx - begin_idx + 1;

		return str.substr(begin_idx, idx_range);
	}

	//处理地图字符串数据
	//字符串->整型
	void load_tile_from_string(Tile& tile, const std::string& str)
	{
		std::string str_tidy = trim_str(str);

		std::string str_value;
		std::vector<int> values;
		std::stringstream str_stream(str_tidy);

		while (std::getline(str_stream, str_value, '\\')) //数据以\分隔
		{
			int value;
			try
			{
				value = std::stoi(str_value); //字符串转整型
			}
			catch (const std::invalid_argument&)
			{
				value = -1;
			}
			values.push_back(value);
		}

		//处理异常数据
		tile.terrain = ((values.size() < 1 || values[0] < 0) ? 0 : values[0]);
		tile.decoration = ((values.size() < 2 ) ? -1 : values[1]);
		tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0) ? 0 : values[2]);
		tile.special_flag = ((values.size() <= 3) ? -1 : values[3]);

	}
	//生成地图缓存
	void generate_map_cache()
	{
		//遍历地图数据
		for (int y = 0; y < get_height(); y++)
		{
			for (int x = 0; x < get_width(); x++)
			{
				const Tile& tile = tile_map[y][x];
				//非特殊位置，继续
				if (tile.special_flag < 0)
				{
					continue;
				}
				//确定home坐标
				if (tile.special_flag == 0)
				{
					idx_home.x = x;
					idx_home.y = y;
				}
				else
				{
					//生成刷怪点路线池
					spawner_route_pool[tile.special_flag] = Route(tile_map,{x,y});
				}
			}
		}
	}
};
#endif // !_MAP_H_
