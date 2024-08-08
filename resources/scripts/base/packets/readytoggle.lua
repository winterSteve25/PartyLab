---@type Packet
local m = {}

m.handler = function(sender, data)
    LobbyData.ready[data.who] = data.val
    
    local all = true
    for _, v in pairs(require("partylab.steam").getCurrentLobby():getAllMembers()) do
        local isReady = LobbyData.ready[v.id]
        if isReady == nil or not isReady then
            all = false
            break
        end
    end
    
    LobbyData.isEveryoneReady = all
end

return m