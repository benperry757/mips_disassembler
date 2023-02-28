// mips_disassembler.cpp
// 
// Author: Benjamin Perry
// ECE-3504 : MIPS Instruction Disassembler
// Created on: February 25th, 2023
// Last Modified: February 28th, 2023
// 
// This program reads a *.obj input file containing instruction code in hexadecimal format
// and converts it to the MIPS instruction format that is easily read by a user.
// The converted instructions are stored in an output file with the same filename as the
// input but with an extension of *.asm in the same directory as the executible file.
// 
// The program is run from the command line in the directory where the executible file is stored.
// 
// Command: 'mips_disassembler *.obj'   where '*.obj' is the desired input file.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>
#include <map>
using namespace std;

// This function does the main conversion of the instructions.
string disassembled_instruction(string line , int count);
// This function converts binary values to decimal values.
int binary_to_decimal(string x);
// This function converts binary values to signed integers.
int binary_to_signed(string x);
bool errorOccurred;
vector<string> addressLabel;
vector<int> addressIndex;

// Maps each register to its respective binary value.
map<string, string> registerMap = {
	{"00000", "$0"},		// $0  : 0
	{"00001", "$at"},		// $at : 1
	{"00010", "$v0"},		// $v0 : 2
	{"00011", "$v1"},		// $v1 : 3
	{"00100", "$a0"},		// $a0 : 4
	{"00101", "$a1"},		// $a1 : 5
	{"00110", "$a2"},		// $a2 : 6
	{"00111", "$a3"},		// $a3 : 7
	{"01000", "$t0"},		// $t0 : 8
	{"01001", "$t1"},		// $t1 : 9
	{"01010", "$t2"},		// $t2 : 10
	{"01011", "$t3"},		// $t3 : 11
	{"01100", "$t4"},		// $t4 : 12
	{"01101", "$t5"},		// $t5 : 13
	{"01110", "$t6"},		// $t6 : 14
	{"01111", "$t7"},		// $t7 : 15
	{"10000", "$s0"},		// $s0 : 16
	{"10001", "$s1"},		// $s1 : 17
	{"10010", "$s2"},		// $s2 : 18
	{"10011", "$s3"},		// $s3 : 19
	{"10100", "$s4"},		// $s4 : 20
	{"10101", "$s5"},		// $s5 : 21
	{"10110", "$s6"},		// $s6 : 22
	{"10111", "$s7"},		// $s7 : 23
	{"11000", "$t8"},		// $t8 : 24
	{"11001", "$t9"},		// $t9 : 25
	{"11010", "$k0"},		// $k0 : 26
	{"11011", "$k1"},		// $k1 : 27
	{"11100", "$gp"},		// $gp : 28
	{"11101", "$sp"},		// $sp : 29
	{"11110", "$fp"},		// $fp : 30
	{"11111", "$ra"},		// $ra : 31
};

int main(int argc, char** argv)
{	
	// Read input file that is typed in command line
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
	ifstream file(filename);
	vector<string> outVector;
	unsigned int address = 0;
	errorOccurred = false;
	int count = 0;
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			string output = disassembled_instruction(line, count);
			// Inserts disassembled instruction line into output vector.
			outVector.push_back(output);
			count++;
		}
	}
	else
	{
		cout << "Error: Unable to Open Input File." << endl;
		return 1;
	}
	// If an error occurred during disassembly, code stops and all lines with errors are shown in the command line.
	if (errorOccurred)
	{
		cout << "Program stopped due to errors. An output wasn't generated. :(" << endl;
		return 1;
	}

	// Insert any address labels that are needed into the output vector.
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < addressIndex.size(); j++)
		{
			// Check if the index matches an index stored for any address labels.
			if (i == addressIndex[j])
			{
				// Increase size of output vector by 1.
				outVector.resize(outVector.size() + 1);
				// Increase line count by 1.
				count++;
				
				for (int k = outVector.size() - 1; k > i; k--)
				{
					// Shift contents of output vector.
					outVector[k] = outVector[k - 1];
				}
				
				// Inserts the address label at the proper location.
				outVector[i] = addressLabel[j];
				
				// remove the address index from the vector as it is no longer needed.
				addressIndex.erase(addressIndex.begin() + j);
				
				// Update address index values to account for the added element in the output vector.
				for (int n = 0; n < addressIndex.size(); n++)
				{
					addressIndex[n] += 1;
				}
			}
		}
	}
	
	// Creates the output file with the same file name as the input but as a *.asm file.
	string extension = ".asm";
	string outputFile = filename.substr(0, filename.length() - 4) + extension;
	cout << "Output file name: '" << outputFile << "'" << endl;
	ofstream outfile(outputFile);
	if (outfile.is_open())
	{
		// Outputs the contents of the output vector to the output file.
		for (int i = 0; i < count; i++)
		{
			outfile << outVector[i] << endl;
		}
	}
	else
	{
		cout << "Error: Unable to Open Output File." << endl;
		return 1;
	}
	return 0;
}

