# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_TYPECLASSES_PUBLIC_HEADER_FILES
        Homomorphism.h
        Show.h
        )

set(_TYPECLASSES_PRIVATE_HEADER_FILES
        )

set(_TYPECLASSES_SOURCE_FILES
        )

src_dir_filelist("${_TYPECLASSES_PUBLIC_HEADER_FILES}" outlst)
set(TYPECLASSES_PUBLIC_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_TYPECLASSES_PRIVATE_HEADER_FILES}" outlst)
set(TYPECLASSES_PRIVATE_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_TYPECLASSES_SOURCE_FILES}" outlst)
set(TYPECLASSES_SOURCE_FILES "${outlst}" PARENT_SCOPE)
