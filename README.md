# MD5 & HMAC-MD5

MD5 and HMAC-MD5 algorithm implemented in C language.

## Usage

```shell
make
```

```shell
./bin/md5 <INPUT_STRING>
./bin/hmacmd5 <INPUT_STRING> -key <KEY>
```

## How It Works

1. Padding Message
   * First fill with a 1, and then fill with multiple 0s, until the information length is equal to 448 modulo 512
   * Pad data length of 64 bits. Note that the data length represents the number of bits, not bytes

2. Initialize MD Buffer
   * Initialize 4 32-bit buffers A, B, C, D

3. Divide Block
   * Divide the padded information into blocks of 512 bits

4. Loop Compression
   * 4 cycles, 16 iterations per cycle, 64 iterations in total
   * Each operation modifies the data in the buffer, and the data in the buffer is used as the input for the next iteration
   * 4 cycles using 4 (F, G, H, I) functions
   * In each of the 16 iterations, we take a different part of the block (g table), add a different fixed value (K table), and loop left by a different number of bits (s table).

5. Output
   * Concatenate 4 32-bit buffers to get 128-bit result

## Reference

* [https://www.rfc-editor.org/rfc/rfc1321.html](https://www.rfc-editor.org/rfc/rfc1321.html)
