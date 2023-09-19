# set( TARGET mod-aurora )

set( SOURCES
   AuroraModule.cpp
   AuroraModule.h
   include/wxcmn.h
   include/version.h
   include/commdefs.h
   include/build_count.h
   include/aurora.h
   include/audtypes.h
   src/afart.cpp
   src/afart.h
   src/afaudio.cpp
   src/afaudio.h
   src/afaxis.cpp
   src/afaxis.h
   src/afcharts.cpp
   src/afcharts.h
   src/afcomplex.cpp
   src/afcomplex.h
   src/afconfig.cpp
   src/afconfig.h
   src/afdata.h
   src/afdataplot.cpp
   src/afdataplot.h
   src/afexports.cpp
   src/afexports.h
   src/affilter.cpp
   src/affilter.h
   src/afplot.cpp
   src/afplot.h
   src/afscope.cpp
   src/afscope.h
   src/afspectrum.cpp
   src/afspectrum.h
   src/afvector.cpp
   src/afvector.h
   src/arrows.h
   src/build_count.cpp
   src/component.cpp
   src/component.h
   src/fir-taps.cpp
   src/firwin.cpp
   src/firwin.h
   src/gpfuncts.cpp
   src/gpfuncts.h
   src/gtprogind.cpp
   src/gtprogind.h
   src/iir-taps.cpp
   src/mroctbd.cpp
   src/mroctbd.h
   src/octfilt.cpp
   src/octfilt.h
   src/octfir.cpp
   src/octfir.h
   src/sigwin.cpp
   src/sigwin.h
   src/stickers.h
   SineSweepGenerator/src/SineSweepDialogs.cpp
   SineSweepGenerator/src/SineSweepDialogs.h
   SineSweepGenerator/src/SineSweepEffect.cpp
   SineSweepGenerator/src/SineSweepEffect.h
   SineSweepGenerator/src/SineSweepGenerator.cpp
   SineSweepGenerator/src/SineSweepGenerator.h
   SineSweepGenerator/src/SineSweepUi.cpp
   SineSweepGenerator/src/SineSweepUi.h
   SineSweepGenerator/src/SineSweepVersion.h
   kissfft/kiss_fft.c
   kissfft/kiss_fftnd.c
   kissfft/kiss_fftndr.c
   kissfft/kiss_fftr.c
   kissfft/kfc.c
)

set( DEFINES
   PRIVATE
      # This is needed until the transition to cmake is complete and
      # the Windows pragmas are removed from ModNullCallback.cpp.
      # Without it, the wxWidgets "debug.h" will define __WXDEBUG__
      # which then causes this module to emit library pragmas for the
      # debug versions of wxWidgets...even if the build is for Release.
      wxDEBUG_LEVEL=0
)

# audacity_module( ${TARGET} "${SOURCES}" "Audacity"
#    "${DEFINES}" "" )

function ( print_list TITLE PRINT_LIST )   
   message( STATUS "${TITLE} : ------------------------------------" )

   foreach( LIST_ELEM ${PRINT_LIST} )
      message( STATUS "${LIST_ELEM}" )      
   endforeach()
   
   message( STATUS "-----------------------------------------------" )
endfunction()

# macro( audacity_module NAME SOURCES IMPORT_TARGETS
   # ADDITIONAL_DEFINES ADDITIONAL_LIBRARIES )
   # audacity_module_fn(
   #    "${NAME}"
   #    "${SOURCES}"
   #    "${IMPORT_TARGETS}"
   #    "${ADDITIONAL_DEFINES}"
   #    "${ADDITIONAL_LIBRARIES}"
   #    "MODULE"
   # )
   # set( GRAPH_EDGES "${GRAPH_EDGES}" PARENT_SCOPE )
# function( audacity_module_fn NAME SOURCES IMPORT_TARGETS
   # ADDITIONAL_DEFINES ADDITIONAL_LIBRARIES LIBTYPE )
set ( NAME "mod-aurora")
# set ( SOURCES )
set ( IMPORT_TARGETS "Audacity")
set ( ADDITIONAL_DEFINES "${DEFINES}")
set ( ADDITIONAL_LIBRARIES "")
set ( LIBTYPE "MODULE")

