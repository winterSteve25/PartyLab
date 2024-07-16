#pragma once
#include <string>

#include "LuaMod.h"

class LuaHook
{
public:
    std::optional<std::shared_ptr<LuaMod>> LoadMod(const std::string& name);
};
