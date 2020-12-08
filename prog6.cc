/*
 * Filename:            prog6.cc
 * Date:                11/25/2020
 * Author:              Ragini Khanduri
 * Email:               rxk162130@utdallas.edu
 * Version:             1.0
 * Copyright:           2020, All Rights Reserved
 *
 * Description:
 *
 *      Build and display a small text based GUI matrix using curses
 *      and the CDK.
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>
#include <sstream>
#include "cdk.h"


/*
 * For grins and giggles, we will define values using the C
 * Preprocessor instead of C or C++ data types.  These symbols (and
 * their values) get inserted into the Preprocessor's symbol table.
 * The names are replaced by their values when seen later in the code.
 */

#define MATRIX_ROWS 6
#define MATRIX_COLS 3
#define BOX_WIDTH 15
#define MATRIX_NAME_STRING "Test Matrix"

using namespace std;

const int maxRecordStringLength = 25;

class BinaryFileRecord
{
    public:
    
        uint8_t strLength;
	char    stringBuffer[maxRecordStringLength];
};

class BinaryFileHeader
{
    public:
        uint32_t magicNumber;		/* Should be 0xFEEDFACE */
	uint32_t versionNumber;
	uint64_t numRecords;

};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // CDK uses offset 1 and C/C++ use offset 0.  Therefore, we create one more 
  // slot than necessary and ignore the value at location 0.
  const char 		*rowTitles[MATRIX_ROWS+1] = {"IGNORE", "R1", "R2", "R3", "R4", "R5", "R6"};
  const char 		*columnTitles[MATRIX_COLS+1] = {"IGNORE", "C1", "C2", "C3"};
  int		colWidths[MATRIX_COLS+1] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[MATRIX_COLS+1] = {vMIXED, vMIXED, vMIXED, vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_ROWS, MATRIX_COLS, MATRIX_ROWS, MATRIX_COLS,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, colWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);
  
  ifstream binInFile("cs3377.bin", ios::in | ios::binary);
  BinaryFileHeader *binHeader = new BinaryFileHeader();
  BinaryFileRecord *binRecord = new BinaryFileRecord();

  binInFile.read((char *) binHeader, sizeof(BinaryFileHeader));

  string magicNum = "Magic: 0x"; 
  string version = "Version: ";
  string numRecords = "NumRecords: ";
  string strlen = "strlen: ";
  string length1 = length2 = length3 = length4 = "";
  string record1 = record2 = record3 = record4 = "";

  stringstream magic;
  magic << hex << uppercase << binHeader->magicNumber;
  magicNum += magic.str();

  stringstream ver;
  ver << hex << binHeader->versionNumber;
  version += ver.str();

  stringstream numRecs;
  numRecs << binHeader->numRecords;
  numRecords += numRecs.str();

  binInFile.read((char*) binRecord, sizeof(BinaryFileRecord));		// Get 1st record
  
  stringstream rec1;
  rec1 << binRecord->stringBuffer;
  record1 += rec1.str();

  stringstream len1;
  len1 << record1.length();
  length1 = strlen + len1.str();

  binInFile.read((char*) binRecord, sizeof(BinaryFileRecord));		// Get 2nd record

  stringstream rec2;
  rec2 << binRecord->stringBuffer;
  record2 += rec2.str();

  stringstream len2;
  len2 << record2.length();
  length2 = strlen + len2.str();

  binInFile.read((char*) binRecord, sizeof(BinaryFileRecord));		// Get 3rd record

  stringstream rec3;
  rec3 << binRecord->stringBuffer;
  record3 += rec3.str();

  stringstream len3;
  len3 << record3.length();
  length3 = strlen + len3.str();

  binInFile.read((char*) binRecord, sizeof(BinaryFileRecord));		// Get 4th record

  stringstream rec4;
  rec4 << binRecord->stringBuffer;
  record4 += rec4.str();

  stringstream len4;
  len4 << record4.length();
  length4 = strlen + len4.str();

  /*
   * Display a message
   */
  
  setCDKMatrixCell(myMatrix, 1, 1, magicNum.c_str());
  setCDKMatrixCell(myMatrix, 1, 2, version.c_str());
  setCDKMatrixCell(myMatrix, 1, 3, numRecords.c_str());
  setCDKMatrixCell(myMatrix, 2, 1, length1.c_str());
  setCDKMatrixCell(myMatrix, 2, 2, record1.c_str());
  setCDKMatrixCell(myMatrix, 3, 1, length2.c_str());
  setCDKMatrixCell(myMatrix, 3, 2, record2.c_str());
  setCDKMatrixCell(myMatrix, 4, 1, length3.c_str());
  setCDKMatrixCell(myMatrix, 4, 2, record3.c_str());
  setCDKMatrixCell(myMatrix, 5, 1, length4.c_str());
  setCDKMatrixCell(myMatrix, 5, 2, record4.c_str());

  drawCDKMatrix(myMatrix, true);    /* required  */

  /* so we can see results */
  /* Wait for user to press a key before exiting  */
  unsigned char anyKey;
  cin >> anyKey;

  // Cleanup screen
  endCDK();
}
