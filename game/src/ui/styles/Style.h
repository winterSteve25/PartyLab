#pragma once

#include "Attribute.h"
#include "layout.h"
#include "raylib.h"
#include "Size.h"
#include "Transition.h"

struct Style
{
    Attribute<Size> width;
    Attribute<Size> height;

    Attribute<Size> marginTop;
    Attribute<Size> marginRight;
    Attribute<Size> marginBottom;
    Attribute<Size> marginLeft;
    
    Attribute<Color> color;
    Attribute<Color> backgroundColor;
    
    Attribute<float> fontSize;
    Attribute<lay_box_flags> alignItems;
    Attribute<lay_layout_flags> alignSelf;

    sol::optional<sol::table> transitions;

    Style();
    Style(const sol::optional<sol::table>& table);

    Style InheritFrom(const Style& other) const;

    template <typename T>
    static sol::optional<T> GetOptionalField(const sol::optional<sol::table>& table, const std::string& field)
    {
        if (!table.has_value()) return sol::optional<T>(sol::nullopt);
        return table.value().get<sol::optional<T>>(field);
    }
};
