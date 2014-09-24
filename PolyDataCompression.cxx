#include "PolyDataCompressionCLP.h"
#include <string>
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

std::string ChangeEndOfFileName ( std::string fileName, std::string change )
{
    if( itksys::SystemTools::GetFilenamePath( fileName ).empty() == true )
    {
        return itksys::SystemTools::GetFilenameWithoutLastExtension( fileName ) + change + ".vtp" ;
    }
    return  itksys::SystemTools::GetFilenamePath( fileName )
            + "/" + itksys::SystemTools::GetFilenameWithoutLastExtension( fileName ) + change + ".vtp" ;
}

vtkSmartPointer< vtkPolyData > ReadFile( const char* fileName , std::string extension )
{
    if( extension == ".vtk" )
    {
        vtkSmartPointer< vtkPolyDataReader > reader = vtkSmartPointer< vtkPolyDataReader >::New() ;
        reader->SetFileName( fileName ) ;
        reader->Update() ;
        return reader->GetOutput() ;
    }
    else if( extension == ".vtp" )
    {
        vtkSmartPointer< vtkXMLPolyDataReader > reader = vtkSmartPointer< vtkXMLPolyDataReader >::New() ;
        reader->SetFileName( fileName ) ;
        reader->Update() ;
        return reader->GetOutput() ;
    }
    else
    {
        return NULL ;
    }
}

int WriteFile( std::string encoding , const char* outputFileName ,
                int compressionLevel , vtkSmartPointer< vtkPolyData > readerPolyData )
{
    vtkSmartPointer< vtkXMLPolyDataWriter > writer = vtkSmartPointer< vtkXMLPolyDataWriter >::New() ;
    writer->SetFileName( outputFileName ) ;
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
    else if( encoding == "ascii" )
    {
        writer->SetDataModeToAscii() ;
    }
    else // should not arrive here. tested in main before already and program should exit at that time.
    {
        return 1 ;
    }
    writer->Update() ;
    return 0 ;
}

int main( int argc, char *argv[] )
{
    std::vector< std::string > fileNameList ;
    bool flagFound = false ;
    for( int i = 0 ; i < argc ; i++ )
    {
        if( flagFound )
        {
            if( argv[ i ][ 0 ] == '-' )
            {
                std::cout << "Please write all your flags before the -f flag. -f flag should be the last flag. "<< std::endl ;
                return EXIT_FAILURE ;
            }
            else
            {
              fileNameList.push_back( argv[ i ] ) ;
            }
        }
        if( !strcmp( argv[ i ] , "-f" ) || !strcmp( argv[ i ] , "--fileNameList" ) )
        {
            flagFound = true ;
        }
    }
    argc = argc - ( fileNameList.empty() ? 0 : fileNameList.size() - 1 ) ;
    PARSE_ARGS ;
    if( !flagFound )
    {
       std::cerr << "Missing fileNameList flag, please give at least one file." << std::endl ;
       return EXIT_FAILURE ;
    }
    std::string append = "-compressed" ;
    for( size_t i = 0 ; i < fileNameList.size() ; i++ )
    {
        std::string testFileName = itksys::SystemTools::GetFilenameName( ( fileNameList[ i ] ) ) ;
        if(  testFileName.find( append + ".vtp" ) != std::string::npos ) //looks if "-compressed" is already at the end of the filename -> if yes, the file will be skipped
        {
            std::cout << fileNameList[ i ] << " already compressed." << std::endl ;
            continue ;
        }
        else
        {
            vtkSmartPointer< vtkPolyData > readerPolyData = vtkSmartPointer< vtkPolyData >::New() ;
            std::string outputFileName = ChangeEndOfFileName( fileNameList[ i ] , append ) ;
            readerPolyData = ReadFile( fileNameList[ i ].c_str() , itksys::SystemTools::GetFilenameLastExtension( fileNameList[ i ] ) ) ;
            if ( readerPolyData == NULL )
            {
                std::cerr << "input file is neither a vtk nor a vtp file." << std::endl ;
            }
            else
            {
            }
            if( !readerPolyData.GetPointer() )
            {
                return EXIT_FAILURE ;
            }
            if( WriteFile( encoding , outputFileName.c_str() , compressionLevel , readerPolyData )  == 1 )
            {
                return EXIT_FAILURE ;
            }
        }
    }
    return EXIT_SUCCESS ;
}