set( TARGET ${NAME} )
set( TARGET_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

message( STATUS "========== Configuring ${TARGET} ==========" )

def_vars()

if (LIBTYPE STREQUAL "MODULE" AND CMAKE_SYSTEM_NAME MATCHES "Windows")
   set( REAL_LIBTYPE SHARED )
else()
   set( REAL_LIBTYPE "${LIBTYPE}" )
endif()
add_library( ${TARGET} ${REAL_LIBTYPE} )

# Manual propagation seems to be necessary from
# interface libraries -- just doing target_link_libraries naming them
# doesn't work as promised

# compute INCLUDES
set( INCLUDES )
list( APPEND INCLUDES PUBLIC "${TARGET_ROOT}" )
list( APPEND INCLUDES PUBLIC "${TARGET_ROOT}/include" )

# compute DEFINES
set( DEFINES )
list( APPEND DEFINES ${ADDITIONAL_DEFINES} )

# send the file to the proper place in the build tree, by setting the
# appropriate property for the platform
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
   set( DIRECTORY_PROPERTY RUNTIME_OUTPUT_DIRECTORY )
else ()
   set( DIRECTORY_PROPERTY LIBRARY_OUTPUT_DIRECTORY )
endif ()

if (LIBTYPE STREQUAL "MODULE")
   set_target_property_all( ${TARGET} ${DIRECTORY_PROPERTY} "${_DESTDIR}/${_MODDIR}" )
   set_target_properties( ${TARGET}
      PROPERTIES
         PREFIX ""
         FOLDER "modules" # for IDE organization
   )

   if( NOT CMAKE_SYSTEM_NAME MATCHES "Windows|Darwin" )
      set_target_property_all(${TARGET} INSTALL_RPATH "$ORIGIN:$ORIGIN/..")
      install( TARGETS ${TARGET} OPTIONAL DESTINATION ${_MODDIR} )
   endif()

   fix_bundle( ${TARGET} )
else()
   set_target_property_all( ${TARGET} ${DIRECTORY_PROPERTY} "${_DESTDIR}/${_PKGLIB}" )
   set_target_properties( ${TARGET}
      PROPERTIES
         PREFIX ""
         FOLDER "libraries" # for IDE organization
   )

   if( NOT CMAKE_SYSTEM_NAME MATCHES "Windows|Darwin" )
      set_target_property_all(${TARGET} INSTALL_RPATH "$ORIGIN")
      install(TARGETS ${TARGET} DESTINATION ${_PKGLIB} )
   endif()
endif()

export_symbol_define( export_symbol "${TARGET}" )
import_symbol_define( import_symbol "${TARGET}" )

list( APPEND DEFINES
   PRIVATE "${export_symbol}"
   INTERFACE "${import_symbol}"
)

set( LOPTS
   PRIVATE
      $<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>
)

# compute LIBRARIES
set( LIBRARIES )

foreach( IMPORT ${IMPORT_TARGETS} )
   list( APPEND LIBRARIES "${IMPORT}" )
endforeach()

list( APPEND LIBRARIES ${ADDITIONAL_LIBRARIES} )

# #   list( TRANSFORM SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/" )

# Compute compilation options.
# Perhaps a another function argument in future to customize this too.
set( OPTIONS )
audacity_append_common_compiler_options( OPTIONS NO )

organize_source( "${TARGET_ROOT}" "" "${SOURCES}" )
target_sources( ${TARGET} PRIVATE ${SOURCES} )
target_compile_definitions( ${TARGET} PRIVATE ${DEFINES} )
target_compile_options( ${TARGET} ${OPTIONS} )

# Additional Include Directories
target_include_directories( ${TARGET} PUBLIC ${TARGET_ROOT} )
target_include_directories( ${TARGET} PUBLIC "${TARGET_ROOT}/include" )
target_include_directories( ${TARGET} PUBLIC "${TARGET_ROOT}/kissfft" )

target_link_options( ${TARGET} PRIVATE ${LOPTS} )
target_link_libraries( ${TARGET} PUBLIC ${LIBRARIES} )

if( NOT CMAKE_SYSTEM_NAME MATCHES "Windows" )
   # Generate-time boolean values must be "0" or "1",
   # not "on", "off", etc. like configure-time booleans
   if (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
      set(nostrip 1)
   else()
      set(nostrip 0)
   endif()
   add_custom_command(
      TARGET "${TARGET}"
      POST_BUILD
      COMMAND "$<IF:$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>,${nostrip}>,echo,strip>" -x $<TARGET_FILE:${TARGET}>
   )
endif()

# define an additional interface library target
make_interface_alias(${TARGET} ${REAL_LIBTYPE})

# collect dependency information just for graphviz
collect_edges( ${TARGET} "${IMPORT_TARGETS}" ${LIBTYPE} )
set( GRAPH_EDGES "${GRAPH_EDGES}" PARENT_SCOPE )

# collect unit test targets if they are present
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
   add_subdirectory(tests)
endif()

set( GRAPH_EDGES "${GRAPH_EDGES}" PARENT_SCOPE )