---@type Scene
local m = {}

local layout = require("api.ui.layout")
local styles = require("api.ui.styles")
local colors = require("api.ui.colors")
local utils = require("api.utils")
local steam = require("api.steam")
local textStyle = {
    color = colors.backgroundColor,
    fontSize = 70,
    marginRight = 0.015,
    marginTop = 0.025,
    marginBottom = 0.025,
}

local function makePlayerUIItem(steamID)
    return {
        style = {
            alignSelf = layout.self.LAY_HFILL,
            alignItems = bit32.bor(layout.items.LAY_ROW, layout.items.LAY_START),
            height = 0.08,
            backgroundColor = colors.foregroundColor,
            marginTop = 0.01,
        },
        {
            type = "steamImage",
            image = steam.getAvatarLarge(steam.getCurrentUserID()),
            style = {
                alignSelf = bit32.bor(layout.self.LAY_VFILL),
                ratio = 1,
            },
        },
        {
            "HAllo"
        }
    }
end

local function playerList(data)
    local lst = {
        id = "playerList",
        style = {
            alignSelf = layout.self.LAY_FILL,
            alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
            clip = true,
        },
    }
    
    -----@type GameLobby
    local lobby = steam.getCurrentLobby()

    for _,v in pairs(lobby:getAllMembers()) do
        table.insert(lst, makePlayerUIItem(v))
    end
    
    return {
        style = {
            height = 0.45,
            backgroundColor = colors.middlegroundColor,
            alignSelf = layout.self.LAY_HFILL,
            alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
        },
        {
            style = {
                backgroundColor = colors.foregroundColor,
                alignSelf = layout.self.LAY_HFILL,
                alignItems = bit32.bor(layout.items.LAY_ROW, layout.items.LAY_START),
            },
            {
                "Players",
                style = utils.mergeTables(textStyle, {
                    marginLeft = 0.015,
                })
            },
        },
        lst
    }
end

local function chat(data)
    return
    {
        style = {
            backgroundColor = colors.middlegroundColor,
            alignSelf = layout.self.LAY_FILL,
            alignItems = layout.items.LAY_COLUMN,
            marginTop = 0.02,
        },
        {
            style = {
                alignItems = layout.items.LAY_ROW,
                alignSelf = layout.self.LAY_FILL,
            },
        },
        {
            style = {
                alignSelf = bit32.bor(layout.self.LAY_HFILL, layout.self.LAY_BOTTOM),
                alignItems = bit32.bor(layout.items.LAY_ROW, layout.items.LAY_JUSTIFY),
                backgroundColor = colors.foregroundColor,
                height = 0.08,
            },
            {
                id = "chatTextField",
                type = "textField",
                placeholder = "Enter something in chat",
                style = {
                    alignSelf = bit32.bor(layout.self.LAY_HFILL, layout.self.LAY_VCENTER),
                    marginLeft = 0.015,
                    marginRight = 0.015,
                    color = colors.backgroundColor:a(100),
                    fontSize = 48,
                },
                onSubmit = function(text)
                    local lobby = steam.getCurrentLobby()
                    lobby:sendChatString(text)
                end,
            },
            {
                style = utils.mergeTables(styles.defaultButton, {
                    color = colors.backgroundColor:a(200),
                    alignSelf = layout.self.LAY_VCENTER,
                    marginRight = 0.015,
                    marginTop = 0,
                    fontSize = 55,
                    hovered = {
                        fontSize = 60,
                        color = colors.backgroundColor,
                    },
                    pressed = {
                        fontSize = 64,
                    },
                }),
                type = "button",
                text = "Send",
                onClick = function()
                    data.query("chatTextField").submit()
                end
            },
        },
    }
end

---@return UI
local function leftPanel(data)
    return {
        style = {
            alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
            alignSelf = bit32.bor(layout.self.LAY_VFILL, layout.self.LAY_LEFT),
            width = 0.4,
            backgroundColor = colors.middlegroundColor,
        },
        {
            style = {
                alignSelf = layout.self.LAY_HFILL,
                alignItems = bit32.bor(layout.items.LAY_LAYOUT),
                backgroundColor = colors.foregroundColor,
            },
            {
                type = "button",
                text = "Leave",
                style = utils.mergeTables(styles.defaultButton, {
                    alignSelf = layout.self.LAY_LEFT,
                    color = colors.backgroundColor:a(200),
                    fontSize = 60,
                    marginTop = 0,
                    marginBottom = 0,
                    marginRight = 0,
                    marginLeft = 0.015,
                    hovered = {
                        color = colors.backgroundColor,
                        fontSize = 66,
                    },
                    pressed = {
                        fontSize = 70,
                    }
                }),
                onClick = function()
                    cpp_leaveLobby()
                    require("api.core").transitionTo(BASE_SCENES.mainmenu)
                end,
            },
            {
                "Game Mode",
                style = utils.mergeTables(textStyle, {
                    alignSelf = layout.self.LAY_CENTER,
                }),
            },
            {
                type = "button",
                text = "Settings",
                style = utils.mergeTables(styles.defaultButton, {
                    alignSelf = layout.self.LAY_RIGHT,
                    color = colors.backgroundColor:a(200),
                    fontSize = 60,
                    marginLeft = 0,
                    marginTop = 0,
                    marginBottom = 0,
                    marginRight = 0.015,
                    hovered = {
                        color = colors.backgroundColor,
                        fontSize = 66,
                    },
                    pressed = {
                        fontSize = 70,
                    }
                }),
                onClick = function()
                    local playerList = data.query("playerList")
                    playerList.addChild(makePlayerUIItem(steam.getCurrentUserID()))
                end
            },
        },
        {
            type = "rendered",
            style = {},
            render = function()
                local settingsOpen = utils.getOr(data, "settings", false)

                if settingsOpen then
                else
                end
            end
        }
    }
end

m.ui = function(data)
    return {
        style = {
            alignItems = layout.items.LAY_ROW,
            alignSelf = layout.self.LAY_FILL,
            margin = 0.02,
        },
        leftPanel(data),
        {
            style = {
                alignSelf = layout.self.LAY_FILL,
                alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
                marginLeft = 30,
            },
            playerList(data),
            chat(data),
        }
    }
end

m.events = {
    lobbyChatMsgReceived = function(sender, msg, type)
        utils.info(sender)
        utils.info(msg)
        utils.info(type)
    end,
    playerEnteredLobby = function(user)
        utils.info("Player entered: " .. user)
    end,
    playerLeftLobby = function(user)
        utils.info("Player left: " .. user)
    end,
}

return m