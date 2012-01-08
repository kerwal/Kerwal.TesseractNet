#include <vcclr.h>

#include "TessBaseAPI.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Kerwal
{
namespace TesseractNet
{
	char* StringToMultiByte(String^ string);
	String^ GetMessageForLastError();

	TessBaseAPI::TessBaseAPI()
		: _tessBaseApi(new tesseract::TessBaseAPI())
	{
	}

	TessBaseAPI::~TessBaseAPI()
	{
		  if(this->_tessBaseApi)
		  {
			  delete this->_tessBaseApi;
			  this->_tessBaseApi = NULL;
		  }
	}

	TessBaseAPI::!TessBaseAPI()
	{
		  if(this->_tessBaseApi)
		  {
			  delete this->_tessBaseApi;
			  this->_tessBaseApi = NULL;
		  }
	}

	  String^ TessBaseAPI::Version()
	  {
		  return Marshal::PtrToStringAnsi((IntPtr)(char *)tesseract::TessBaseAPI::Version());
	  }
	  
	  void TessBaseAPI::SetInputName(String^ name)
	  {
		  char *newName = StringToMultiByte(name);
		  this->_tessBaseApi->SetInputName(newName);
	  }

	  char* StringToMultiByte(String^ string)
	  {
		  // retrieve the internal pointer to the array of WCHARs and pin it so the garbage collector leaves it alone
		  pin_ptr<const WCHAR> wString = PtrToStringChars(string); // TODO can this fail and how?
		  int size = WideCharToMultiByte(CP_ACP, 0, wString, -1, NULL, 0, NULL, NULL);
		  // TODO check for failure and throw exception if so
		  char *cString = (char *)malloc(size); // TODO can this fail and how?
		  if(!WideCharToMultiByte(CP_ACP, 0, wString, -1, cString, size, NULL, NULL))
		  {
			  if(cString) delete cString;
			  throw gcnew Exception(GetMessageForLastError());
		  }
		  return cString;
	  }

	  String^ GetMessageForLastError()
	  {
			  WCHAR* wErrorMessage = NULL;
			  int nLen = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), (LPWSTR)&wErrorMessage, 1, NULL);
			  String^ errorMessage = Marshal::PtrToStringUni((IntPtr)wErrorMessage);
			  LocalFree(wErrorMessage);
			  return errorMessage;
	  }
} // namespace TesseractNet
} // namespace Kerwal