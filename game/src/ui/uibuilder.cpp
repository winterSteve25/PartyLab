#include "uibuilder.h"

#include "BuiltUI.h"
#include "raylib.h"

namespace ui_builder
{
    BuiltUI Build(UIComponent& component)
    {
        lay_context ctx;
        BuiltUI built(&ctx);
        
        lay_init_context(&ctx);
        lay_reserve_items_capacity(&ctx, 32);
        
        lay_id root = lay_item(&ctx);
        lay_set_size_xy(&ctx, root, GetScreenWidth(), GetScreenHeight());
        Build(component, &ctx, &built, root);
        lay_run_context(&ctx);

        built.Bake();
        lay_destroy_context(&ctx);
        
        return built;
    }

    void Build(UIComponent& component, lay_context* ctx, BuiltUI* built_ui, lay_id root)
    {
        UIBuildInstruction instruction = component.Build(ctx, built_ui);
        lay_insert(ctx, root, instruction.id);
        built_ui->AddBuiltInstr(instruction);
    }
}
