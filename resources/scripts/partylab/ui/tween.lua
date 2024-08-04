local m = {}

---@param from number
---@param to number
---@param duration number
---@return Tween
m.createLinear = function(from, to, duration)
    return Tween(from, to, duration, function(t) return t end)
end

---@param from number
---@param to number
---@param duration number
---@param ease fun(t: number): number
---@return Tween
m.create = function(from, to, duration, ease)
    return Tween(from, to, duration, ease)
end

return m