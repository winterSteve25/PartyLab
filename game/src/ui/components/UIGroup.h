#pragma once
#include "UIElement.h"
#include "ui/properties/base/BooleanProperty.h"

class UIGroup : public UIElement
{
public:
    
    explicit UIGroup(const sol::table& table);
    ~UIGroup() override;

    void Update() override;
    void Render(const lay_context* ctx) override;
    void ApplyDefaultStyles() override;
    void ApplyStyles(const Style& style, bool doTransition) override;
    void AdjustLayout(lay_context* ctx) override;

    sol::optional<UIElement*> Find(const std::string& id) override;
    sol::table CreateLuaObject(lua_State* L) override;
    void AddOffset(float x, float y) override;

    void AddChild(const sol::table& table);
    void AddToLayout(lay_context* ctx, lay_id root) override;

protected:
    float m_scrollOffsetX;
    float m_scrollOffsetY;

private:
    std::vector<UIElement*> m_components;
    std::vector<sol::table> m_queuedAddChild;
    std::vector<int> m_queuedRemoveChild;
    std::vector<sol::protected_function> m_queuedRemoveChildPredicate;
    bool m_clearAllChildren;

    BooleanProperty m_enableClipping;
};
