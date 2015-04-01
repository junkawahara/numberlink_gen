# numberlink_gen
numberlink instance generator

Install:

Clone this project and just type

```
make
```

Usage:

```
./makegrid m n | ./nlgen 
```

generates all the instances of numberlink for m x n grid.


Example:

```
./makegrid.exe 4 4 | ./nlgen
```

Output:

```
# of vertices = 16, # of edges = 24
begin enumeration...
# of solutions = 16410
# of ZDD nodes = 4431
end enumeration
time = 0.281 sec.
```

The line "# of solutions = 16410" represents the number of instances of numberlink for 4 x 4 grid.

This version outputs only the number of instances. The feature of outputting all the instances is under construction.
