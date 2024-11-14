#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display();

char* itoa(int value, char* buffer, int base) {
    int i = 0, n = abs(value);
    while (n) {
        int r = n % base;
        buffer[i++] = (r >= 10) ? ('A' + (r - 10)) : ('0' + r);
        n /= base;
    }
    if (value < 0 && base == 10) buffer[i++] = '-';
    buffer[i] = '\0';
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
    return buffer;
}

int main() {
    char label[10], opcode[10], operand[10];
    char mnemonic[4][10] = {"LDA", "STA", "LDCH", "STCH"};
    char code[4][10] = {"33", "44", "53", "57"};
    char start[10], addr[10] = "0000", staddr[10], opaddr[10], temp[10];
    int actual_len = 0;
    FILE *fp1, *fp2, *fp3, *fp4;
    int i, j;

    if ((fp1 = fopen("output.txt", "r")) == NULL || (fp2 = fopen("symtab.txt", "r")) == NULL || (fp3 = fopen("intermediate.txt", "r")) == NULL || (fp4 = fopen("objcode.txt", "w")) == NULL) {
        printf("Error opening one or more files.\n");
        exit(1);
    }

    fscanf(fp1, "%s%s%s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        fprintf(fp4, "H^%s^00%s^00%s\n", label, operand, addr);
        strcpy(staddr, operand);
        fscanf(fp1, "%s%s%s%s", addr, label, opcode, operand);
    }
    fprintf(fp4, "T^00%s^", addr);

    while (strcmp(opcode, "END") != 0) {
        if (strcmp(opcode, "BYTE") == 0) {
            fprintf(fp4, "^");
            for (i = 2; i < strlen(operand) - 1; i++) {
                itoa(operand[i], temp, 16);
                fprintf(fp4, "%s", temp);
            }
            actual_len += strlen(operand) - 3;
        } else if (strcmp(opcode, "WORD") == 0) {
            itoa(atoi(operand), temp, 16);
            fprintf(fp4, "^00000%s", temp);
            actual_len += 3;
        } else if (!(strcmp(opcode, "RESB") == 0 || strcmp(opcode, "RESW") == 0)) {
            for (j = 0; j < 4; j++) {
                if (strcmp(opcode, mnemonic[j]) == 0) {
                    fprintf(fp4, "^%s", code[j]);
                    break;
                }
            }
            rewind(fp2);
            fscanf(fp2, "%s%s", label, opaddr);
            while (strcmp(label, operand) != 0) fscanf(fp2, "%s%s", label, opaddr);
            fprintf(fp4, "%s", opaddr);
            actual_len += 3;
        }
        fscanf(fp1, "%s%s%s%s", addr, label, opcode, operand);
        if (actual_len >= 30) {
            fprintf(fp4, "\nT^00%s^", addr);
            actual_len = 0;
        }
    }

    fprintf(fp4, "\nE^00%s", staddr);
    fclose(fp1); fclose(fp2); fclose(fp3); fclose(fp4);

    printf("Contents of Object code file:\n\n");
    display();
    return 0;
}

void display() {
    char ch;
    FILE *fp;

    printf("Intermediate file as converted into object code:\n\n");
    if ((fp = fopen("intermediate.txt", "r"))) {
        while ((ch = fgetc(fp)) != EOF) putchar(ch);
        fclose(fp);
    }

    printf("\nThe contents of Symbol Table:\n\n");
    if ((fp = fopen("symtab.txt", "r"))) {
        while ((ch = fgetc(fp)) != EOF) putchar(ch);
        fclose(fp);
    }

    printf("\nThe contents of Output file:\n\n");
    if ((fp = fopen("output.txt", "r"))) {
        while ((ch = fgetc(fp)) != EOF) putchar(ch);
        fclose(fp);
    }

    printf("\nThe contents of Object code file:\n\n");
    if ((fp = fopen("objcode.txt", "r"))) {
        while ((ch = fgetc(fp)) != EOF) putchar(ch);
        fclose(fp);
    }
}
