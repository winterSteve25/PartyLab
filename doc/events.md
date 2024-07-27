# Events
Events are at the core of mods, they allow mods to hook into the process of 
the game and interact with it.

## Add Scenes Event
- Named `addScenes`
- Handler Signature `(registrar: (Scene) -> void) -> void`

the `registrar` is a function that takes in a Scene object and registers it,
a scene object is simply a table defined as follows:

```lua
---@class Scene
---@field load fun()
---@field cleanup fun()
---@field ui fun(): UI
---@field render fun()
---@field renderOverlay fun()
---@field update fun()
local Scene = {}
```

this definition can be found in `api.types` lua file