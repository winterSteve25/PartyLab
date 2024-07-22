#include "LuaUI.h"

#include <format>
#include <map>

#include "components/UIButton.h"
#include "components/UIText.h"
#include "lua/LuaConstants.h"

static std::map<std::string, std::function<UIElement*(const sol::table&)>> CONSTRUCTORS{
    {"button", [](const sol::table& table) { return new UIButton(table); }}
};

LuaUI::LuaUI(const sol::table& table): UIElement(table)
{
    layCtx = new lay_context;
    lay_init_context(layCtx);
    lay_reserve_items_capacity(layCtx, 64);
    layParent = lay_item(layCtx);
    layId = layParent;
    lay_set_size_xy(layCtx, layParent, GetScreenWidth(), GetScreenHeight());
    lay_set_behave(layCtx, layParent, lay_layout_flags::LAY_FILL);
    Init(table);
    m_components.reserve(64);
    m_isMain = true;
}

LuaUI::LuaUI(lay_context* context, const sol::table& totalTable): UIElement(totalTable)
{
    layCtx = context;
    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();
    m_components.reserve(64);
    m_isMain = false;
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

    if (m_isMain)
    {
        lay_destroy_context(layCtx);
    }
}

void LuaUI::Render()
{
    UIElement::Render();
}

void LuaUI::RenderOverlay()
{
    UIElement::RenderOverlay();
    for (UIElement* renderable : m_components)
    {
        renderable->RenderOverlay();
    }
}

void LuaUI::Update()
{
    if (m_isMain)
    {
        lay_run_context(layCtx);
    }
    
    bool screenChanged = false;
    if (m_isMain)
    {
        int tscreenWidth = GetScreenWidth();
        int tscreenHeight = GetScreenHeight();

        if (tscreenWidth != m_screenWidth || tscreenHeight != m_screenHeight)
        {
            lay_set_size_xy(layCtx, layParent, tscreenWidth, tscreenHeight);
            m_screenWidth = tscreenWidth;
            m_screenHeight = tscreenHeight;
            screenChanged = true;
        }
    }

    for (UIElement* renderable : m_components)
    {
        renderable->Update();
        if (!screenChanged) continue;
        renderable->Init();
    }

    UIElement::Update();
}

void LuaUI::Init()
{
    UIElement::Init();
    for (UIElement* comp : m_components)
    {
        comp->Init();
    }
}

void LuaUI::Init(const sol::table& table)
{
    sol::optional<std::string> type = table[UI_PROP_TYPE];
    if (type.has_value())
    {
        if (!CONSTRUCTORS.contains(type.value()))
        {
            TraceLog(LOG_ERROR, std::format("Unknown UI component type: {}", type.value()).c_str());
            return;
        }

        CreateUIElement(table, CONSTRUCTORS[type.value()]);
    }
    else
    {
        sol::optional<sol::object> first = table[1];
        if (!first.has_value()) return;
        if (first.value().is<std::string>())
        {
            CreateUIElement(table, [&first](const sol::table& table)
            {
                return new UIText(table, first.value().as<std::string>());
            });
        }
        else
        {
            LuaUI* group = dynamic_cast<LuaUI*>(CreateUIElement(table, [this](const sol::table& t)
            {
                return new LuaUI(layCtx, t);
            }));

            for (const auto& pair : table.pairs())
            {
                const sol::object& key = pair.first;
                const sol::object& val = pair.second;
                if (key.is<std::string>()) continue;
                group->Init(val);
            }
        }
    }
}

UIElement* LuaUI::CreateUIElement(
    const sol::table& table,
    const std::function<UIElement*(const sol::table&)>& ctor
)
{
    UIElement* comp = ctor(table);
    lay_id newComp = lay_item(layCtx);
    comp->layCtx = layCtx;
    comp->layId = newComp;
    comp->layParent = layParent;
    lay_insert(layCtx, layParent, newComp);
    comp->Init();
    m_components.push_back(comp);
    return comp;
}