string disassembled_instruction(string line, int count)
{
	string comma = ", ";
	// Prints error if line is not 8 Hex values.
	if (line.length() != 8)
	{
		cout << "Error: Invalid Hex Length on Line: " << line << endl;
		errorOccurred = true;
		return "error";
	}
	string instruction;
	// Convert Hex string to binary
	for (char hex : line)
	{
		switch (toupper(hex))
		{
		case '0': instruction += "0000"; break;
		case '1': instruction += "0001"; break;
		case '2': instruction += "0010"; break;
		case '3': instruction += "0011"; break;
		case '4': instruction += "0100"; break;
		case '5': instruction += "0101"; break;
		case '6': instruction += "0110"; break;
		case '7': instruction += "0111"; break;
		case '8': instruction += "1000"; break;
		case '9': instruction += "1001"; break;
		case 'A': instruction += "1010"; break;
		case 'B': instruction += "1011"; break;
		case 'C': instruction += "1100"; break;
		case 'D': instruction += "1101"; break;
		case 'E': instruction += "1110"; break;
		case 'F': instruction += "1111"; break;
		default: 
			cout << "Error: Invalid Hex Digit on Line: " << line << endl;
			errorOccurred = true;
			return "error";
		}
		
	}

	// Disassemble into the instruction formatting
	string opcode = instruction.substr(0, 6);
	string rs = instruction.substr(6, 5);
	string rt = instruction.substr(11, 5);
	string rd = instruction.substr(16, 5);
	string shamt = instruction.substr(21, 5);
	string funct = instruction.substr(26, 6);
	string immediate = instruction.substr(16, 16);

	// Based on the opcode, the respective instruction is returned
	if (opcode == "000000")
	{
		// For R-type instructions, the funct is used to determine the respective instruction
		// add : 0x20
		if (funct == "100000")
		{
			string out = "\tadd\t";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);
			
			return (out + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// addu : 0x21
		if (funct == "100001")
		{
			string out = "\taddu\t";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// and : 0x24
		if (funct == "100100")
		{
			string out = "\tand\t";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// nor : 0x27
		if (funct == "100111")
		{
			string out = "\tnor\t";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// or : 0x25
		if (funct == "100101")
		{
			string out = "\tor\t";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// slt : 0x2A
		if (funct == "101010")
		{
			string out = "\tslt\t";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// sltu : 0x2B
		if (funct == "101011")
		{
			string out = "\tsltu\t";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// sll : 0x00
		if (funct == "000000")
		{
			string out = "\tsll\t";
			auto rdMapped = registerMap.find(rd);
			auto rtMapped = registerMap.find(rt);
			int shamt_decimal = binary_to_decimal(shamt);

			return (out + rdMapped->second + comma + rtMapped->second + comma + to_string(shamt_decimal));
		}
		// srl : 0x02
		if (funct == "000010")
		{
			string out = "\tsrl\t";
			auto rdMapped = registerMap.find(rd);
			auto rtMapped = registerMap.find(rt);
			int shamt_decimal = binary_to_decimal(shamt);

			return (out + rdMapped->second + comma + rtMapped->second + comma + to_string(shamt_decimal));
		}
		// sub : 0x22
		if (funct == "100010")
		{
			string out = "\tsub\t";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// subu : 0x23
		if (funct == "100011")
		{
			string out = "\tsubu\t";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		else
		{
			cout << "Error: Invalid Funct on Line: " << line << endl;
			errorOccurred = true;
			return "error";
		}
	}
	// addi : 0x08
	else if (opcode == "001000")
	{
		string out = "\taddi\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + rsMapped->second + comma + to_string(immediate_decimal));
	}
	// addiu : 0x09
	else if (opcode == "001001")
	{
		string out = "\taddiu\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + rsMapped->second + comma + to_string(immediate_decimal));
	}
	// andi : 0x0C
	else if (opcode == "001100")
	{
		string out = "\tandi\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + rsMapped->second + comma + to_string(immediate_decimal));
	}
	// beq : 0x04
	else if (opcode == "000100")
	{
		string out = "\tbeq\t";
		auto rsMapped = registerMap.find(rs);
		auto rtMapped = registerMap.find(rt);
		int signed_immediate = binary_to_signed(immediate);
		int addr = (count + signed_immediate + 1);	// decimal value of offset address
		// Checks if there is already an address index stored. 
		// If not, adds the address index to the addressIndex vector.
		if (find(addressIndex.begin(), addressIndex.end(), addr) == addressIndex.end())
		{
			addressIndex.push_back(addr);
		}
		int offset = addr * 4;
		stringstream stream;
		stream << hex << offset;
		string hex_addr = stream.str();
		while (hex_addr.length() < 4)
		{
			hex_addr = "0" + hex_addr;
		}
		string address = "Addr_" + hex_addr;
		string cool_addr = address + ":";
		// Checks if there is already an address label stored. 
		// If not, adds the address label to the addressLabel vector.
		if (find(addressLabel.begin(), addressLabel.end(), cool_addr) == addressLabel.end())
		{
			addressLabel.push_back(cool_addr);
		}

		return (out + rsMapped->second + comma + rtMapped->second + comma + address);
	}
	// bne : 0x05
	else if (opcode == "000101")
	{
		string out = "\tbeq\t";
		auto rsMapped = registerMap.find(rs);
		auto rtMapped = registerMap.find(rt);
		int signed_immediate = binary_to_signed(immediate);
		int addr = (count + signed_immediate + 1);	// decimal value of offset address
		// Checks if there is already an address index stored. 
		// If not, adds the address index to the addressIndex vector.
		if (find(addressIndex.begin(), addressIndex.end(), addr) == addressIndex.end())
		{
			addressIndex.push_back(addr);
		}
		int offset = addr * 4;
		stringstream stream;
		stream << hex << offset;
		string hex_addr = stream.str();
		while (hex_addr.length() < 4)
		{
			hex_addr = "0" + hex_addr;
		}
		string address = "Addr_" + hex_addr;
		string cool_addr = address + ":";
		// Checks if there is already an address label stored. 
		// If not, adds the address label to the addressLabel vector.
		if (find(addressLabel.begin(), addressLabel.end(), cool_addr) == addressLabel.end())
		{
			addressLabel.push_back(cool_addr);
		}

		return (out + rsMapped->second + comma + rtMapped->second + comma + address);
	}
	// lbu : 0x24
	else if (opcode == "100100")
	{
		string out = "\tlbu\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		string rsParentheses = "(" + rsMapped->second + ")";
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + to_string(immediate_decimal) + rsParentheses);
	}
	// lhu : 0x25
	else if (opcode == "100101")
	{
		string out = "\tlhu\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		string rsParentheses = "(" + rsMapped->second + ")";
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + to_string(immediate_decimal) + rsParentheses);
	}
	// ll : 0x30
	else if (opcode == "110000")
	{
		string out = "\tll\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		string rsParentheses = "(" + rsMapped->second + ")";
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + to_string(immediate_decimal) + rsParentheses);
	}
	// lui : 0x0F
	else if (opcode == "001111")
	{
		string out = "\tlui\t";
		auto rtMapped = registerMap.find(rt);
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + to_string(immediate_decimal));
	}
	// lw : 0x23
	else if (opcode == "100011")
	{
		string out = "\tlw\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		string rsParentheses = "(" + rsMapped->second + ")";
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + to_string(immediate_decimal) + rsParentheses);
	}
	// ori : 0x0D
	else if (opcode == "001101")
	{
		string out = "\tori\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + rsMapped->second + comma + to_string(immediate_decimal));
	}
	// slti : 0x0A
	else if (opcode == "001010")
	{
		string out = "\tslti\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + rsMapped->second + comma + to_string(immediate_decimal));
	}
	// sltiu : 0x0B
	else if (opcode == "001011")
	{
		string out = "\tsltiu\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + rsMapped->second + comma + to_string(immediate_decimal));
	}
	// sb : 0x28
	else if (opcode == "101000")
	{
		string out = "\tsb\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		string rsParentheses = "(" + rsMapped->second + ")";
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + to_string(immediate_decimal) + rsParentheses);
	}
	// sc : 0x38
	else if (opcode == "111000")
	{
		string out = "\tsc\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		string rsParentheses = "(" + rsMapped->second + ")";
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + to_string(immediate_decimal) + rsParentheses);
	}
	// sh : 0x29
	else if (opcode == "101001")
	{
		string out = "\tsh\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		string rsParentheses = "(" + rsMapped->second + ")";
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + to_string(immediate_decimal) + rsParentheses);
	}
	// sw : 0x2B
	else if (opcode == "101011")
	{
		string out = "\tsw\t";
		auto rtMapped = registerMap.find(rt);
		auto rsMapped = registerMap.find(rs);
		string rsParentheses = "(" + rsMapped->second + ")";
		int immediate_decimal = binary_to_decimal(immediate);

		return (out + rtMapped->second + comma + to_string(immediate_decimal) + rsParentheses);
	}
	else
	{
		cout << "Error: Invalid Opcode on Line: " << line << endl;
		errorOccurred = true;
		return "error";
	}
}

int binary_to_decimal(string x)
{
	int decimalValue = 0;
	int base = 1;
	for (int i = x.length() - 1; i >= 0; i--)
	{
		if (x[i] == '1')
		{
			decimalValue += base;
		}
		base *= 2;
	}
	return decimalValue;
}

int binary_to_signed(string x)
{
	int signed_int = 0;
	
	if (x[0] == '1')
	{
		for (int i = x.length() - 1; i >= 0; i--)
		{
			if (x[i] == '1')
			{
				x[i] = '0';
			}
			else if (x[i] == '0')
			{
				x[i] = '1';
			}
		}
		signed_int = (binary_to_decimal(x) + 1) * -1;
	}
	else
	{
		signed_int = binary_to_decimal(x);
	}
	return signed_int;
}