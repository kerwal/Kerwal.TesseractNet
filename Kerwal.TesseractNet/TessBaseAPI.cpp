#include <vcclr.h>

#include "TessBaseAPI.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace Kerwal
{
namespace TesseractNet
{
	const char* StringToMultiByte(String^ string);
	String^ GetMessageForLastError();
	String^ GetMessageForErrNo();

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
		  const char* cName = StringToMultiByte(name);
		  this->_tessBaseApi->SetInputName(cName);
	  }
	  
	  void TessBaseAPI::SetOutputName(String^ name)
	  {
		  const char* cName = StringToMultiByte(name);
		  this->_tessBaseApi->SetOutputName(cName);
	  }
	  
	  bool TessBaseAPI::SetVariable(String^ name, String^ value)
	  {
		  const char* cName = StringToMultiByte(name);
		  const char* cValue = StringToMultiByte(value);
		  return this->_tessBaseApi->SetVariable(cName, cValue);
	  }

	  bool TessBaseAPI::GetIntVariable(String^ name, [Out] int% value)
	  {
		  const char* cName = StringToMultiByte(name);
		  pin_ptr<int> pValue = &value;
		  return this->_tessBaseApi->GetIntVariable(cName, (int*)value);
	  }

	  bool TessBaseAPI::GetBoolVariable(String^ name, [Out] bool% value)
	  {
		  const char* cName = StringToMultiByte(name);
		  pin_ptr<bool> pValue = &value;
		  return this->_tessBaseApi->GetBoolVariable(cName, (bool*)value);
	  }

	  bool TessBaseAPI::GetDoubleVariable(String^ name, [Out] double% value)
	  {
		  const char* cName = StringToMultiByte(name);
		  pin_ptr<double> pValue = &value;
		  return this->_tessBaseApi->GetDoubleVariable(cName, (double*)pValue);
	  }

	  const char* StringToMultiByte(String^ string)
	  {
		  // retrieve the internal pointer to the array of WCHARs and pin it so the garbage collector leaves it alone
		  pin_ptr<const WCHAR> wString = PtrToStringChars(string); // TODO can this fail and how?
		  // get the necessary size of the buffer
		  int size = WideCharToMultiByte(CP_ACP, 0, wString, -1, NULL, 0, NULL, NULL);
		  if(!size)
		  {
			  wString = nullptr;
			  throw gcnew Exception(GetMessageForLastError());
		  }
		  char *cString = (char *)malloc(size);
		  if(!cString)
		  {
			  wString = nullptr;
			  throw gcnew Exception(GetMessageForErrNo());
		  }
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

	  String^ GetMessageForErrNo()
	  {
		  // TODO maybe just use _wcserror_s(&buff, NULL) instead?
		  errno_t code = 0;
		  errno_t error = 0;
		  error = _get_errno(&code);
		  if(error) throw gcnew Exception("_get_errno returned '"+error+"'");
		  WCHAR message[72];
		  error = _wcserror_s(message, 72, code);
		  if(error) throw gcnew Exception("_get_errno returned '"+error+"'");
		  return Marshal::PtrToStringUni((IntPtr)message);
	  }
} // namespace TesseractNet
} // namespace Kerwal