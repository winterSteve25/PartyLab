#pragma once
#include "UIGroup.h"

class UIScrollableGroup : public UIGroup
{
public:
    explicit UIScrollableGroup(const sol::table& table);
    void Update() override;
    void Render(const lay_context* ctx) override;
    sol::table CreateLuaObject(lua_State* L) override;
protected:
    void OnScrolled(const lay_context* ctx, float deltaX, float deltaY) override;
private:
    float m_targetScrollOffsetX;
    float m_targetScrollOffsetY;
    float m_minScrollX;
    float m_minScrollY;
    float m_scrollBufferLimit;
    bool m_overflowX;
    bool m_overflowY;

    float m_targetPercentageX;
    float m_targetPercentageY;
};
