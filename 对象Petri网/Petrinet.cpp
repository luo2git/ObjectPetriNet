#include "Petrinet.h"

/*********************************************Ѱ�ҿɼ�����Ǩ********************************************/
vector<string> Petrinet::search_firable_transition(multimap<string, shared_ptr<Token>> m)
{
    vector<string>firable_trans;
    set<string>possible_firable_trans;
    //�洢��token�Ŀ�������ñ�Ǩ�����ǿɼ�����Ǩ�������м������possible_firable_trans���洢
    possible_firable_trans = Get_possible_firable_trans(m);
    //��possible_firable_trans���ÿ����Ǩ����ɸѡ(1.ǰ�ÿ�����Ҫ�л��϶�Ӧ���͵�token 2.���ÿ�����Ҫ��������Ҫ��)
    for (auto it = possible_firable_trans.begin(); it != possible_firable_trans.end(); ++it) {
        if (judge_possible_firable_trans(m, it->c_str())) {
            firable_trans.emplace_back(it->c_str());
        }
    }
    return firable_trans;
}
/****function::Get_possible_firable_trans****/
set<string> Petrinet::Get_possible_firable_trans(multimap<string, shared_ptr<Token>> m) {
    set<string>possible_firable_trans;
    for (auto itr = m.begin(); itr != m.end(); ++itr) {
        auto& place_name = itr->first;
        //for (auto post_trans : places[place_name]->post_arcs) {
        //    possible_firable_trans.emplace(post_trans);
        //}
    }
    return possible_firable_trans;
}
/****function::judge_possible_firable_trans****/
bool Petrinet::judge_possible_firable_trans(multimap<string, shared_ptr<Token>> m, string trans_name)
{
    //�жϸı�Ǩ��ǰ�ÿ����Ƿ���������
    //1.ǰ�ÿ����ж�Ӧ�����п�
    int a = 0;
    for (string pre_place_name : transitions[trans_name]->pre_places) {
        if (m.find(pre_place_name) == m.end()) {
            return false;
        }
    }
    //2.ǰ�ÿ����е��п�Ӧ�����㻡�϶���Ĺ���
    for (string pre_place_name : transitions[trans_name]->pre_places) {
        bool place_lock = false;
        for (int i = 0; i < transitions[trans_name]->fire_rule[pre_place_name].size(); ++i) {
            for (auto it = m.equal_range(pre_place_name).first; it != m.equal_range(pre_place_name).second; ++it) {
                if (it->second->token_attribute == transitions[trans_name]->fire_rule[pre_place_name][i]) {
                    place_lock = true;
                    break;
                }
                else if (it->second->token_attribute != transitions[trans_name]->fire_rule[pre_place_name][i] && i == transitions[trans_name]->fire_rule[pre_place_name].size() - 1) {
                    return false;
                }
            }
            if (place_lock == true) { break; }
        }
    }
    //3.���ÿ�����������Ҫ��
    for (string post_place_name : transitions[trans_name]->post_places) {
        if (places[post_place_name]->capacity == 1 && m.find(post_place_name) != m.end()) {
            return false;
        }
    }
    return true;
}

