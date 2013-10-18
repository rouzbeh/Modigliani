local M={}
-- Initialise 
local n = AlphaN(-0)/(AlphaN(-0)+BetaN(-0))
M.q10factor = 0

-- Deterministic
function M.set_temp(new_temp)
   M.q10factor = math.pow(q10, (new_temp - base_temperature) / 10.0)
end

function M.compute_conductance()
   return n*n*n*n
end

function M.open_channels()
   return n*n*n*n
end

function M.step_current(voltage)
   local an = AlphaN(voltage)
   local bn = BetaN(voltage)
   an = M.q10factor * an
   bn = M.q10factor * bn
   n = n+timestep * ((1.0 - n) * an - n * bn)
end

-- Stochastic part

M.number_states = 5
M.open_states = {5}
M.minV = -200
M.maxV = 200
M.step = 0.01

function M.get_probability(start, stop, voltage)
   if (start == stop-1) then
      return (6-stop)*AlphaN(voltage)
   end
   if (stop == start-1) then
      return stop*BetaN(voltage)
   end
   return 0
end

function M.get_q10(start, stop)
   return q10
end

return M
