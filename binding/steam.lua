
m.awaits = {
    CB_AvatarImageLoaded = WaitFor(0)
}

local function idPredicate(steamID)
    return function(id) 
        return id == steamID
    end
end

---@param steamID string
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

---@param steamID string
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

---@param steamID string
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
