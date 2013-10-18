local exponential = math.exp
local time = require('timestep')

timestep = time.timestep
set_timestep = time.set_timestep

-- Kinetics
base_temperature = 6.3
q10m = 1
q10h = 1


function AlphaM(vM)
   if vM==-40 then return 1 end;
   return 0.1*(vM+40)/(1-exponential(-0.1*(vM+40)))
end
    
function BetaM(vM)
   return 4*exponential(-0.0556*(vM+65))
end

function AlphaH(vM)
   return 0.07*exponential(-0.05*(vM+65))
end

function BetaH(vM)
   return 1/(1+exponential(-0.1*(vM+35)))
end

local channel_functions=require('8_state_sodium')

number_states = channel_functions.number_states
open_states = channel_functions.open_states
minV = channel_functions.minV
maxV = channel_functions.maxV
step = channel_functions.step

compute_conductance = channel_functions.compute_conductance
open_channels = channel_functions.open_channels
get_probability = channel_functions.get_probability
get_q10 = channel_functions.get_q10
step_current = channel_functions.step_current
set_temp = channel_functions.set_temp
