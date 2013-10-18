-- module
local M={}
M.q10mfactor = 0
M.q10hfactor = 0
-- Generic
M.m = AlphaM(-0)/(AlphaM(-0)+BetaM(-0))
M.h = AlphaH(-0)/(AlphaH(-0)+BetaH(-0))

-- Deterministic
function M.set_temp(new_temp)
   M.q10mfactor = math.pow(q10m, (new_temp - base_temperature) / 10.0)
   M.q10hfactor = math.pow(q10h, (new_temp - base_temperature) / 10.0)
end

function M.compute_conductance()
   return M.m*M.m*M.m*M.h
end

function M.open_channels()
   return M.m*M.m*M.m*M.h
end

function M.step_current(voltage)
   local am = AlphaM(voltage)
   local bm = BetaM(voltage)
   local ah = AlphaH(voltage)
   local bh = BetaH(voltage)
   am = M.q10mfactor * am
   bm = M.q10mfactor * bm
   ah = M.q10hfactor * ah
   bh = M.q10hfactor * bh
   M.m = M.m+timestep * ((1.0 - M.m) * am - M.m * bm)
   M.h = M.h+timestep * ((1.0 - M.h) * ah - M.h * bh)
end

-- Stochastic part
M.number_states = 8
M.open_states = {4}
M.minV = -200
M.maxV = 200
M.step = 0.01

function M.get_probability(start, stop, voltage)
   -- One step to right
   if (start == stop-1 and start < 4) then
      return (5-stop)*AlphaM(voltage)
   end
   if (start == stop-1 and start > 4) then
      return (9-stop)*AlphaM(voltage)
   end

   -- One step left
   if (start == stop+1 and stop < 4) then
      return stop*BetaM(voltage)
   end
   if (start == stop+1 and stop > 4) then
      return (stop-4)*BetaM(voltage)
   end

   -- One step down
   if(stop == start + 4) then
      return BetaH(voltage)
   end

   -- One step up
   if (stop == start - 4) then
      return AlphaH(voltage)
   end
   return 0
end

function M.get_q10(start, stop)
   if (math.abs(start-stop)==4) then
      return q10h
   else
      return q10m
   end
end

return M
