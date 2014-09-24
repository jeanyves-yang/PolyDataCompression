# Add standard remote object stores to user's
# configuration.
list(APPEND ExternalData_URL_TEMPLATES
  "http://slicer.kitware.com/midas3/api/rest?method=midas.bitstream.download&checksum=%(hash)"
  )

# Test checking that the program cannot run without the flag -f
add_test(NAME PolyDataCompressionFlag COMMAND $<TARGET_FILE:PolyDataCompression>
        -e ascii)
set_tests_properties(PolyDataCompressionFlag PROPERTIES WILL_FAIL true )

# Test checking that the file compressed is properly written
ExternalData_Add_Test(PolyDataCompressionData NAME PolyDataCompressionCorrectOutput COMMAND $<TARGET_FILE:PolyDataCompression>
    -e binary
    -f DATA{Data/baseline.vtk}
  )

#Test checking that a file in the current directory can be compressed
ExternalData_Expand_Arguments(PolyDataCompressionData expandedBaselinePath DATA{Data/baseline.vtk} )
get_filename_component( FileDirectory ${expandedBaselinePath} DIRECTORY)
ExternalData_Add_Test(PolyDataCompressionData NAME PolyDataCompressionCurrentDir WORKING_DIRECTORY ${FileDirectory} COMMAND $<TARGET_FILE:PolyDataCompression>
    -e binary
    -f baseline.vtk#We do not specify the path of the file with DATA{} since we want to test that PolyDataCompression works when giving a file with no path
  )

# Test checking that * character is expanded (only checking on UNIX systems). We search for bash
if( UNIX )
  find_program( bashPath bash )
  if( bashPath )
    ExternalData_Add_Test(PolyDataCompressionData NAME PolyDataCompressionStarExpansion COMMAND ${bashPath} -c
    "$<TARGET_FILE:PolyDataCompression> -e binary -f ${FileDirectory}/*.vtk"
    )
  endif()
endif()
# Test checking that the help can be printed even if no input file is given.
add_test(NAME PolyDataCompressionHelp COMMAND $<TARGET_FILE:PolyDataCompression>
    --help
  )

# Test checking that the version can be printed even if no input file is given.
add_test(NAME PolyDataCompressionVersion COMMAND $<TARGET_FILE:PolyDataCompression>
    --version
  )

set_tests_properties(PolyDataCompressionVersion PROPERTIES FAIL_REGULAR_EXPRESSION "Unspecified" )

# Add a build target to populate the real data.
ExternalData_Add_Target(PolyDataCompressionData)
