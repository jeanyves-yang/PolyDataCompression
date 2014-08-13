#include "VTKCompressionCLP.h"
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

std::string ExtensionOfFile( std::string fileName )
{
    std::string extension ;
    extension = fileName.substr( fileName.find_last_of(".")+1,fileName.size()-fileName.find_last_of( "." ) + 1 ) ;
    return extension ;
}

std::string ChangeEndOfFileName ( std::string fileName, std::string change )
{
  std::string extension = ExtensionOfFile( fileName ) ;
  fileName.replace( fileName.end()-extension.length()-1 , fileName.end() , change ) ;
  fileName = fileName + ".vtp" ;
  return fileName;
}

int main( int argc, char *argv[] )
{
  std::vector <std::string> fileNameList;
  char * flagFileName ( "-f" ) ;
  int count = 1 ;
  while( strcmp( argv[ count ] , flagFileName )  )
  {
    if( count >= argc-1 )
    {
      std::cout << "Missing -f flag" <<std::endl ;
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
      std::cout<<"Please move any flags before the -f flag. -f flag should be the last flag. "<<std::endl;
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
  for( size_t i=0 ; i <  fileNameList.size() ; i++ )
  {
    if( !strstr( fileNameList[i].c_str() , "compressed" ) )
    {
      std::cout<<fileNameList[i]<<std::endl;
      std::string outputFileName = fileNameList[i] ;
      std::string extension = ExtensionOfFile( outputFileName ) ;
      std::string replacement = "-compressed" ;
      outputFileName = ChangeEndOfFileName( outputFileName , replacement) ;

      if( extension =="vtk" )
      {
        vtkSmartPointer< vtkPolyDataReader > reader = vtkSmartPointer< vtkPolyDataReader >::New() ;
        reader->SetFileName( fileNameList[i].c_str() ) ;
        reader->Update() ;
        vtkSmartPointer< vtkXMLPolyDataWriter > writer = vtkSmartPointer< vtkXMLPolyDataWriter >::New() ;
        writer->SetFileName( outputFileName.c_str() ) ;
        writer->SetInputData( reader->GetOutput() ) ;
        vtkZLibDataCompressor *compressor = dynamic_cast<vtkZLibDataCompressor*> (writer->GetCompressor()) ;
        if( compressor )
        {
          compressor->SetCompressionLevel( compressorLevel ) ;
        }
        if( !strcmp( encoding.c_str() , "binary" ) )
        {
          writer->SetDataModeToBinary();
        }
        else if(!strcmp( encoding.c_str() , "appended" ) )
        {
          writer->SetDataModeToAppended();
        }
        else
        {
          writer->SetDataModeToAscii();
        }
        writer->Update() ;
      }
      else
      {
          vtkSmartPointer< vtkXMLPolyDataReader > reader = vtkSmartPointer< vtkXMLPolyDataReader >::New() ;
          reader->SetFileName( fileNameList[i].c_str() ) ;
          reader->Update() ;
          vtkSmartPointer< vtkXMLPolyDataWriter > writer = vtkSmartPointer< vtkXMLPolyDataWriter >::New() ;
          writer->SetFileName( outputFileName.c_str() ) ;
          writer->SetInputData( reader->GetOutput() ) ;
          vtkZLibDataCompressor *compressor = dynamic_cast<vtkZLibDataCompressor*> (writer->GetCompressor()) ;
          if( compressor )
          {
            compressor->SetCompressionLevel( compressorLevel ) ;
          }
          if( !strcmp( encoding.c_str() , "BINARY" ) )
          {
            writer->SetDataModeToBinary();
          }
          else if(!strcmp( encoding.c_str() , "APPENDED" ) )
          {
            writer->SetDataModeToAppended();
          }
          else
          {
            writer->SetDataModeToAscii();
          }
          writer->Update() ;
      }
    }
    else
    {}
  }
  return EXIT_SUCCESS ;
}
