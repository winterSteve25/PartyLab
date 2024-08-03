---@type Packet
local m = {}

m.handler = function(sender, data)
    LobbyReadyStatus[sender.id] = data
end

return m