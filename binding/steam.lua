
m.CB_AvatarImageLoaded = WaitFor(0)

---@return number handle to the image, can be used with UISteamImage 
---This is a coroutine
m.getAvatarLarge = function()
    local handle = cpp_getSteamAvatar_Large()

    if handle == -1 then
        coroutine.yield(m.CB_AvatarImageLoaded)
        handle = cpp_getSteamAvatar_Large()
    end
    
    return handle
end

---@return number handle to the image, can be used with UISteamImage 
---This is a coroutine
m.getAvatarMedium = function()
    local handle = cpp_getSteamAvatar_Medium()

    if handle == -1 then
        coroutine.yield(m.CB_AvatarImageLoaded)
        handle = cpp_getSteamAvatar_Medium()
    end

    return handle
end

---@return number handle to the image, can be used with UISteamImage 
---This is a coroutine
m.getAvatarSmall = function()
    local handle = cpp_getSteamAvatar_Small()

    if handle == -1 then
        coroutine.yield(m.CB_AvatarImageLoaded)
        handle = cpp_getSteamAvatar_Small()
    end

    return handle
end
