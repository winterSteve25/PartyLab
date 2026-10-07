---@type Packet
local m = {
    deserializer = function(mem) end,
    serializer = function(mem, data) end,
    handler = function(sender, data) end
}

return m
