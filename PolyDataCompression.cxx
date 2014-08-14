#include "PolyDataCompressionCLP.h"
#include "string.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkZLibDataCompressor.h>
#include <itksys/SystemTools.hxx>

std::string ChangeEndOfFileName ( std::string& fileName, std::string& change )
{
    return  itksys::SystemTools::GetFilenamePath( fileName )
            + "/" + itksys::SystemTools::GetFilenameWithoutLastExtension( fileName ) + change + ".vtp" ;
}

vtkSmartPointer< vtkPolyData > ReadFile( std::string fileName , std::string extension )
{
    if( extension == ".vtk" )
    {
        vtkSmartPointer< vtkPolyDataReader > reader = vtkSmartPointer< vtkPolyDataReader >::New() ;
        reader->SetFileName( fileName.c_str() ) ;
        reader->Update() ;
        return reader->GetOutput() ;
    }
    else if( extension == ".vtp" )
    {
        vtkSmartPointer< vtkXMLPolyDataReader > reader = vtkSmartPointer< vtkXMLPolyDataReader >::New() ;
        reader->SetFileName( fileName.c_str() ) ;
        reader->Update() ;
        return reader->GetOutput() ;
    }
    std::cout << "input file is not a vtk or a vtp file." << std::endl ;

    exit( 1 ) ;
}

void WriteFile( std::string encoding , std::string outputFileName , int compressionLevel , vtkSmartPointer< vtkPolyData > readerPolyData )
{
    vtkSmartPointer< vtkXMLPolyDataWriter > writer = vtkSmartPointer< vtkXMLPolyDataWriter >::New() ;
    writer->SetFileName( outputFileName.c_str() ) ;
    writer->SetInputData( readerPolyData ) ;
    vtkZLibDataCompressor *compressor = dynamic_cast< vtkZLibDataCompressor* > ( writer->GetCompressor() ) ;
    if( compressor )
    {
        compressor->SetCompressionLevel( compressionLevel ) ;
    }
    if( encoding == "binary" )
    {
        writer->SetDataModeToBinary() ;
    }
    else if( encoding == "appended" )
    {
        writer->SetDataModeToAppended() ;
    }
    else if( encoding =="ascii" )
    {
        writer->SetDataModeToAscii() ;
    }
    else // should not arrive here. tested in main before already and program should exit at that time.
    {
        std::cout << "encoding parameter should be set as binary, appended or ascii." << std::endl ;
        exit( 1 ) ;
    }
    writer->Update() ;
}

int main( int argc, char *argv[] )
{
    std::vector< std::string > fileNameList;
    std::string flagFileName = "-f" ;
    int count = 1 ;
    while( strcmp( argv[ count ] , flagFileName.c_str() )  )
    {
        if( count >= argc-1 )
        {
            std::cout << "Missing -f flag" << std::endl ;
            std::cout << "USAGE:" << std::endl << "     ./VTKCompression  [-e] [<std::string>] "
                      << std::endl << "                       [-c <integer>] "
                      << std::endl << "                       [-f <std::vector<std::string>>] "
                      << std::endl << "                       <std::string>] [--] [--version] [-h]" << std::endl ;
            return EXIT_FAILURE ;
        }
        count ++ ;
    }
    count ++ ;
    while( count < argc )
    {
        if( argv[ count ][ 0 ] == '-' )
        {
            std::cout << "Please write all your flags before the -f flag. -f flag should be the last flag. "<< std::endl ;
            return EXIT_FAILURE ;
        }
        else
        {
            fileNameList.push_back( argv[ count ] ) ;
        }
        count ++ ;
    }
    argc=argc-fileNameList.size() - 1 ;
    PARSE_ARGS ;
    if( encoding != "binary" && encoding != "appended" && encoding != "ascii" )
    {
        std::cout << "encoding parameter should be set as binary, appended or ascii." << std::endl ;
        return EXIT_FAILURE ;
    }
    std::string append = "-compressed" ;
    for( size_t i = 0 ; i < fileNameList.size() ; i++ )
    {
        std::string strippedFileName = itksys::SystemTools::GetFilenameWithoutLastExtension( fileNameList[ i ] ) ;
        if( strippedFileName.find_last_of( append ) != strippedFileName.length()-1 ) //looks if "-compressed" is already at the end of the filename> if yes, the file will be skipped
        {
            std::cout << fileNameList[ i ] << " already compressed." << std::endl ;
            continue ;
        }
        else
        {
            vtkSmartPointer< vtkPolyData > readerPolyData = vtkSmartPointer< vtkPolyData >::New() ;
            std::string outputFileName = ChangeEndOfFileName( fileNameList[ i ] , append ) ;
            readerPolyData = ReadFile( fileNameList[ i ] , itksys::SystemTools::GetFilenameLastExtension( fileNameList[ i ] ) ) ;
            WriteFile( encoding , outputFileName , compressionLevel , readerPolyData ) ;
        }
    }
    return EXIT_SUCCESS ;
}
