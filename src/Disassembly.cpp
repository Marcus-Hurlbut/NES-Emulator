#include "Disassembly.h"

Disassembly::Disassembly()
{
	// Initialize Hex Map
	hex_lookup[0] = "00";	hex_lookup[1] = "01";	hex_lookup[2] = "02";	hex_lookup[3] = "03";	hex_lookup[4] = "04";	hex_lookup[5] = "05";	hex_lookup[6] = "06";	hex_lookup[7] = "07";
	hex_lookup[8] = "08";	hex_lookup[9] = "09";	hex_lookup[10] = "0A";	hex_lookup[11] = "0B";	hex_lookup[12] = "0C";	hex_lookup[13] = "0D";	hex_lookup[14] = "0E";	hex_lookup[15] = "0F";

	hex_lookup[16] = "10";	hex_lookup[17] = "11";	hex_lookup[18] = "12";	hex_lookup[19] = "13";	hex_lookup[20] = "14";	hex_lookup[21] = "15";	hex_lookup[22] = "16";	hex_lookup[23] = "17";
	hex_lookup[24] = "18";	hex_lookup[25] = "19";	hex_lookup[26] = "1A";	hex_lookup[27] = "1B";	hex_lookup[28] = "1C";	hex_lookup[29] = "1D";	hex_lookup[30] = "1E";	hex_lookup[31] = "1F";
	
	hex_lookup[32] = "20";	hex_lookup[33] = "21";	hex_lookup[34] = "22";	hex_lookup[35] = "23";	hex_lookup[36] = "24";	hex_lookup[37] = "25";	hex_lookup[38] = "26";	hex_lookup[39] = "27";
	hex_lookup[40] = "28";	hex_lookup[41] = "29";	hex_lookup[42] = "2A";	hex_lookup[43] = "2B";	hex_lookup[44] = "2C";	hex_lookup[45] = "2D";	hex_lookup[46] = "2E";  hex_lookup[47] = "2F";
	
	hex_lookup[48] = "30";	hex_lookup[49] = "31";	hex_lookup[50] = "32";	hex_lookup[51] = "33";	hex_lookup[52] = "34";	hex_lookup[53] = "35";	hex_lookup[54] = "36";	hex_lookup[55] = "37";
	hex_lookup[56] = "38";	hex_lookup[57] = "39";	hex_lookup[58] = "3A";	hex_lookup[59] = "3B";	hex_lookup[60] = "3C";	hex_lookup[61] = "3D";	hex_lookup[62] = "3E";	hex_lookup[63] = "3F";

	hex_lookup[64] = "40";	hex_lookup[65] = "41";	hex_lookup[66] = "42";	hex_lookup[67] = "43";	hex_lookup[68] = "44";	hex_lookup[69] = "45";	hex_lookup[70] = "46";	hex_lookup[71] = "47";
	hex_lookup[72] = "48";	hex_lookup[73] = "49";	hex_lookup[74] = "4A";	hex_lookup[75] = "4B";	hex_lookup[76] = "4C";	hex_lookup[77] = "4D";	hex_lookup[78] = "4E";	hex_lookup[79] = "4F";

	hex_lookup[80] = "50";	hex_lookup[81] = "51";	hex_lookup[82] = "52";	hex_lookup[83] = "53";	hex_lookup[84] = "54";	hex_lookup[85] = "55";	hex_lookup[86] = "56";	hex_lookup[87] = "57";
	hex_lookup[88] = "58";	hex_lookup[89] = "59";	hex_lookup[90] = "5A";	hex_lookup[91] = "5B";	hex_lookup[92] = "5C";	hex_lookup[93] = "5D";	hex_lookup[94] = "5E";	hex_lookup[95] = "5F";
}

Disassembly::~Disassembly()
{

}


// Convert integer to hex string
inline std::string Disassembly::formatHex(uint8_t& n)
{
	return hex_lookup[n];
}


// Called from CPU to Log Disassembly
void Disassembly::logDisassembly(uint8_t &opcode, uint8_t &operands[2], int numOpers, uint16_t &addr)
{
	// Get Hex String of Opcode
	nDisassembly.opcode = formatHex(opcode);

	// Get Hex String of Operands
	if (numOpers == 0)
	{
		nDisassembly.operand[0] = "  ";
		nDisassembly.operand[1] = "  ";
	}
	else if (numOpers == 1)
	{
		nDisassembly.operand[0] = formatHex(operands[0]);
		nDisassembly.operand[1] = "  ";
	}
	else
	{
		nDisassembly.operand[0] = formatHex(operands[0]);
		nDisassembly.operand[1] = formatHex(operands[1]);
	};

	// Get Hex String of Address (Program Counter)
	uint8_t hi = (addr >> 8) & 0x00FF;
	uint8_t lo = (addr) & 0x00FF;
	nDisassembly.pcaddr = formatHex(hi);
	nDisassembly.pcaddr += formatHex(lo);

	// Get Instructions Disassembly from Lookup Table


}	


void Disassembly::formatDisassemblyLine()
{
	switch (nDisassembly.addrmode)
	{
		case "IMP":
			nDisassembly.line = pcaddr;
			nDisassembly.line += ": ";
			nDisassembly.line += nDisassembly.instruction;
			nDisassembly.line += " [";
			nDisassembly.line += nDisassembly.addrmode;
			nDisassembly.line += "]";
			break;

		case "ACC":

	};
}

