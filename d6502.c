/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  d6502- 6502 disassembler                                         */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

static FILE *fp;
static int ch;
#define NEXTCH (master++, ch = fgetc(fp))
#define GETADDR \
    NEXTCH; \
    addr = ch; \
    NEXTCH; \
    addr = addr | (ch << 8);
static unsigned int addr;
static unsigned int master = -1;

static void process(void);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: d6502 <object file name> [start address]\n");
        return (EXIT_FAILURE);
    }
    fp = fopen(*(argv+1), "rb");
    if (fp == NULL)
    {
        printf("unable to open file %s\n", *(argv+1));
        return (EXIT_FAILURE);
    }
    if (argc > 2)
    {
        master = (unsigned int)strtol(*(argv+2), NULL, 0) - 1;
    }
    process();
    return (EXIT_SUCCESS);
}    

static void process(void)
{
    while ((NEXTCH) != EOF)
    {
        printf("%.4X  ", master);
        switch (ch)
        {
            case 0x69:
                printf("ADC   #$%.2X\n", NEXTCH);
                break;
            case 0x65:
                printf("ADC   $%.2X\n", NEXTCH);
                break;
            case 0x75:
                printf("ADC   $%.2X, X\n", NEXTCH);
                break;
            case 0x6d:
                printf("ADC   $%.2X\n", NEXTCH);
                break;
            case 0x7d:
                printf("ADC   $%.2X, X\n", NEXTCH);
                break;
            case 0x79:
                printf("ADC   $%.2X, Y\n", NEXTCH);
                break;
            case 0x61:
                printf("ADC   ($%.2X, X)\n", NEXTCH);
                break;
            case 0x71:
                printf("ADC   ($%.2X), Y\n", NEXTCH);
                break;
            case 0x29:
                printf("AND   #$%.2X\n", NEXTCH);
                break;
            case 0x25:
                printf("AND   $%.2X\n", NEXTCH);
                break;
            case 0x35:
                printf("AND   $%.2X,X\n", NEXTCH);
                break;
            case 0x2d:
                GETADDR;
                printf("AND   $%.4X\n", addr);
                break;
            case 0x3d:
                GETADDR;
                printf("AND   $%.4X,X\n", addr);
                break;
            case 0x39:
                GETADDR;
                printf("AND   $%.4X,Y\n", addr);
                break;
            case 0x21:
                printf("AND   ($%.2X, X)\n", NEXTCH);
                break;
            case 0x31:
                printf("AND   ($%.2X), Y\n", NEXTCH);
                break;
            case 0x0a:
                printf("ASL   A\n");
                break;
            case 0x06:
                printf("ASL   $%.2X\n", NEXTCH);
                break;
            case 0x16:
                printf("ASL   $%.2X, X\n", NEXTCH);
                break;
            case 0x0e:
                GETADDR;
                printf("ASL   $%.4X\n", addr);
                break;
            case 0x1e:
                GETADDR;
                printf("ASL   $%.4X, X\n", addr);
                break;
            case 0x90:
                NEXTCH;
                if (ch > 0x80)
                {
                    printf("BCC   $%.2X\n", master + ch + 1 - 256);
                }
                else
                {
                    printf("BCC   $%.4X\n", master + ch + 1);
                }
                break;
            case 0xb0:
                NEXTCH;
                if (ch > 0x80)
                {
                    printf("BCS   $%.2X\n", master + ch + 1 - 256);
                }
                else
                {
                    printf("BCS   $%.4X\n", master + ch + 1);
                }
                break;
            case 0xf0:
                NEXTCH;
                if (ch > 0x80)
                {
                    printf("BEQ   $%.2X\n", master + ch + 1 - 256);
                }
                else
                {
                    printf("BEQ   $%.4X\n", master + ch + 1);
                }
                break;
            case 0x24:
                printf("BIT   $%.2X\n", NEXTCH);
                break;
            case 0x2c:
                GETADDR;
                printf("BIT   $%.4X\n", addr);
                break;
            case 0x30:
                NEXTCH;
                if (ch > 0x80)
                {
                    printf("BMI   $%.2X\n", master + ch + 1 - 256);
                }
                else
                {
                    printf("BMI   $%.4X\n", master + ch + 1);
                }
                break;
            case 0xd0:                
                NEXTCH;
                if (ch > 0x80)
                {
                    printf("BNE   $%.2X\n", master + ch + 1 - 256);
                }
                else
                {
                    printf("BNE   $%.4X\n", master + ch + 1);
                }
                break;
            case 0x10:
                NEXTCH;
                if (ch > 0x80)
                {
                    printf("BPL   $%.2X\n", master + ch + 1 - 256);
                }
                else
                {
                    printf("BPL   $%.4X\n", master + ch + 1);
                }
                break;
            case 0x00:
                printf("BRK\n");
                break;
            case 0x50:
                NEXTCH;
                if (ch > 0x80)
                {
                    printf("BVC   $%.2X\n", master + ch + 1 - 256);
                }
                else
                {
                    printf("BVC   $%.4X\n", master + ch + 1);
                }
                break;
            case 0x70:
                NEXTCH;
                if (ch > 0x80)
                {
                    printf("BVS   $%.2X\n", master + ch + 1 - 256);
                }
                else
                {
                    printf("BVS   $%.4X\n", master + ch + 1);
                }
                break;
            case 0x18:
                printf("CLC\n");
                break;
            case 0xd8:
                printf("CLD\n");
                break;
            case 0x58:
                printf("CLI\n");
                break;
            case 0xb8:
                printf("CLV\n");
                break;
            case 0xc9:
                printf("CMP   #$%.2X\n", NEXTCH);
                break;
            case 0xc5:
                printf("CMP   $%.2X\n", NEXTCH);
                break;
            case 0xd5:
                printf("CMP   $%.2X, X\n", NEXTCH);
                break;
            case 0xcd:
                GETADDR;
                printf("CMP   $%.4X\n", addr);
                break;
            case 0xdd:
                GETADDR;
                printf("CMP   $%.4X, X\n", addr);
                break;
            case 0xd9:
                GETADDR;
                printf("CMP   $%.4X, Y\n", addr);
                break;
            case 0xc1:
                printf("CMP   ($%.2X, X)\n", NEXTCH);
                break;
            case 0xd1:
                printf("CMP   ($%.2X), Y\n", NEXTCH);
                break;
            case 0xe0:
                printf("CPX   #$%.2X\n", NEXTCH);
                break;
            case 0xe4:
                printf("CPX   $%.2X\n", NEXTCH);
                break;
            case 0xec:
                GETADDR;
                printf("CPX   $%.4X\n", addr);
                break;
            case 0xc0:
                printf("CPY   #$%.2X\n", NEXTCH);
                break;
            case 0xc4:
                printf("CPY   $%.2X\n", NEXTCH);
                break;
            case 0xcc:
                GETADDR;
                printf("CPY   $%.4X\n", addr);
                break;
            case 0xc6:
                printf("DEC   $%.2X\n", NEXTCH);
                break;
            case 0xd6:
                printf("DEC   $%.2X, X\n", NEXTCH);
                break;
            case 0xce:
                GETADDR;
                printf("DEC   $%.4X\n", addr);
                break;
            case 0xde:
                GETADDR;
                printf("DEC   $%.4X, X\n", addr);
                break;
            case 0xca:
                printf("DEX\n");
                break;
            case 0x88:
                printf("DEY\n");
                break;
            case 0x49:
                printf("EOR   #$%.2X\n", NEXTCH);
                break;
            case 0x45:
                printf("EOR   $%.2X\n", NEXTCH);
                break;
            case 0x55:
                printf("EOR   $%.2X, X\n", NEXTCH);
                break;
            case 0x4d:
                GETADDR;
                printf("EOR   $%.4X\n", addr);
                break;
            case 0x5d:
                GETADDR;
                printf("EOR   $%.4X, X\n", addr);
                break;
            case 0x59:
                GETADDR;
                printf("EOR   $%.4X, Y\n", addr);
                break;
            case 0x41:
                printf("EOR   ($%.2X, X)\n", NEXTCH);
                break;
            case 0x51:
                printf("EOR   ($%.2X), Y\n", NEXTCH);
                break;
            case 0xe6:
                printf("INC   $%.2X\n", NEXTCH);
                break;
            case 0xf6:
                printf("INC   $%.2X, X\n", NEXTCH);
                break;
            case 0xee:
                GETADDR;
                printf("INC   $%.4X\n", addr);
                break;
            case 0xfe:
                GETADDR;
                printf("INC   $%.4X, X\n", addr);
                break;
            case 0xe8:
                printf("INX\n");
                break;
            case 0xc8:
                printf("INY\n");
                break;
            case 0x4c:
                GETADDR;
                printf("JMP   $%.4X\n", addr);
                break;
            case 0x6c:
                GETADDR;
                printf("JMP   ($%.4X)\n", addr);
                break;
            case 0x20:
                GETADDR;
                printf("JSR   $%.4X\n", addr);
                break;
            case 0xa9:
                printf("LDA   #$%.2X\n", NEXTCH);
                break;
            case 0xa5:
                printf("LDA   $%.2X\n", NEXTCH);
                break;
            case 0xb5:
                printf("LDA   $%.2X, X\n", NEXTCH);
                break;
            case 0xad:
                GETADDR;
                printf("LDA   $%.4X\n", addr);
                break;
            case 0xbd:
                GETADDR;
                printf("LDA   $%.4X, X\n", addr);
                break;
            case 0xb9:
                GETADDR;
                printf("LDA   $%.4X, Y\n", addr);
                break;
            case 0xa1:
                printf("LDA   ($%.2X, X)\n", NEXTCH);
                break;
            case 0xb1:
                printf("LDA   ($%.2X), Y\n", NEXTCH);
                break;
            case 0xa2:
                printf("LDX   #$%.2X\n", NEXTCH);
                break;
            case 0xa6:
                printf("LDX   $%.2X\n", NEXTCH);
                break;
            case 0xb6:
                printf("LDX   $%.2X, Y\n", NEXTCH);
                break;
            case 0xae:
                GETADDR;
                printf("LDX   $%.4X\n", addr);
                break;
            case 0xbe:
                GETADDR;
                printf("LDX   $%.4X, Y\n", addr);
                break;
            case 0xa0:
                printf("LDY   #$%.2X\n", NEXTCH);
                break;
            case 0xa4:
                printf("LDY   $%.2X\n", NEXTCH);
                break;
            case 0xb4:
                printf("LDY   $%.2X, X\n", NEXTCH);
                break;
            case 0xac:
                GETADDR;
                printf("LDY   $%.4X\n", addr);
                break;
            case 0xbc:
                GETADDR;
                printf("LDY   $%.4X, X\n", addr);
                break;
            case 0x4a:
                printf("LSR   A\n");
                break;
            case 0x46:
                printf("LSR   $%.2X\n", NEXTCH);
                break;
            case 0x56:
                printf("LSR   $%.2X, X\n", NEXTCH);
                break;
            case 0x4e:
                GETADDR;
                printf("LSR   $%.4X\n", addr);
                break;
            case 0x5e:
                GETADDR;
                printf("LSR   $%.4X, X\n", addr);
                break;
            case 0xea:
                printf("NOP\n");
                break;
            case 0x09:
                printf("ORA   #$%.2X\n", NEXTCH);
                break;
            case 0x05:
                printf("ORA   $%.2X\n", NEXTCH);
                break;
            case 0x15:
                printf("ORA   $%.2X, X\n", NEXTCH);
                break;
            case 0x0d:
                GETADDR;
                printf("ORA   $%.4X\n", addr);
                break;
            case 0x1d:
                GETADDR;
                printf("ORA   $%.4X, X\n", addr);
                break;
            case 0x19:
                GETADDR;
                printf("ORA   $%.4X, Y\n", addr);
                break;
            case 0x01:
                GETADDR;
                printf("ORA   ($%.4X, X)\n", addr);
                break;
            case 0x11:
                GETADDR;
                printf("ORA   ($%.4X), Y\n", addr);
                break;
            case 0x48:
                printf("PHA\n");
                break;
            case 0x08:
                printf("PHP\n");
                break;
            case 0x68:
                printf("PLA\n");
                break;
            case 0x28:
                printf("PLP\n");
                break;
            case 0x2a:
                printf("ROL   A\n");
                break;
            case 0x26:
                printf("ROL   $%.2X\n", NEXTCH);
                break;
            case 0x36:
                printf("ROL   $%.2X, X\n", NEXTCH);
                break;
            case 0x2e:
                GETADDR;
                printf("ROL   $%.4X\n", addr);
                break;
            case 0x3e:
                GETADDR;
                printf("ROL   $%.4X, X\n", addr);
                break;
            case 0x6a:
                printf("ROR   A\n");
                break;
            case 0x66:
                printf("ROR   $%.2X\n", NEXTCH);
                break;
            case 0x76:
                printf("ROR   $%.2X, X\n", NEXTCH);
                break;
            case 0x6e:
                GETADDR;
                printf("ROR   $%.4X\n", addr);
                break;
            case 0x7e:
                GETADDR;
                printf("ROR   $%.4X, X\n", addr);
                break;
            case 0x40:
                printf("RTI\n");
                break;
            case 0x60:
                printf("RTS\n");
                break;
            case 0xe9:
                printf("SBC   #$%.2X\n", NEXTCH);
                break;
            case 0xe5:
                printf("SBC   $%.2X\n", NEXTCH);
                break;
            case 0xf5:
                printf("SBC   $%.2X, X\n", NEXTCH);
                break;
            case 0xed:
                GETADDR;
                printf("SBC   $%.4X\n", addr);
                break;
            case 0xfd:
                GETADDR;
                printf("SBC   $%.4X, X\n", addr);
                break;
            case 0xf9:
                GETADDR;
                printf("SBC   $%.4X, Y\n", addr);
                break;
            case 0xe1:
                printf("SBC   ($%.2X, X)\n", NEXTCH);
                break;
            case 0xf1:
                printf("SBC   ($%.2X), Y\n", NEXTCH);
                break;
            case 0x38:
                printf("SEC\n");
                break;
            case 0xf8:
                printf("SED\n");
                break;
            case 0x78:
                printf("SEI\n");
                break;
            case 0x85:
                printf("STA   $%.2X\n", NEXTCH);
                break;
            case 0x95:
                printf("STA   $%.2X, X\n", NEXTCH);
                break;
            case 0x8d:
                GETADDR;
                printf("STA   $%.4X\n", addr);
                break;
            case 0x9d:
                GETADDR;
                printf("STA   $%.4X, X\n", addr);
                break;
            case 0x99:
                GETADDR;
                printf("STA   $%.4X, Y\n", addr);
                break;
            case 0x81:
                printf("STA   ($%.2X, X)\n", NEXTCH);
                break;
            case 0x91:
                printf("STA   ($%.2X), Y\n", NEXTCH);
                break;
            case 0x86:
                printf("STA   $%.2X\n", NEXTCH);
                break;
            case 0x96:
                printf("STA   $%.2X, Y\n", NEXTCH);
                break;
            case 0x8e:
                GETADDR;
                printf("STA   $%.4X\n", addr);
                break;
            case 0x84:
                printf("STY   $%.2X\n", NEXTCH);
                break;
            case 0x94:
                printf("STY   $%.2X, X\n", NEXTCH);
                break;
            case 0x8c:
                GETADDR;
                printf("STY   $%.4X\n", addr);
                break;
            case 0xaa:
                printf("TAX\n");
                break;
            case 0xa8:
                printf("TAY\n");
                break;
            case 0xba:
                printf("TSX\n");
                break;
            case 0x8a:
                printf("TXA\n");
                break;
            case 0x9a:
                printf("TXS\n");
                break;
            case 0x98:
                printf("TYA\n");
                break;
            default:
                printf("???   %.2X\n", ch);
                break;
        }
    }
    return;
}

