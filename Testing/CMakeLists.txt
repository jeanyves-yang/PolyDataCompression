# Test checking that the program cannot run without the flag -f
add_test(NAME PolyDataCompressionFlag COMMAND $<TARGET_FILE:PolyDataCompression>
        -e ASCII)
set_tests_properties(PolyDataCompressionFlag PROPERTIES WILL_FAIL true )

# Test checking that the file compressed is properly written
add_test(NAME PolyDataCompressionCorrectOutput COMMAND $<TARGET_FILE:PolyDataCompression>
    -e binary
    -f Data/*.vtk
  )

# Test checking that the help can be printed even if no input file is given. 
add_test(NAME PolyDataCompressionHelp COMMAND $<TARGET_FILE:PolyDataCompression>
    --help
  )

# Test checking that the version can be printed even if no input file is given. 
add_test(NAME PolyDataCompressionVersion COMMAND $<TARGET_FILE:PolyDataCompression>
    --version
  )

set_tests_properties(PolyDataCompressionVersion PROPERTIES FAIL_REGULAR_EXPRESSION "Unspecified" )
