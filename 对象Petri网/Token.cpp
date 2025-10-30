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