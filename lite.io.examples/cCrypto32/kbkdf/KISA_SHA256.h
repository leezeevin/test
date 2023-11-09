/**
@file KISA_SHA_256.h
@brief SHA256 ��ȣ �˰���
@author Copyright (c) 2013 by KISA
@remarks http://seed.kisa.or.kr/
*/

#ifndef _KISA_KISA_SHA256_H
#define _KISA_KISA_SHA256_H

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
	#define IN
#endif

#ifndef INOUT
	#define INOUT
#endif

#define USER_LITTLE_ENDIAN

#if defined(USER_BIG_ENDIAN)
	#define USING_BIG_ENDIAN
#elif defined(USER_LITTLE_ENDIAN)
	#define USING_LITTLE_ENDIAN
#else
	#if 0
		#define USING_BIG_ENDIAN
	#elif defined(_MSC_VER)
		#define USING_LITTLE_ENDIAN
	#else
		#error
	#endif
#endif

	//typedef unsigned long ULONG;
	//typedef ULONG* ULONG_PTR;
	
	typedef uint32_t ULONG;
	typedef ULONG* ULONG_PTR;

	typedef uint32_t UINT;
	typedef UINT* UINT_PTR;

	typedef int32_t SINT;
	typedef SINT* SINT_PTR;

	typedef uint8_t UCHAR;
	typedef UCHAR* UCHAR_PTR;

	typedef uint8_t BYTE;

#define KISA_SHA256_DIGEST_BLOCKLEN	64
#define KISA_SHA256_DIGEST_VALUELEN	32

	typedef struct {
		UINT uChainVar[KISA_SHA256_DIGEST_VALUELEN / 4];
		UINT uHighLength;
		UINT uLowLength;
		BYTE szBuffer[KISA_SHA256_DIGEST_BLOCKLEN];
	} KISA_SHA256_INFO;

	/**
	@brief ���⺯���� ���̺����� �ʱ�ȭ�ϴ� �Լ�
	@param Info : KISA_SHA256_Process ȣ�� �� ���Ǵ� ����ü
	*/
	void KISA_SHA256_Init(OUT KISA_SHA256_INFO* Info);

	/**
	@brief ���⺯���� ���̺����� �ʱ�ȭ�ϴ� �Լ�
	@param Info : KISA_SHA256_Init ȣ���Ͽ� �ʱ�ȭ�� ����ü(���������� ���ȴ�.)
	@param pszMessage : ����� �Է� ��
	@param inLen : ����� �Է� �� ����
	*/
	void KISA_SHA256_Process(OUT KISA_SHA256_INFO* Info, IN const BYTE* pszMessage, IN UINT uDataLen);

	/**
	@brief �޽��� �����̱�� ���� �����̱⸦ ������ �� ������ �޽��� ����� ������ �����Լ��� ȣ���ϴ� �Լ�
	@param Info : KISA_SHA256_Init ȣ���Ͽ� �ʱ�ȭ�� ����ü(���������� ���ȴ�.)
	@param pszDigest : ��ȣ��
	*/
	void KISA_SHA256_Close(OUT KISA_SHA256_INFO* Info, IN BYTE* pszDigest);

	/**
	@brief ����� �Է� ���� �ѹ��� ó��
	@param pszMessage : ����� �Է� ��
	@param pszDigest : ��ȣ��
	@remarks ���������� KISA_SHA256_Init, KISA_SHA256_Process, KISA_SHA256_Close�� ȣ���Ѵ�.
	*/
	void KISA_SHA256_Encrpyt(IN const BYTE* pszMessage, IN UINT uPlainTextLen, OUT BYTE* pszDigest);

#ifdef  __cplusplus
}
#endif

#endif