// import the input/output and vector libraries 
#include <iostream>
#include <vector>
using namespace std;

// For R-Format Instructions: seperates instruction into proper field variables
void r_format( unsigned int opcode, unsigned int reg1, unsigned int reg2,  unsigned int inst ){
    
    // Bitmasks for R-format specific variables (register3 and function code) 
    // SHAMT is assumed to be 0 so it is excluded from this program
    int reg3_mask = 0x0000F800;
    int func_mask = 0x3F;

    unsigned int reg3 = (inst & reg3_mask) >> 11;
    unsigned int func = (inst & func_mask);

    // Output the correct function based off of the func code variable
    switch (func) {
        case 0x20 :
            cout << "add ";
            break;
        case 0x22 :
            cout << "sub ";
            break;
        case 0x24 :
            cout << "and ";
            break;
        case 0x25 :
            cout << "or  ";
            break;
        case 0x2A :
            cout << "slt ";
            break;
		// If none of the above cases are true, there is something wrong with the func (function) code, 
    	// or it is an operation that is not tested in this program.
        default :
            cout << "Something went wrong. Please check your function code. ";
    }

    // Output the rest of the instruction in the correct format
    cout << "$" << reg3 << ", $" << reg1 << ", $" << reg2 << endl;


}

// For I-Format Instructions: seperates instruction into proper field variables
void i_format(unsigned int opcode, unsigned int reg1, unsigned int reg2, unsigned int inst, unsigned int pc)
{
    // Bitmasks for I-format specific variables (offset) 
    unsigned int offset_mask = 0xFFFF;

    short offset = (inst & offset_mask);

    // Check opcode and pair it with either lw or sw, since they have a different output format and offset processing as beq and bne
    if (opcode == 0x23 || opcode == 0x2B)
    {
        if (opcode == 0x23)
            cout << "lw  ";
        else
            cout << "sw  ";

        // Output the rest of the instruction in the correct format
        cout << "$" << reg2 << ", " << offset << " ($" << reg1 << ")" << endl;
    }

    // If opcode corresponded to neither lw nor sw, check if it corresponds to beq or bne operations
    else if (opcode == 0x4 || opcode == 0x5)
    {
        if (opcode == 0x4)
            cout << "beq ";
        else
            cout << "bne ";
				
				// shift the current offset to the left by 2 bits to take into consideration the 2 zeros 
				// that are not shown in the pc-relative offset
				offset = offset << 2;

				// add the pc-relative offset to the currect pc to find the target address
				int target = pc + offset;

        // Output the rest of the instruction in the correct format
        cout << "$" << reg1 << ", $" << reg2 << ", address " << hex << target << dec << endl;

    }

    // If none of the above cases are true, there is something wrong with the inputted opcode, 
    // or it is an operation that is not tested in this program.
    else
        cout << "Something went wrong. Please check your opcode. ";
}

// takes input instruction, checks opcode, reg1, & reg2 and determines the correct format function to use
void getFormat (unsigned int inst, unsigned int pc){

    // Bitmasks for common variables across all formats
    unsigned int opcode_mask = 0xFC000000;
    unsigned int reg1_mask = 0x03E00000;
    unsigned int reg2_mask = 0x001F0000;

    // AND and shift operations with the original instruction in order to isolate and store the specific fields of each variable
    unsigned int opcode = (inst & opcode_mask) >> 26;
    unsigned int reg1 = (inst & reg1_mask) >> 21;
    unsigned int reg2 = (inst & reg2_mask) >> 16;
    
    if ( opcode == 0 )  
        // R Format 
        r_format(opcode, reg1, reg2, inst);
    else 
        // I Format 
        i_format(opcode, reg1, reg2, inst, pc);
}



int main() {
    unsigned int pc = 0x9A040;  //starting address for the first instruction 

    // array of instructions to be disassembled
    unsigned int inst[] = { 0x032BA020, 0x8CE90014, 0x12A90003, 0x022DA822, 0xADB30020, 0x02697824, 0xAE8FFFF4, 0x018C6020, 0x02A4A825, 0x158FFFF7, 0x8ECDFFF0 };

    // iterate through the array   
    for ( unsigned int instruct : inst) {

        // output address in hexadecimal form
        cout << hex << uppercase << pc << "   " << dec;

        // increment address by 4 bits 
		// Do this BEFORE processing the instruction in case of encountering a beq or bne
        pc = pc + 4; 

        // The format and operation of instruction and output it
        getFormat(instruct, pc);

    }

}