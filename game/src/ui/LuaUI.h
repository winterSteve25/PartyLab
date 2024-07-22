#pragma once
#include <vector>

#include "layout.h"
#include "components/UIElement.h"

class LuaUI : public UIElement
{
public:
    LuaUI(const sol::table& table);
    ~LuaUI() override;

    UIElement* CreateUIElement(const sol::table& table, const std::function<UIElement*(const sol::table&)>& ctor);

    void RenderOverlay() override;
    void Render() override;
    void Update() override;
    void Init() override;

private:
    LuaUI(lay_context* context, const sol::table& totalTable);
    void Init(const sol::table& table);

    std::vector<UIElement*> m_components;
    int m_screenWidth;
    int m_screenHeight;

    bool m_isMain;
};
