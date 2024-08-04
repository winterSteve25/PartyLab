---@class UIObject
---@field onHoverEnter fun()
---@field onHoverExit fun()
local UIObject = {
}

function UIObject:updateRect(pos, size)
    self.pos = pos
    self.size = size
    self.hovered = false
end

function UIObject:check()
    local isHoveringL = require("partylab.rendering").isMouseHovering(self.pos, self.size)

    if isHoveringL and not self.hovered then
        if self.onHoverEnter ~= nil then
            self.onHoverEnter()
        end
    end

    if not isHoveringL and self.hovered then
        if self.onHoverExit ~= nil then
            self.onHoverExit()
        end
    end
end

function UIObject:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

return UIObject