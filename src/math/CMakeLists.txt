# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_MATH_PUBLIC_HEADER_FILES
        MathUtils.h
        RNG.h
        PARENT_SCOPE
        )

set(_MATH_PRIVATE_HEADER_FILES
        DefaultRNG.h
        Partition.h
        PARENT_SCOPE
        )

set(_MATH_SOURCE_FILES
        DefaultRNG.cpp
        MathUtils.cpp
        Partition.cpp
        RNG.cpp
        PARENT_SCOPE
        )
