#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
// Parameters:
//
//   float amount;
//   sampler2D texSampler;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   amount       c0       1
//   texSampler   s0       1
//

ps_2_0
def c1, 1, 0, 0, 0
dcl t0.xy
dcl_2d s0
add r0.xy, t0, -c0.x
add r1.xy, t0, c0.x
texld r0, r0, s0
texld r1, r1, s0
texld r2, t0, s0
mov r2.x, r0.x
mov r2.z, r1.z
mov r2.w, c1.x
mov oC0, r2

// approximately 9 instruction slots used (3 texture, 6 arithmetic)
#endif

const BYTE chromatic_aberration[] =
{
      0,   2, 255, 255, 254, 255,
     43,   0,  67,  84,  65,  66,
     28,   0,   0,   0, 127,   0,
      0,   0,   0,   2, 255, 255,
      2,   0,   0,   0,  28,   0,
      0,   0,   0,   1,   0,   0,
    120,   0,   0,   0,  68,   0,
      0,   0,   2,   0,   0,   0,
      1,   0,   0,   0,  76,   0,
      0,   0,   0,   0,   0,   0,
     92,   0,   0,   0,   3,   0,
      0,   0,   1,   0,   0,   0,
    104,   0,   0,   0,   0,   0,
      0,   0,  97, 109, 111, 117,
    110, 116,   0, 171,   0,   0,
      3,   0,   1,   0,   1,   0,
      1,   0,   0,   0,   0,   0,
      0,   0, 116, 101, 120,  83,
     97, 109, 112, 108, 101, 114,
      0, 171,   4,   0,  12,   0,
      1,   0,   1,   0,   1,   0,
      0,   0,   0,   0,   0,   0,
    112, 115,  95,  50,  95,  48,
      0,  77, 105,  99, 114, 111,
    115, 111, 102, 116,  32,  40,
     82,  41,  32,  72,  76,  83,
     76,  32,  83, 104,  97, 100,
    101, 114,  32,  67, 111, 109,
    112, 105, 108, 101, 114,  32,
     49,  48,  46,  49,   0, 171,
     81,   0,   0,   5,   1,   0,
     15, 160,   0,   0, 128,  63,
      0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,
     31,   0,   0,   2,   0,   0,
      0, 128,   0,   0,   3, 176,
     31,   0,   0,   2,   0,   0,
      0, 144,   0,   8,  15, 160,
      2,   0,   0,   3,   0,   0,
      3, 128,   0,   0, 228, 176,
      0,   0,   0, 161,   2,   0,
      0,   3,   1,   0,   3, 128,
      0,   0, 228, 176,   0,   0,
      0, 160,  66,   0,   0,   3,
      0,   0,  15, 128,   0,   0,
    228, 128,   0,   8, 228, 160,
     66,   0,   0,   3,   1,   0,
     15, 128,   1,   0, 228, 128,
      0,   8, 228, 160,  66,   0,
      0,   3,   2,   0,  15, 128,
      0,   0, 228, 176,   0,   8,
    228, 160,   1,   0,   0,   2,
      2,   0,   1, 128,   0,   0,
      0, 128,   1,   0,   0,   2,
      2,   0,   4, 128,   1,   0,
    170, 128,   1,   0,   0,   2,
      2,   0,   8, 128,   1,   0,
      0, 160,   1,   0,   0,   2,
      0,   8,  15, 128,   2,   0,
    228, 128, 255, 255,   0,   0
};
