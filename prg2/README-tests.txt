Short explanation of test files:

Files named "...-in.txt" are input files containing commands for different tests.

Files named "...-out.txt" are output files containing expected output of the corresponding input.

You can either just run the input file with command read "...-in.txt" or compare your own output with the expected output with command testread "...-in.txt" "...-out.txt" (the graphical UI's file selector allows you to select both files at once by control-clicking with mouse).

Files named "perftest-....txt" are performance tests of various types of operations. They can be run with command read "perftest-....txt".

Correctness tests:
- simpletest-compulsory/all-in/out.txt: Really simple test to find most obvious bugs
- example-compulsory/non-compulsory-in/out.txt: The example in the assignment description, showing basic operation of all commands
- basictest-compulsory/all-in/out.txt: A little bit more testing with example's data
- biggertest-compulsory/all-in/out.txt: Somewhat bigger test with more beacons

Performance tests:
- perftest-fibres.txt: Test adding, removing and getting fibres
- perftest-route_any.txt: Test route_any
- perftest-compulsory.txt: Test all of the above
- perftest-route_least_xpoints.txt: Test route_least_xpoints
- perftest-route_fastest.txt: Test route_fastest
- perftest-route_fibre_cycle.txt: Test route_fibre_cycle
- perftest-trim_fibre_network.txt: Test trim_fibre_network
- perftest-inbeam.txt: Test non-compulsory path_inbeam
- perftest-all.txt: Test all of the above (compulsory+non-compulsory)

More info on testing on the course web pages!
