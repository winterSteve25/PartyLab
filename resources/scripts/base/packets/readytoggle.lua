---@type Packet
local m = {}

m.handler = function(sender, data)
    LobbyReadyStatus[data.who] = data.val
end

return m