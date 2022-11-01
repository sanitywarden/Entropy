function round(number, precision)
    local str = string.format("%%0.%sf", precision)
    local number = string.format(str, number)
    return number
end

function printError(where, what)
    print(where .. ": " .. what)
    os.execute("pause")
end