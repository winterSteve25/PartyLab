#pragma once
#include "UIComponent.h"

class UIBox : public UIComponent
{
public:
    /**
     * Creates the UIBox
     * @param size Desired size of the box
     * @param child Child object, this object will manage the memory
     */
    UIBox(lay_vec2 size, UIComponent* child);
    ~UIBox() override;
    UIBuildInstruction Build(lay_context* context) override;
private:
    UIComponent* m_child;
    lay_vec2 m_size;
};
