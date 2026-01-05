#include "Login.h"
#include "json.hpp"


bool SendLoginRequest(const std::string &username, const std::string &password)
{
    // 构建登录请求的JSON数据
    nlohmann::json loginRequest;
    loginRequest["username"] = username;
    loginRequest["password"] = password;
    // 发送请求到服务器
    //SendToServer(loginRequest.dump()); // JSON 对象 → 字符串
    return false;
}