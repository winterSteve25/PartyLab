#pragma once

enum LuaWaitForEvent : unsigned int;

class WaitFor
{
public:
    WaitFor(LuaWaitForEvent e);
    LuaWaitForEvent e;
};
