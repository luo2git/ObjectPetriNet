#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<unordered_map>
#include<utility>
using namespace std;
class Transition {
public:
    string trans_name;//��Ǩ����
    vector<string>pre_places;//ǰ�ÿ�������
    vector<string>post_places;//���ÿ�������
    unordered_map<string, vector<string>>fire_rule;//�������򣨺��ÿ���������Ӧ���ļ��ϣ�
};