/****�������б��ʼ��****/
void Petrinet::list_initialization()
{
    shared_ptr<Node>node_initialization = make_shared<Node>();
    node_initialization->marking = m0;
    node_initialization->cost = 0;
    node_list.emplace(createKey(m0), node_initialization);
    open_list.emplace(node_initialization);
}
/****����������״̬keyֵ***/
string Petrinet::createKey(multimap<string, shared_ptr<Token>> m)
{
    string key = "";
    for (auto itr1 = m.begin(); itr1 != m.end(); ++itr1) {
        if (itr1->second->token_attribute != "control") {
            key = key + itr1->first;
            for (auto itr2 = itr1->second->state.begin(); itr2 != itr1->second->state.end(); ++itr2) {
                key = key + itr2->first + to_string(itr2->second);
            }
        }
    }
    return key;
}
/****�����������ɼ�����Ǩ,����½ڵ�***/
void Petrinet::fire_trans_get_newnode(shared_ptr<Node>expand_node_temp, shared_ptr<Node> new_node, string trans_name)
{
    //��ȥǰ�ÿ����е��п�ʱ��Ҫ�洢����������п���Ҫ�̳е���Ϣ
    map<string, int>state_temp;//����״̬{�п����ͣ�����}
    int arc_num = -1;//��ס�������͵ı�ţ�ǰ�ú�����һ��
    int lamda = 0;//�����ñ�Ǩ��õ�δ�仯���пϷ����ȴ�ʱ��ı仯
    //��ȥ�ñ�Ǩǰ�ÿ������п�
    for (string pre_place_name : transitions[trans_name]->pre_places) {
        auto& fire_rule = transitions[trans_name]->fire_rule;
        for (int i = 0; i < fire_rule[pre_place_name].size(); i++) {
            //
            if (expand_node_temp->marking.equal_range(pre_place_name).first->second->token_attribute == fire_rule[pre_place_name][i]) {
                arc_num = i;
            }
            else if (arc_num >= 0) { break; }
        }
        auto cut_token_ptr = expand_node_temp->marking.equal_range(pre_place_name).first;
        if (lamda < places[pre_place_name]->delay - cut_token_ptr->second->waiting_time) {
            lamda = places[pre_place_name]->delay - cut_token_ptr->second->waiting_time;
        }
        state_temp.insert(cut_token_ptr->second->state.begin(), cut_token_ptr->second->state.end());
        expand_node_temp->marking.erase(cut_token_ptr);
    }
    //m��δ�仯�пϵ�waiting_time�仯 lamda����
    for (auto itr = expand_node_temp->marking.begin(); itr != expand_node_temp->marking.end(); itr++) {
        itr->second->waiting_time = itr->second->waiting_time + lamda;
        if (itr->second->waiting_time > places[itr->second->inplace]->delay) {
            itr->second->waiting_time = places[itr->second->inplace]->delay;
        }
    }
    //���ÿ�������п�
    for (string post_place_name : transitions[trans_name]->post_places) {
        auto new_token = make_shared<Token>();
        new_token->inplace = post_place_name;
        new_token->waiting_time = 0;
        new_token->token_attribute = transitions[trans_name]->fire_rule[post_place_name][arc_num];
        //�̳�
        new_token->state = new_token->inheritToken(state_temp, new_token->token_attribute);
        expand_node_temp->marking.emplace(post_place_name, new_token);
        new_node->marking = expand_node_temp->marking;
    }
    new_node->cost = expand_node_temp->cost + lamda;
    pair<shared_ptr<Node>, string>father_node_action_temp;
    father_node_action_temp.first = expand_node;
    father_node_action_temp.second = trans_name;
    new_node->fathernode_action.emplace_back(father_node_action_temp);
}
/****�����������½ڵ��¾��жϺ�Ĵ���***/
void Petrinet::newnode_deal(shared_ptr<Node> new_node)
{
    bool judge_new_node_result = judage_new_node(new_node);
    if (judge_new_node_result == 0) {
        expand_node->son_action.emplace_back(new_node, Fire_tran);
        node_list.emplace(createKey(new_node->marking), new_node);
        open_list.emplace(new_node);
    }
    else if (judge_new_node_result == 2) {
        expand_node->son_action.emplace_back(new_node, Fire_tran);
        node_list.emplace(createKey(new_node->marking), new_node);
        open_list.emplace(new_node);
    }
}
/****�������¾ɽڵ��ж�(0:���ظ�״̬���½ڵ������ʱ�����ж��޷�ȷ���Ľڵ� 1:ʱ�����ж�Ϊ��ȫ�ظ��Ľڵ㣨��Ҫ��ס�ڵ���ڵġ�����ڵ㡱��������Ҫ��ߣ�2:ʱ�����ж�ȷ����ȫ�½ڵ㣨��Ҫ��ס�ڵ���ڵġ����ڵ㡱��������Ҫɾ����3:���ýڵ�)***/
int Petrinet::judage_new_node(shared_ptr<Node> new_node)
{
    string node_key = createKey(new_node->marking);
    if (node_list.count(node_key) == 0) {
        return 0;
    }
    else {
        //��ͬmarking��node
        vector<int>new_node_v = new_node->get_waitingtime();
        auto range = node_list.equal_range(node_key);
        for (auto it = range.first; it != range.second; ++it) {
            //����map�Դ�˳�������ܴ��ڲ��Ͻ��ĵط�
            vector<int>old_node_v = it->second->get_waitingtime();
            int big_time = 0;
            int equal_time = 0;
            for (int i = 0; i < new_node_v.size(); ++i) {
                if (new_node_v[i] - new_node->cost + it->second->cost > old_node_v[i]) {
                    big_time++;
                }
                else if (new_node_v[i] - new_node->cost + it->second->cost == old_node_v[i]) {
                    equal_time++;
                }
            }
            if (equal_time == new_node_v.size() && new_node->cost == it->second->cost) {
                pair<shared_ptr<Node>, string> new_fathernode_action;
                new_fathernode_action.first = expand_node;
                new_fathernode_action.second = Fire_tran;
                it->second->fathernode_action.emplace_back(new_fathernode_action);
                return 1;
            }
            else {
                if (big_time == new_node_v.size()) {
                    node_list.erase(it);
                    return 2;
                }
                else if (big_time == 0) {
                    return 3;
                }
            }
        }
    }
    return 0;
}
/***************************************dijkstra_search**********************************************************/
void Petrinet::dijskstra_search()
{
    list_initialization();
    while (is_dijkstra_continue()) {
        expand_node = open_list.top();
        open_list.pop();
        expand_node->id = expand_num;
        //cout << expand_node->cost << endl;
        auto expand_node_firable_trans = search_firable_transition(expand_node->marking);
        while (!expand_node_firable_trans.empty()) {
            expand_num++;
            //ָ�����ֻ�ǽ���ַָ��ͬһ����ַ
            shared_ptr<Node> expand_node_temp = make_shared<Node>();
            expand_node_temp->cost = expand_node->cost;
            for (auto itr = expand_node->marking.begin(); itr != expand_node->marking.end(); ++itr) {
                shared_ptr<Token>token_temp = make_shared<Token>();
                token_temp->waiting_time = itr->second->waiting_time;
                token_temp->inplace = itr->second->inplace;
                token_temp->token_attribute = itr->second->token_attribute;
                token_temp->state = itr->second->state;
                expand_node_temp->marking.emplace(token_temp->inplace, token_temp);
            }
            expand_node_temp->fathernode_action = expand_node->fathernode_action;
            shared_ptr<Node>new_node = make_shared<Node>();//����չ�����Ľڵ�Ĺ�������
            Fire_tran = expand_node_firable_trans[0];
            auto fire_tran = expand_node_firable_trans[0];
            expand_node_firable_trans.erase(expand_node_firable_trans.begin());
            fire_trans_get_newnode(expand_node_temp, new_node, fire_tran);
            newnode_deal(new_node);
        }
    }
}
/****������dijistra�Ƿ��������***/
bool Petrinet::is_dijkstra_continue()
{
    auto end_node = open_list.top();
    for (auto target_node : target_m) {
        string place_name;
        string token_attr;
        int num = 0;
        tie(place_name, token_attr, num) = target_node;
        if (end_node->marking.count(place_name) == 0) {
            return true;
        }
        else {
            auto range = end_node->marking.equal_range(place_name);
            int eauqal_time = 0;
            for (auto itr1 = range.first; itr1 != range.second; ++itr1) {
                if (itr1->second->state[token_attr] == num) {
                    eauqal_time++;
                }
            }
            if (eauqal_time == 0) {
                return true;
            }
        }
    }
    best_node = end_node;
    best_node->id = expand_num + 1;
    best_node->is_best = true;
    return false;
}

deque<tuple<multimap<string, shared_ptr<Token>>, string,int>> Petrinet::Createbestpath()
{
    deque<tuple<multimap<string, shared_ptr<Token>>, string,int>>path_temp;
    bool continue_flag_back = true;
    auto best_nextnode = make_shared<Node>();
    string best_tran;
    auto best_node_temp = best_node;
    while (continue_flag_back) {
        tuple<multimap<string, shared_ptr<Token>>, string,int>state_temp1;//״̬����Ǩ��gֵ
        for (auto& father_node : best_node_temp->fathernode_action) {
            if (father_node.first->marking == m0 && father_node.first->cost == 0) {
                continue_flag_back = false;
                break;
            }
        }
        int best_cost = 10000;
        for (auto& father_node : best_node_temp->fathernode_action) {
            if (father_node.first->cost < best_cost) {
                best_cost = father_node.first->cost;
                best_nextnode = father_node.first;
                best_tran = father_node.second;
            }
        }

        best_node_temp = best_nextnode;
        state_temp1 = make_tuple(best_node_temp->marking, best_tran,best_cost);
        path_temp.emplace_front(state_temp1);
        best_nextnode->is_best = true;
    }
    return path_temp;
}
