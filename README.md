VTKCompression
==============

Compress VTK or VTP files, allows to select the data type (binary, ASCII or appended), the level of compression (between 0 and 9, the difference however is barely noticeable) and enable the use of the wildcard (*) with SlicerExecutionModel.

Exemple of USAGE: ./VTKCompression -e binary -c 3 -f /Data/*.vtk

NB: The wildcard (*) works in command line, does not work when set as a run argument under QTCreator environment. Also, is not available from within Slicer and that tool should probably not be used as a Slier module even though it is based on SlicerExecutionModel.
