#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;

const char* const sourceDirection = "./rect/Rect.asm";
const char* const midDirection = "./rect/Rect.mid";
const char* const finalDirection = "./rect/Rect.hack";

char iBuffer[1024];
char oBuffer[16];

void sharpen(char* const text) {
    int _length = strlen(text);
    int j = 0;
    for (int i = 0; i < _length; ++i) {
        if (text[i] == '/' && text[i + 1] == '/') {
            break;
        }
        if (text[i] != ' ' && text[i] != '\n') {
            text[j++] = text[i];
        }

    }
    text[j] = '\0';
}

int s2i(const char* text) {
    int result = 0;
    int weight = 10;
    if (*text == '0') {
        weight = 2;
    }
    while (*text) {
        if (*text >= '0' && *text <= '9') {
            result = result * weight + *text - '0';
        } else {
            return -1;
        }
        ++text;
    }
    return result;
}

void preDefine(unordered_map<string, int>& table) {
    table["R0"] = 0;
    table["R1"] = 1;
    table["R2"] = 2;
    table["R3"] = 3;
    table["R4"] = 4;
    table["R5"] = 5;
    table["R6"] = 6;
    table["R7"] = 7;
    table["R8"] = 8;
    table["R9"] = 9;
    table["R10"] = 10;
    table["R11"] = 11;
    table["R12"] = 12;
    table["R13"] = 13;
    table["R14"] = 14;
    table["R15"] = 15;
    table["SCREEN"] = 16384;
    table["KBD"] = 24576;
    table["SP"] = 0;
    table["LCL"] = 1;
    table["ARG"] = 2;
    table["THIS"] = 3;
    table["THAT"] = 4;
}

void firstPass() {
    unordered_map<string, int>variableTable;
    unordered_map<string, int>jumpTable;
    preDefine(variableTable);
    int variableIndex = 15;
    int lineNum = 0;

    FILE* sourceFile = fopen(sourceDirection, "r");
    while (fgets(iBuffer, 1024, sourceFile)) {
        sharpen(iBuffer);
        int iBufferLength = (int)strlen(iBuffer);
        if (iBufferLength == 0) {
            continue;
        }
        if (iBuffer[0] == '(') {
            iBuffer[iBufferLength - 1] = '\0';
            string sign(iBuffer + 1);
            jumpTable[sign] = lineNum;
            continue;
        }
        ++lineNum;
    }
    fclose(sourceFile);

    sourceFile = fopen(sourceDirection, "r");
    FILE* outFile = fopen(midDirection, "w");
    while (fgets(iBuffer, 1024, sourceFile)) {
        sharpen(iBuffer);
        int iBufferLength = (int)strlen(iBuffer);
        if (iBufferLength == 0) {
            continue;
        }
        if (iBuffer[0] == '@') {
            int num = s2i(iBuffer + 1);
            if (num == -1) {
                string sign(iBuffer + 1);
                if (variableTable.count(sign)) {
                    num = variableTable[sign];
                } else if(jumpTable.count(sign)) {
                    num = jumpTable[sign];
                } else {
                    variableTable[sign] = ++variableIndex;
                    num = variableIndex;
                }
            }
            fprintf(outFile, "@%d\n", num);
        } else {
            if (iBuffer[0] == '(') {
                continue;
            }
            fprintf(outFile, "%s\n", iBuffer);
        }
    }
    fclose(sourceFile);
    fclose(outFile);
}

void secondPass() {
    unordered_map<string, string>compTable;
    unordered_map<string, string>destTable;
    unordered_map<string, string>jumpTable;
    compTable["0"] = "0101010";
    compTable["1"] = "0111111";
    compTable["-1"] = "0111010";
    compTable["D"] = "0001100";
    compTable["A"] = "0110000";
    compTable["!D"] = "0001101";
    compTable["!A"] = "0110001";
    compTable["-D"] = "0001111";
    compTable["-A"] = "0110011";
    compTable["D+1"] = "0011111";
    compTable["A+1"] = "0110111";
    compTable["D-1"] = "0001110";
    compTable["A-1"] = "0110010";
    compTable["D+A"] = "0000010";
    compTable["D-A"] = "0010011";
    compTable["A-D"] = "0000111";
    compTable["D&A"] = "0000000";
    compTable["D|A"] = "0010101";
    compTable["M"] = "1110000";
    compTable["!M"] = "1110001";
    compTable["-M"] = "1110011";
    compTable["M+1"] = "1110111";
    compTable["M-1"] = "1110010";
    compTable["D+M"] = "1000010";
    compTable["D-M"] = "1010011";
    compTable["M-D"] = "1000111";
    compTable["D&M"] = "1000000";
    compTable["D|M"] = "1010101";

    destTable["M"] = "001";
    destTable["D"] = "010";
    destTable["MD"] = "011";
    destTable["A"] = "100";
    destTable["AM"] = "101";
    destTable["AD"] = "110";
    destTable["AMD"] = "111";

    jumpTable["JGT"] = "001";
    jumpTable["JEQ"] = "010";
    jumpTable["JGE"] = "011";
    jumpTable["JLT"] = "100";
    jumpTable["JNE"] = "101";
    jumpTable["JLE"] = "110";
    jumpTable["JMP"] = "111";

    FILE* sourceFile = fopen(midDirection, "r");
    FILE* outFile = fopen(finalDirection, "w");
    while (fgets(iBuffer, 1024, sourceFile)) {
        sharpen(iBuffer);
        if (iBuffer[0] == '@') {
            oBuffer[0] = '0';
            int num = s2i(iBuffer + 1);
            for (int i = 15; i > 0; --i, num /= 2) {
                oBuffer[i] = (num&1) + '0';
            }
        } else {
            oBuffer[0] = oBuffer[1] = oBuffer[2] = '1';
            string dest, comp, jump;
            int index = 0;
            while (iBuffer[index] != '=' && iBuffer[index] != ';' && iBuffer[index] != '\0') {
                dest += iBuffer[index];
                ++index;
            }
            if (iBuffer[index] == '=') {
                ++index;
                while (iBuffer[index] != ';' && iBuffer[index] != '\0') {
                    comp += iBuffer[index];
                    ++index;
                }
                if (iBuffer[index] == ';') {
                    ++index;
                    while (iBuffer[index] != '\0') {
                        jump += iBuffer[index];
                        ++index;
                    }
                }
            } else {
                swap(comp, dest);
                if (iBuffer[index] == ';') {
                    ++index;
                    while (iBuffer[index] != '\0') {
                        jump += iBuffer[index];
                        ++index;
                    }
                }
            }

            string code;
            if (jump.empty()) {
                oBuffer[13] = oBuffer[14] = oBuffer[15] = '0';
            } else {
                code = jumpTable[jump];
                for (int i = 0; i < 3; ++i) {
                    oBuffer[i + 13] = code[i];
                }
            }
            if (dest.empty()) {
                oBuffer[10] = oBuffer[11] = oBuffer[12] = '0';
            } else {
                code = destTable[dest];
                for (int i = 0; i < 3; ++i) {
                    oBuffer[i + 10] = code[i];
                }
            }
            code = compTable[comp];
            for (int i = 0; i < 7; ++i) {
                oBuffer[3 + i] = code[i];
            }
        }
        fprintf(outFile, "%s\n", oBuffer);
    }
    fclose(sourceFile);
    fclose(outFile);
}

int main() {
    firstPass();
    secondPass();
    return 0;
}
