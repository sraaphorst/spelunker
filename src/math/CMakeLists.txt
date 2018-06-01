# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_MATH_PUBLIC_HEADER_FILES
        MathUtils.h
        RNG.h
        )

set(_MATH_PRIVATE_HEADER_FILES
        DefaultRNG.h
        )

set(_MATH_SOURCE_FILES
        MathUtils.cpp
        RNG.cpp
        DefaultRNG.cpp
        )

src_dir_filelist("${_MATH_PUBLIC_HEADER_FILES}" outlst)
set(MATH_PUBLIC_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_MATH_PRIVATE_HEADER_FILES}" outlst)
set(MATH_PRIVATE_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_MATH_SOURCE_FILES}" outlst)
set(MATH_SOURCE_FILES "${outlst}" PARENT_SCOPE)