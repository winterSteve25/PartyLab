#include "UIBox.h"

UIBox::UIBox(const lay_vec2 size, UIComponent* child) : m_child(child), m_size(size)
{
}

UIBox::~UIBox()
{
    delete m_child;
}

static void RenderBox(lay_vec4 box)
{
    DrawRectangle(box[0], box[1], box[2], box[3], RED);
}

UIBuildInstruction UIBox::Build(lay_context* context)
{
    lay_id id = lay_item(context);
    lay_set_size(context, id, m_size);
    return UIBuildInstruction(id, RenderBox, nullptr);
}
