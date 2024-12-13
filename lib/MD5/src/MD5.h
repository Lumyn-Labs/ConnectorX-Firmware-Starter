#ifndef _MD5_
#define _MD5_

// Define Arduino Library
#ifndef Arduino_h
#include <Arduino.h>
#endif

// Define the MD5 Hashing Class
#define E(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))

// Define the MD5 Hashing Class
#define STEP(f, a, b, c, d, x, t, s)                         \
  (a) += f((b), (c), (d)) + (x) + (t);                       \
  (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
  (a) += (b);

// Define the MD5 Hashing Class
#define MD5_SET(n)                                               \
  (this->_Block[(n)] = (unsigned long)ptr[(n) * 4] |             \
                       ((unsigned long)ptr[(n) * 4 + 1] << 8) |  \
                       ((unsigned long)ptr[(n) * 4 + 2] << 16) | \
                       ((unsigned long)ptr[(n) * 4 + 3] << 24))
#define MD5_GET(n) (this->_Block[(n)])

// MD5 Hashing Class
class MD5 {
  // Private Context
 private:
  // Internal state
  uint64_t _Lo;
  uint64_t _Hi;
  uint64_t _A;
  uint64_t _B1;
  uint64_t _C1;
  uint64_t _D;

  // Internal data buffer
  uint8_t _Buffer[64];
  uint64_t _Block[16];

  // Process the given data
  const void* Process(const void* data, size_t length) {
    // Declare Pointer
    const uint8_t* ptr;

    // Declare Variables
    uint64_t a;
    uint64_t b;
    uint64_t c;
    uint64_t d;
    uint64_t saved_a;
    uint64_t saved_b;
    uint64_t saved_c;
    uint64_t saved_d;

    // Declare the pointer
    ptr = (uint8_t*)data;

    // Load the internal state
    a = this->_A;
    b = this->_B1;
    c = this->_C1;
    d = this->_D;

    // Process the data
    do {
      // Load the internal state
      saved_a = a;
      saved_b = b;
      saved_c = c;
      saved_d = d;

      /* Round 1
       * E() has been used instead of F() because F() is already defined in the
       * Arduino core
       */
      STEP(E, a, b, c, d, MD5_SET(0), 0xd76aa478, 7)
      STEP(E, d, a, b, c, MD5_SET(1), 0xe8c7b756, 12)
      STEP(E, c, d, a, b, MD5_SET(2), 0x242070db, 17)
      STEP(E, b, c, d, a, MD5_SET(3), 0xc1bdceee, 22)
      STEP(E, a, b, c, d, MD5_SET(4), 0xf57c0faf, 7)
      STEP(E, d, a, b, c, MD5_SET(5), 0x4787c62a, 12)
      STEP(E, c, d, a, b, MD5_SET(6), 0xa8304613, 17)
      STEP(E, b, c, d, a, MD5_SET(7), 0xfd469501, 22)
      STEP(E, a, b, c, d, MD5_SET(8), 0x698098d8, 7)
      STEP(E, d, a, b, c, MD5_SET(9), 0x8b44f7af, 12)
      STEP(E, c, d, a, b, MD5_SET(10), 0xffff5bb1, 17)
      STEP(E, b, c, d, a, MD5_SET(11), 0x895cd7be, 22)
      STEP(E, a, b, c, d, MD5_SET(12), 0x6b901122, 7)
      STEP(E, d, a, b, c, MD5_SET(13), 0xfd987193, 12)
      STEP(E, c, d, a, b, MD5_SET(14), 0xa679438e, 17)
      STEP(E, b, c, d, a, MD5_SET(15), 0x49b40821, 22)

      /* Round 2 */
      STEP(G, a, b, c, d, MD5_GET(1), 0xf61e2562, 5)
      STEP(G, d, a, b, c, MD5_GET(6), 0xc040b340, 9)
      STEP(G, c, d, a, b, MD5_GET(11), 0x265e5a51, 14)
      STEP(G, b, c, d, a, MD5_GET(0), 0xe9b6c7aa, 20)
      STEP(G, a, b, c, d, MD5_GET(5), 0xd62f105d, 5)
      STEP(G, d, a, b, c, MD5_GET(10), 0x02441453, 9)
      STEP(G, c, d, a, b, MD5_GET(15), 0xd8a1e681, 14)
      STEP(G, b, c, d, a, MD5_GET(4), 0xe7d3fbc8, 20)
      STEP(G, a, b, c, d, MD5_GET(9), 0x21e1cde6, 5)
      STEP(G, d, a, b, c, MD5_GET(14), 0xc33707d6, 9)
      STEP(G, c, d, a, b, MD5_GET(3), 0xf4d50d87, 14)
      STEP(G, b, c, d, a, MD5_GET(8), 0x455a14ed, 20)
      STEP(G, a, b, c, d, MD5_GET(13), 0xa9e3e905, 5)
      STEP(G, d, a, b, c, MD5_GET(2), 0xfcefa3f8, 9)
      STEP(G, c, d, a, b, MD5_GET(7), 0x676f02d9, 14)
      STEP(G, b, c, d, a, MD5_GET(12), 0x8d2a4c8a, 20)

      /* Round 3 */
      STEP(H, a, b, c, d, MD5_GET(5), 0xfffa3942, 4)
      STEP(H, d, a, b, c, MD5_GET(8), 0x8771f681, 11)
      STEP(H, c, d, a, b, MD5_GET(11), 0x6d9d6122, 16)
      STEP(H, b, c, d, a, MD5_GET(14), 0xfde5380c, 23)
      STEP(H, a, b, c, d, MD5_GET(1), 0xa4beea44, 4)
      STEP(H, d, a, b, c, MD5_GET(4), 0x4bdecfa9, 11)
      STEP(H, c, d, a, b, MD5_GET(7), 0xf6bb4b60, 16)
      STEP(H, b, c, d, a, MD5_GET(10), 0xbebfbc70, 23)
      STEP(H, a, b, c, d, MD5_GET(13), 0x289b7ec6, 4)
      STEP(H, d, a, b, c, MD5_GET(0), 0xeaa127fa, 11)
      STEP(H, c, d, a, b, MD5_GET(3), 0xd4ef3085, 16)
      STEP(H, b, c, d, a, MD5_GET(6), 0x04881d05, 23)
      STEP(H, a, b, c, d, MD5_GET(9), 0xd9d4d039, 4)
      STEP(H, d, a, b, c, MD5_GET(12), 0xe6db99e5, 11)
      STEP(H, c, d, a, b, MD5_GET(15), 0x1fa27cf8, 16)
      STEP(H, b, c, d, a, MD5_GET(2), 0xc4ac5665, 23)

      /* Round 4 */
      STEP(I, a, b, c, d, MD5_GET(0), 0xf4292244, 6)
      STEP(I, d, a, b, c, MD5_GET(7), 0x432aff97, 10)
      STEP(I, c, d, a, b, MD5_GET(14), 0xab9423a7, 15)
      STEP(I, b, c, d, a, MD5_GET(5), 0xfc93a039, 21)
      STEP(I, a, b, c, d, MD5_GET(12), 0x655b59c3, 6)
      STEP(I, d, a, b, c, MD5_GET(3), 0x8f0ccc92, 10)
      STEP(I, c, d, a, b, MD5_GET(10), 0xffeff47d, 15)
      STEP(I, b, c, d, a, MD5_GET(1), 0x85845dd1, 21)
      STEP(I, a, b, c, d, MD5_GET(8), 0x6fa87e4f, 6)
      STEP(I, d, a, b, c, MD5_GET(15), 0xfe2ce6e0, 10)
      STEP(I, c, d, a, b, MD5_GET(6), 0xa3014314, 15)
      STEP(I, b, c, d, a, MD5_GET(13), 0x4e0811a1, 21)
      STEP(I, a, b, c, d, MD5_GET(4), 0xf7537e82, 6)
      STEP(I, d, a, b, c, MD5_GET(11), 0xbd3af235, 10)
      STEP(I, c, d, a, b, MD5_GET(2), 0x2ad7d2bb, 15)
      STEP(I, b, c, d, a, MD5_GET(9), 0xeb86d391, 21)

      // Update the internal state
      a += saved_a;
      b += saved_b;
      c += saved_c;
      d += saved_d;

      // Move the pointer
      ptr += 64;

    } while (length -= 64);

    // Save the internal state
    this->_A = a;
    this->_B1 = b;
    this->_C1 = c;
    this->_D = d;

    // Return the pointer
    return ptr;
  }

  // Public Context
 public:
  // Constructor
  explicit MD5() {
    // Reset internal state
    this->Reset();
  }

  // Reset internal state
  void Reset(void) {
    // Reset the internal state
    this->_A = 0x67452301;
    this->_B1 = 0xefcdab89;
    this->_C1 = 0x98badcfe;
    this->_D = 0x10325476;

    // Reset the internal data buffer
    this->_Lo = 0;
    this->_Hi = 0;

    // Reset the internal data buffer
    memset(_Buffer, 0, 64 * sizeof(unsigned char));
    memset(_Block, 0, 16 * sizeof(unsigned long));
  }

  // Add data to the interative hash solution
  void Update(const void* data, uint32_t size) {
    // Declare the used and free variables
    uint64_t _Saved_lo;
    uint64_t _Used, _Free;

    // Save the internal state
    _Saved_lo = this->_Lo;

    // Update the internal state
    if ((this->_Lo = (_Saved_lo + size) & 0x1fffffff) < _Saved_lo) this->_Hi++;

    // Update the internal state
    this->_Hi += size >> 29;

    // Define the used and free variables
    _Used = _Saved_lo & 0x3f;

    // Process the data
    if (_Used) {
      // Define the free variable
      _Free = 64 - _Used;

      if (size < _Free) {
        memcpy(&this->_Buffer[_Used], data, size);
        return;
      }

      memcpy(&this->_Buffer[_Used], data, _Free);
      data = (unsigned char*)data + _Free;
      size -= _Free;

      // Process the data
      this->Process(this->_Buffer, 64);
    }

    if (size >= 64) {
      data = this->Process(data, size & ~(size_t)0x3f);
      size &= 0x3f;
    }

    memcpy(this->_Buffer, data, size);
  }

  // Finalize the hash to the given 16-byte buffer
  void Finalize(uint8_t* _Result) {
    // Define the used and free variables
    uint32_t _Used, _Free;

    _Used = this->_Lo & 0x3f;

    this->_Buffer[_Used++] = 0x80;

    _Free = 64 - _Used;

    if (_Free < 8) {
      memset(&this->_Buffer[_Used], 0, _Free);
      this->Process(this->_Buffer, 64);
      _Used = 0;
      _Free = 64;
    }

    memset(&this->_Buffer[_Used], 0, _Free - 8);

    this->_Lo <<= 3;
    this->_Buffer[56] = this->_Lo;
    this->_Buffer[57] = this->_Lo >> 8;
    this->_Buffer[58] = this->_Lo >> 16;
    this->_Buffer[59] = this->_Lo >> 24;
    this->_Buffer[60] = this->_Hi;
    this->_Buffer[61] = this->_Hi >> 8;
    this->_Buffer[62] = this->_Hi >> 16;
    this->_Buffer[63] = this->_Hi >> 24;

    this->Process(this->_Buffer, 64);

    // Calculate the _Result
    _Result[0] = this->_A;
    _Result[1] = this->_A >> 8;
    _Result[2] = this->_A >> 16;
    _Result[3] = this->_A >> 24;
    _Result[4] = this->_B1;
    _Result[5] = this->_B1 >> 8;
    _Result[6] = this->_B1 >> 16;
    _Result[7] = this->_B1 >> 24;
    _Result[8] = this->_C1;
    _Result[9] = this->_C1 >> 8;
    _Result[10] = this->_C1 >> 16;
    _Result[11] = this->_C1 >> 24;
    _Result[12] = this->_D;
    _Result[13] = this->_D >> 8;
    _Result[14] = this->_D >> 16;
    _Result[15] = this->_D >> 24;
  }

  // Calculate the hash of the given null-terminated string constant
  static void Hash(const char* str, unsigned char* hash) {
    // Create a new MD5 instance
    MD5 md5 = MD5();

    // Update the hash with the given string
    for (; *str; str++) md5.Update(str, 1);

    // Finalize the hash
    md5.Finalize(hash);
  }

  // Calculate the hash of the given buffer
  static void Hash(const void* buffer, size_t size, unsigned char* hash) {
    // Create a new MD5 instance
    MD5 md5 = MD5();

    // Update the hash with the given buffer
    md5.Update(buffer, size);

    // Finalize the hash
    md5.Finalize(hash);
  }

  // Produce a hex digest of the given hash
  static void Digest(const unsigned char* hash, char* digest) {
    static const char digits[17] = "0123456789abcdef";

    for (int i = 0; i < 16; ++i) {
      digest[i * 2] = digits[hash[i] >> 4];
      digest[(i * 2) + 1] = digits[hash[i] & 0x0F];
    }

    digest[32] = 0;
  }
};

#endif