#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>
#include <math.h>
#include <bitset>

using namespace std;

int dataword_size;

char my_xor(char a, char b) {
    int ret = a == b? '0': '1';
    return ret;
}

string mod2div(string data, string generator) {
    string divident  = data;
    string remainder;
    int idx;
    int i, j;
    idx = (int)generator.length() - 1;
    // generator 길이 -1 만큼 dataword 뒤에 0 붙이기.
    for(i = 0; i < idx; i++) 
        divident += '0';
    // generator = divisor
    // 나머지: generator 길이 까지는 divident가 그대로 내려옴
    remainder = divident.substr(0, idx);
    for(i = 0; i< (int)data.length(); i++) {
        string temp;
        remainder.push_back(divident[idx]);
        if (remainder[0] == '1')
        {
            for (j = 1; j < (int) generator.length(); j++)
                temp += my_xor(remainder[j], generator[j]);
            remainder = temp;
        }
        else
        {
            for(j = 1; j<(int)generator.length(); j++) 
                temp += my_xor(remainder[j], '0');
            remainder = temp;
        }
        idx++;
    }
    return data + remainder;
}

int main(int argc, char *argv[]) {

    FILE *input_fp, *output_fp;
    int i, j;
    unsigned char store, size;
    string data;
    string generator;
    string result = "";
    string coded_bin = "";
    if(argc != 5) {
        fprintf(stderr, "usage: ./crc_encoder input_file output_file generator dataword_size\n");
        exit(1);
    }

    if((input_fp = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "input file open error.\n");
        exit(1);
    }

    if((output_fp = fopen(argv[2], "wb")) == NULL) {
        fprintf(stderr, "output file open error.\n");
        exit(1);
    }

    dataword_size = atoi(argv[4]);
    if(dataword_size != 4 && dataword_size != 8) {
        fprintf(stderr, "dataword size must be 4 or 8.\n");
        exit(1);    
    }
    
    generator = argv[3];
    store = fgetc(input_fp);
    while(feof(input_fp) == 0) {
        int ascii = store;
        bitset<8> bs(ascii);
        data = bs.to_string();
    
        if(dataword_size == 4) {
            string temp1 = mod2div(data.substr(0, 4), generator);
            string temp2 = mod2div(data.substr(4), generator);
            result = temp1 + temp2;
        }
        else {
            result = mod2div(data, generator);
        }

        coded_bin += result;
        
        store = fgetc(input_fp);
    }
    
    coded_bin.length() % 8 != 0 ? size = int(8 - (coded_bin.length()%8)) : 0;
    
    // padding bit 수 입력
    fprintf(output_fp, "%c", size);
    for(j = 0; j < size; j++) 
    {
        coded_bin.insert(0, 1, '0');
    }

    for(j = 0; j< (int)coded_bin.length()/8; j++) {
        unsigned char temp = 0;
        bitset<8> bs2(coded_bin.substr(8*j, 8));;
        int ascii2 = bs2.to_ulong();
        temp = char(ascii2);
        fprintf(output_fp, "%c", temp);
    }
    fclose(input_fp);
    fclose(output_fp);
    return 0;
}