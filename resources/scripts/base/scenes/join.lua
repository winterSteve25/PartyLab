---@type Scene
local m = {}

local layout = require("partylab.ui.layout")
local utils = require("partylab.utils")
local styles = require("partylab.ui.styles")
local steam = require("partylab.steam")
local colors = require("partylab.ui.colors")
local ease = require("partylab.ui.ease")

local function getFriendItem(id)
    return
    {
        style = {
            color = colors.backgroundColor,
            alignSelf = layout.self.LAY_HFILL,
            alignItems = bit32.bor(layout.items.LAY_ROW, layout.items.LAY_START),
            height = 0.06,
            marginTop = 0.03,
            propagateEvents = true,
            transition = {
                height = Transition(90, ease.easeOutExpo)
            },
            hovered = {
                height = 0.07,
            },
        },
        {
            type = "steamImage",
            image = steam.getAvatarMedium(id),
            style = {
                alignSelf = bit32.bor(layout.self.LAY_VFILL, layout.self.LAY_LEFT),
                ratio = 1,
            },
        },
        {
            type = "text",
            text = steam.getSteamUsername(id),
            style = {
                marginLeft = 0.01,
                fontSize = 48,
                alignSelf = bit32.bor(layout.self.LAY_VCENTER, layout.self.LAY_LEFT),
            },
        },
        onClick = function()
            local lobby = steam.getFriendLobby(id)
            
            if lobby == nil then
                return
            end
            
            TransitionData.lobbyStatus = LOBBY_STATUS.JOINING
            TransitionData.joining = lobby
            require("partylab.core").transitionTo(SCENES.waitingLobby)
        end
    }
end

m.ui = function(data)
    local children = {}
    local anyone = false

    for _, v in pairs(steam.getFriends()) do
        if steam.isPlayingSameGame(v) then
            table.insert(children, getFriendItem(v))
            anyone = true
        end
    end

    if not anyone then
        table.insert(children, {
            "No one is playing at the moment",
        })
    end

    return
    {
        style = {
            alignSelf = layout.self.LAY_FILL,
            alignItems = layout.items.LAY_LAYOUT,
        },
        {
            "Back",
            style = utils.mergeTables(styles.defaultButton, {
                alignSelf = layout.self.LAY_TOP_LEFT,
                marginLeft = styles.defaultButton.marginTop,
                fontSize = 72,
                hovered = {
                    fontSize = 90,
                },
                pressed = {
                    fontSize = 100,
                },
            }),
            onClick = function()
                require("partylab.core").transitionTo(SCENES.mainmenu)
            end
        },
        {
            style = {
                alignItems = layout.items.LAY_COLUMN,
                alignSelf = layout.self.LAY_TOP,
                marginTop = 0.2,
                clip = true,
            },
            {
                "Join a Friend!",
                style = {
                    fontSize = 128,
                    marginBottom = 0.05,
                },
            },
            {
                type = "scrollable",
                style = {
                    alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
                    clip = true,
                    height = 0.5,
                    width = anyone and 0.23 or 0, -- anyone ? 0.23 : 0
                },
                children = children,
            }
        }
    }
end

return m