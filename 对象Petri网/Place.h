#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<utility>
using namespace std;
class Place {
public:
	string place_name;//����������
	int delay;//������ʱ��
	int capacity;//��������
	vector<string>pre_arcs;//�������������ǰ�ñ�Ǩ֮��Ķ�Ӧ��ϵ
	vector<string>post_arcs;//������������ĺ��ñ�Ǩ֮��Ķ�Ӧ��ϵ
};