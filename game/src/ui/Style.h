#pragma once
#include "sol/sol.hpp"

/**
 * A wrapper around a lua table, contains helper methods to get default values of style properties of a UI component
 */
class Style
{
public:
    Style(lua_State* state, const sol::optional<sol::table>& table);
    Style InheritFrom(const Style& other) const;

    template <typename T>
    T GetOr(const std::string& key, const T& def) const
    {
        sol::optional<T> t = m_table[key];
        return t.value_or(def);
    }

    template <typename T>
    sol::optional<T> Get(const std::string& key) const
    {
        sol::optional<T> t = m_table[key];
        return t;
    }

    template <typename T>
    static sol::optional<T> GetOptionalField(const sol::optional<sol::table>& table, const std::string& field)
    {
        if (!table.has_value()) return sol::optional<T>(sol::nullopt);
        return table.value().get<sol::optional<T>>(field);
    }

private:
    Style(const sol::table& table);
    sol::table m_table;
};
