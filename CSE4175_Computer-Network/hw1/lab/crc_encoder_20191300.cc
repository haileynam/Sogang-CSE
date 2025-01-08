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

unsigned char binary_converter(string pass) {
    unsigned char value = 0;
    bitset<8> bs(pass);
    int ascii = bs.to_ulong();
    value = char(ascii);
    //cout <<"pass: " << pass << " value: " << value;
    return value;
}

string char2bin(unsigned char value) {
    int ascii = value;
    bitset<8> bs(ascii);
    string s = bs.to_string();
    //cout << "value: " << value << " s: " << s << endl;
    return s;
}

char my_xor(char a, char b) {
    int ret = a == b? '0': '1';
    return ret;
}

string calculate_encoded_data(string data, string generator) {
    string divident  = data;
    string remainder;
    int idx = (int)(generator.length() - 1);
    // generator 길이 -1 만큼 dataword 뒤에 0 붙이기.
    for(int i = 0; i < (int)generator.length()-1; i++) 
        divident.push_back('0');
    // generator = divisor
    // 나머지: generator 길이 까지는 divident가 그대로 내려옴
    remainder = divident.substr(0, idx);
    for(int i = 0; i< (int)data.length(); i++) {
        string temp;
        remainder.push_back(divident[idx]);
        if (remainder[0] == '1')
        {
            for (int j = 1; j < (int) generator.length(); j++)
                temp.push_back(my_xor(remainder[j], generator[j]));
            remainder = temp;
        }
        else
        {
            for(int j = 1; j<(int)generator.length(); j++) 
                temp.push_back(my_xor(remainder[j], '0'));
            remainder = temp;
        }
        idx++;
    }
    return data + remainder;
}

int main(int argc, char *argv[]) {

    FILE *input_fp, *output_fp;
    unsigned char store, size;
    string data;
    string generator;
    string result,ans;
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
        data = char2bin(store);
        if(dataword_size == 4) {
            string temp1 = calculate_encoded_data(data.substr(0, 4), generator);
            string temp2 = calculate_encoded_data(data.substr(4, 4), generator);
            result = temp1 + temp2;
        }
        else {
            result = calculate_encoded_data(data, generator);
        }
        for(int i = 0; i<(int)result.length(); i++) 
            ans.push_back(result[i]);
        store = fgetc(input_fp);
    }
    //cout << "ans: "<< ans << endl;
    // ans.length() % 8 == 0? size = 0 : (8-((int)ans.length()%8));
    ans.length() % 8 != 0 ? size = int(8 - (ans.length()%8)) : 0;
    //fwrite(&size, sizeof(unsigned char), 1, output_fp);
    // padding bit 수 입력
    fprintf(output_fp, "%c", size);
    for(int j = 0; j < size; j++) 
    {
        ans.insert(0, 1, '0');
    }
    //cout << "ans: "<< ans << endl;

    for(int j = 0; j< (int)ans.length()/8; j++) {
        unsigned char temp = binary_converter(ans.substr(8*j, 8));
        fprintf(output_fp, "%c", temp);
        //fwrite(&temp, sizeof(unsigned char), 1, output_fp);
    }
    fclose(input_fp);
    fclose(output_fp);
    return 0;
}
