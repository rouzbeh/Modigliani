local M={}

M.timestep = 0.1
function M.set_timestep(new_timestep)
   timestep = new_timestep
end

return M
