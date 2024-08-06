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

local UIObject = require("partylab.ui.object")
local tween = require("partylab.ui.tween")
local rendering = require("partylab.rendering")
local layout = require("partylab.ui.layout")
local styles = require("partylab.ui.styles")
local colors = require("partylab.ui.colors")
local utils = require("partylab.utils")
local steam = require("partylab.steam")
local textStyle = {
    color = colors.backgroundColor,
    fontSize = 70,
    marginRight = 0.015,
    marginTop = 0.025,
    marginBottom = 0.025,
}

local function switchGameMode(data, increment)
    data.scroll = data.scroll + increment
    local network = require("partylab.network")
    network.sendPacketReliable(network.targets.Everyone, PACKETS.chooseGamemode, data.scroll)
end

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

                local isReady = LobbyData.ready[steamID.id]
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
        type = "scrollable",
        id = "playerList",
        style = {
            alignSelf = layout.self.LAY_FILL,
            alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
            clip = true,
        },
        children = {},
    }

    for _, v in pairs(currentLobby:getAllMembers()) do
        table.insert(lst.children, makePlayerUIItem(v))
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
                    local network = require("partylab.network")
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

local function enterOrLeaveLobbyMsg(player, left)
    local style = {
        marginLeft = 0,
        marginTop = 0,
        marginRight = 0,
        marginBottom = 0,
        alignSelf = layout.self.LAY_HFILL,
        marginTop = 0.03,
        marginLeft = 0.015,
        fontSize = 36,
        color = colors.backgroundColor:newA(200)
    }
    
    if left then
        return
        {
            type = "text",
            text = steam.getSteamUsername(player) .. " left the lobby",
            style = style,
        }
    else
        return
        {
            type = "text",
            text = steam.getSteamUsername(player) .. " entered the lobby",
            style = style,
        }
    end
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
            type = "scrollable",
            id = "chatList",
            style = {
                alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
                alignSelf = layout.self.LAY_FILL,
                clip = true,
            },
            children = {
                {
                    "There is nothing in chat",
                    style = {
                        color = colors.backgroundColor:newA(150),
                        marginTop = 0.03,
                    },
                }
            },
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
                    if string.len(text) == 0 then
                        return
                    end

                    local list = uiData.query("chatList")

                    if chatEmpty then
                        list.clearChildren()
                        chatEmpty = false
                    end

                    currentLobby:sendChatString(text)
                    list.addChild(makeChatUIItem(selfSteamID, text, steam.chatTypes.ChatMessage))
                    list.scrollToY(1.0)
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

