#pragma once
#include "ui/BuiltUI.h"
#include "ui/UIBuildInstruction.h"

class UIComponent
{
public:
    virtual ~UIComponent() = default;
    virtual UIBuildInstruction Build(lay_context* ctx, BuiltUI* built_ui) = 0;
};
