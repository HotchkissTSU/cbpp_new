local M = {}

local FUNC = {
    name = "push_back",
    retv = "size_t"
}

M.class = {
    Var = 1,
    Func = 2,
    Lambda = 3,
    Class = 4
}

function M.ScriptArgs() 
    return {}
end

function M.FindClass(name)
    return {
        name = "undef",
        parents = {},
        members = {},
        methods = {}
    }
end

function M.EntityType(name)

end

return M
