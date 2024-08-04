local behave = {
    -- attachments (bit 5-8)
    -- fully valid when parent uses LAY_LAYOUT model
    -- partially valid when in LAY_FLEX model
    -- anchor to left item or left side of parent
    LAY_LEFT = 0x020,
    -- anchor to top item or top side of parent
    LAY_TOP = 0x040,
    -- anchor to right item or right side of parent
    LAY_RIGHT = 0x080,
    -- anchor to bottom item or bottom side of parent
    LAY_BOTTOM = 0x100,
    -- anchor to both left and right item or parent borders
    LAY_HFILL = 0x0a0,
    -- anchor to both top and bottom item or parent borders
    LAY_VFILL = 0x140,
    -- center horizontally, with left margin as offset
    LAY_HCENTER = 0x000,
    -- center vertically, with top margin as offset
    LAY_VCENTER = 0x000,
    -- center in both directions, with left/top margin as offset
    LAY_CENTER = 0x000,
    -- anchor to all four directions
    LAY_FILL = 0x1e0,
    -- When in a wrapping container, put this element on a new line.
    LAY_BREAK = 0x200,
    -- Top | Left
    LAY_TOP_LEFT = 96,
}

local layout = {
    -- flex-direction (bit 0+1)
    -- left to right
    LAY_ROW = 0x002,
    -- top to bottom
    LAY_COLUMN = 0x003,

    -- model (bit 1)

    -- free layout
    LAY_LAYOUT = 0x000,
    -- flex model
    LAY_FLEX = 0x002,

    -- flex-wrap (bit 2)
    -- single-line
    LAY_NOWRAP = 0x000,
    -- multi-line, wrap left to right
    LAY_WRAP = 0x004,

    -- justify-content (start, end, center, space-between)
    -- at start of row/column
    LAY_START = 0x008,
    -- at center of row/column
    LAY_MIDDLE = 0x000,
    -- at end of row/column
    LAY_END = 0x010,
    -- insert spacing to stretch across whole row/column
    LAY_JUSTIFY = 0x018
}

return {
    self = behave,
    items = layout,
}