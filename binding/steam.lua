---@class ChatType
m.chatTypes = {
    Invalid = 0,
    ChatMessage = 1,
    Typing = 2,
    InviteGame = 3,
    Emote = 4,
    LeftConversation = 6,
    Entered = 7,
    WasKicked = 8,
    WasBanned = 9,
    Disconnected = 10,
    HistoricalChat = 11,
    LinkBlocked = 14,
}

---@class ChatRoomEnterResponse
m.chatRoomEnterResponse = {
    Success = 1,
    DoesntExist = 2,
    NotAllowed = 3,
    Full = 4,
    Error = 5,
    Banned = 6,
    Limited = 7,
    ClanDisabled = 8,
    CommunityBan = 9,
    MemberBlockedYou = 10,
    YouBlockedMember = 11,
    RatelimitExceeded = 15,
}

m.awaits = {
    CB_AvatarImageLoaded = WaitFor(0)
}

local function idPredicate(steamID)
    return function(id)
        return id:equals(steamID)
    end
end

---@param steamID SteamID
---@return number handle to the image, can be used with UISteamImage 
---This is a coroutine
m.getAvatarLarge = function(steamID)
    return function()
        local handle = cpp_getSteamAvatar_Large(steamID)

        if handle == -1 then
            coroutine.yield(m.awaits.CB_AvatarImageLoaded, idPredicate(steamID))
            handle = cpp_getSteamAvatar_Large(steamID)
        end

        return handle
    end
end

---@param steamID SteamID
---@return number handle to the image, can be used with UISteamImage 
---This is a coroutine
m.getAvatarMedium = function(steamID)
    return function()
        local handle = cpp_getSteamAvatar_Medium(steamID)

        if handle == -1 then
            coroutine.yield(m.WaitFor_CB_AvatarImageLoaded, idPredicate(steamID))
            handle = cpp_getSteamAvatar_Medium(steamID)
        end

        return handle
    end
end

---@param steamID SteamID
---@return number handle to the image, can be used with UISteamImage 
---This is a coroutine
m.getAvatarSmall = function(steamID)
    return function()
        local handle = cpp_getSteamAvatar_Small(steamID)

        if handle == -1 then
            coroutine.yield(m.WaitFor_CB_AvatarImageLoaded, idPredicate(steamID))
            handle = cpp_getSteamAvatar_Small(steamID)
        end

        return handle
    end
end
