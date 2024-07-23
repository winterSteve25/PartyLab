#include "UIGroup.h"

#include "ui/LuaUI.h"

UIGroup::UIGroup(const sol::table& table)
    : UIElement(table)
{
    m_components.reserve(8);

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

void UIGroup::Render(const lay_context* ctx)
{
    UIElement::Render(ctx);
    for (UIElement* renderable : m_components)
    {
        renderable->Render(ctx);
    }
}

void UIGroup::ApplyDefaultStyles()
{
    UIElement::ApplyDefaultStyles();
    for (UIElement* renderable : m_components)
    {
        renderable->ApplyDefaultStyles();
    }
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
