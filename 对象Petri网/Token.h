#pragma once
#include<iostream>
#include<string>
#include<memory>
#include<map>
using namespace std;


class Token {
public:
	string inplace;//���ڿ���
	int waiting_time;//Token�ڸÿ����ڵ��ѵȴ�ʱ��
	string token_attribute;//�п����ͣ�agv\workpiece\robot\agv_workpiece\action_in(agv+workpiece+robot)\action_out(agv+workpiece+robot)��
	map<string, int>state;//����״̬{�п����ͣ�����}

	map<string, int>createToken(string& token_attribute);//�����п�״̬,����m0
	map<string, int>inheritToken(map<string, int>last_state, const string& token_attribute);//�̳�ǰ�п�״̬������,��ʶ����
};