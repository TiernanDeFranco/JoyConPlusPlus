#pragma once
#include "shared_joycons.h"
#include <map>
#include <vector>
#include <memory>
#include <string>

void set_player_lamps_once(); // You must implement this elsewhere

std::map<std::wstring, std::shared_ptr<JoyCon>>& discover_all_joycons(
    std::map<std::wstring, std::shared_ptr<JoyCon>>& joycons,
    std::vector<std::wstring>& unbound,
    bool pairing = false
);
