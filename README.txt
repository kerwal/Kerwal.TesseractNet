To Do
*****
1. Finish defining methods in TessBaseAPI (in progress).

How to Compile on Windows
*************************
1. Download source for tesseract 3.01 (original file name is tesseract-3.01.tar.gz).
2. Extract the tesseract-3.01 folder in the source package to the Kerwal.TesseractNet project folder.
3. Download and the Visual Studio 2008/2010 Solutions package for tesseract 3.01 (original file name is tesseract-3.01-win_vs.zip).
4. Extract and merge the tesseract-3.01 folder in the VS Solutions package into the Kerwal.Tesseract\tesseract-3.01 folder.
5. Open tesseract.sln in Kerwal.Tesseract\tesseract-3.01\vs2010 in VC++ 2010 (or ...\vs2008 if VC++ 2008).
6. Open the Configuration Properties of the tesseract project (Right-click tesseract in Solution Explorer then click Properties).
7. Set (General > Target Extension) to .dll.
8. Set (General > Configuration Type) to Dynamic Library (.dll).
9. Add TESSDLL_EXPORTS to (C/C++ > Preprocessor > Preprocessor Definitions).
10. Build tesseract only - it should build the dependencies in order and link into a DLL file.

To be continued...
