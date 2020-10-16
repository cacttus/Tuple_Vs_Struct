# Tuple_Vs_Struct
Testing efficiency of std::tuple vs struct for multiple return types.


| Compiler | Result |
| :------: | :---: |
| MSVC | Tuple in Release: 4% faster. Debug: 50% slower. |
| G++9 | Tuple with -O3: 5% faster. Without -O3 (debug): 40% slower. |
