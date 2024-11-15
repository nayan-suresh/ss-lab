#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void convert(char h[12]);

char bitmask[12];
char bit[12] = {0};

void main() {
    char start[6], input[10], binary[12], relocbit, ch;
    int address, opcode, addr, actualadd, len, i, tlen;
    FILE *fp1, *fp2;

    printf("\n\n Enter the actual starting address: ");
    scanf("%x", &address);

    fp1 = fopen("RInput.txt", "r");
    fp2 = fopen("ROutput.txt", "w");

    if (fp1 == NULL || fp2 == NULL) {
        printf("Error opening files\n");
        return;
    }

    fscanf(fp1, "%s", input);
    fprintf(fp2, "--- 07 \n");
    fprintf(fp2, " ADDRESS\tCONTENT\n");
    fprintf(fp2, " ----90 rr \n");

    while (strcmp(input, "E") != 0) {
        if (strcmp(input, "H") == 0) {
            fscanf(fp1, "%s", input);
            fscanf(fp1, "%x", &address);
            fscanf(fp1, "%x", &tlen);
            fscanf(fp1, "%s", input);
        }
        
        if (strcmp(input, "T") == 0) {
            fscanf(fp1, "%x", &address);
            fscanf(fp1, "%x", &tlen);
            fscanf(fp1, "%s", bitmask);
            convert(bitmask);

            len = strlen(bit);

            if (len >= 11) {
                len = 10;
            }

            for (i = 0; i < len; i++) {
                fscanf(fp1, "%x", &opcode);
                fscanf(fp1, "%x", &addr);
                relocbit = bit[i];
                
                if (relocbit == '0') {
                    actualadd = addr;
                } else {
                    actualadd = addr + address;
                }

                fprintf(fp2, "\n%X\t\t%X", address, actualadd);
                address += 3;
            }
            fscanf(fp1, "%s", input);
        }
    }

    fprintf(fp2, "-------crrr errr \n");
    fclose(fp1);
    fclose(fp2);

    printf("\n\n The contents of output file (ROutput.txt):\n");

    fp2 = fopen("ROutput.txt", "r");
    ch = fgetc(fp2);
    while (ch != EOF) {
        printf("%c", ch);
        ch = fgetc(fp2);
    }
    fclose(fp2);
}

void convert(char h[12]) {
    int i;
    strcpy(bit, "");
    int l = strlen(h);

    for (i = 0; i < l; i++) {
        switch (h[i]) {
            case '@':
                strcat(bit, "@");
                break;
            case '1':
                strcat(bit, "1");
                break;
            case '2':
                strcat(bit, "16");
                break;
            case '3':
                strcat(bit, "11");
                break;
            case '4':
                strcat(bit, "100");
                break;
            case '5':
                strcat(bit, "101");
                break;
            case '6':
                strcat(bit, "110");
                break;
            case '7':
                strcat(bit, "111");
                break;
            case '8':
                strcat(bit, "1000");
                break;
            case '9':
                strcat(bit, "1001");
                break;
            case 'A':
                strcat(bit, "1010");
                break;
            case 'B':
                strcat(bit, "1011");
                break;
            case 'C':
                strcat(bit, "1100");
                break;
            case 'D':
                strcat(bit, "1101");
                break;
            case 'E':
                strcat(bit, "1110");
                break;
            case 'F':
                strcat(bit, "1111");
                break;
            default:
                printf("Invalid character in hex string: %c\n", h[i]);
                break;
        }
    }
}
