local exponential = math.exp
local time = require('timestep')

timestep = time.timestep
set_timestep = time.set_timestep

-- General data
base_temperature = 6.3
q10 = 1

-- Kinetics
function AlphaN(vM)
    if vM==-55 then return 0.01 end;
    return 0.01*(vM+55)/(1-math.exp(-0.1*(vM+55)))
end

function BetaN(vM)
   return 0.125*math.exp(-0.0125*(vM+65))
end


local channel_functions=require('5_state_potassium')

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
