m.info = function(info) 
    cpp_info(tostring(info))
end

m.error = function(error)
    cpp_error(tostring(error))
end

m.warning = function(warning)
    cpp_warning(tostring(warning))
end
