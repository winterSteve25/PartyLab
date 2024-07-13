#pragma once
#include "layout.h"

struct UIRunInstruction
{
    lay_vec4 rect;
    void(*renderer)(lay_vec4);
    void(*updater)(lay_vec4);
    UIRunInstruction(lay_vec4 rect, void(*renderer)(lay_vec4), void(*updater)(lay_vec4));
};
