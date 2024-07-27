#pragma once
#include <vector>

#include "layout.h"
#include "components/UIElement.h"

/**
 * A UI defined in lua via a table
 */
class LuaUI
{
public:
    LuaUI(const sol::protected_function& supplier);
    ~LuaUI();

    void Render();
    void Update();

    static void ParseTable(std::vector<UIElement*>* collection, const sol::table& table);
private:
    static UIElement* CreateUIElement(std::vector<UIElement*>* collection, const sol::table& table, const std::function<UIElement*(const sol::table&)>& ctor);
    
    std::vector<UIElement*> m_components;
    sol::protected_function m_uiSupplier;
    sol::table m_customData;
    
    int m_screenWidth;
    int m_screenHeight;
    lay_context m_layCtx;

    bool m_needsRebuild;
};
