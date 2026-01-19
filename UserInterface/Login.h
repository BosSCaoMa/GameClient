#pragma once

#include <string>

bool SendLoginRequest(const std::string& username, const std::string& password);
bool SendRegisterRequest(const std::string& username, const std::string& password);