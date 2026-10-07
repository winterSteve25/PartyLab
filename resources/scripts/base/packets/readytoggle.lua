---@type Packet
local m = {
    handler = function(sender, data)
        LobbyData.ready[data.who] = data.val
    end
}

return m
