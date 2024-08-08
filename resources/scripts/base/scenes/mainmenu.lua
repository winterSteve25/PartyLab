---@type Scene
local m = {}

local utils = require("partylab.utils")
local colors = require("partylab.ui.colors")
local layout = require("partylab.ui.layout")
local styles = require("partylab.ui.styles")
local core = require("partylab.core")
local rendering = require("partylab.rendering")

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
            "Join Game",
            style = styles.defaultButton,
            onClick = function()
                core.transitionTo(SCENES.join)
            end
        },
        {
            "Host Game",
            style = styles.defaultButton,
            onClick = function()
                TransitionData.lobbyStatus = LOBBY_STATUS.HOSTING
                core.transitionTo(SCENES.waitingLobby)
            end
        },
        {
            "Quit",
            style = styles.defaultButton,
            onClick = function()
                core.exit()
            end
        },
    },
}

function m.ui(data)
    return ui
end

m.events = {
    joinLobbyRequest = function(lobby, friend)
        TransitionData.lobbyStatus = LOBBY_STATUS.JOINING
        TransitionData.joining = lobby
        core.transitionTo(SCENES.waitingLobby)
    end,
}

return m