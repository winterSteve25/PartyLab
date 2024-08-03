---@type Scene
local m = {}

local colors = require("api.ui.colors")
local layout = require("api.ui.layout")
local styles = require("api.ui.styles")
local core = require("api.core")

local ui = {
    style = {
        alignItems = layout.items.LAY_COLUMN,
        alignSelf = layout.self.LAY_TOP_LEFT,
        marginTop = 0.2,
        marginLeft = 0.16,
    },
    {
        style = {
            alignItems = layout.items.LAY_ROW,
        },
        {
            "Party",
            style = {
                color = colors.textColor,
                fontSize = 200,
                marginRight = 0.02,
            },
        },
        {
            "Lab",
            style = {
                color = colors.accentColor,
                fontSize = 200,
            },
        },
    },
    {
        style = {
            alignItems = layout.items.LAY_COLUMN,
            alignSelf = layout.self.LAY_LEFT,
            marginTop = 0.05,
            marginLeft = 0.03,
        },
        {
            type = "button",
            text = "Join Game",
            style = styles.defaultButton,
            onClick = function()
            end
        },
        {
            type = "button",
            text = "Host Game",
            style = styles.defaultButton,
            onClick = function()
                TransitionData.lobbyStatus = LOBBY_STATUS.HOSTING
                core.transitionTo(SCENES.waitingLobby)
            end
        },
        {
            type = "button",
            text = "Quit",
            style = styles.defaultButton,
            onClick = function()
                core.exit()
            end
        },
    },
}

m.ui = function(data)
    return ui
end

m.events = {
    joinLobbyRequest = function(lobby, friend)
        TransitionData.lobbyStatus = LOBBY_STATUS.JOINING
        TransitionData.joining = lobby
        core.transitionTo(SCENES.waitingLobby)
    end
}

return m