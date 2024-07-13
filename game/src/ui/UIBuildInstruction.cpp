#include "UIBuildInstruction.h"

UIBuildInstruction::UIBuildInstruction(const lay_id id, void(*renderer)(lay_vec4), void(*updater)(lay_vec4)): id(id), renderer(renderer), updater(updater)
{
}