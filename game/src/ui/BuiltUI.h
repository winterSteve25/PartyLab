#pragma once
#include <vector>

#include "UIBuildInstruction.h"
#include "UIRunInstruction.h"
#include "core/Renderable.h"

class BuiltUI final : public Renderable
{
public:
    void Render(Camera2D& camera) override;
    void Update() override;
    void Bake();
    void AddBuiltInstr(const UIBuildInstruction& instr);
    
    ~BuiltUI() override;
    BuiltUI(lay_context* ctx);

private:
    // will be invalidated once the built ui has been baked
    lay_context* m_ctx;
    std::vector<UIRunInstruction> m_instructions;
    std::vector<UIBuildInstruction> m_buildInstructions;
};
