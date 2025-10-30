#pragma once
#include"Place.h"
#include"Transition.h"
#include"Token.h"
#include<set>
#include<queue>
/***************************************Node**********************************************************/
class Node {
public:
	multimap<string, shared_ptr<Token>>marking;//�ڵ��ʶ��unorderdmap(��ϣ)��map���������
	vector<pair<shared_ptr<Node>, string>>fathernode_action;//���ڵ㼰�伤����Ǩ����������
	vector<pair<shared_ptr<Node>, string>>son_action;//�ӽڵ㼰�伤����Ǩ
	bool is_best;//����·�����
	int cost;//����ʱ��
	int id;//�ڵ�id
	double h;//����ʽ����Ԥ��ֵ

	vector<int>get_waitingtime() {
		vector<int>waitingtime;
		for (auto it = marking.begin(); it != marking.end(); ++it) {
			if (it->second->token_attribute != "control") {
				waitingtime.emplace_back(it->second->waiting_time);
			}
			
		}
		return waitingtime;
	}
};
/***************************************Petrinet**********************************************************/
class Petrinet {
public:
	unordered_map<string, shared_ptr<Place>>places;//����������petri�������п����Ķ��󣬸�ʽΪ{"����id": ��������ָ��}
	unordered_map<string, shared_ptr<Transition>>transitions;//����������petri�������б�Ǩ�Ķ��󣬸�ʽΪ{ "��Ǩid": ��Ǩ����ָ�� }
	multimap<string, shared_ptr<Token>>m0;//��ʼ״̬{"inplace":�п�ָ��}
	vector<tuple<string, string, int>>target_m;//Ŀ��״̬{"inplace","attritube"}
	multimap<string, shared_ptr<Token>>new_marking;
	shared_ptr<Node>best_node;//����Ŀ���
	shared_ptr<Node>expand_node = make_shared<Node>();//��չ�ڵ�Ĺ�������
	string Fire_tran;//������Ǩ�洢
	int expand_num = 0;//��չ����

	deque<tuple<multimap<string, shared_ptr<Token>>, string,int>>bestpath;//����·��(״̬�ͼ�����Ǩ��gֵ)��˫�˶���

	vector<string>search_firable_transition(multimap<string, shared_ptr<Token>>m);//Ѱ�ҿɼ�����Ǩ  F1
	set<string>Get_possible_firable_trans(multimap<string, shared_ptr<Token>> m);//�洢��token�Ŀ�������ñ�Ǩ�����ǿɼ�����Ǩ�������м������possible_firable_trans���洢 F1-1
	bool judge_possible_firable_trans(multimap<string, shared_ptr<Token>> m, string trans_name);//��possible_firable_trans���ÿ����Ǩ����ɸѡ(1.ǰ�ÿ�����Ҫ�л��϶�Ӧ���͵�token 2.���ÿ�����Ҫ��������Ҫ��) F1-2

	multimap<string, shared_ptr<Node>>node_list;//�ڵ��

	struct CompareByCost {
		bool operator()(shared_ptr<Node>& p1, shared_ptr<Node>& p2) const {
			return (p1->cost) > (p2->cost);  // ������������
		}
	};
	priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, CompareByCost>open_list;//δ��չ�ڵ��

	void list_initialization();//�б��ʼ�� F3_1
	string createKey(multimap<string, shared_ptr<Token>>m);//������״̬��keyֵ F3_1_1
	void fire_trans_get_newnode(shared_ptr<Node> expand_node_temp, shared_ptr<Node> new_node, string trans_name);//�����ɼ�����Ǩ,����½ڵ� F3_2
	void newnode_deal(shared_ptr<Node> new_node);//�����½ڵ��¾��жϺ�Ĵ��� F3_3
	int judage_new_node(shared_ptr<Node> new_node);//�¾ɽڵ��ж� F3_3_1 (0:���ظ�״̬���½ڵ������ʱ�����ж��޷�ȷ���Ľڵ� 1:ʱ�����ж���ȫ�ظ��ڵ㣨��Ҫ��ס�ڵ���ڵġ�����ڵ㡱��������Ҫ��ߣ�2:ʱ�����ж�ȷ����ȫ�½ڵ㣨��Ҫ��ס�ڵ���ڵġ����ڵ㡱��������Ҫɾ����)

	/***************************************dijkstra_search**********************************************************/
	void dijskstra_search();//dijkstra�����㷨 F4
	bool is_dijkstra_continue();//dijistra�Ƿ�������� F4_1
	deque<tuple<multimap<string, shared_ptr<Token>>, string,int>>Createbestpath();
};
