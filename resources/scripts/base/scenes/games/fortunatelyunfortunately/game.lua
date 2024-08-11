---@type Scene
local m = {}

local layout = require("partylab.ui.layout")
---@type SyncedList
local submitted

m.load = function()
    submitted = SyncedList("submittedPeople", false)
end

m.cleanup = function()
    submitted = nil
end

m.ui = function(data)
    submitted:clearAddSubscribers()
    submitted:subscribeAdd(function(i, newVal) 
        require("partylab.utils").info(newVal.id)
    end)
    
    submitted:clearRemoveSubscribers()
    submitted:subscribeRemove(function(index, removedVal)
        require("partylab.utils").info("Removed: " .. tostring(removedVal.id))
    end)
    
    return
    {
        id = "list",
        style = {
            alignSelf = layout.self.LAY_CENTER,
            alignItems = layout.items.LAY_COLUMN,
        },
        {
            "Complete the sentence",
        },
        {
            type = "text",
            id = "num",
            text = "Hi",
            onClick = function()
                submitted:add(require("partylab.steam").getCurrentUserID())
            end
        }
    }
end

return m