#pragma once
#include <vector>

#include "layout.h"
#include "components/UIElement.h"

class LuaUI
{
public:
    LuaUI(const sol::table& table);
    ~LuaUI();

    void Render();
    void Update();

    static UIElement* CreateUIElement(std::vector<UIElement*>* collection, const sol::table& table, const std::function<UIElement*(const sol::table&)>& ctor);
    static void ParseTable(std::vector<UIElement*>* collection, const sol::table& table);
private:
    std::vector<UIElement*> m_components;
    int m_screenWidth;
    int m_screenHeight;
    lay_context m_layCtx;
};
