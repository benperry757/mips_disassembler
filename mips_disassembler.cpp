// mips_disassembler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
using namespace std;

string disassembled_instruction(bitset<32> instruction);
string out_array[5000];

int main(int argc, char** argv)
{
	// Read input file that in typed in command line
	char* inputFile;
	if (argc == 2)
	{
		inputFile = argv[1];
		cout << "Entered input file: '" << inputFile << "'" << endl;
		// Check that the proper file extension was entered on the input file *.obj
		if (strchr(inputFile, '.obj') == NULL)
		{
			cout << "Invalid file type. Use *.obj files only." << endl;
			return 1;
		}
	}
	else
	{
		cout << "Error Occured. Try Again." << endl;
		return 1;
	}
	
	string filename = argv[1];
	cout << "test filename string" << endl;
	ifstream file(filename);
	cout << "test filename ifstream" << endl;
	if (file.is_open())
	{
		cout << "test file is open" << endl;
		string line;
		cout << "test line string" << endl;
		int count = 0;
		cout << "test count" << endl;
		while (getline(file, line))
		{
			cout << "test getline while" << endl;
			//bitset<32> instruction(line);
			cout << "test bitset" << endl;
			//string output = disassembled_instruction(instruction);

			//cout << output;
			/*if (output != "error")
			{
				out_array[count++] = output;
			}
			else
			{
				cout << "Error on line: " << line << endl;
			}*/
		}
	}
	else
	{
		cout << "Unable to open file" << endl;
		return 1;
	}

	string extension = ".asm";
	string outputFile = filename.substr(0, filename.length() - 4) + extension;
	cout << "Output file name: '" << outputFile << "'" << endl;
	ofstream outfile(outputFile);
	return 0;
}

string disassembled_instruction(bitset<32> instruction)
{
	// Disassemble into the instruction formatting
	int opcode = instruction.to_ulong() >> 26;
	int rs = (instruction.to_ulong() >> 21) & 0x1F;
	int rt = (instruction.to_ulong() >> 16) & 0x1F;
	int rd = (instruction.to_ulong() >> 11) & 0x1F;
	int shamt = (instruction.to_ulong() >> 6) & 0x1F;
	int funct = instruction.to_ulong() & 0x3F;

	// Based on the opcode, the respective instruction is returned
	switch (opcode)
	{
	case 0x00:
		// For R-type instructions, the funct is used to determine the respective instruction
		switch (funct)
		{
		case 0x20:

		case 0x21:

		case 0x24:

		case 0x08:

		case 0x27:

		case 0x25:

		case 0x2A:

		case 0x2B:

		case 0x00:

		case 0x02:

		case 0x22:

		case 0x23:

		default:
			return "error";

		}
	
	case 0x08:

	case 0x09:

	case 0x0C:

	case 0x04:

	case 0x05:

	case 0x02:

	case 0x03:

	case 0x24:

	case 0x25:

	case 0x30:

	case 0x0F:

	case 0x23:

	case 0x0D:

	case 0x0A:

	case 0x0B:

	case 0x28:

	case 0x38:

	case 0x29:

	case 0x2B:

	default:
		return "error";
	}
	return "test";
}
