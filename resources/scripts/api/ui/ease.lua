local m = {}

m.easeLinear = function(t)
    return t
end

m.easeInSine = function(t)
    return 1 - math.cos((t * math.pi) / 2)
end

m.easeOutSine = function(t)
    return math.sin((t * math.pi) / 2)
end

m.easeInOutSine = function(t)
    return -(math.cos(math.pi * t) - 1) / 2
end

m.easeInQuad = function(t)
    return t * t
end

m.easeOutQuad = function(t)
    return 1 - (1 - t) * (1 - t)
end

m.easeInOutQuad = function(t)
    if t < 0.5 then
        return 2 * t * t
    else
        return 1 - ((-2 * t + 2)^2) / 2
    end
end

m.easeInCubic = function(t)
    return t * t * t
end

m.easeOutCubic = function(t)
    return 1 - (1 - t)^3
end

m.easeInOutCubic = function(t)
    if t < 0.5 then
        return 4 * t * t * t
    else
        return 1 - ((-2 * t + 2)^3) / 2
    end
end

m.easeInQuart = function(t)
    return t * t * t * t
end

m.easeOutQuart = function(t)
    return 1 - (1 - t)^4
end

m.easeInOutQuart = function(t)
    if t < 0.5 then
        return 8 * t * t * t * t
    else
        return 1 - (-2 * t + 2)^4 / 2
    end
end

m.easeInQuint = function(t)
    return t * t * t * t * t
end

m.easeOutQuint = function(t)
    return 1 - (1 - t)^5
end

m.easeInOutQuint = function(t)
    if t < 0.5 then
        return 16 * t * t * t * t * t
    else
        return 1 - (-2 * t + 2)^5 / 2
    end
end

m.easeInExpo = function(t)
    if t == 0 then
        return 0
    else
        return 2^(10 * t - 10)
    end
end

m.easeOutExpo = function(t)
    if t == 1 then
        return 1
    else
        return 1 - 2^(-10 * t)
    end
end

m.easeInOutExpo = function(t)
    if t == 0 then
        return 0
    elseif t == 1 then
        return 1
    elseif t < 0.5 then
        return 2^(20 * t - 10) / 2
    else
        return (2 - 2^(-20 * t + 10)) / 2
    end
end

m.easeInCirc = function(t)
    return 1 - math.sqrt(1 - t^2)
end

m.easeOutCirc = function(t)
    return math.sqrt(1 - (t - 1)^2)
end

m.easeInOutCirc = function(t)
    if t < 0.5 then
        return (1 - math.sqrt(1 - (2 * t)^2)) / 2
    else
        return (math.sqrt(1 - (-2 * t + 2)^2) + 1) / 2
    end
end

m.easeInBack = function(t)
    local c1 = 1.70158
    local c3 = c1 + 1

    return c3 * t * t * t - c1 * t * t
end

m.easeOutBack = function(t)
    local c1 = 1.70158
    local c3 = c1 + 1

    return 1 + c3 * (t - 1)^3 + c1 * (t - 1)^2
end

m.easeInOutBack = function(t)
    local c1 = 1.70158
    local c2 = c1 * 1.525

    if t < 0.5 then
        return (2 * t)^2 * ((c2 + 1) * (2 * t) - c2) / 2
    else
        return (2 * t - 2)^2 * ((c2 + 1) * (t * 2 - 2) + c2) / 2 + 1
    end
end

m.easeInElastic = function(t)
    local c4 = (2 * math.pi) / 3

    if t == 0 then
        return 0
    elseif t == 1 then
        return 1
    else
        return -2^(10 * t - 10) * math.sin((t * 10 - 10.75) * c4)
    end
end

m.easeOutElastic = function(t)
    local c4 = (2 * math.pi) / 3

    if t == 0 then
        return 0
    elseif t == 1 then
        return 1
    else
        return 2^(-10 * t) * math.sin((t * 10 - 0.75) * c4) + 1
    end
end

m.easeInOutElastic = function(t)
    local c5 = (2 * math.pi) / 4.5

    if t == 0 then
        return 0
    elseif t == 1 then
        return 1
    elseif t < 0.5 then
        return -(2^(20 * t - 10) * math.sin((20 * t - 11.125) * c5)) / 2
    else
        return (2^(-20 * t + 10) * math.sin((20 * t - 11.125) * c5)) / 2 + 1
    end
end

return m