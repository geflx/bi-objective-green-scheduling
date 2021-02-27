# Benchmark Instances

Instances are from the [2021's paper about this bi-objective problem](https://github.com/ORresearcher/A-bi-objective-heuristic-approach-for-green-identical-parallel-machine-scheduling), and accordingly to authors, the above 90 instances were subdivided in three groups: 30 small-scale, 30 medium-large and  30 very-large.
3 files for each instance j = 1,...,90:

    Data_cj.txt : time slots energy prices
    Data_pj.txt : jobs processing times
    Data_ej.txt : machines (unitary) energy price

**Note**: I refactored all instances to make file handling easier and more simple. Now there are just 90 files with the following structure:

    RData_number.txt:
    K N M           (Number of time slots/jobs/machines)
    T1, T2, ..., Tk (Time slots energy prices)
    J1, J2, ..., Jn (Jobs processing times)
    H1, H2, ..., Hm (Machines unitary energy price)
    
