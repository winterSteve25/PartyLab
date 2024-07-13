#pragma once
#include "BuiltUI.h"
#include "components/UIComponent.h"

namespace ui_builder
{
    BuiltUI Build(UIComponent& component);
    void Build(UIComponent& component, lay_context* ctx, BuiltUI* built_ui, lay_id root);
}
