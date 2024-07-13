#pragma once
#include "layout.h"

struct UIBuildInstruction
{
    lay_id id;
    void(*renderer)(lay_vec4);
    void(*updater)(lay_vec4);
    UIBuildInstruction(lay_id id, void(*renderer)(lay_vec4), void(*updater)(lay_vec4));
};
