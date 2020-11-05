#pragma once
#include <string>
using namespace std;
int checkSymb(int a, int b, unsigned char Symb, unsigned char map[3][3]) {
    return (map[a][b] == Symb) ? 1 : 0;
}
bool Check(unsigned char k[9], unsigned char Symb) {
    unsigned char map[3][3] = { 0 };
    for (int i = 0; i < 9; i++) 
        map[i / 3][i % 3] = k[i];
    
    bool End = false;
    int w1 = 0, w2 = 0, w3 = 0, w4 = 0;
    for (int i = 0; i < 3; i++) {
        for (int k = 0; k < 3; k++) {
            w1 += checkSymb(i, k, Symb, map);
            w2 += checkSymb(k, i, Symb, map);
        }
        if (w1 == 3 or w2 == 3) {
            End = true;
            break;
        }
        else { w1 = 0; w2 = 0;}
        w3 += checkSymb(i, i, Symb, map);
        w4 += checkSymb(i, (2 - i), Symb, map);
        
    }
    if (w3 == 3 or w4 == 3) {
        End = true;
    }
    return End;
}

int Analys(unsigned char kMAP[9], unsigned char Symb1, unsigned char Symb2) {
    unsigned char MAP[3][3] = { 0 };
    for (int i = 0; i < 9; i++)
        MAP[i / 3][i % 3] = kMAP[i];
    int Result = 0;
    int w1 = 0, w2 = 0, w3 = 0, w4 = 0;
    for (int i = 0; i < 3; i++) {
        for (int k = 0; k < 3; k++) {
            w1 += checkSymb(i, k, Symb1, MAP);
            w2 += checkSymb(k, i, Symb1, MAP);
        }
        if (w1 == 2) {
            for (int k = 0; k < 3; k++)
                if (checkSymb(i, k, 0, MAP)) {
                    Result = (i + 1) * 10 + k + 1;
                    return Result;
                }
        }
        if (w2 == 2) {
            for (int k = 0; k < 3; k++)
                if (checkSymb(k, i, 0, MAP)) {
                    Result = (k + 1) * 10 + i + 1;
                    return Result;
                }
        }
        w1 = 0; w2 = 0;
        w3 += checkSymb(i, i, Symb1, MAP);
        w4 += checkSymb(i, (2 - i), Symb1, MAP);
    }
    if (w3 == 2) {
        for (int k = 0; k < 3; k++)
            if (checkSymb(k, k, 0, MAP)) {
                Result = (k + 1) * 10 + k + 1;
                return Result;
            }
    }
    else if (w4 == 2) {
        for (int k = 0; k < 3; k++)
            if (checkSymb(k, 2 - k, 0, MAP)) {
                Result = (k + 1) * 10 + 2 - k + 1;
                return Result;
            }
    }
    return Result;
}