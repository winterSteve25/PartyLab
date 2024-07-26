#include "Style.h"

#include <format>

Style::Style(lua_State* state, const sol::optional<sol::table>& table):
    m_table(table.has_value() ? table.value() : sol::state::create_table(state))
{
}

Style::Style(const sol::table& table) : m_table(table)
{
}

Style Style::InheritFrom(const Style& other) const
{
    sol::table result = sol::state::create_table(m_table.lua_state());

    for (auto pair : other.m_table)
    {
        result[pair.first] = pair.second;
    }

    for (auto pair : m_table)
    {
        result[pair.first] = pair.second;
    }

    return Style(result);
}