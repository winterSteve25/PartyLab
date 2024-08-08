---@type Packet
local m = {}

m.deserializer = function(mem) end
m.serializer = function(mem, data) end
m.handler = function(sender, data) end

return m