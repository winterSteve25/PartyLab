---@class UIObject
---@field onHoverEnter fun(self: UIObject)
---@field onHoverExit fun(self: UIObject)
---@field onClick fun(self: UIObject)
local UIObject = {
}

function UIObject:update(pos, size)
    if self.tween ~= nil then
        self.tween:update()
    end
    
    local rendering = require("partylab.rendering")
    local isHoveringL = rendering.isMouseHovering(pos, size)

    if isHoveringL and not self.hovered then
        self.hovered = true
        if self.onHoverEnter ~= nil then
            self:onHoverEnter()
        end
    end

    if not isHoveringL and self.hovered then
        self.hovered = false
        if self.onHoverExit ~= nil then
            self:onHoverExit()
        end
    end

    if isHoveringL and rendering.isMouseLeftClicked() then
        if self.onClick ~= nil then
            self.onClick()
        end
    end
end

---@return UIObject
function UIObject:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    self.hovered = false
    return o
end

return UIObject