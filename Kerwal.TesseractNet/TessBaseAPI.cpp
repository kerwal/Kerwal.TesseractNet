#include <vcclr.h>

#include "TessBaseAPI.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

class STRING;

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
		return Marshal::PtrToStringAuto((IntPtr)(char*)tesseract::TessBaseAPI::Version());
	}
	  
	void TessBaseAPI::SetInputName(String^ name)
	{
		const char* cName = StringToMultiByte(name);
		this->_tessBaseApi->SetInputName(cName);
		free((char*)cName);
	}

	void TessBaseAPI::SetOutputName(String^ name)
	{
		const char* cName = StringToMultiByte(name);
		this->_tessBaseApi->SetOutputName(cName);
		free((char*)cName);
	}
	
	bool TessBaseAPI::SetVariable(String^ name, String^ value)
	{
		const char* cName = StringToMultiByte(name);
		const char* cValue = StringToMultiByte(value);
		bool result = this->_tessBaseApi->SetVariable(cName, cValue);
		free((char*)cName);
		free((char*)cValue);
		return result;
	}

	bool TessBaseAPI::GetIntVariable(String^ name, [Out] int% value)
	{
		const char* cName = StringToMultiByte(name);
		pin_ptr<int> pValue = &value;
		bool result = this->_tessBaseApi->GetIntVariable(cName, pValue);
		free((char*)cName);
		return result;
	}

	bool TessBaseAPI::GetBoolVariable(String^ name, [Out] bool% value)
	{
		const char* cName = StringToMultiByte(name);
		pin_ptr<bool> pValue = &value;
		bool result = this->_tessBaseApi->GetBoolVariable(cName, pValue);
		free((char*)cName);
		return result;
	}

	bool TessBaseAPI::GetDoubleVariable(String^ name, [Out] double% value)
	{
		const char* cName = StringToMultiByte(name);
		pin_ptr<double> pValue = &value;
		bool result = this->_tessBaseApi->GetDoubleVariable(cName, pValue);
		free((char*)cName);
		return result;
	}

	String^ TessBaseAPI::GetStringVariable(String^ name)
	{
		const char* cName = StringToMultiByte(name);
		const char* cValue = this->_tessBaseApi->GetStringVariable(cName);
		free((char*)cName);
		if(cValue == NULL) return nullptr;
		return Marshal::PtrToStringAuto((IntPtr)(char*)cValue);
	}

	void TessBaseAPI::PrintVariables(String^ path, String^ mode)
	{
		FILE* file = NULL;
		pin_ptr<const WCHAR> wPath = PtrToStringChars(path);
		pin_ptr<const WCHAR> wMode = PtrToStringChars(mode);
		errno_t result = _wfopen_s(&file, wPath, wMode);
		if(result) throw gcnew Exception("_wfopen_s() returned '"+result+"' - "+GetMessageForErrNo()+".");
		this->_tessBaseApi->PrintVariables(file);
		// TODO should I be paranoid about closing the file if fflush() errors?
		if(fflush(file) == EOF) throw gcnew Exception("fflush() returned 'EOF'.");
		if(fclose(file) == EOF) throw gcnew Exception("fclose() returned 'EOF'.");
		file = NULL;
	}

	bool TessBaseAPI::GetVariableAsString(String^ name, [Out] String^ val)
	{
		const char* cName = StringToMultiByte(name);
		STRING sValue;
		bool result = this->_tessBaseApi->GetVariableAsString(cName, &sValue);
		free((char*)cName);
		val = gcnew String(sValue.string()); // TODO can this be improved?
		return result;
	}

	int TessBaseAPI::Init(String^ datapath, String^ language, OcrEngineMode mode, List<String^>^ configs, List<String^>^ vars_vec, List<String^>^ vars_values, bool set_only_init_params)
	{
		const char* cDatapath = StringToMultiByte(datapath);
		const char* cLanguage = StringToMultiByte(language);
		int numConfigs = (configs != nullptr) ? configs->Count : 0;
		const char** cpConfigs = (configs != nullptr) ? new const char*[numConfigs] : NULL;
		if(cpConfigs != NULL)
		{
			int newConfigIndex = 0;
			for each(String^ config in configs)
			{
				cpConfigs[newConfigIndex] = StringToMultiByte(config);
				newConfigIndex++;
			}
		}
		GenericVector<STRING>* gvVars_vec = (vars_vec != nullptr) ? new GenericVector<STRING>(vars_vec->Count) : NULL;
		if(gvVars_vec != NULL)
		{
			for each(String^ var in vars_vec)
			{
				const char* cVar = StringToMultiByte(var);
				gvVars_vec->push_front(cVar);
				free((char*)cVar); // TODO make sure this isn't deleting the copy in gvVars_vec
			}
		}
		GenericVector<STRING>* gvVars_values = (vars_values != nullptr) ? new GenericVector<STRING>(vars_values->Count) : NULL;
		if(gvVars_values != NULL)
		{
			for each(String^ value in vars_values)
			{
				const char* cValue = StringToMultiByte(value);
				gvVars_values->push_front(cValue);
				free((char*)cValue); // TODO make sure this isn't deleting the copy in gvVars_values
			}
		}
		int result = this->_tessBaseApi->Init(cDatapath, cLanguage, static_cast<tesseract::OcrEngineMode>(mode), (char**)cpConfigs, numConfigs, gvVars_vec, gvVars_values, set_only_init_params);
		if(gvVars_values != NULL) delete gvVars_values;
		if(gvVars_vec != NULL) delete gvVars_vec;
		if(cpConfigs != NULL)
		{
			for(int i = 0; i < numConfigs; i++)
			{
				free((char*)cpConfigs[i]);
				//cpConfigs[i] = NULL; // <-- not necessary if delete[] is called immediately after the loop
			}
			delete[] cpConfigs;
		}
		free((char*)cLanguage);
		free((char*)cDatapath);
		return result;
	}

	  // The string returned by this function must be deallocated by calling "free()" on it
	  const char* StringToMultiByte(String^ string)
	  {
		  // retrieve the internal pointer to the array of WCHARs and pin it so the garbage collector leaves it alone
		  pin_ptr<const WCHAR> wString = PtrToStringChars(string);
		  // get the necessary size of the buffer
		  int size = WideCharToMultiByte(CP_ACP, 0, wString, -1, NULL, 0, NULL, NULL);
		  if(!size)
		  {
			  wString = nullptr;
			  throw gcnew Exception("WideCharToMultiByte() returned '0' - "+GetMessageForLastError()+".");
		  }
		  // allocate the buffer on the stack
		  char *cString = (char*)malloc(size);
		  if(!cString)
		  {
			  wString = nullptr;
			  throw gcnew Exception("malloc() returned 'null' - "+GetMessageForErrNo()+".");
		  }
		  // transcode and copy the string to the buffer
		  if(!WideCharToMultiByte(CP_ACP, 0, wString, -1, cString, size, NULL, NULL))
		  {
			  if(cString) delete cString;
			  throw gcnew Exception("WideCharToMultiByte() returned '0' - "+GetMessageForLastError()+".");
		  }
		  // return the new ANSI encoded string
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
		  if(error) throw gcnew Exception("_get_errno() returned '"+error+"'.");
		  WCHAR message[72];
		  error = _wcserror_s(message, 72, code);
		  if(error) throw gcnew Exception("_get_errno() returned '"+error+"'.");
		  return Marshal::PtrToStringUni((IntPtr)message);
	  }
} // namespace TesseractNet
} // namespace Kerwal