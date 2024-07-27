#include "UIGroup.h"

#include <format>

#include "layout.h"
#include "ui/LuaUI.h"

UIGroup::UIGroup(const sol::table& table):
    UIElement(table),
    m_scrollOffsetX(0),
    m_scrollOffsetY(0)
{
    m_components.reserve(4);

    sol::optional<sol::table> children = table["children"];
    if (!children.has_value()) return;

    table.for_each([this](auto pair)
    {
        sol::object key = pair.first;
        sol::object val = pair.second;
        if (key.is<std::string>()) return;
        AddChild(val);
    });
}

UIGroup::~UIGroup()
{
    for (int i = 0; i < m_components.size(); i++)
    {
        UIElement* pointer = m_components[i];
        delete pointer;
        m_components.erase(m_components.begin() + i);
        i--;
    }
}

void UIGroup::Update()
{
    for (const sol::table& t : m_queuedAddChild)
    {
        AddChild(t);
    }

    m_queuedAddChild.clear();

    for (UIElement* child : m_components)
    {
        child->Update();
    }
}

void UIGroup::Render(const lay_context* ctx)
{
    UIElement::Render(ctx);

    Vector2 xy = GetPos(ctx);
    Vector2 size = GetSize(ctx);
    BeginScissorMode(xy.x, xy.y, size.x, size.y);

    for (UIElement* renderable : m_components)
    {
        renderable->offsetX += m_scrollOffsetX;
        renderable->offsetY += m_scrollOffsetY;
        renderable->Render(ctx);
        renderable->offsetX -= m_scrollOffsetX;
        renderable->offsetY -= m_scrollOffsetY;
    }

    EndScissorMode();
}

void UIGroup::ApplyDefaultStyles()
{
    UIElement::ApplyDefaultStyles();
    for (UIElement* renderable : m_components)
    {
        renderable->ApplyDefaultStyles();
    }
}

sol::optional<UIElement*> UIGroup::Find(const std::string& id)
{
    auto par = UIElement::Find(id);
    if (par.has_value()) return par;

    for (UIElement* child : m_components)
    {
        auto c = child->Find(id);
        if (c.has_value()) return c;
    }

    return sol::optional<UIElement*>(sol::nullopt);
}

sol::table UIGroup::CreateLuaObject(lua_State* L)
{
    auto t = UIElement::CreateLuaObject(L);

    t["addChild"] = [this](const sol::table& table)
    {
        m_queuedAddChild.push_back(table);
    };

    return t;
}

void UIGroup::AddChild(const sol::table& table)
{
    LuaUI::ParseTable(&m_components, table);
}

void UIGroup::AddToLayout(lay_context* ctx, lay_id root)
{
    UIElement::AddToLayout(ctx, root);

    for (UIElement* child : m_components)
    {
        child->AddToLayout(ctx, id);
    }
}

void UIGroup::OnScrolled(const lay_context* ctx, float deltaX, float deltaY)
{
    float multiplier = 16.0f;
    
    lay_id lastChild = lay_last_child(ctx, id);

    // no children
    if (lastChild == LAY_INVALID_ID) return;
    
    lay_vec4 lastChildRect = lay_get_rect(ctx, lastChild);
    lay_vec4 lastChildMargin = lay_get_margins(ctx, lastChild);
    lay_vec4 thisRect = lay_get_rect(ctx, id);

    if (deltaX < 0 && lastChildRect[0] + lastChildRect[2] + m_scrollOffsetX + lastChildMargin[2] > thisRect[0] + thisRect[2])
    {
        m_scrollOffsetX += deltaX * multiplier;
    }

    if (deltaY < 0 && lastChildRect[1] + lastChildRect[3] + m_scrollOffsetY + lastChildMargin[3] > thisRect[1] + thisRect[3])
    {
        m_scrollOffsetY += deltaY * multiplier;
    }

    lay_id firstChild = lay_first_child(ctx, id);
    lay_vec4 firstChildRect = lay_get_rect(ctx, firstChild);
    lay_vec4 firstChildMargin = lay_get_margins(ctx, firstChild);
    
    if (deltaX > 0 && firstChildRect[0] + m_scrollOffsetX - firstChildMargin[0] < thisRect[0])
    {
        m_scrollOffsetX += deltaX * multiplier;
    }

    if (deltaY > 0 && firstChildRect[1] + m_scrollOffsetY - firstChildMargin[1] < thisRect[1])
    {
        m_scrollOffsetY += deltaY * multiplier;
    }
}
