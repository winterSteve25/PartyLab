#include "BuiltUI.h"

BuiltUI::~BuiltUI()
{
    delete m_ctx;
}

BuiltUI::BuiltUI(lay_context* ctx) :
    m_ctx(ctx),
    m_instructions(std::vector<UIRunInstruction>()),
    m_buildInstructions(std::vector<UIBuildInstruction>())
{
    m_instructions.reserve(32);
    m_buildInstructions.reserve(32);
}

void BuiltUI::AddBuiltInstr(const UIBuildInstruction& instr)
{
    m_buildInstructions.push_back(instr);
}


void BuiltUI::Bake()
{
    for (UIBuildInstruction build : m_buildInstructions)
    {
        UIRunInstruction run(lay_get_rect(m_ctx, build.id), build.renderer, build.updater);
        m_instructions.push_back(run);
    }
}

void BuiltUI::Update()
{
    for (UIRunInstruction instr : m_instructions)
    {
        if (instr.updater == nullptr) continue;
        instr.updater(instr.rect);
    }
}

void BuiltUI::Render(Camera2D& camera)
{
    for (UIRunInstruction instr : m_instructions)
    {
        if (instr.renderer == nullptr) continue;
        instr.renderer(instr.rect);
    }
}


