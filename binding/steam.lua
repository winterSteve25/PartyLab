m.getAvatarLarge = function()
    local handle = cpp_getSteamAvatar_Large()

    if handle == -1 then
        return 0
    end
end