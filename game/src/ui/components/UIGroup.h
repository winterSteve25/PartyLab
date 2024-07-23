#pragma once
#include "UIElement.h"

class UIGroup : public UIElement
{
public:
    explicit UIGroup(const sol::table& table);
    ~UIGroup() override;

    void Render(const lay_context* ctx) override;
    void ApplyDefaultStyles() override;

    void AddChild(const sol::table& table);
    void AddToLayout(lay_context* ctx, lay_id root) override;
private:
    std::vector<UIElement*> m_components;
};
