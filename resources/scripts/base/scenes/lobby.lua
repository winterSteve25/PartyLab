---@type Scene
local m = {}

---@type UIData
local uiData = nil
local chatEmpty = true
local settingsOpen = false

local readyTexture = nil
local hostTexture = nil
local selfTexture = nil
local arrowLeftTexture = nil

---@type GameLobby
local currentLobby = nil
local hostSteamID = nil
local selfSteamID = nil

local flux = nil

local rendering = require("api.rendering")
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
        data = {
            steamID = steamID,
        },
        style = {
            alignSelf = layout.self.LAY_HFILL,
            alignItems = bit32.bor(layout.items.LAY_ROW, layout.items.LAY_START),
            height = 0.08,
            backgroundColor = colors.foregroundColor,
            marginTop = 0.01,
        },
        {
            type = "steamImage",
            image = steam.getAvatarLarge(steamID),
            style = {
                alignSelf = bit32.bor(layout.self.LAY_VFILL),
                ratio = 1,
                marginLeft = 0.015,
            },
        },
        {
            steam.getSteamUsername(steamID),
            style = {
                alignSelf = layout.self.LAY_VCENTER,
                color = colors.backgroundColor:newA(230),
                marginLeft = 0.015,
            },
        },
        {
            type = "rendered",
            data = {
                isSelf = steamID == selfSteamID,
            },
            style = {
                alignSelf = layout.self.LAY_FILL,
                marginTop = 0.025,
                marginBottom = 0.025,
                marginLeft = 0.01,
            },
            render = function(pos, size, data)
                local eachSize = size
                local sizeY = size.y

                local selfTextureSize = rendering.getTextureSize(selfTexture)
                local hostTextureSize = rendering.getTextureSize(hostTexture)
                local readyTextureSize = rendering.getTextureSize(readyTexture)

                local selfRatio = selfTextureSize.x / selfTextureSize.y
                local hostRatio = hostTextureSize.x / hostTextureSize.y
                local readyRatio = readyTextureSize.x / readyTextureSize.y

                local offset = 0
                local screenwidth = utils.getScreenWidth()

                if data.isSelf then
                    local newx = sizeY * selfRatio
                    rendering.drawTexture(selfTexture, pos, eachSize:newX(newx))
                    offset = offset + newx + screenwidth * 0.005
                end

                if hostSteamID == steamID then
                    rendering.drawTexture(hostTexture, pos + Vector2(offset, 0), eachSize:newX(sizeY * hostRatio))
                end

                local isReady = LobbyReadyStatus[steamID.id]
                if isReady ~= nil and isReady then
                    local newx = sizeY * readyRatio
                    rendering.drawTexture(readyTexture, pos + Vector2(size.x - newx - screenwidth * 0.015, 0), eachSize:newX(newx))
                end
            end
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

    for _, v in pairs(currentLobby:getAllMembers()) do
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
                alignItems = bit32.bor(layout.items.LAY_LAYOUT),
            },
            {
                "Players",
                style = utils.mergeTables(textStyle, {
                    marginLeft = 0.015,
                    marginRight = 0.015,
                    alignSelf = layout.self.LAY_LEFT,
                })
            },
            {
                type = "toggle",
                idleText = "Ready",
                toggledText = "Unready",
                style = utils.mergeTables(styles.defaultButton, {
                    alignSelf = layout.self.LAY_RIGHT,
                    color = colors.backgroundColor,
                    fontSize = 48,
                    marginLeft = 0.015,
                    marginTop = 0,
                    marginBottom = 0,
                    marginRight = 0.015,
                    hovered = {
                        color = colors.backgroundColor,
                        fontSize = 55,
                    },
                    pressed = {
                        fontSize = 60,
                    }
                }),
                onToggle = function(val)
                    local network = require("api.network")
                    network.sendPacketReliable(network.targets.Everyone, PACKETS.readyToggle, {
                        who = selfSteamID.id,
                        val = val
                    })
                end
            },
        },
        lst
    }
end

