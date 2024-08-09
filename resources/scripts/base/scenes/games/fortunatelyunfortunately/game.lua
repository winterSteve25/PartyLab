---@type Scene
local m = {}

local num = SyncedVar("num", true, 0)

m.ui = function(data)
    num:clearSubscribers()
    num:subscribe(function(newVal)
        data.query("num").setText(tostring(newVal))
    end)
    
    return
    {
        {
            "Complete the sentence: ",
        },
        {
            type = "text",
            id = "num",
            text = tostring(num:get()),
            onClick = function()
                num:set(num:get() + 1)
            end
        }
    }
end

return m