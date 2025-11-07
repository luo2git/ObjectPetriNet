#pragma once
#include"Place.h"
#include"Transition.h"
#include<iostream>
#include<string>
#include<memory>
#include<map>
using namespace std;


class Token {
public:
	string inplace;//所在库所
	int waiting_time;//Token在该库所内的已等待时间
	string token_attribute;//托肯类型（agv\workpiece\robot\agv_workpiece\action_in(agv+workpiece+robot)\action_out(agv+workpiece+robot)）
	map<string, int>state;//库所状态{托肯类型：数量}

	map<string, int>createToken(string& token_attribute);//创造托肯状态,生成m0
	map<string, int>inheritToken(map<string, int>last_state, const string& token_attribute);//继承前托肯状态的属性,标识更新
};

class SpaceZone{
public:
	string ID;//身份标识
	map<string,vector<vector<vector<float>>>> boundary_position;//空间区域边界位置，(位置ID,3维向量（x,y,z）)表示
	map<string, vector<vector<vector<float>>>> in;//空间区域入口位置，(位置ID,3维向量（x,y,z）)表示
	map<string, vector<vector<vector<float>>>> out;//空间区域出口位置，(位置ID,3维向量（x,y,z）)表示
	map<string, shared_ptr<Place>>places;//库所状态{托肯类型：数量}

	map<string, int>space_zone(string& token_attribute);//生成空间区域
};

class Workpiece {
public:
	string ID;//身份标识
	ProcessFlow process_flow; // 工作流程<工序名称，所需设备名称>
	string current_process; // 当前工序
	map<string, vector<vector<vector<float>>>> position;//("位置",3维向量（x,y,z）)表示
	map<string, vector<vector<vector<float>>>> velocity;//(“速度”,3维向量（x,y,z）)表示
	map<string, vector<vector<vector<float>>>> acceleration;//("加速度",3维向量（x,y,z）)表示
	shared_ptr<SpaceZone> space_zone;//所处空间区域
	float time_in_space_zone; // 在空间区域内的停留时间
	shared_ptr<Place> place;//所处库所
	float time_in_place; // 在库所内的停留时间
	vector<pair<string, shared_ptr<Transition>>>schedule;//工作流程调度<工序名称，变迁对象指针>

	map<string, int>workpiece(string& token_attribute);//生成空间区域
};

class Device{
public:
	string ID;//身份标识
	shared_ptr<SpaceZone>SpaceZone; // 设备所处的空间区域
	map<string, vector<vector<vector<float>>>> Position;//("位置",3维向量（x,y,z）)表示
	map<string, vector<vector<vector<float>>>> Velocity;//(“速度”,3维向量（x,y,z）)表示
	map<string, vector<vector<vector<float>>>> Acceleration;//("加速度",3维向量（x,y,z）)表示
	shared_ptr<Place> Place;//所处库所
	shared_ptr<SpaceZone> SpaceZone;//所处空间区域
	float TimeInSpaceZone; // 在空间区域内的停留时间
	shared_ptr<Place> Place;//所处库所
	float TimeInPlace; // 在库所内的停留时间
	vector<pair<string, shared_ptr<Transition>>>schedule;//工作流程调度<工序名称，变迁对象指针>

	map<string, int>Dvc(string& token_attribute);//生成空间区域
};

class Human {
public:
	string ID;//身份标识
	shared_ptr<SpaceZone>SpaceZone; // 设备所处的空间区域
	map<string, vector<vector<vector<float>>>> position;//("位置",3维向量（x,y,z）)表示
	map<string, vector<vector<vector<float>>>> velocity;//(“速度”,3维向量（x,y,z）)表示
	map<string, vector<vector<vector<float>>>> acceleration;//("加速度",3维向量（x,y,z）)表示
	shared_ptr<Place> place;//所处库所
	shared_ptr<SpaceZone> space_zone;//所处空间区域
	float time_in_space_zone; // 在空间区域内的停留时间
	shared_ptr<Place> place;//所处库所
	float time_in_place; // 在库所内的停留时间
	vector<pair<string, shared_ptr<Transition>>>schedule;//工作流程调度<工序名称，变迁对象指针>

	map<string, int>human(string& token_attribute);//生成空间区域
};

class ProcessPlace {
public:
	string ID;//身份标识
	int delay;//库所的时延
	int capacity;//库所容量
	shared_ptr<SpaceZone>space_zone; // 加工所需空间区域
	vector<pair<string, shared_ptr<Device>>>devices;//加工所需设备<设备名称，设备对象指针>
	vector<pair<string, shared_ptr<Human>>>humans;//加工所需人力资源<人力资源名称，人力资源对象指针>

	vector<pair<string, shared_ptr<Transition>>>pre_transitions;//前置变迁集合<变迁名称，变迁对象指针>
	vector<pair<string, shared_ptr<Transition>>>post_transitions;//后置变迁集合<变迁名称，变迁对象指针>
};

class ProcessTransition {
public:
	string ID;//身份标识
	int delay;//库所的时延
	int capacity;//库所容量
	shared_ptr<SpaceZone>space_zone; // 加工所需空间区域

	vector<pair<string, shared_ptr<Transition>>>pre_places;//前置库所集合<库所名称，库所对象指针>
	vector<pair<string, shared_ptr<Transition>>>post_places;//后置库所集合<库所名称，库所对象指针>
};



class ProcessFlow {
public:
	unordered_map<string, shared_ptr<Place>>places;//保存的是这个petri网里所有库所的对象，格式为{"库所id": 库所对象指针}
	unordered_map<string, shared_ptr<Transition>>transitions;//保存的是这个petri网里所有变迁的对象，格式为{ "变迁id": 变迁对象指针 }
	unordered_map<string, shared_ptr<Place>>initial_marking;//初始标识{"inplace":托肯指针}
	unordered_map<string, shared_ptr<Place>>targe_tmarking;//目标标识{"inplace":托肯指针}
	shared_ptr<Place> current_marking;//当前标识
	vector<pair<string, shared_ptr<Place>>> process_path;//已处理路径<库所名称，库所对象指针>

	vector<shared_ptr<Place>>next_psble_marking(shared_ptr<Place>CurrentMarking);//寻找下一个可能的标识
	void update_marking(vector<pair<string, shared_ptr<Device>>> transition_device,vector<pair<string, shared_ptr<Human>>> transition_human);//更新下一个标识的指针
}