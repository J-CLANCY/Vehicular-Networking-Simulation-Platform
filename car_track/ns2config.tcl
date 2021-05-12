
# set number of nodes
set opt(nn) 52

# set activity file
set opt(af) $opt(config-path)
append opt(af) /ns2act.tcl

# set mobility file
set opt(mf) $opt(config-path)
append opt(mf) /ns2mobility.tcl

# set start/stop time
set opt(start) 0.0
set opt(stop) 337.0

# set floor size
set opt(x) 985.99
set opt(y) 1149.36
set opt(min-x) -1.44
set opt(min-y) 22.03

