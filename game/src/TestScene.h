#include "layout.h"
#include "core/Scene.h"

class TestScene : public Scene
{
public:
    void Load() override
    {
        lay_init_context(&ctx);
        lay_reserve_items_capacity(&ctx, 32);
    }

    void Cleanup() override
    {
        lay_destroy_context(&ctx);
    }

    void RenderOverlay() override
    {
        lay_reset_context(&ctx);
        
        lay_id root = lay_item(&ctx);
        lay_id whole = lay_item(&ctx);
        lay_id p1 = lay_item(&ctx);
        lay_id p2 = lay_item(&ctx);

        lay_set_size_xy(&ctx, root, GetScreenWidth(), GetScreenHeight());
        lay_set_size_xy(&ctx, p1, 200, 200);
        lay_set_size_xy(&ctx, p2, 100, 200);

        lay_set_behave(&ctx, root, LAY_FILL);

        lay_set_behave(&ctx, whole, LAY_LEFT | LAY_TOP);
        lay_set_contain(&ctx, whole, LAY_ROW);

        lay_set_margins_ltrb(&ctx, whole, 100, 40, 0, 0);
        lay_set_margins_ltrb(&ctx, p1, 20, 20, 0, 20);
        
        lay_insert(&ctx, whole, p1);
        lay_insert(&ctx, whole, p2);
        lay_insert(&ctx, root, whole);
        lay_run_context(&ctx);

        auto w = lay_get_rect(&ctx, whole);
        auto pp1 = lay_get_rect(&ctx, p1);
        auto pp2 = lay_get_rect(&ctx, p2);
        
        DrawRectangle(w[0], w[1], w[2], w[3], BLACK);
        DrawRectangle(pp1[0], pp1[1], pp1[2], pp1[3], RED);
        DrawRectangle(pp2[0], pp2[1], pp2[2], pp2[3], GREEN);
    }

    void Update() override
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            Core::INSTANCE->transitionManager.TransitionTo(1);
        }
    }

    lay_context ctx;
};
