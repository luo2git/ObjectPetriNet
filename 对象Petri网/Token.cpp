#include "Token.h"

inline int extractNumberFromString(const std::string attr, const std::string str) {
    size_t Pos = str.find(attr);
    size_t Numstart = Pos + attr.length();
    std::string numberStr;
    for (size_t i = Numstart; i < str.size(); ++i) {
        if (std::isdigit(str[i])) {
            numberStr += str[i];
        }
        else {
            break;
        }
    }
    // 将提取的数字字符串转换为整数
    int number = std::stoi(numberStr);
    return number;
}
map<string, int> Token::createToken(string& token_attribute)
{
    if (token_attribute.find("agv") != string::npos && token_attribute.find("workpiece") == string::npos && token_attribute.find("robot") == string::npos) {
        map<string, int>data_temp;
        data_temp.emplace("agv", extractNumberFromString("agv", token_attribute));
        token_attribute = "agv";
        return data_temp;
    }
    else if (token_attribute.find("control") != string::npos) {
        map<string, int>data_temp;
        data_temp.emplace("control", 1);
        return data_temp;
    }
    else if (token_attribute.find("agv") == string::npos && token_attribute.find("workpiece") != string::npos && token_attribute.find("robot") == string::npos) {
        map<string, int>data_temp;
        data_temp.emplace("workpiece", extractNumberFromString("workpiece", token_attribute));
        token_attribute = "workpiece";
        return data_temp;
    }
    else if (token_attribute.find("agv") == string::npos && token_attribute.find("workpiece") == string::npos && token_attribute.find("robot") != string::npos) {
        map<string, int>data_temp;
        data_temp.emplace("robot", extractNumberFromString("robot", token_attribute));
        token_attribute = "robot";
        return data_temp;
    }
    else if ((token_attribute.find("agv") != string::npos && token_attribute.find("workpiece") != string::npos && token_attribute.find("robot") == string::npos)) {
        map<string, int>data_temp;
        data_temp.emplace("agv", extractNumberFromString("agv", token_attribute));
        data_temp.emplace("workpiece", extractNumberFromString("workpiece", token_attribute));
        token_attribute = "agv_workpiece";
        return data_temp;
    }
    
}

map<string, int> Token::inheritToken(map<string, int> last_state, const string& token_attribute)
{
    map<string, int>state;
    if (token_attribute.find("agv") != string::npos && token_attribute.find("workpiece") == string::npos) {
        state.emplace("agv", last_state["agv"]);
    }
    else if (token_attribute.find("agv") == string::npos && token_attribute.find("workpiece") != string::npos) {
        state.emplace("workpiece", last_state["workpiece"]);

    }
    else if (token_attribute.find("agv") != string::npos && token_attribute.find("workpiece") != string::npos) {
        state.emplace("agv", last_state["agv"]);
        state.emplace("workpiece", last_state["workpiece"]);

    }
    else if (token_attribute.find("robot") != string::npos) {
        state.emplace("robot", last_state["robot"]);
    }
    else if (token_attribute.find("control") != string::npos) {
        state.emplace("control", 1);
    }
    else {
        state.emplace("agv", last_state["agv"]);
        state.emplace("workpiece", last_state["workpiece"]);
        state.emplace("robot", last_state["robot"]);
    }

    return state;
}

vector<shared_ptr<Place>> ProcessFlow::next_psble_marking(shared_ptr<Place>CurrentMarking)
{  vector<shared_ptr<Place>>next_marking;
   for (int i = 0; i < CurrentMarking->post_places.size(); ++i) {
        next_marking.push_back(CurrentMarking->post_places[i]); // 对每个元素执行操作
   }
   return next_marking;
}

void ProcessFlow::update_marking(vector<pair<string, shared_ptr<Device>>> transition_device, vector<pair<string, shared_ptr<Human>>> transition_human) {
    vector<shared_ptr<Place>>next_marking;
    next_marking = next_psble_marking(current_marking);
    vector<int> next_index;
    for (int i = 0; i < next_marking.size(); ++i) {
        if (next_marking[i]->devices(size) <= transition_device.size() & next_marking[i]->humans(size) <= transition_human.size()) {
            next_index.push_back(i);
        }
    }
    self.current_marking = next_marking[next_index[0]];

}