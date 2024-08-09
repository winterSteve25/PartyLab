---@type Packet
local m = {}

m.handler = function(sender, data)
    LobbyData.ready[data.who] = data.val
end

return m