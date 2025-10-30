#pragma once
#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include"rapidjson/istreamwrapper.h"
#include"Place.h"
#include"Transition.h"
#include"Petrinet.h"
#include<memory>
#include<tuple>
Petrinet petrinet;

constexpr auto Place_path = "./data/agv_way_place.json";
constexpr auto Transition_path = "./data/agv_way_trans.json";
constexpr auto Token_path = "./data/agv_way_token1_4.json";

//读取json文件，初始化库所信息
inline void read_place_json() {
    //流文件打开json文件
    std::ifstream ifs(Place_path);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
    }
    // 读取JSON数据
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    //是否解析成功
    if (doc.HasParseError()) {
        std::cerr << "Failed to parse JSON" << std::endl;
    }
    // 遍历每个对象
    for (auto itr1 = doc.MemberBegin(); itr1 != doc.MemberEnd(); ++itr1) {
        auto place_ptr = make_shared<Place>();
        place_ptr->place_name = itr1->name.GetString();
        place_ptr->capacity = itr1->value["capacity"].GetInt();
        place_ptr->delay = itr1->value["delay"].GetInt();
        for (int i = 0; i < itr1->value["pre_arcs"].GetArray().Size(); i++) {
            place_ptr->pre_arcs.emplace_back(itr1->value["pre_arcs"][i].GetString());
        }
        for (int i = 0; i < itr1->value["post_arcs"].GetArray().Size(); i++) {
            place_ptr->post_arcs.emplace_back(itr1->value["post_arcs"][i].GetString());
        }
        petrinet.places.emplace(place_ptr->place_name, place_ptr);
    }
}
//读取json文件，初始化变迁信息
inline void read_trans_json() {
    //流文件打开json文件
    std::ifstream ifs(Transition_path);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
    }
    // 读取JSON数据
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    //是否解析成功
    if (doc.HasParseError()) {
        std::cerr << "Failed to parse JSON" << std::endl;
    }
    // 遍历每个对象
    for (auto itr1 = doc.MemberBegin(); itr1 != doc.MemberEnd(); ++itr1) {
        auto trans_ptr = make_shared<Transition>();
        trans_ptr->trans_name = itr1->name.GetString();
        auto test = itr1->value.GetObject();
        auto test1 = test["pre_arcs"].GetObject().begin()->name.GetString();
        for (auto itr2 = itr1->value.GetObject()["pre_arcs"].GetObject().begin(); itr2 != itr1->value.GetObject()["pre_arcs"].GetObject().end(); ++itr2) {
            trans_ptr->pre_places.emplace_back(itr2->name.GetString());
            vector<string>arcs_temp;
            for (int i = 0; i < itr2->value.GetArray().Size(); i++) {
                arcs_temp.emplace_back(itr2->value.GetArray()[i].GetString());//此处的int可以作为后续程序维护升级，代表激发规则的类型
            }
            trans_ptr->fire_rule.emplace(itr2->name.GetString(), arcs_temp);
        }
        for (auto itr2 = itr1->value.GetObject()["post_arcs"].GetObject().begin(); itr2 != itr1->value.GetObject()["post_arcs"].GetObject().end(); ++itr2) {
            trans_ptr->post_places.emplace_back(itr2->name.GetString());
            vector<string>arcs_temp;
            for (int i = 0; i < itr2->value.GetArray().Size(); i++) {
                arcs_temp.emplace_back(itr2->value.GetArray()[i].GetString());
            }
            trans_ptr->fire_rule.emplace(itr2->name.GetString(), arcs_temp);
        }
        petrinet.transitions.emplace(trans_ptr->trans_name, trans_ptr);
    }
}
//读取json文件，初始化变迁信息
inline void read_tokens_json() {
    //流文件打开json文件
    std::ifstream ifs(Token_path);
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
    }
    // 读取JSON数据
    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    //是否解析成功
    if (doc.HasParseError()) {
        std::cerr << "Failed to parse JSON" << std::endl;
    }
    //auto test = doc["original_state"].GetObject().begin()->value["inplace"].GetString();
    for (auto itr1 = doc["original_state"].GetObject().begin(); itr1 != doc["original_state"].GetObject().end(); ++itr1) {
        auto token_ptr = make_shared<Token>();
        token_ptr->inplace = itr1->value["inplace"].GetString();
        token_ptr->waiting_time = itr1->value["waiting_time"].GetInt();
        token_ptr->token_attribute = itr1->value["token_attribute"].GetString();
        token_ptr->state = token_ptr->createToken(token_ptr->token_attribute);
        petrinet.m0.emplace(token_ptr->inplace, token_ptr);
    }
    for (auto itr1 = doc["target_state"].GetObject().begin(); itr1 != doc["target_state"].GetObject().end(); ++itr1) {
        auto token_ptr = make_shared<Token>();
        token_ptr->inplace = itr1->value["inplace"].GetString();
        token_ptr->token_attribute = itr1->value["token_attribute"].GetString();
        token_ptr->state = token_ptr->createToken(token_ptr->token_attribute);
        for (auto it = token_ptr->state.begin(); it != token_ptr->state.end(); ++it) {
            auto target_temp = make_tuple(token_ptr->inplace, it->first, it->second);
            petrinet.target_m.emplace_back(target_temp);
        }
    }
}