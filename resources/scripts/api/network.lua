local m = {}

---@class NetworkTarget
m.targets = {
    Everyone = 0,
    EveryoneElse = 1,
    Host = 2,
    Player = 3,
}

---@class NetworkFlags check Steam API for more information at https://partner.steamgames.com/doc/api/steamnetworkingtypes#message_sending_flags
m.flags = {
    Unreliable = 0,
    NoNagle = 1,
    UnreliableNoNagle = bit32.bor(0, 1),
    NoDelay = 4,
    UnreliableNoDelay = bit32.bor(0, 4, 1),
    Reliable = 8,
    ReliableNoNagle = bit32.bor(8, 1),
    UseCurrentThread = 16,
    AutoRestartBrokenSession = 32,
}

---@param target NetworkTarget
---@param type number
m.sendPacketReliable = function(target, type, data)
    cpp_sendPacket(target, type, data, m.flags.Reliable)
end

---@param target NetworkTarget
---@param type number
m.sendPacket = function(target, type, data)
    cpp_sendPacket(target, type, data, m.flags.Unreliable)
end

---@param player SteamID
---@param type number
m.sendPacketTo = function(player, type, data)
    cpp_sendPacketTo(player, type, data, m.flags.Unreliable)
end

---@param player SteamID
---@param type number
m.sendPacketToReliable = function(player, type, data)
    cpp_sendPacketTo(player, type, data, m.flags.Reliable)
end

---@param target NetworkTarget
---@param type number
---@param flags NetworkFlags
m.sendPacketWithFlags = function(target, type, data, flags) 
    cpp_sendPacket(target, type, data, flags)
end

---@param player SteamID
---@param type number
---@param flags NetworkFlags
m.sendPacketToWithFlags = function(player, type, data, flags) 
    cpp_sendPacketTo(player, type, data, flags)
end

return m