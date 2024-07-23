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
---@field ui UI
---@field render fun()
---@field renderOverlay fun()
---@field update fun()
local Scene = {}