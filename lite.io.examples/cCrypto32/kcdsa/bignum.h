#ifndef _BIGNUM_H
#define _BIGNUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "kcdsa_type.h"

	////////	Define the Endianness	////////
#undef BIG_ENDIAN
#undef LITTLE_ENDIAN

#if defined(USER_BIG_ENDIAN)
#define BIG_ENDIAN
#elif defined(USER_LITTLE_ENDIAN)
#define LITTLE_ENDIAN
#else
#if 0
#define BIG_ENDIAN		//	Big-Endian machine with pointer casting
#elif defined(_MSC_VER)
#define LITTLE_ENDIAN	//	Little-Endian machine with pointer casting
#else
//#error
#endif
#endif

/*************** Definitions / Macros  ************************************/
#define DIGITSIZE			4
#define BitsInDIGIT			32 //(8*DIGITSIZE)
#define CheckBitDIGIT(A, k)	CHECK_BIT_D(A, k)
#define SetBitDIGIT(A, k)	SET_BIT_D(A, k)

#define isEven0(A)		( ( (A)[0]&1 )==0 )
#define isOdd0(A)		( ( (A)[0]&1 )==1 )

#define BN_MAX_BITS		3072			//	Long #�� �ִ� ��Ʈ�� : 512/768/1024/2048//3072
#define MaxDIGIT		96 //((BN_MAX_BITS-1)/BitsInDIGIT+1)	//	Long #�� �ִ� �ڸ���
#define MAX_SEED_LEN	64	//(512/8) //	in BYTEs

/*************** Macros ***************************************************/
////////	bit control macros	////////
//	bit ����
#define NOT(x)				(~(x))
//	check k-th bits of A (array of uint32_t)
#define CHECK_BIT_B(A, k)	( 1 & ( (A)[(k)>>3] >> ((k) & ( 8-1)) ) )
#define CHECK_BIT_D(A, k)	( 1 & ( (A)[(k)>>5] >> ((k) & (32-1)) ) )
//	set k-th bits of A (array of uint32_t)
#define SET_BIT_B(A, k)		(A)[(k)>>3] |= ((uint32_t)1 << ((k) & ( 8-1)) )
#define SET_BIT_D(A, k)		(A)[(k)>>5] |= ((uint32_t)1 << ((k) & (32-1)) )
//	clear k-th bits of A (array of uint32_t)
#define CLEAR_BIT_B(A, k)	(A)[(k)>>3] &= NOT((uint32_t)1 << ((k) & ( 8-1)) )
#define CLEAR_BIT_D(A, k)	(A)[(k)>>5] &= NOT((uint32_t)1 << ((k) & (32-1)) )
//	change k-th bits of A (array of uint32_t)
#define CHANGE_BIT_B(A, k)	(A)[(k)>>3] ^= ((uint32_t)1 << ((k) & ( 8-1)) )
#define CHANGE_BIT_D(A, k)	(A)[(k)>>5] ^= ((uint32_t)1 << ((k) & (32-1)) )

/*************** New Data Types *******************************************/
////////	Determine data types depand on the processor and compiler.
//#define BOOL	int					//	1-bit data type
#define BYTE		uint8_t		//	unsigned 1-byte data type
//#define WORD	uint32_t		//	unsigned 4-bytes data type


#if defined(_MSC_VER)
#define DWORD	uint64_t		//	unsigned 8-bytes data type
#else
#define DWORD	uint64_t	//	unsigned 8-bytes data type
#endif


#if defined(DWORD)
#define DOUBLE_DIGIT	DWORD
#endif


////	BIGNUM..
	typedef struct {
		uint32_t	Length;		//	��ȿ�� DIGIT�� ���� ��Ÿ��
		uint32_t	Space;		//	MUST : MemoryLen>=DataLen+1
		uint32_t	*pData;		//	������ �����Ͱ� ����� �ּ�
	} BIGNUM;

	/*************** Definitions / Macros  ************************************/
#define CTR_SUCCESS					0
#define CTR_VERIFY_FAIL				1
#define CTR_FATAL_ERROR				2
#define CTR_INVALID_POINTER			3
#define CTR_INVALID_ALG_PARAMS		4
#define CTR_MEMORY_ALLOC_ERROR		5
#define CTR_BUFFER_TOO_SMALL		6
#define CTR_INVALID_DATA_LEN		7
#define CTR_INVALID_SIGNATURE_LEN	8

#define ERROR_MemLen1				21	//	input : DataLen<=MemoryLen
#define ERROR_MemLen2				22	//	output : DataLen<=MemoryLen
#define ERROR_OverModulus			23	//	modulus oper. : NO input > modulus
#define CTR_BN_NEGATIVE_RESULT		24	//	������ �������� ����

/*************** Global Variables *****************************************/
	extern BIGNUM	BN_Zero, BN_One, BN_Two;

	/*************** Prototypes ***********************************************/
	//########################################
	//	uint32_t �������� ����/������ �Լ�
	//	uint32_t array�� �ٽɿ��� �Լ�
	//########################################

	//########################################
	//	BIGNUM�� ���� �Լ� 
	//########################################

	//	Create "BIGNUM" data and return the pointer
	BIGNUM	*CreateBigNum(
		uint32_t		dMemoryLen);	//	in unsigned ints

