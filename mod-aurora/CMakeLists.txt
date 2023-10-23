set ( TARGET mod-aurora)

set( SOURCES
   AuroraModule.cpp
   AuroraModule.h
   include/wxcmn.h
   include/version.h
   include/commdefs.h
   include/build_count.h
   include/aurora.h
   include/audtypes.h
   src/arrows.h
   src/ArtProvider.cpp
   src/ArtProvider.h
   src/Audio.cpp
   src/Audio.h
   src/Axis.cpp
   src/Axis.h
   src/build_count.cpp
   src/Chart.cpp
   src/Chart.h
   src/Complex.cpp
   src/Complex.h
   src/Config.cpp
   src/Config.h
   src/DataHolder.h
   src/DataPlot.cpp
   src/DataPlot.h
   src/Exports.cpp
   src/Exports.h
   src/Filter.cpp
   src/Filter.h
   src/fir-taps.cpp
   src/gpfuncts.cpp
   src/gpfuncts.h
   src/GTProgressIndicator.cpp
   src/GTProgressIndicator.h
   src/iir-taps.cpp
   src/Plot.cpp
   src/Plot.h
   src/SCFirWin.cpp
   src/SCFirWin.h
   src/SCOctaveBandAnalysis.cpp
   src/SCOctaveBandAnalysis.h
   src/SCOctaveBandFilter.cpp
   src/SCOctaveBandFilter.h
   src/SCOctaveBandFir.cpp
   src/SCOctaveBandFir.h
   src/Scope.cpp
   src/Scope.h
   src/SCSignalWindows.cpp
   src/SCSignalWindows.h
   src/Spectrum.cpp
   src/Spectrum.h
   src/stickers.h
   src/UIComponents.cpp
   src/UIComponents.h
   src/Vector.cpp
   src/Vector.h
   kissfft/kiss_fft.c
   kissfft/kiss_fftnd.c
   kissfft/kiss_fftndr.c
   kissfft/kiss_fftr.c
   kissfft/kfc.c
   SineSweepGenerator/SineSweepGeneratorDialog.cpp
   SineSweepGenerator/SineSweepGeneratorDialog.h
   SineSweepGenerator/SineSweepGenerator.cpp
   SineSweepGenerator/SineSweepGenerator.h
   SineSweepGenerator/SineSweepVersion.h
   # Kirkeby/KirkebyBase.cpp
   # Kirkeby/KirkebyBase.h
   # Kirkeby/KirkebyDialogs.cpp
   # Kirkeby/KirkebyDialogs.h
   # Kirkeby/KirkebyEffect.cpp
   # Kirkeby/KirkebyEffect.h
   # Kirkeby/KirkebyUi.cpp
   # Kirkeby/KirkebyUi.h
   # Kirkeby/KirkebyVersion.h
   # AcParameters/AcParametersDialogs.h
   # AcParameters/AcParametersBase.cpp
   # AcParameters/AcParametersBase.h
   # AcParameters/AcParametersData.cpp
   # AcParameters/AcParametersData.h
   # AcParameters/AcParametersDialogs.cpp
   # AcParameters/AcParameters.h
   # AcParameters/AcParametersEffect.cpp
   # AcParameters/AcParametersEffect.h
   # AcParameters/AcParametersExports.cpp
   # AcParameters/AcParametersExports.h
   # AcParameters/AcParametersPlot.cpp
   # AcParameters/AcParametersPlot.h
   # AcParameters/AcParametersSpectrum.h
   # AcParameters/AcParametersTrack.cpp
   # AcParameters/AcParametersTrack.h
   # AcParameters/AcParametersUi.cpp
   # AcParameters/AcParametersUi.h
   # AcParameters/AcParametersVersion.h
   # AcParameters/AcTResults.cpp
   # AcParameters/AcTResults.h
   # AcParameters/AcTSchroederDecay.cpp
   # AcParameters/AcTSchroederDecay.h
   # AcParameters/sizes.h
   # SpeechTransmissionIndex/ModulationTransferFunctionsTable.cpp
   # SpeechTransmissionIndex/ModulationTransferFunctionsTable.h
   # SpeechTransmissionIndex/STI.cpp
   # SpeechTransmissionIndex/STI.h
   # SpeechTransmissionIndex/STIData.cpp
   # SpeechTransmissionIndex/STIData.h
   # SpeechTransmissionIndex/STIDialogs.cpp
   # SpeechTransmissionIndex/STIDialogs.h
   # SpeechTransmissionIndex/STIEffect.cpp
   # SpeechTransmissionIndex/STIEffect.h
   # SpeechTransmissionIndex/STIExports.cpp
   # SpeechTransmissionIndex/STIExports.h
   # SpeechTransmissionIndex/STIPlot.cpp
   # SpeechTransmissionIndex/STIPlot.h
   # SpeechTransmissionIndex/STIResults.cpp
   # SpeechTransmissionIndex/STIResults.h
   # SpeechTransmissionIndex/STISpectrum.cpp
   # SpeechTransmissionIndex/STISpectrum.h
   # SpeechTransmissionIndex/STISpectrumDialog.cpp
   # SpeechTransmissionIndex/STISpectrumDialog.h
   # SpeechTransmissionIndex/STISpectrumWxDialog.cpp
   # SpeechTransmissionIndex/STISpectrumWxDialog.h
   # SpeechTransmissionIndex/STITrack.cpp
   # SpeechTransmissionIndex/STITrack.h
   # SpeechTransmissionIndex/STIUi.cpp
   # SpeechTransmissionIndex/STIUi.h
   # TimeHistoryAnalyzer/ThAnalyzerData.cpp
   # TimeHistoryAnalyzer/ThAnalyzerData.h
   # TimeHistoryAnalyzer/ThAnalyzerDialogs.cpp
   # TimeHistoryAnalyzer/ThAnalyzerDialogs.h
   # TimeHistoryAnalyzer/ThAnalyzerEffect.cpp
   # TimeHistoryAnalyzer/ThAnalyzerEffect.h
   # TimeHistoryAnalyzer/ThAnalyzerExports.cpp
   # TimeHistoryAnalyzer/ThAnalyzerExports.h
   # TimeHistoryAnalyzer/ThAnalyzerNbook.h
   # TimeHistoryAnalyzer/ThAnalyzerPlot.cpp
   # TimeHistoryAnalyzer/ThAnalyzerPlot.h
   # TimeHistoryAnalyzer/ThAnalyzerTrack.cpp
   # TimeHistoryAnalyzer/ThAnalyzerTrack.h
   # TimeHistoryAnalyzer/ThAnalyzerUi.cpp
   # TimeHistoryAnalyzer/ThAnalyzerUi.h
   # TimeHistoryAnalyzer/TimeHistoryAnalyzer.cpp
   # TimeHistoryAnalyzer/TimeHistoryAnalyzer.h
   # XFunctions/equations.h
   # XFunctions/correlator.cpp
   # XFunctions/correlator.h
   # XFunctions/XFunctionsArt.cpp
   # XFunctions/XFunctionsArt.h
   # XFunctions/XFunctionsData.cpp
   # XFunctions/XFunctionsData.h
   # XFunctions/XFunctionsDialogs.cpp
   # XFunctions/XFunctionsDialogs.h
   # XFunctions/XFunctionsEffect.cpp
   # XFunctions/XFunctionsEffect.h
   # XFunctions/XFunctionsExports.cpp
   # XFunctions/XFunctionsExports.h
   # XFunctions/XFunctionsPlot.cpp
   # XFunctions/XFunctionsPlot.h
   # XFunctions/XFunctionsUi.cpp
   # XFunctions/XFunctionsUi.h
   # Convolver/AuroraConvolver.cpp
   # Convolver/AuroraConvolver.h
   # Convolver/convolver.cpp
   # Convolver/convolver.h
   # Convolver/ConvolverDialogs.cpp
   # Convolver/ConvolverDialogs.h
   # Convolver/ConvolverEffect.cpp
   # Convolver/ConvolverEffect.h
   # Convolver/ConvolverPlot.cpp
   # Convolver/ConvolverPlot.h
   # Convolver/ConvolverUi.cpp
   # Convolver/ConvolverUi.h
   # Convolver/multivolver.cpp
   # Convolver/multivolver.h
   # Convolver/TrackProperties.cpp
   # Convolver/TrackProperties.h
   # Convolver/res/down.h
   # Convolver/res/left.h
   # Convolver/res/right.h
   # Convolver/res/up.h
   # Dummy/DummyAcParameters.cpp
   # Dummy/DummyAcParameters.h
   Dummy/DummyAnalyse.cpp
   Dummy/DummyAnalyse.h
   Dummy/DummyConvolver.cpp
   Dummy/DummyConvolver.h
   Dummy/DummyGenerator.cpp
   Dummy/DummyGenerator.h
   Dummy/DummyKirkeby.cpp
   Dummy/DummyKirkeby.h
   Dummy/DummySineSweepGenerator.cpp
   Dummy/DummySineSweepGenerator.h
   Dummy/DummySpeechTransmissionIndex.cpp
   Dummy/DummySpeechTransmissionIndex.h
   Dummy/DummyTimeHistoryAnalyzer.cpp
   Dummy/DummyTimeHistoryAnalyzer.h
   Dummy/DummyXFunctions.cpp
   Dummy/DummyXFunctions.h
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

# audacity_module( ${TARGET} "${SOURCES}" "Audacity" "${DEFINES}" "" )
# set_target_properties( ${TARGET} PROPERTIES EXCLUDE_FROM_ALL YES )

#---------------------------------------------------------------------
# Below is a slight modification of the audacity_module cmake macro and audacity_module_fn function
# to allow for adding extra header search paths.


# Basically set the arguments that would hae been set if passed in the original function
set ( IMPORT_TARGETS "Audacity")
set ( ADDITIONAL_DEFINES "${DEFINES}")
set ( ADDITIONAL_LIBRARIES "")
set ( LIBTYPE "MODULE")
#set( TARGET ${NAME} )
set( TARGET_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

message( STATUS "========== Configuring ${TARGET} ==========" )

def_vars()

if (LIBTYPE STREQUAL "MODULE" AND CMAKE_SYSTEM_NAME MATCHES "Windows")
   set( REAL_LIBTYPE SHARED )
else()
   set( REAL_LIBTYPE "${LIBTYPE}" )
endif()


add_library( ${TARGET} ${REAL_LIBTYPE} )

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
add_custom_command(
   TARGET
      ${TARGET}
   COMMAND
      \n\n
      echo \"Modify the module Timestamp and setting enum in the config to avoid having to launch and relaunch Audcaity\"\n
      EXP=\"mod-aurora=[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]T[0-9][0-9]:[0-9][0-9]:[0-9][0-9]\"\n
      NEW_TIME=\$\(stat -t '%FT%T' -x \"\${CODESIGNING_FOLDER_PATH}\" | grep  Modify | grep -o -E '\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}'\) \n
      sed -i.bak \"s/mod-aurora=[0-9]/mod-aurora=1/\" \"\${HOME}/Library/Application Support/audacity/audacity.cfg\" \n
      sed -i.bak \"s/\${EXP}/mod-aurora=\${NEW_TIME}/\" \"\${HOME}/Library/Application Support/audacity/audacity.cfg\" \n
      \n\n
   POST_BUILD
)
endif()

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

# ------------------------------------------------------------------------------
# Additional Include Directories

target_include_directories( ${TARGET} PUBLIC ${TARGET_ROOT} )
target_include_directories( ${TARGET} PUBLIC "${TARGET_ROOT}/include" )
target_include_directories( ${TARGET} PUBLIC "${TARGET_ROOT}/kissfft" )

# ------------------------------------------------------------------------------

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

