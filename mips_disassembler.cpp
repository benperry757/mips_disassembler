// mips_disassembler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>
#include <map>
using namespace std;

string disassembled_instruction(string line);
int binary_to_decimal(string x);
bool errorOccurred;

map<string, string> registerMap = {
	{"00000", "$0"},		// $0 : 0
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
	//Read input file that is typed in command line
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
	errorOccurred = false;
	int count = 0;
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			string output = disassembled_instruction(line);
			cout << output << endl;
			outVector.push_back(output);
			count++;
		}
	}
	else
	{
		cout << "Unable to open file." << endl;
		return 1;
	}
	if (errorOccurred)
	{
		return 1;
	}
	string extension = ".asm";
	string outputFile = filename.substr(0, filename.length() - 4) + extension;
	cout << "Output file name: '" << outputFile << "'" << endl;
	ofstream outfile(outputFile);
	if (outfile.is_open())
	{
		for (int i = 0; i < count; i++)
		{
			outfile << outVector[i];
		}
	}
	else
	{
		cout << "Unable to open output file." << endl;
	}
	return 0;
}

string disassembled_instruction(string line)
{
	string comma = ", ";
	// Prints error if line is not 8 Hex values.
	if (line.length() != 8)
	{
		cout << "Hex Length Error on Line: " << line << endl;
		errorOccurred = true;
		return "error";
	}
	string instruction;
	// Convert Hex string to binary
	for (char hex : line)
	{
		switch (hex)
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
		case 'a': instruction += "1010"; break;
		case 'b': instruction += "1011"; break;
		case 'c': instruction += "1100"; break;
		case 'd': instruction += "1101"; break;
		case 'e': instruction += "1110"; break;
		case 'f': instruction += "1111"; break;
		default: 
			cout << "Error: Invalid Hex Digit on Line: " << line << endl;
			errorOccurred = true;
			return "error";
		}
		
	}
	cout << instruction << endl;
	// Disassemble into the instruction formatting
	string opcode = instruction.substr(0, 6);
	string rs = instruction.substr(6, 5);
	string rt = instruction.substr(11, 5);
	string rd = instruction.substr(16, 5);
	string shamt = instruction.substr(21, 5);
	string funct = instruction.substr(26, 6);

	// Based on the opcode, the respective instruction is returned
	if (opcode == "000000")
	{
		// For R-type instructions, the funct is used to determine the respective instruction
		// add : 0x20
		if (funct == "100000")
		{
			string out = "$add";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);
			
			return (out + comma + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// addu : 0x21
		if (funct == "100001")
		{
			string out = "$addu";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + comma + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// and : 0x24
		if (funct == "100100")
		{
			string out = "$and";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + comma + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// nor : 0x27
		if (funct == "100111")
		{
			string out = "$nor";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + comma + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// or : 0x25
		if (funct == "100101")
		{
			string out = "$or";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + comma + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// slt : 0x2A
		if (funct == "101010")
		{
			string out = "$slt";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + comma + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// sltu : 0x2B
		if (funct == "101011")
		{
			string out = "$sltu";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + comma + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// sll : 0x00
		if (funct == "000000")
		{
			string out = "$sll";
			auto rdMapped = registerMap.find(rd);
			auto rtMapped = registerMap.find(rt);
			int shamt_decimal = binary_to_decimal(shamt);

			return (out + comma + rdMapped->second + comma + rtMapped->second + comma + to_string(shamt_decimal));
		}
		// srl : 0x02
		if (funct == "000010")
		{
			string out = "$srl";
			auto rdMapped = registerMap.find(rd);
			auto rtMapped = registerMap.find(rt);
			int shamt_decimal = binary_to_decimal(shamt);

			return (out + comma + rdMapped->second + comma + rtMapped->second + comma + to_string(shamt_decimal));
		}
		// sub : 0x22
		if (funct == "100010")
		{
			string out = "$sub";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + comma + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		// subu : 0x23
		if (funct == "100011")
		{
			string out = "$subu";
			auto rdMapped = registerMap.find(rd);
			auto rsMapped = registerMap.find(rs);
			auto rtMapped = registerMap.find(rt);

			return (out + comma + rdMapped->second + comma + rsMapped->second + comma + rtMapped->second);
		}
		else
		{
			cout << "Error on Line: " << line << endl;
			errorOccurred = true;
			return "error";
		}
	}
	// addi : 0x08
	else if (opcode == "001000")
	{
		//TODO
	}
	// addiu : 0x09
	else if (opcode == "001001")
	{
		//TODO
	}
	// andi : 0x0C
	else if (opcode == "001100")
	{
		//TODO
	}
	// beq : 0x04
	else if (opcode == "000100")
	{
		//TODO
	}
	// bne : 0x05
	else if (opcode == "000101")
	{
		//TODO
	}
	// j : 0x02
	else if (opcode == "000010")
	{
		//TODO
	}
	// jal : 0x03
	else if (opcode == "000011")
	{
		//TODO
	}
	// lbu : 0x24
	else if (opcode == "100100")
	{
		//TODO
	}
	// lhu : 0x25
	else if (opcode == "100101")
	{
		//TODO
	}
	// ll : 0x30
	else if (opcode == "110000")
	{
		//TODO
	}
	// lui : 0x0F
	else if (opcode == "001111")
	{
		//TODO
	}
	// lw : 0x23
	else if (opcode == "100011")
	{
		//TODO
	}
	// ori : 0x0D
	else if (opcode == "001101")
	{
		//TODO
	}
	// slti : 0x0A
	else if (opcode == "001010")
	{
		//TODO
	}
	// sltiu : 0x0B
	else if (opcode == "001011")
	{
		//TODO
	}
	// sb : 0x28
	else if (opcode == "101000")
	{
		//TODO
	}
	// sc : 0x38
	else if (opcode == "111000")
	{
		//TODO
	}
	// sh : 0x29
	else if (opcode == "101001")
	{
		//TODO
	}
	// sw : 0x2B
	else if (opcode == "000100")
	{
		//TODO
	}
	else
	{
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
