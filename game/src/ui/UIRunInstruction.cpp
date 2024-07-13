#include "UIRunInstruction.h"

UIRunInstruction::UIRunInstruction(lay_vec4 rect, void (*renderer)(lay_vec4), void(*updater)(lay_vec4)) : rect(rect), renderer(renderer), updater(updater)
{
}

