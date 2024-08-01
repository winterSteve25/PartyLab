---@type Scene
local m = {}
local lobbyReady = false

m.load = function()
    cpp_hostLobby()
end

m.events = {}
m.events.enteredLobby = function(lobby, response)
    lobbyReady = true
end

m.update = function()
    if not lobbyReady then
        return
    end

    require("api.core").transitionTo(BASE_SCENES.lobby)
end

m.ui = function(data)
    return
    {
        "Creating Lobby",
        style = {
            alignSelf = require("api.ui.layout").self.LAY_CENTER,
            fontSize = 128,
        },
    }
end

return m