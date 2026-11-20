#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "playfair.h"
#include "bmp.h"

void print_usage()
{
    printf("Usage: ./cipher <mode> <algorithm> <key> <text>\n");
    printf("Modes: encrypt, decrypt\n");
    printf("Algorithms: playfair, vigenere, bit, bmp\n");
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        print_usage();
        return 1;
    }
    
    const char *mode = argv[1];
    const char *algorithm = argv[2];
    const char *key = argv[3];
    const char *text = argv[4];
    
    char *result = NULL;
    unsigned char *bit_result = NULL;
    
    bool encrypt = strcmp(mode, "encrypt") == 0;
    bool decrypt = strcmp(mode, "decrypt") == 0;
    
    if (!encrypt && !decrypt)
    {
        print_usage();
        return 1;
    }
    
    if (strcmp(algorithm, "playfair") == 0)
    {
        result = encrypt ? playfair_encrypt(key, text) : playfair_decrypt(key, text);
    }
    else if (strcmp(algorithm, "vigenere") == 0)
    {
        result = encrypt ? vigenere_encrypt(key, text) : vigenere_decrypt(key, text);
    }
    else if (strcmp(algorithm, "bit") == 0)
    {
        bit_result = encrypt ? bit_encrypt(text) : (unsigned char *)bit_decrypt((unsigned char *)text);
        result = (char *)bit_result;
    }
    else if (strcmp(algorithm, "bmp") == 0)
    {
        bit_result = encrypt ? bmp_encrypt(key, text) : (unsigned char *)bmp_decrypt(key, (unsigned char *)text);
        result = (char *)bit_result;
    }
    else
    {
        print_usage();
        return 1;
    }
    
    if (result)
    {
        printf("Result: %s\n", result);
        free(result);
    }
    else
    {
        printf("Error during processing.\n");
        return 1;
    }
    
    return 0;
}