local function makeChatUIItem(sender, msg, type)
    return
    {
        style = {
            color = colors.backgroundColor,
            alignSelf = layout.self.LAY_HFILL,
            alignItems = bit32.bor(layout.items.LAY_ROW, layout.items.LAY_START),
            height = 0.06,
            marginTop = 0.03,
        },
        {
            type = "steamImage",
            image = steam.getAvatarMedium(sender),
            style = {
                alignSelf = bit32.bor(layout.self.LAY_VFILL, layout.self.LAY_LEFT),
                ratio = 1,
                marginLeft = 0.015,
            },
        },
        {
            type = "text",
            text = msg,
            style = utils.mergeTables(textStyle, {
                marginLeft = 0.01,
                marginRight = 0,
                marginTop = 0,
                marginBottom = 0,
                fontSize = 48,
                alignSelf = bit32.bor(layout.self.LAY_VCENTER, layout.self.LAY_LEFT)
            })
        }
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
            id = "chatList",
            style = {
                alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
                alignSelf = layout.self.LAY_FILL,
                clip = true,
            },
            {
                "There is nothing in chat",
                style = {
                    color = colors.backgroundColor:newA(150),
                    marginTop = 0.03,
                },
            }
        },
        {
            style = {
                alignSelf = bit32.bor(layout.self.LAY_HFILL, layout.self.LAY_BOTTOM),
                alignItems = bit32.bor(layout.items.LAY_ROW, layout.items.LAY_JUSTIFY),
                backgroundColor = colors.foregroundColor,
                height = 0.08,
                marginTop = 0.015,
            },
            {
                id = "chatTextField",
                type = "textField",
                placeholder = "Enter something in chat",
                style = {
                    alignSelf = bit32.bor(layout.self.LAY_HFILL, layout.self.LAY_VCENTER),
                    marginLeft = 0.015,
                    marginRight = 0.015,
                    color = colors.backgroundColor:newA(100),
                    fontSize = 48,
                },
                onSubmit = function(text)
                    local list = uiData.query("chatList")

                    if chatEmpty then
                        list.clearChildren()
                        chatEmpty = false
                    end

                    currentLobby:sendChatString(text)
                    list.addChild(makeChatUIItem(selfSteamID, text, steam.chatTypes.ChatMessage))
                end,
            },
            {
                style = utils.mergeTables(styles.defaultButton, {
                    color = colors.backgroundColor:newA(200),
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
                    color = colors.backgroundColor:newA(200),
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
                    require("api.core").transitionTo(SCENES.mainmenu)
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
                    color = colors.backgroundColor:newA(200),
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
                end
            },
        },
        {
            id = "gameModesSelection",
            type = "rendered",
            data = {
                gameModes = require("api.core").getAllGameModes(),
                scroll = 2,
                loadedTextures = {}, -- should be in format: scrollIndex = texHandle
            },
            style = {
                alignSelf = layout.self.LAY_FILL,
            },
            render = function(pos, size, thisData)
                ---@type GameMode[]
                local gameModes = thisData.gameModes
                local min = thisData.scroll - 1
                local max = thisData.scroll + 1

                -- unload unneeded textures
                for k, v in pairs(thisData.loadedTextures) do
                    if k < min or k > max then
                        rendering.unloadTexture(v)
                        thisData.loadedTextures[k] = nil
                    end
                end

                rendering.beginScissor(pos, size);

                for i = min, max, 1 do
                    if i > 0 and i <= table.getn(gameModes) then
                        local offset = -(max - i) + 1
                        local abs = math.abs(offset)
                        local alpha = abs * 50;
                        local sizeMul = 1 - abs * 0.1
                        local length = size.x * 0.7 * sizeMul
                        local centerX = (size.x - length) / 2
                        
                        -- load needed textures
                        if thisData.loadedTextures[i] == nil then
                            thisData.loadedTextures[i] = rendering.loadTexture(gameModes[i].iconLocation)
                        end

                        local texture = thisData.loadedTextures[i]
                        rendering.drawTextureWithTint(
                                texture,
                                Vector2(pos.x + centerX + offset * length * 1.15, pos.y - size.y * 0.1 + (size.y - length) / 2),
                                Vector2(length, length),
                                Color(255, 255, 255, 255 - alpha)
                        )
                    end
                end

                rendering.endScissor()

                local screenWidth = utils.getScreenWidth()
                local screenHeight = utils.getScreenHeight()
                
                local name = gameModes[thisData.scroll].name
                local fontSize = 70 * (screenWidth / 1920 + screenHeight / 1080) * 0.5
                local textSize = rendering.measureText(name, fontSize)
                local textPos = pos + Vector2((size.x - textSize.x) / 2, size.y * 0.85 - textSize.y)
                
                local arrowSize = rendering.getTextureSize(arrowLeftTexture);
                local arrowNewSize = Vector2((arrowSize.x / arrowSize.y) * textSize.y * 0.8, textSize.y * 0.8)
                local arrowY = textSize.y * 0.35
                
                rendering.drawText(name, fontSize, textPos, colors.backgroundColor);
                rendering.drawTexture(arrowLeftTexture, textPos + Vector2(-arrowNewSize.x - textSize.x * 0.5, arrowY), arrowNewSize)
                rendering.drawTextureFlip(arrowLeftTexture, textPos + Vector2(textSize.x * 1.5, arrowY), arrowNewSize, true, false)
            end
        }
    }
end

m.ui = function(data)
    uiData = data
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
        local list = uiData.query("chatList")

        if sender == selfSteamID then
            return
        end

        if chatEmpty then
            chatEmpty = false
            list.clearChildren()
        end

        list.addChild(makeChatUIItem(sender, msg, type))
    end,
    playerEnteredLobby = function(user)
        uiData.query("playerList").addChild(makePlayerUIItem(user))

        --- when a new player joins if you are the host, tell the new player who is ready and who isn't
        if hostSteamID ~= selfSteamID then
            return
        end

        local network = require("api.network")
        for _, v in pairs(currentLobby:getAllMembers()) do
            local isReady = LobbyReadyStatus[v.id]
            network.sendPacketToReliable(user, PACKETS.readyToggle, {
                who = v,
                val = isReady ~= nil and isReady
            })
        end
    end,
    playerLeftLobby = function(user)
        if user == hostSteamID then
            hostSteamID = currentLobby:getHost()
        end

        uiData.query("playerList").removeChildWithPredicate(function(index, item)
            return item.data.steamID == user
        end)
    end,
}

m.load = function()
    readyTexture = rendering.loadTexture("resources/check.png")
    hostTexture = rendering.loadTexture("resources/crown.png")
    selfTexture = rendering.loadTexture("resources/self.png")
    arrowLeftTexture = rendering.loadTexture("resources/arrow_left.png")
    
    currentLobby = steam.getCurrentLobby()
    hostSteamID = currentLobby:getHost()
    selfSteamID = steam.getCurrentUserID()
end

m.cleanup = function()
    rendering.unloadTexture(readyTexture)
    rendering.unloadTexture(hostTexture)
    rendering.unloadTexture(selfTexture)
    rendering.unloadTexture(arrowLeftTexture)
    
    currentLobby = nil
    hostSteamID = nil
    selfSteamID = nil
    
    flux = nil
end

m.update = function()
end

return m
