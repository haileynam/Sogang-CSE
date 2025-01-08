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

int mod2div(string data, string generator) {
    string divident = data;
    string remainder;
    int idx = (int)(generator.length() - 1);
    remainder = divident.substr(0, idx);
    for(int i = 0; i<(int)data.length() - (int)generator.length() + 1; i++) {
        string temp;
        remainder.push_back(divident[idx]);
        if(remainder[0] == '1') 
		{
            for(int j = 1; j<(int)generator.length(); j++) 
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

    for(int i = 0; i<(int)remainder.length(); i++) {
		int ret = remainder[i] == '0' ? 1 : 0;
        //if(remainder[i] != '0') return false;
		if (ret == 0) return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {
    FILE *input_fp, *output_fp, *result_fp;
    string generator, result, codeword, ch[3], ans;
    char zero_padding, st;
    int size, codeword_cnt = 0, error_cnt = 0;
    if(argc != 6) {
        fprintf(stderr, "usage: ./crc_decoder input_file output_file result_file generator dataword_size\n");
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

    if((result_fp = fopen(argv[3], "wb")) == NULL) {
        fprintf(stderr, "result file open error.\n");
        exit(1);
    }

    generator = argv[4];
    dataword_size = atoi(argv[5]);
    if(dataword_size != 4 && dataword_size != 8) {
        fprintf(stderr, "dataword size must be 4 or 8.\n");
        exit(1);    
    }

	// 한세트의 codeword 길이 = dataword size + generator size -1
    int len = dataword_size + (int)(generator.length()) - 1;
	// zero_padding 개수를 char로 입력했었음 -> char
    zero_padding = fgetc(input_fp);
    
    // size: 쪼개진 갯수 * len. 
	if (dataword_size == 4)
		size = 2 * len;
	else 
		size = len;
    
	fscanf(input_fp, "%c", &st);
    //fread(&st, sizeof(unsigned char), 1, input_fp);
    while(feof(input_fp) == 0) {
        result = char2bin(st);
        for(int i = 0; i<(int)result.length(); i++) 
			ans.push_back(result[i]);
		fscanf(input_fp, "%c", &st);
        //fread(&st, sizeof(unsigned char), 1, input_fp);
    }

	// zero padding 제거
	ans = ans.substr(zero_padding);
    
	for(int j = 0; j < (int)ans.length() / size; j++) {
        string s;
        string temp = ans.substr(j*size, size);

        for(int k = 0; k< size/len; k++) {
            string codeword = temp.substr(k*len, len);
            codeword_cnt++;
            if(mod2div(codeword, generator) == 0) 
				error_cnt++;
            s = s + codeword.substr(0, dataword_size);
        }
        fprintf(output_fp, "%c", binary_converter(s));
        //fputc(binary_converter(s), output_fp);
    }
    fprintf(result_fp, "%d %d", codeword_cnt, error_cnt);
    fclose(input_fp);
    fclose(output_fp);
    fclose(result_fp);
    return 0;
}