local function makeGameModeSwitcher()
    return
    {
        id = "gameModesSelection",
        type = "rendered",
        data = {
            gameModes = require("partylab.core").getAllGameModes(),
            scroll = 1,
            loadedTextures = {}, -- should be in format: scrollIndex = texHandle
            textureLocations = {}, -- should be in format: texHandle = Vector2 pos
            textureLocTweens = {}, -- should be in format: texHandle = { x = x tween, y = y tween }
            textureSizes = {}, -- should be in format: texHandle = Vector2 size
            textureSizeTweens = {}, -- should be in format: texHandle = { x = x tween, y = y tween }
            arrowLeftObject = UIObject:new {
                tween = tween.create(1, 1.2, 0.15, require("partylab.ui.ease").easeOutExpo),
                onHoverEnter = function(self)
                    self.tween:runForward()
                end,
                onHoverExit = function(self)
                    self.tween:runBackward()
                end,
            },
            arrowRightObject = UIObject:new {
                tween = tween.create(1, 1.2, 0.15, require("partylab.ui.ease").easeOutExpo),
                onHoverEnter = function(self)
                    self.tween:runForward()
                end,
                onHoverExit = function(self)
                    self.tween:runBackward()
                end,
            },
        },
        onDestroy = function(thisData)
            for _, v in pairs(thisData.loadedTextures) do
                rendering.unloadTexture(v)
            end
        end,
        style = {
            alignSelf = layout.self.LAY_FILL,
        },
        render = function(pos, size, thisData)
            local amIhost = hostSteamID == selfSteamID

            if not amIhost then
                thisData.scroll = LobbyData.gameMode
            end

            ---@type GameMode[]
            local gameModes = thisData.gameModes
            local numbGameModes = table.getn(gameModes)
            local min = thisData.scroll - 1
            local max = thisData.scroll + 1

            -- unload unneeded textures
            for k, v in pairs(thisData.loadedTextures) do
                if k < min - 1 or k > max + 1 then
                    rendering.unloadTexture(v)
                    thisData.loadedTextures[k] = nil
                end
            end

            for _, v in pairs(thisData.textureLocTweens) do
                v.x:update()
                v.y:update()
            end

            for _, v in pairs(thisData.textureSizeTweens) do
                v.x:update()
                v.y:update()
            end

            rendering.beginScissor(pos, size);

            for i = min - 1, max + 1, 1 do
                if i > 0 and i <= numbGameModes then
                    -- load needed textures
                    if thisData.loadedTextures[i] == nil then
                        thisData.loadedTextures[i] = rendering.loadTexture(gameModes[i].iconLocation)
                    end

                    if i >= min and i <= max then
                        local offset = -(max - i) + 1
                        local abs = math.abs(offset)
                        local alpha = abs * 50;
                        local sizeMul = 1 - abs * 0.1
                        local length = size.x * 0.7 * sizeMul
                        local centerX = (size.x - length) * 0.5
                        local texture = thisData.loadedTextures[i]

                        local actualPos = Vector2(pos.x + centerX + offset * length * 1.15, pos.y - size.y * 0.1 + (size.y - length) * 0.5)
                        local actualSize = Vector2(length, length)
                        local prevPos = thisData.textureLocations[texture]
                        local prevSize = thisData.textureSizes[texture]

                        if prevPos ~= actualPos and thisData.textureLocTweens[texture] == nil then
                            if prevPos ~= nil then
                                local tx = tween.createLinear(prevPos.x, actualPos.x, 0.2)
                                tx:runForward()

                                local ty = tween.createLinear(prevPos.y, actualPos.y, 0.2)
                                ty:runForward()

                                tx:update()
                                ty:update()

                                thisData.textureLocTweens[texture] = { x = tx, y = ty }
                            end
                            thisData.textureLocations[texture] = actualPos
                        end

                        if prevSize ~= actualSize and thisData.textureSizeTweens[texture] == nil then
                            if prevSize ~= nil then
                                local tx = tween.createLinear(prevSize.x, actualSize.x, 0.2)
                                tx:runForward()

                                local ty = tween.createLinear(prevSize.y, actualSize.y, 0.2)
                                ty:runForward()

                                tx:update()
                                ty:update()

                                thisData.textureSizeTweens[texture] = { x = tx, y = ty }
                            end
                            thisData.textureSizes[texture] = actualSize
                        end

                        if thisData.textureLocTweens[texture] ~= nil then
                            actualPos.x = thisData.textureLocTweens[texture].x:get()
                            actualPos.y = thisData.textureLocTweens[texture].y:get()

                            if actualPos == thisData.textureLocations[texture] then
                                thisData.textureLocTweens[texture] = nil
                            end
                        end

                        if thisData.textureSizeTweens[texture] ~= nil then
                            actualSize.x = thisData.textureSizeTweens[texture].x:get()
                            actualSize.y = thisData.textureSizeTweens[texture].y:get()

                            if actualSize == thisData.textureSizes[texture] then
                                thisData.textureSizeTweens[texture] = nil
                            end
                        end

                        rendering.drawTextureWithTint(
                                texture,
                                actualPos,
                                actualSize,
                                Color(255, 255, 255, 255 - alpha)
                        )

                        -- click on game mode on the side switches it
                        if amIhost then
                            if offset ~= 0 then
                                local mp = rendering.getMousePosition()
                                -- dont question it
                                if (offset == 1 and mp.x > actualPos.x and mp.y > actualPos.y and mp.x < pos.x + size.x and mp.y < actualPos.y + length) or
                                        (offset == -1 and mp.x > pos.x and mp.y > actualPos.y and mp.x < actualPos.x + length and mp.y < actualPos.y + length)
                                then
                                    if rendering.isMouseLeftClicked() then
                                        switchGameMode(thisData, offset)
                                    end
                                end
                            end
                        end
                    end
                end
            end

            rendering.endScissor()

            local screenWidth = utils.getScreenWidth()
            local screenHeight = utils.getScreenHeight()
            local name = gameModes[thisData.scroll].name
            local fontSize = 70 * (screenWidth / 1920 + screenHeight / 1080) * 0.5
            local textSize = rendering.measureText(name, fontSize)
            local textPos = pos + Vector2((size.x - textSize.x) * 0.5, size.y * 0.85 - textSize.y)

            rendering.drawText(name, fontSize, textPos, colors.backgroundColor)
            if amIhost then

                if rendering.isMouseHovering(pos, size) then
                    if rendering.isMouseLeftClicked() then
                        local deltaM = -utils.signof(rendering.getMouseDelta().x)
                        if deltaM ~= 0 and thisData.scroll + deltaM <= numbGameModes and thisData.scroll + deltaM >= 1 then
                            switchGameMode(thisData, deltaM)
                        end
                    end

                    local deltaM = -utils.signof(rendering.getMouseWheel().y)
                    if deltaM ~= 0 and thisData.scroll + deltaM <= numbGameModes and thisData.scroll + deltaM >= 1 then
                        switchGameMode(thisData, deltaM)
                    end
                end

                local arrowSize = rendering.getTextureSize(arrowLeftTexture);
                local arrowNewSize = Vector2((arrowSize.x / arrowSize.y) * textSize.y, textSize.y)
                local origin = arrowNewSize * 0.5
                local arrowY = textSize.y * 0.35
                local leftArrPos = textPos + Vector2(-arrowNewSize.x - textSize.x * 0.3 - arrowNewSize.x * 0.5, arrowY + arrowNewSize.y * 0.4)
                local rightArrPos = textPos + Vector2(textSize.x * 1.42 + arrowNewSize.x * 0.5, arrowY + arrowNewSize.y * 0.4)

                if thisData.arrowLeftObject.onClick == nil then
                    thisData.arrowLeftObject.onClick = function(self)
                        if thisData.scroll > 1 then
                            switchGameMode(thisData, -1)
                        end
                    end
                end

                if thisData.arrowRightObject.onClick == nil then
                    thisData.arrowRightObject.onClick = function(self)
                        if thisData.scroll < numbGameModes then
                            switchGameMode(thisData, 1)
                        end
                    end
                end

                thisData.arrowLeftObject:update(leftArrPos - arrowNewSize * 0.5, arrowNewSize)
                thisData.arrowRightObject:update(rightArrPos - arrowNewSize * 0.5, arrowNewSize)

                rendering.drawTextureCustom(
                        arrowLeftTexture,
                        leftArrPos,
                        arrowNewSize * thisData.arrowLeftObject.tween:get(),
                        origin,
                        false,
                        false,
                        Color(255, 255, 255, 255)
                )
                rendering.drawTextureCustom(
                        arrowLeftTexture,
                        rightArrPos,
                        arrowNewSize * thisData.arrowRightObject.tween:get(),
                        origin,
                        true,
                        false,
                        Color(255, 255, 255, 255)
                )
            end
        end
    }
