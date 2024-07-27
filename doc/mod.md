# Defining a mod

A mod is a directory with a file named `mod.lua` in it. 
`mod.lua` should return a table with the following content
- `name` a string defining the display name of the mod
- `description` optional, a string defining the description of the mod
- `events` optional, a table defining the event handlers for the mod

## Event handlers
The events table should in the format of
```lua
{
    eventName = function(...)
        -- handler body
    end
}
```
the event handler may receive parameters depending on the event.