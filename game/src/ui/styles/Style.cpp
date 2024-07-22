#include "Style.h"

#include "lua/LuaConstants.h"

Style::Style():
    alignItems(Attribute(lay_box_flags::LAY_START, true)),
    alignSelf(Attribute(lay_layout_flags::LAY_LEFT, true))
{
}

Style::Style(const sol::optional<sol::table>& table): Style()
{
    if (!table.has_value()) return;
    {
        sol::optional<sol::object> w = GetOptionalField<sol::object>(table, UI_PROP_STYLE_WIDTH);
        width = Attribute(w.map_or([](const sol::object& o) { return Size(o); }, Size()), !w.has_value());
        sol::optional<sol::object> h = GetOptionalField<sol::object>(table, UI_PROP_STYLE_HEIGHT);
        height = Attribute(h.map_or([](const sol::object& o) { return Size(o); }, Size()), !h.has_value());
    }

    {
        sol::optional<sol::object> margin = GetOptionalField<sol::object>(table, UI_PROP_STYLE_MARGIN);

        Size ss;

        if (margin.has_value())
        {
            ss = Size(margin.value());
            marginLeft = Attribute(ss, false);
            marginRight = Attribute(ss, false);
            marginBottom = Attribute(ss, false);
            marginTop = Attribute(ss, false);
        }

        sol::optional<sol::object> l = GetOptionalField<sol::object>(table, UI_PROP_STYLE_MARGIN_LEFT);
        marginLeft = Attribute(l.map_or([](const sol::object& o) { return Size(o); }, Size(marginLeft.val)),
                               !(l.has_value() || margin.has_value()));

        sol::optional<sol::object> r = GetOptionalField<sol::object>(table, UI_PROP_STYLE_MARGIN_RIGHT);
        marginRight = Attribute(r.map_or([](const sol::object& o) { return Size(o); }, Size(marginRight.val)),
                                !(r.has_value() || margin.has_value()));

        sol::optional<sol::object> b = GetOptionalField<sol::object>(table, UI_PROP_STYLE_MARGIN_BOTTOM);
        marginBottom = Attribute(b.map_or([](const sol::object& o) { return Size(o); }, Size(marginBottom.val)),
                                 !(b.has_value() || margin.has_value()));

        sol::optional<sol::object> t = GetOptionalField<sol::object>(table, UI_PROP_STYLE_MARGIN_TOP);
        marginTop = Attribute(t.map_or([](const sol::object& o) { return Size(o); }, Size(marginTop.val)),
                              !(t.has_value() || margin.has_value()));
    }

    {
        sol::optional<Color> c = GetOptionalField<Color>(table, UI_PROP_STYLE_COLOR);
        color = Attribute(c.value_or(WHITE), !c.has_value());

        sol::optional<Color> bg = GetOptionalField<Color>(table, UI_PROP_STYLE_BACKGROUND_COLOR);
        backgroundColor = Attribute(bg.value_or(Color{0, 0, 0, 0}), !bg.has_value());
    }

    {
        sol::optional<float> fs = GetOptionalField<float>(table, UI_PROP_STYLE_FONTSIZE);
        fontSize = Attribute(fs.value_or(64), !fs.has_value());
    }

    {
        sol::optional<lay_box_flags> boxFlags = GetOptionalField<lay_box_flags>(table, UI_PROP_STYLE_ALIGNITEMS);
        alignItems = Attribute(boxFlags.value_or(lay_box_flags::LAY_START), !boxFlags.has_value());

        sol::optional<lay_layout_flags> layoutFlags = GetOptionalField<
            lay_layout_flags>(table, UI_PROP_STYLE_ALIGNSELF);
        alignSelf = Attribute(layoutFlags.value_or(lay_layout_flags::LAY_LEFT), !layoutFlags.has_value());
    }

    transitions = GetOptionalField<sol::table>(table, UI_PROP_STYLE_TRANSITIONS);
}

Style Style::InheritFrom(const Style& other) const
{
    Style style = *this;

    style.width.InheritFrom(other.width);
    style.height.InheritFrom(other.height);

    style.marginTop.InheritFrom(other.marginTop);
    style.marginBottom.InheritFrom(other.marginBottom);
    style.marginLeft.InheritFrom(other.marginLeft);
    style.marginRight.InheritFrom(other.marginRight);

    style.color.InheritFrom(other.color);
    style.backgroundColor.InheritFrom(other.backgroundColor);

    style.fontSize.InheritFrom(other.fontSize);
    style.alignItems.InheritFrom(other.alignItems);
    style.alignSelf.InheritFrom(other.alignSelf);

    if (!style.transitions.has_value())
    {
        style.transitions = other.transitions;
    }

    return style;
}