end

local function makeSettingsPanel()
    return
    {
        id = "settingsPanel",
        style = {
            alignSelf = layout.self.LAY_FILL,
            alignItems = bit32.bor(layout.items.LAY_COLUMN, layout.items.LAY_START),
        },
        {
            "Hallo",
            style = {
                color = colors.backgroundColor,
            },
        }
    }
end

---@return UI
local function leftPanel(data)
    return {
        id = "leftPanel",
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
                    require("partylab.core").transitionTo(SCENES.mainmenu)
                end,
            },
            {
                "Game Mode",
                style = utils.mergeTables(textStyle, {
                    alignSelf = layout.self.LAY_CENTER,
                    marginLeft = 0,
                    marginRight = 0,
                }),
            },
            --{
            --    type = "toggle",
            --    idleText = "Settings",
            --    toggledText = "Game Modes",
            --    style = utils.mergeTables(styles.defaultButton, {
            --        alignSelf = layout.self.LAY_RIGHT,
            --        color = colors.backgroundColor:newA(200),
            --        fontSize = 60,
            --        marginLeft = 0,
            --        marginTop = 0,
            --        marginBottom = 0,
            --        marginRight = 0.015,
            --        hovered = {
            --            color = colors.backgroundColor,
            --            fontSize = 66,
            --        },
            --        pressed = {
            --            fontSize = 70,
            --        }
            --    }),
            --    onToggle = function(val)
            --        if val then
            --            data.query("gameModesSelection").remove()
            --            data.query("leftPanel").addChild(makeSettingsPanel())
            --        else
            --            data.query("settingsPanel").remove()
            --            data.query("leftPanel").addChild(makeGameModeSwitcher())
            --        end
            --    end
            --},
        },
        makeGameModeSwitcher(),
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
        if sender == selfSteamID then
            return
        end

        local list = uiData.query("chatList")

        if chatEmpty then
            chatEmpty = false
            list.clearChildren()
        end

        list.addChild(makeChatUIItem(sender, msg, type))
        list.scrollToY(1.0)
    end,
    playerEnteredLobby = function(user)
        uiData.query("playerList").addChild(makePlayerUIItem(user))


        local list = uiData.query("chatList")

        if chatEmpty then
            chatEmpty = false
            list.clearChildren()
        end
        
        list.addChild(enterOrLeaveLobbyMsg(user, false))
        list.scrollToY(1.0)

        --- when a new player joins if you are the host, tell the new player who is ready and who isn't
        if hostSteamID ~= selfSteamID then
            return
        end

        local network = require("partylab.network")
        for _, v in pairs(currentLobby:getAllMembers()) do
            local isReady = LobbyData.ready[v.id]
            network.sendPacketToReliable(user, PACKETS.readyToggle, {
                who = v,
                val = isReady ~= nil and isReady
            })
        end

        -- which game mode is active
        network.sendPacketToReliable(user, PACKETS.chooseGamemode, LobbyData.gameMode)
    end,
    playerLeftLobby = function(user)
        local list = uiData.query("chatList")

        if chatEmpty then
            chatEmpty = false
            list.clearChildren()
        end
        
        list.addChild(enterOrLeaveLobbyMsg(user, true))
        list.scrollToY(1.0)
        
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
end

local time = 0

m.renderOverlay = function()
    local core = require("partylab.core")
    ---@type GameMode
    local gamemode = core.getAllGameModes()[LobbyData.gameMode]

    if not LobbyData.isEveryoneReady
            or currentLobby:getMemberCount() < gamemode.minPlayers
            or currentLobby:getMemberCount() > gamemode.maxPlayers
    then
        time = 0
        return
    end

    time = time + utils.getDeltaTime();
    local screenSize = Vector2(utils.getScreenWidth(), utils.getScreenHeight())
    rendering.drawRectangle(Vector2(0, 0), screenSize, Color(0, 0, 0, 180))
    local text = "Starting in "

    if time < 1 then
        text = text .. "3.."
    elseif time < 2 then
        text = text .. "2.."
    elseif time < 3 then
        text = text .. "1.."
    else
        core.transitionTo(gamemode.scene)
        text = "NOW!"
    end

    local pos = rendering.getCenter(rendering.measureText(text, 240), screenSize)
    rendering.drawText(text, 240, pos, colors.backgroundColor)
end

return m
