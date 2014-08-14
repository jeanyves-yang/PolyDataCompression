# Test checking that the program cannot run without the flag -f
add_test(NAME PolyDataCompressionFlag COMMAND $<TARGET_FILE:PolyDataCompression>
        -e ASCII)
set_tests_properties(PolyDataCompressionFlag PROPERTIES WILL_FAIL true )

# Test checking that the file compressed is properly written
add_test(NAME PolyDataCompressionCorrectOutput COMMAND $<TARGET_FILE:PolyDataCompression>
    -e binary
    -f Data/*.vtk
  )
