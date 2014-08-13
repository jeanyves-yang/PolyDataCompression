set(TEMP_DIR ${Threshold_BINARY_DIR}/Testing/Temporary )
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Data )

# Test checking that the program cannot run without the flag -f
add_test(NAME VTKCompressionFlag COMMAND $<TARGET_FILE:VTKCompression>
        -e ASCII)
set_tests_properties(VTKCompressionFlag PROPERTIES WILL_FAIL true )

# Test checking that the file compressed is properly written
add_test(NAME VTKCompressionCorrectOutput COMMAND $<TARGET_FILE:VTKCompression>
    -e binary
    -f Data/*.vtk
  )
