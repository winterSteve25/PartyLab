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
---@field id string
---@field type string
---@field data table
---@field style Style
---@field onDestroy fun(data: table)
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
---@field ui fun(data: UIData): UI
---@field render fun()
---@field renderOverlay fun()
---@field update fun()
---@field events SceneEventHandler
local Scene = {}

---@class UIData
---@field query fun(id: string): UIElementData
---@field rebuild fun()
local UIData = {}

---@class UIElementData
---@field data table
---@field remove fun()
local UIElementData = {}

---@class Size
---@field measure
---@field type 
local Size = {}

---@class SteamID Equality should be compared using SteamID:equals(other)
local SteamID = {}

---@class GameLobby
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

---@return number
function GameLobby:getMemberLimit() end

---@return number
function GameLobby:getMemberCount() end

---@class Packet
---@field serializer nil | fun(memory: MemoryWriter, data: any)
---@field deserializer nil | fun(memory: MemoryReader): any
---@field handler fun(sender: SteamID, data: any)
local Packet = {}

---@class GameMode
---@field name string
---@field description string
---@field iconLocation string
---@field maxPlayers number
---@field minPlayers number
---@field scene number
local GameMode = {}

---@class SceneEventHandler
---@field playerEnteredLobby fun(user: SteamID)
---@field playerLeftLobby fun(user: SteamID)
---@field lobbyChatMsgReceived fun(sender: SteamID, msg: string, type: ChatType)
---@field enteredLobby fun()
---@field joinLobbyRequest fun(lobby: SteamID, friend: SteamID)
local SceneEventHandler = {}

---@class GlobalEventHandler
---@field addScenes fun(registrar: fun(scene: Scene): number)
---@field addPackets fun(registrar: fun(packet: Packet): number)
---@field addGameModes fun(registrar: fun(gamemode: GameMode))
local GlobalEventHandler = {}

---@class MemoryWriter 
local MemoryWriter = {}

---@param num number
function MemoryWriter:writeUI8(num) end

---@param num number
function MemoryWriter:writeUI16(num) end

---@param num number
function MemoryWriter:writeUI32(num) end

---@param num number
function MemoryWriter:writeI8(num) end

---@param num number
function MemoryWriter:writeI16(num) end

---@param num number
function MemoryWriter:writeI32(num) end

---@param num number
function MemoryWriter:writeFloat(num) end

---@param num number
function MemoryWriter:writeDouble(num) end

---@param str string
function MemoryWriter:writeString(str) end

---@param b boolean
function MemoryWriter:writeBool(b) end

---@param o any
function MemoryWriter:writeObject(o) end

---@param id SteamID
function MemoryWriter:writeSteamID(o) end

---@class MemoryReader
local MemoryReader = {}

---@return number
function MemoryReader:readUI8() end

---@return number
function MemoryReader:readUI16() end

---@return number
function MemoryReader:readUI32() end

---@return number
function MemoryReader:readI8() end

---@return number
function MemoryReader:readI16() end

---@return number
function MemoryReader:readI32() end

---@return number
function MemoryReader:readFloat() end

---@return number
function MemoryReader:readDouble() end

---@return string
function MemoryReader:readString() end

---@return boolean
function MemoryReader:readBool() end

---@return any
function MemoryReader:readObject() end

---@return SteamID
function MemoryReader:readSteamID() end

---@class Tween
local Tween = {}
function Tween:update() end
function Tween:runForward() end
function Tween:runBackward() end
function Tween:reset() end
function Tween:get() end

---@class SyncedVar
local SyncedVar = {}

function SyncedVar:set(val) end
function SyncedVar:get() end
function SyncedVar:clearSubscribers() end

---@param user SteamID
function SyncedVar:update(user) end

---@param callback fun(newVal): void
---@return number
function SyncedVar:subscribe(callback) end

---@param index number
function SyncedVar:unsubscribe(index) end

---@class SyncedList : SyncedVar
local SyncedList = {}

function SyncedList:add(val) end
function SyncedList:remove(index) end
function SyncedList:clearAddSubscribers() end
function SyncedList:clearRemoveSubscribers() end

---@param callback fun(index, newVal): void
---@return number
function SyncedList:subscribeAdd(callback) end

---@param index number
function SyncedList:unsubscribeAdd(index) end

---@param callback fun(index, removedVal): void
---@return number
function SyncedList:subscribeRemove(callback) end

---@param index number
function SyncedList:unsubscribeRemove(index) end
