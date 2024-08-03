---@type Scene
local m = {}

local uiData = nil
local lobbyReady = false

m.load = function()
    if TransitionData.lobbyStatus == LOBBY_STATUS.HOSTING then
        cpp_hostLobby()
    else
        cpp_joinLobby(TransitionData.joining)
    end
end

m.events = {}
m.events.enteredLobby = function(lobby, response)
    lobbyReady = true
end

m.update = function()
    if not lobbyReady then
        return
    end

    LobbyReadyStatus = {}
    require("api.core").transitionTo(SCENES.lobby)
end

m.ui = function(data)
    uiData = data
    local text = ""

    if TransitionData.lobbyStatus == LOBBY_STATUS.HOSTING then
        text = "Creating Lobby"
    else
        text = "Joining Lobby"
    end

    return
    {
        text,
        style = {
            alignSelf = require("api.ui.layout").self.LAY_CENTER,
            fontSize = 128,
        },
    }
end

return m