//	Destroy "BIGNUM" data
	void	DestroyBigNum(
		BIGNUM		*BN_Src);		//	pointer of BIGNUM to be destroyed

//
	uint32_t	BN2OS(
		BIGNUM	*BN_Src,	//	Source integer
		uint32_t	dDstLen,	//	Destination Length in BYTEs
		uint8_t	*pbDst);	//	Destination Octet string pointer
	uint32_t	OS2BN(
		uint8_t	*pbSrc,		//	Source Octet string pointer
		uint32_t	dSrcLen,	//	Source Length in BYTEs
		BIGNUM	*BN_Dst);	//	Destination uint32_t array pointer

//	Long Number Copy : BN_Dst <- BN_Src
	uint32_t BN_Copy(BIGNUM *BN_Dst, BIGNUM *BN_Src);

	//	Long Number Compare : return the sign of (BN_Src1 - BN_Src2)
	int32_t BN_Cmp(BIGNUM *BN_Src1, BIGNUM *BN_Src2);

	//	Long Random Number : BN_Dst <- 'BitLen'-uint32_t random long number
	uint32_t BN_Rand(BIGNUM *BN_Dst, uint32_t BitLen);

	//########################################
	//	BIGNUM�� �ٽɿ��� �Լ�
	//########################################

	//	Long Number (NumOfShift)-bits shift left : BN_Dst = BN_Src << NumOfShift
	uint32_t BN_SHL(BIGNUM *BN_Dst, BIGNUM *BN_Src, uint32_t NumOfShift);
	//	Long Number (NumOfShift)-bits shift right : BN_Dst = BN_Src >> NumOfShift
	uint32_t BN_SHR(BIGNUM *BN_Dst, BIGNUM *BN_Src, uint32_t NumOfShift);

	//	Long Nymber Addition : BN_Dst <- BN_Src1 + BN_Src2
	uint32_t BN_Add(BIGNUM *BN_Dst, BIGNUM *BN_Src1, BIGNUM *BN_Src2);
	//	Long Number Subtraction : BN_Dst <- BN_Src1 - BN_Src2
	uint32_t BN_Sub(BIGNUM *BN_Dst, BIGNUM *BN_Src1, BIGNUM *BN_Src2);

	//	Long Number Multiple : BN_Dst <- BN_Src1 * BN_Src2
	//			the most general multiple function
	uint32_t BN_Mul(BIGNUM *BN_Dst, BIGNUM *BN_Src1, BIGNUM *BN_Src2);

	//	Long Number Division : BN_Res <- BN_Dividend div BN_Divisor
	uint32_t BN_Div(BIGNUM *BN_Quotient, BIGNUM *BN_Remainder,
		BIGNUM *BN_Dividend, BIGNUM *BN_Divisor);

	//########################################
	//	BIGNUM�� Modulus ���� �Լ�
	//########################################

	//	Long Number Modular addtion :
	//				BN_Dst <- BN_Src1 + BN_Src2 mod BN_Modulus
	uint32_t BN_ModAdd(BIGNUM *BN_Dst, BIGNUM *BN_Src1, BIGNUM *BN_Src2,
		BIGNUM *BN_Modulus);
	//	Long Number Modular subtraction :
	//				BN_Dst <- BN_Src1 - BN_Src2 mod BN_Modulus
	uint32_t BN_ModSub(BIGNUM *BN_Dst, BIGNUM *BN_Src1, BIGNUM *BN_Src2,
		BIGNUM *BN_Modulus);

	//	Long Number Modular Reduction - Classical Algorithm
	uint32_t BN_ModRed(BIGNUM *BN_Dst, BIGNUM *BN_Src, BIGNUM *BN_Modulus);
	//	Long Number Modular Multiple - Classical Algorithm
	uint32_t BN_ModMul(BIGNUM *BN_Res, BIGNUM *BN_Src1, BIGNUM *BN_Src2,
		BIGNUM *BN_Modulus);

	//	Extended Euclid Algorithm
	//		return CTR_SUCCESS	if gcd(BN_Src,BN_Modulus)==1 :
	//					BN_Dst <- BN_Src^-1 mod BN_Modulus
	//		return !CTR_SUCCESS if gcd(BN_Src,BN_Modulus)!=1 :
	//					BN_Dst <- gcd(BN_Src, BN_Modulus)
	uint32_t BN_ModInv(BIGNUM *BN_Dst, BIGNUM *BN_Src, BIGNUM *BN_Modulus);

	//	Long Number Modular Exponential Algorithm - 
	//		Window Algorithm and Montgomery Reduction Algorithm
	uint32_t BN_ModExp(BIGNUM *BN_Dst, BIGNUM *BN_Base, BIGNUM *BN_Exponent,
		BIGNUM *BN_Modulus);

	//########################################
	//	��Ÿ ���� ���� �Լ�
	//########################################

	//
	uint32_t MillerRabin(
		BIGNUM		*BN_Num);

	/*************** END OF FILE **********************************************/

#ifdef __cplusplus
}
#endif
#endif	//	_BIGNUM_H
