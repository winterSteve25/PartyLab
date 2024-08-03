---@meta types

---@class Vector2 Created by calling Vector2(x, y)
---@field x number
---@field y number
local Vector2 = {}

---@class Color Created by calling Color(r, g, b, a)
---@field r number
---@field g number
---@field b number
---@field a number
local Color = {}

---@class UI
---@field type string
---@field style Style
---@field children UI[]
local UI = {}

---@class Style
---@field hovered Style
---@field pressed Style
---
---@field width number
---@field height number
---
---@field margin number
---@field marginLeft number
---@field marginTop number
---@field marginRight number
---@field marginBottom number
---
---@field color Color
---@field backgroundColor Color
---
---@field fontSize number
---@field alignItems number use values from `api.layout.items`, can be OR-ed
---@field alignSelf number use values from `api.layout.self`, can be OR-ed
---
---@field transition table
local Style = {}

---@class Transition
---@field duration number
---@field ease string | fun(t: number): number
local Transition = {}

---@class ModDescriptor
---@field name string
---@field description string
---@field events table
local ModDescriptor = {}

---@class Scene
---@field load fun()
---@field cleanup fun()
---@field ui fun(): UI
---@field render fun()
---@field renderOverlay fun()
---@field update fun()
---@field events SceneEventHandler
local Scene = {}

---@class Size
---@field measure
---@field type 
local Size = {}

---@class SteamID Equality should be compared using SteamID:equals(other)
local SteamID = {}

---@class GameLobby
---@field getHost fun(): SteamID
---@field sendChatString fun(msg: string)
---@field getAllMembers fun(): SteamID[]
---@field setData fun(key: string, value: string)
---@field getData fun(key: string): string
local GameLobby = {}

---@return SteamID
function GameLobby:getHost() end 

---@param msg string
function GameLobby:sendChatString(msg) end

---@return SteamID[]
function GameLobby:getAllMembers() end

---@param key string
---@param value string
function GameLobby:setData(key, value) end

---@param key string
---@return string
function GameLobby:getData(key) end

---@class Packet
---@field serializer nil | fun(memory: MemoryWriter, data: any)
---@field deserializer nil | fun(memory: MemoryReader): any
---@field handler fun(sender: SteamID, data: any)
local Packet = {}

---@class SceneEventHandler
---@field playerEnteredLobby fun(user: SteamID)
---@field playerLeftLobby fun(user: SteamID)
---@field lobbyChatMsgReceived fun(sender: SteamID, msg: string, type: ChatType)
---@field enteredLobby fun()
---@field joinLobbyRequest fun(lobby: SteamID, friend: SteamID)
local SceneEventHandler = {}

---@class GlobalEventHandler
---@field addScenes fun(registrar: fun(scene: Scene)): number
---@field addPackets fun(registrar: fun(packet: Packet)): number
local GlobalEventHandler = {}

---@class MemoryWriter 
local MemoryWriter = {}

---@param num number
function MemoryWriter:writeI16(num) end

---@param num number
function MemoryWriter:writeI32(num) end

---@param num number
function MemoryWriter:writeF32(num) end

---@param num number
function MemoryWriter:writeF64(num) end

---@param str string
function MemoryWriter:writeString(str) end

---@param b boolean
function MemoryWriter:writeBool(b) end

---@class MemoryReader
local MemoryReader = {}

---@return number
function MemoryReader:readI16() end

---@return number
function MemoryReader:readI32() end

---@return number
function MemoryReader:readF32() end

---@return number
function MemoryReader:readF64() end

---@return string
function MemoryReader:readString() end

---@return boolean
function MemoryReader:readBool() end
