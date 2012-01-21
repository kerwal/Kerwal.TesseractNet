Objective
*********
Write a simple C++/CLI .Net wrapper to tesseract-ocr so it can be used easily in C#, but keep the classes as similar to the
original as possible. For now the scope of this project only focuses on the tesseract::TessBaseAPI class. I hope to expand
the scope in the near future.

Current State
*************
Only the most useful methods are implemented right now. Currently, it is only possible to retrieve the text from image files
on disk - not in memory...yet. Only the Debug build configuration is available right now. For those not familiar with tesseract,
it uses the Pix class from leptonica (liblept) for storing and processing image data. Leptonica can read and save image files in
JPEG, TIFF, PNG, Z, and GIF formats.

To Do
*****
1. Update build configurations to target appropriate LIB and DLL files for Debug or Release.
	- Debug should target tesseract-debug.lib and post build should copy tesseract-debug.dll to Debug folder.
	- Release should target tesseract.lib and post build should copy tesseract.dll to Release folder.
2. Finish defining methods in TessBaseAPI (in progress).
3. Improve error catching for tesseract::TessBaseAPI:: method calls.

How to Compile on Windows (Debug only)
*************************
1. Download source for tesseract 3.01 (original file name is tesseract-3.01.tar.gz).
2. Extract the tesseract-3.01 folder in the source package to the Kerwal.TesseractNet project folder.
3. Download the Visual Studio 2008/2010 Solutions package for tesseract 3.01 (original file name is tesseract-3.01-win_vs.zip).
4. Extract and merge the tesseract-3.01 folder in the VS Solutions package into the Kerwal.Tesseract\tesseract-3.01 folder.
5. Open tesseract.sln in Kerwal.Tesseract\tesseract-3.01\vs2010 in VC++ 2010 (or ...\vs2008 if VC++ 2008).
6. Open the Configuration Properties of the tesseract project (Right-click tesseract in Solution Explorer then click Properties).
7. Set (General > Target Extension) to .dll.
8. Set (General > Configuration Type) to Dynamic Library (.dll).
9. Add TESSDLL_EXPORTS to (C/C++ > Preprocessor > Preprocessor Definitions).
10. Build tesseract only - it should build the dependencies in order and link into a DLL file.
11. Open Kerwal.TesseractNet and compile.

How to Use
**********
1. Reference (and copy) the Kerwal.TesseractNet.dll.
2. Copy tesseract-debug.dll from Kerwal.TesseractNet\tesseract-3.01\vs2010\Debug.
3. "using Kerwal.TesseractNet".
4. Use TessBaseAPI like in C++ - with only a few differences. See example below.
5. Compile.
6. Copy tessdata folder to output directory. tessdata should be available from where you got tesseract-3.01.tar.gz

Example Usage
*************
// print the version of the Tesseract library
Console.WriteLine(TessBaseAPI.Version());
// initialize the base API
TessBaseAPI tessBaseApi = new TessBaseAPI();
tessBaseApi.Init("", null); // default to "eng"
// set the image to process and retrieve a pointer to the Pix object
IntPtr pix = tessBaseApi.SetImage("device-2012-01-09-210231.png");
// set where to look for text
tessBaseApi.SetRectangle(0, 615, 137, 31);
// recognize and look for text - can return null
string text = tessBaseApi.GetUTF8Text();
// release the Pix object [IMPORTANT!]
tessBaseApi.FreePix(pix);
Console.WriteLine(text);
Console.ReadKey();