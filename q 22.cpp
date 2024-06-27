#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Function to perform initial permutation (IP) and inverse IP
void initial_permutation(uint8_t *block) {
    uint8_t temp = *block;
    *block = ((temp & 0x80) >> 7) | ((temp & 0x40) >> 5) | ((temp & 0x20) >> 3) | ((temp & 0x10) >> 1) |
             ((temp & 0x08) << 1) | ((temp & 0x04) << 3) | ((temp & 0x02) << 5) | ((temp & 0x01) << 7);
}

void inverse_permutation(uint8_t *block) {
    uint8_t temp = *block;
    *block = ((temp & 0x80) >> 7) | ((temp & 0x40) >> 5) | ((temp & 0x20) >> 3) | ((temp & 0x10) >> 1) |
             ((temp & 0x08) << 1) | ((temp & 0x04) << 3) | ((temp & 0x02) << 5) | ((temp & 0x01) << 7);
}

// Function to perform S-DES encryption/decryption
uint8_t sdes(uint8_t input, uint8_t key, int mode) {
    // Initial Permutation
    initial_permutation(&input);

    // Placeholder for actual S-DES logic (simplified here)
    uint8_t left = (input & 0xF0) >> 4;
    uint8_t right = input & 0x0F;

    // Simplified Feistel structure (for illustration purposes)
    uint8_t temp = right;
    if (mode == 0) {  // Encrypt
        right = left ^ (right ^ key);
    } else {  // Decrypt
        left = right ^ (left ^ key);
    }
    left = temp;

    // Combine left and right parts
    input = (left << 4) | right;

    // Inverse Permutation
    inverse_permutation(&input);

    return input;
}

// Function to perform CBC mode encryption
void cbc_encrypt(uint8_t *plaintext, uint8_t *ciphertext, uint8_t key, uint8_t iv, size_t length) {
    uint8_t block, prev_block = iv;

    for (size_t i = 0; i < length; i++) {
        block = plaintext[i] ^ prev_block;
        ciphertext[i] = sdes(block, key, 0);
        prev_block = ciphertext[i];
    }
}

// Function to perform CBC mode decryption
void cbc_decrypt(uint8_t *ciphertext, uint8_t *plaintext, uint8_t key, uint8_t iv, size_t length) {
    uint8_t block, prev_block = iv;

    for (size_t i = 0; i < length; i++) {
        block = sdes(ciphertext[i], key, 1);
        plaintext[i] = block ^ prev_block;
        prev_block = ciphertext[i];
    }
}

int main() {
    uint8_t iv = 0xAA;  // Binary: 1010 1010
    uint8_t key = 0xFD; // Binary: 01111 11101
    uint8_t plaintext[] = {0x01, 0x23}; // Binary: 0000 0001 0010 0011
    size_t length = sizeof(plaintext) / sizeof(plaintext[0]);

    uint8_t ciphertext[length];
    uint8_t decrypted[length];

    // Encrypt the plaintext
    cbc_encrypt(plaintext, ciphertext, key, iv, length);

    // Decrypt the ciphertext
    cbc_decrypt(ciphertext, decrypted, key, iv, length);

    // Print the results
    printf("Plaintext: ");
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", plaintext[i]);
    }
    printf("\n");

    printf("Ciphertext: ");
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    printf("Decrypted: ");
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", decrypted[i]);
    }
    printf("\n");

    return 0;
}
