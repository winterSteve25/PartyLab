#include "LuaUI.h"

#include <format>

#include "components/UIButton.h"
#include "components/UIGroup.h"
#include "components/UIRendered.h"
#include "components/UIScrollableGroup.h"
#include "components/UISteamImage.h"
#include "components/UIText.h"
#include "components/UITextField.h"
#include "components/UIToggleButton.h"
#include "core/Core.h"
#include "lua/lua_utils.h"

#define CONSTRUCT() lua_utils::UnwrapResult<sol::table>(m_uiSupplier(m_customData), "Failed to build UI by calling the constructor")

static std::unordered_map<std::string, std::function<UIElement*(const sol::table&)>> CONSTRUCTORS{
    {"button", [](const sol::table& table) { return new UIButton(table); }},
    {"text", [](const sol::table& table) { return new UIText(table); }},
    {"group", [](const sol::table& table) { return new UIGroup(table); }},
    {"scrollable", [](const sol::table& table) { return new UIScrollableGroup(table); }},
    {"rendered", [](const sol::table& table) { return new UIRendered(table); }},
    {"steamImage", [](const sol::table& table) { return new UISteamImage(table); }},
    {"textField", [](const sol::table& table) { return new UITextField(table); }},
    {"toggle", [](const sol::table& table) { return new UIToggleButton(table); }},
};

LuaUI::LuaUI(const sol::protected_function& supplier):
    m_uiSupplier(supplier),
    m_customData(sol::state::create_table(supplier.lua_state())),
    m_screenWidth(GetScreenWidth()),
    m_screenHeight(GetScreenHeight()),
    m_needsRebuild(false)
{
    m_customData["rebuild"] = [this]() { m_needsRebuild = true; };
    m_customData["query"] = [this](const std::string& id) -> sol::optional<sol::table>
    {
        for (UIElement* element : m_components)
        {
            auto v = element->Find(id);
            if (v.has_value()) return v.value()->CreateLuaObject(m_uiSupplier.lua_state());
        }

        return sol::optional<sol::table>(sol::nullopt);
    };

    lay_init_context(&m_layCtx);
    lay_reserve_items_capacity(&m_layCtx, 64);

    ParseTable(&m_components, CONSTRUCT());
}

LuaUI::~LuaUI()
{
    for (int i = 0; i < m_components.size(); i++)
    {
        UIElement* pointer = m_components[i];
        delete pointer;
        m_components.erase(m_components.begin() + i);
        i--;
    }

    lay_destroy_context(&m_layCtx);
}

void LuaUI::Render()
{
    lay_reset_context(&m_layCtx);

    lay_id root = lay_item(&m_layCtx);
    lay_set_size_xy(&m_layCtx, root, static_cast<lay_scalar>(m_screenWidth),
                    static_cast<lay_scalar>(m_screenHeight));
    lay_set_behave(&m_layCtx, root, LAY_FILL);

    for (UIElement* renderable : m_components)
    {
        renderable->AddToLayout(&m_layCtx, root);
    }

    lay_run_context(&m_layCtx);

    for (UIElement* renderable : m_components)
    {
        renderable->AdjustLayout(&m_layCtx);
    }

    lay_run_context(&m_layCtx);

    for (UIElement* renderable : m_components)
    {
        renderable->Render(&m_layCtx);
    }
}

void LuaUI::Update()
{
    if (m_needsRebuild)
    {
        for (int i = 0; i < m_components.size(); i++)
        {
            UIElement* pointer = m_components[i];
            delete pointer;
            m_components.erase(m_components.begin() + i);
            i--;
        }

        ParseTable(&m_components, CONSTRUCT());
        m_needsRebuild = false;
    }

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

    int tscreenWidth = GetScreenWidth();
    int tscreenHeight = GetScreenHeight();

    if (tscreenWidth == m_screenWidth && tscreenHeight == m_screenHeight) return;

    m_screenWidth = tscreenWidth;
    m_screenHeight = tscreenHeight;

    for (UIElement* renderable : m_components)
    {
        renderable->ApplyDefaultStyles();
    }
}

void LuaUI::ParseTable(std::vector<UIElement*>* collection, const sol::table& table)
{
    sol::optional<std::string> type = table["type"];

    if (type.has_value())
    {
        if (!CONSTRUCTORS.contains(type.value()))
        {
            TraceLog(LOG_ERROR, std::format("Unknown UI component type: {}", type.value()).c_str());
            return;
        }

        CreateUIElement(collection, table, CONSTRUCTORS[type.value()]);
    }
    else
    {
        sol::optional<sol::object> first = table[1];
        if (first.has_value() && first.value().is<std::string>())
        {
            CreateUIElement(collection, table, [&first](const sol::table& table)
            {
                return new UIText(table, first.value().as<std::string>());
            });
        }
        else
        {
            UIGroup* group = dynamic_cast<UIGroup*>(CreateUIElement(collection, table, [](const sol::table& t)
            {
                return new UIGroup(t);
            }));

            table.for_each([group](auto pair)
            {
                sol::object key = pair.first;
                sol::object val = pair.second;
                if (key.is<std::string>()) return;
                group->AddChild(val);
            });
        }
    }
}

UIElement* LuaUI::CreateUIElement(
    std::vector<UIElement*>* collection,
    const sol::table& table,
    const std::function<UIElement*(const sol::table&)>& ctor
)
{
    UIElement* comp = ctor(table);
    comp->ApplyDefaultStyles();
    collection->push_back(comp);
    return comp;
}
