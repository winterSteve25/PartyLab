#include "UIGroup.h"

#include <format>

#include "layout.h"
#include "lua/lua_utils.h"
#include "ui/LuaUI.h"
#include "ui/properties/properties.h"

UIGroup::UIGroup(const sol::table& table):
    UIElement(table),
    m_clearAllChildren(false),
    m_enableClipping(properties::CanClipProp()),
    m_scrollOffsetX(0),
    m_scrollOffsetY(0)
{
    m_components.reserve(4);

    sol::optional<sol::table> children = table["children"];
    if (!children.has_value()) return;
    children.value().for_each([this](auto pair)
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
    if (m_clearAllChildren)
    {
        for (int i = 0; i < m_components.size(); i++)
        {
            UIElement* pointer = m_components[i];
            delete pointer;
            m_components.erase(m_components.begin() + i);
            i--;
        }
        m_clearAllChildren = false;
    }

    for (const int& i : m_queuedRemoveChild)
    {
        if (i < 0 || i > m_components.size()) continue;
        UIElement* pointer = m_components[i];
        delete pointer;
        m_components.erase(m_components.begin() + i);
    }

    m_queuedRemoveChild.clear();

    for (const sol::protected_function& pred : m_queuedRemoveChildPredicate)
    {
        for (int i = 0; i < m_components.size(); i++)
        {
            UIElement* pointer = m_components[i];

            if (!lua_utils::UnwrapResult<bool>(pred(i, pointer->CreateLuaObject(pred.lua_state())),
                                               "Failed to run remove child predicate"))
                continue;

            delete pointer;
            m_components.erase(m_components.begin() + i);
            i--;
        }
    }

    m_queuedRemoveChildPredicate.clear();

    for (const sol::table& t : m_queuedAddChild)
    {
        AddChild(t);
    }

    m_queuedAddChild.clear();

    for (int i = 0; i < m_components.size(); i++)
    {
        UIElement* element = m_components[i];

        if (element->markedDead)
        {
            delete element;
            m_components.erase(m_components.begin() + i);
            i--;
            continue;
        }

        element->Update();
    }
}

void UIGroup::Render(const lay_context* ctx)
{
    Vector2 xy = GetPos(ctx);
    Vector2 size = GetSize(ctx);

    UIElement::Render(ctx);

    if (m_enableClipping.Get())
    {
        BeginScissorMode(xy.x, xy.y, size.x, size.y);
    }

    for (UIElement* renderable : m_components)
    {
        renderable->AddOffset(m_scrollOffsetX, m_scrollOffsetY);
        renderable->Render(ctx);
        renderable->AddOffset(-m_scrollOffsetX, -m_scrollOffsetY);
    }

    if (m_enableClipping.Get())
    {
        EndScissorMode();
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

void UIGroup::ApplyStyles(const Style& style, bool doTransition)
{
    UIElement::ApplyStyles(style, doTransition);
    m_enableClipping.Set(style, doTransition);
}

void UIGroup::AdjustLayout(lay_context* ctx)
{
    UIElement::AdjustLayout(ctx);
    for (UIElement* element : m_components)
    {
        element->AdjustLayout(ctx);
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

    return {sol::nullopt};
}

sol::table UIGroup::CreateLuaObject(lua_State* L)
{
    auto t = UIElement::CreateLuaObject(L);

    t["addChild"] = [this](const sol::table& table)
    {
        m_queuedAddChild.push_back(table);
    };

    t["clearChildren"] = [this]()
    {
        m_clearAllChildren = true;
    };

    t["removeChild"] = [this](const int& index)
    {
        m_queuedRemoveChild.push_back(index);
    };

    t["removeChildWithPredicate"] = [this](const sol::protected_function& predicate)
    {
        m_queuedRemoveChildPredicate.push_back(predicate);
    };

    return t;
}

void UIGroup::AddOffset(float x, float y)
{
    UIElement::AddOffset(x, y);
    for (UIElement* child : m_components)
    {
        child->AddOffset(x, y);
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