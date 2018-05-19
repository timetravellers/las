find_package(PkgConfig REQUIRED)

if(PKGCONFIG_FOUND)
  if(${CMAKE_VERSION} VERSION_LESS "3.1.0")
    set(ENV{PKG_CONFIG_PATH} "${PETSC_DIR}/${PETSC_ARCH}/lib/pkgconfig/")
  else()
    list(APPEND CMAKE_PREFIX_PATH "${PETSC_DIR}/${PETSC_ARCH}/lib/pkgconfig/")
    set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH true)
  endif()

  if(PETSC_FIND_REQUIRED)
    set(_PETSC_OPTS "REQUIRED")
  endif()
  if(PETSC_FIND_QUIETLY)
    set(_PETSC_OPTS "QUIET")
  endif()
  if(PETSC_FIND_REQUIRED AND PETSC_FIND_QUIETLY)
    set(_PETSC_OPTS "REQUIRED QUIET")
  endif()

  pkg_check_modules(PETSC ${_PETSC_OPTS} PETSc)

  #TODO : set the below XXX_LIBRARIES and XXX_INCLUDE_DIRS for petsc external libs
  if(PETSC_FOUND)
    # todo : finding the external packages as PETSc dependencies currently relies on this *.cmake file
    #        it would be preferable for this functionality to be included in the relavent FindXXX.cmake files
    #        for each package
    include(${PETSC_DIR}/${PETSC_ARCH}/lib/petsc/conf/PETScBuildInternal.cmake)
    set(PETSC_INCLUDE_DIRS "${PETSC_STATIC_INCLUDE_DIRS}")
    set(PETSC_LIBRARIES "${PETSC_STATIC_LDFLAGS};${PETSC_STATIC_LIBRARIES}")
    if(PETSC_ZOLTAN_LIB)
      message(STATUS "Found Zoltan library as PETSc dependency.")
      set(ZOLTAN_FOUND true)
      set(ZOLTAN_LIBRARIES ${PETSC_ZOLTAN_LIB} ${PETSC_PTSCOTCH_LIB} ${PETSC_PTSCOTCH_LIB} ${PETSC_SCOTCH_LIB} ${PETSC_SCOTCHERR_LIB})
      set(ZOLTAN_INCLUDE_DIRS )
    endif()
    if(PETSC_PARMETIS_LIB)
      message(STATUS "Found Metis/Parmetis library as PETSc dependency.")
      set(PARMETIS_FOUND true)
      set(METIS_FOUND true)
      set(PARMETIS_LIBRARIES ${PETSC_PARMETIS_LIB} ${PETSC_METIS_LIB})
      set(METIS_LIBRARIES ${PETSC_METIS_LIB})
      set(PARMETIS_INCLUDE_DIRS )
      set(METIS_INCLUDE_DIRS )
    endif()
    if(PETSC_HDF5_LIB)
      message(STATUS "Found HDF5 library as PETSc dependency.")
      set(HDF5_FOUND true)
      set(HDF5_LIBRARIES ${PETSC_NDF5HL_FORTRAN_LIB} ${PETSC_HDF5_FORTRAN_LIB} ${PETSC_HDF5_HL_LIB} ${PETSC_HDF5_LIB})
      set(HDF5_INCLUDE_DIRS )
    endif()
  endif(PETSC_FOUND)

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PETSC DEFAULT_MSG PETSC_LIBRARIES PETSC_INCLUDE_DIRS)
mark_as_advanced(PETSC_LIBRARIES PETSC_INCLUDE_DIRS)
