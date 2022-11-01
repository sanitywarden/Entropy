function containsWord(str, phrase)
    if rawlen(str) < rawlen(phrase) then
        return false
    end
    
    index = string.find(str, phrase)
    if index == nil then
        return false 
    else
        return true
    end
end

function startsWith(str, phrase)
    if rawlen(str) < rawlen(phrase) then
        return false
    end

    local contains = containsWord(str, phrase)
    if contains then
        local length = rawlen(phrase)
        local start = string.sub(str, 1, length)
        
        return start == phrase
    end

    return false
end

function endsWith(str, phrase)
    if rawlen(str) < rawlen(phrase) then
        return false
    end

    local contains = containsWord(str, phrase)
    if contains then
        local phrase_length = rawlen(phrase)
        local string_length = rawlen(str)
        local start_from = string_length - phrase_length + 1
        local substr = string.sub(str, start_from, string_length)

        return substr == phrase
    end

    return false
end

function toLower(str) 
    return string.lower(str)
end

function toUpper(str)
    return string.upper(str)
end

function capiatlise(str)
    first_letter = string.upper(string.sub(str, 1, 1))
    rest = string.sub(str, 2, rawlen(str))
    return first_letter .. rest 
end

function readAfter(str, phrase)
    if rawlen(str) < rawlen(phrase) then
        return ""
    end

    index_start, index_end = string.find(str, phrase)
    after = string.sub(str, index_end + 1, rawlen(str))
    return after
end

function readBefore(str, phrase)
    if rawlen(str) < rawlen(phrase) then
        return ""
    end
    
    index_start, index_end = string.find(str, phrase)
    before = string.sub(str, 1, index_start - 1)
    return before
end

function subString(str, index_from, index_to)
    return string.sub(str, index_from, index_to)
end

function split(str, character)
    local list = {}
    local last_cut = 1
    local length = rawlen(str)

    for i = 1, length, 1 do
        if string.sub(str, i, i) == character then
            cut = string.sub(str, last_cut, i - 1)
            last_cut = i + 1
            table.insert(list, cut)
        end
    end

    return list
end