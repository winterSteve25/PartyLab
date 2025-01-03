#pragma once
#include "UIElement.h"

/**
 * A ui element that can be supplied a custom function to do the rendering
 */
class UIRendered : public UIElement
{
public:
    UIRendered(const sol::table& table);
    void Render(const lay_context* pos) override;

private:
    static const std::string fail_callback_txt;
    sol::protected_function m_renderer;
};
