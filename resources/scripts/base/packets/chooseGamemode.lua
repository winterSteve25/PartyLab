---@type Packet
local m = {}

m.handler = function(sender, data)
    LobbyData.gameMode = data
end

return m
