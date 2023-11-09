#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "kcdsa.h"

uint32_t KISA_KCDSA_CreateObject(KISA_KCDSA	**kcdsa)
{
	int32_t i = 0;
	uint32_t		tt = 0;
	KISA_KCDSA	*BN_Key;

	if (kcdsa == NULL)
		return CTR_INVALID_POINTER;

	*kcdsa = BN_Key = (KISA_KCDSA *)malloc(sizeof(KISA_KCDSA));
	if (BN_Key == NULL)		goto LABEL_END0;
	memset((uint8_t *)BN_Key, 0, sizeof(KISA_KCDSA));

	tt = (BN_MAX_BITS - 1) / BitsInDIGIT + 1;
	if ((BN_Key->KCDSA_P = CreateBigNum(tt + 1)) == NULL)	goto LABEL_END0;
	if ((BN_Key->KCDSA_G = CreateBigNum(tt + 1)) == NULL)	goto LABEL_END0;
	if ((BN_Key->KCDSA_y = CreateBigNum(tt + 1)) == NULL)	goto LABEL_END0;
	for (i = 0; i < (signed)(tt + 1); i++)
	{
		BN_Key->KCDSA_P->pData[i] = 0;
		BN_Key->KCDSA_G->pData[i] = 0;
		BN_Key->KCDSA_y->pData[i] = 0;
	}
	tt = (256 - 1) / BitsInDIGIT + 1;
	if ((BN_Key->KCDSA_Q = CreateBigNum(tt + 1)) == NULL)	goto LABEL_END0;
	if ((BN_Key->KCDSA_x = CreateBigNum(tt + 1)) == NULL)	goto LABEL_END0;
	for (i = 0; i < (signed)(tt + 1); i++)
	{
		BN_Key->KCDSA_Q->pData[i] = 0;
		BN_Key->KCDSA_x->pData[i] = 0;
	}
	BN_Key->Count = 0;
	BN_Key->SeedLen = MAX_SEED_LEN;

	return CTR_SUCCESS;

LABEL_END0:
	KISA_KCDSA_DestroyObject(kcdsa);
	return CTR_MEMORY_ALLOC_ERROR;
}

uint32_t KCDSA_PRNG_SHA_224(
	SHA224_ALG_INFO	*SHA224_AlgInfo,
	uint8_t		*pbSrc,
	uint32_t		dSrcByteLen,
	uint8_t		*pbDst,
	uint32_t		dDstBitLen)
{
	uint8_t		Count = 0, DigestValue[SHA224_DIGEST_VALUELEN] = { 0, };
	uint32_t		i;
	uint32_t tempLen = dSrcByteLen;
	uint8_t *tempSrc = (uint8_t *)malloc((uint8_t)tempLen);

	for (i = 0; i < dSrcByteLen; i++)
		tempSrc[i] = pbSrc[i];

	i = ((dDstBitLen + 7) & 0xFFFFFFF8) / 8;

	for (Count = 0;; Count++) {
		KCDSA_SHA224_Init(SHA224_AlgInfo);
		KCDSA_SHA224_Update(SHA224_AlgInfo, tempSrc, tempLen);
		KCDSA_SHA224_Update(SHA224_AlgInfo, &Count, 1);
		KCDSA_SHA224_Final(SHA224_AlgInfo, DigestValue);

		if (i >= SHA224_DIGEST_VALUELEN) {
			i -= SHA224_DIGEST_VALUELEN;
			memcpy(pbDst + i, DigestValue, SHA224_DIGEST_VALUELEN);
			if (i == 0)	break;
		}
		else {
			memcpy(pbDst, DigestValue + SHA224_DIGEST_VALUELEN - i, i);
			break;
		}
	}

	i = dDstBitLen & 0x07;
	if (i)
		pbDst[0] &= ((uint32_t)1 << i) - 1;

	free(tempSrc);

	return CTR_SUCCESS;
}

uint32_t KCDSA_PRNG_SHA_256(
	SHA256_ALG_INFO	*SHA256_AlgInfo,
	uint8_t		*pbSrc,
	uint32_t		dSrcByteLen,
	uint8_t		*pbDst,
	uint32_t		dDstBitLen)
{
	uint8_t		Count = 0, DigestValue[SHA256_DIGEST_VALUELEN] = { 0, };
	uint32_t		i = 0;
	uint32_t tempLen = dSrcByteLen;
	uint8_t *tempSrc = (uint8_t *)malloc((uint8_t)tempLen);

	for (i = 0; i < dSrcByteLen; i++)
		tempSrc[i] = pbSrc[i];

	i = ((dDstBitLen + 7) & 0xFFFFFFF8) / 8;

	for (Count = 0;; Count++) {
		KCDSA_SHA256_Init(SHA256_AlgInfo);
		KCDSA_SHA256_Update(SHA256_AlgInfo, tempSrc, tempLen);
		KCDSA_SHA256_Update(SHA256_AlgInfo, &Count, 1);
		KCDSA_SHA256_Final(SHA256_AlgInfo, DigestValue);

		if (i >= SHA256_DIGEST_VALUELEN) {
			i -= SHA256_DIGEST_VALUELEN;
			memcpy(pbDst + i, DigestValue, SHA256_DIGEST_VALUELEN);
			if (i == 0)	break;
		}
		else {
			memcpy(pbDst, DigestValue + SHA256_DIGEST_VALUELEN - i, i);
			break;
		}
	}

	i = dDstBitLen & 0x07;
	if (i)
		pbDst[0] &= (1 << i) - 1;

	free(tempSrc);

	return CTR_SUCCESS;
}

uint32_t Generate_Random(
	BIGNUM	*XKEY,
	uint8_t *pbSrc,
	uint32_t dSrcByteLen,
	uint32_t *X,
	uint32_t XBitLen,
	KISA_KCDSA	*kcdsa,
	uint32_t HASH)
{
	int32_t i, j;
	uint32_t ret;
	uint8_t *bzTmp1, *bzTmp2;

	BIGNUM *VAL = NULL, *BN_Tmp1 = NULL;
	SHA224_ALG_INFO	SHA224_AlgInfo;
	SHA256_ALG_INFO	SHA256_AlgInfo;

	if (HASH == SHA224)
	{
		bzTmp1 = (uint8_t *)malloc(224 / 8 + 1);
		bzTmp2 = (uint8_t *)malloc(224 / 8 + 1);
		for (j = 0; j < SHA224_DIGEST_VALUELEN / 4; j++)
			SHA224_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j < 4; j++)
			SHA224_AlgInfo.Count[j] = 0;
		for (j = 0; j < SHA224_DIGEST_BLOCKLEN; j++)
			SHA224_AlgInfo.Buffer[j] = 0;
	}
	else if (HASH == SHA256)
	{
		bzTmp1 = (uint8_t *)malloc(256 / 8 + 1);
		bzTmp2 = (uint8_t *)malloc(256 / 8 + 1);
		for (j = 0; j < SHA256_DIGEST_VALUELEN / 4; j++)
			SHA256_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j < 4; j++)
			SHA256_AlgInfo.Count[j] = 0;
		for (j = 0; j < SHA256_DIGEST_BLOCKLEN; j++)
			SHA256_AlgInfo.Buffer[j] = 0;
	}
	else {
		ret = CTR_INVALID_ALG_PARAMS;
		GOTO_END;
	}

	if ((VAL = CreateBigNum(XBitLen / 32)) == NULL)			goto LABEL_END;
	if ((BN_Tmp1 = CreateBigNum(XBitLen / 32)) == NULL)			goto LABEL_END;

	for (j = 0; j < (int32_t)(XBitLen / 32 + 1); j++)
	{
		VAL->pData[j] = 0;
		BN_Tmp1->pData[j] = 0;
	}

	if (HASH == SHA224)
	{
		ret = KCDSA_PRNG_SHA_224(&SHA224_AlgInfo, pbSrc, dSrcByteLen, bzTmp1, XBitLen);			GOTO_END;
	}
	else if (HASH == SHA256)
	{
		ret = KCDSA_PRNG_SHA_256(&SHA256_AlgInfo, pbSrc, dSrcByteLen, bzTmp1, XBitLen);			GOTO_END;
	}

	ret = OS2BN(bzTmp1, XBitLen / 8, BN_Tmp1);	GOTO_END;

	ret = BN_Add(VAL, XKEY, BN_Tmp1);	GOTO_END;

	if (VAL->pData[XBitLen / 32] != 0)
	{
		VAL->pData[XBitLen / 32] = 0;
		VAL->Length -= 1;
		VAL->Space -= 1;
	}

	ret = BN2OS(VAL, VAL->Length * 4, bzTmp1);									GOTO_END;

	if (HASH == SHA224)
	{
		ret = KCDSA_PRNG_SHA_224(&SHA224_AlgInfo, bzTmp1, VAL->Length * 4, bzTmp2, XBitLen);			GOTO_END;
	}
	else
	{
		ret = KCDSA_PRNG_SHA_256(&SHA256_AlgInfo, bzTmp1, VAL->Length * 4, bzTmp2, XBitLen);			GOTO_END;
	}

	ret = OS2BN(bzTmp2, XBitLen / 8, BN_Tmp1);	GOTO_END;

	while (BN_Cmp(BN_Tmp1, kcdsa->KCDSA_Q) >= 0) {
		ret = BN_Sub(BN_Tmp1, BN_Tmp1, kcdsa->KCDSA_Q);			GOTO_END;
	}

	for (i = 0; i < (int32_t)(XBitLen / 32); i++)
		X[i] = BN_Tmp1->pData[i];

	free(bzTmp1);
	free(bzTmp2);

LABEL_END:
	if (VAL != NULL)		DestroyBigNum(VAL);
	if (BN_Tmp1 != NULL)	DestroyBigNum(BN_Tmp1);
	return ret;
}

uint32_t KISA_KCDSA_GenerateKeyPair(
	KISA_KCDSA		*KCDSA_Key,
	uint8_t		*pbSrc,
	uint32_t			dSrcByteLen,
	uint32_t			qLen,
	uint32_t			HASH)
{
	int32_t j = 0;
	uint32_t		i = 0;
	uint32_t		ret;
	BIGNUM		*BN_Tmp1 = NULL;
	BIGNUM		*XKEY = NULL;

	if (KCDSA_Key == NULL)		return CTR_INVALID_POINTER;

	i = KCDSA_Key->KCDSA_P->Length;
	if ((BN_Tmp1 = CreateBigNum(i + 1)) == NULL)	goto LABEL_END;
	if ((XKEY = CreateBigNum(qLen / 32)) == NULL)	goto LABEL_END;

	if (KCDSA_Key->KCDSA_x->Length == 0) {
		// b ��Ʈ�� ������ ���� XKEY ����
		srand((unsigned)time(NULL));
		ret = BN_Rand(XKEY, qLen);			GOTO_END;

		/*p = 2048, q = 224, SHA-224 test vector*/
		/*XKEY->pData[0] = 0xa89150be;
		XKEY->pData[1] = 0xeff64b4c;
		XKEY->pData[2] = 0x4b90ffdf;
		XKEY->pData[3] = 0x046d5de1;
		XKEY->pData[4] = 0xd61495ea;
		XKEY->pData[5] = 0x20d9ba54;
		XKEY->pData[6] = 0xf910456a;
		XKEY->Length = 7;
		XKEY->Space = 8;*/

		/*p = 2048, q = 224, SHA-256 test vector*/
		/*XKEY->pData[0] = 0xa89150be;
		XKEY->pData[1] = 0xeff64b4c;
		XKEY->pData[2] = 0x4b90ffdf;
		XKEY->pData[3] = 0x046d5de1;
		XKEY->pData[4] = 0xd61495ea;
		XKEY->pData[5] = 0x20d9ba54;
		XKEY->pData[6] = 0xf910456a;
		XKEY->Length = 7;
		XKEY->Space = 8;*/

		/*p = 2048, q = 256, SHA-256 test vector*/
		/*XKEY->pData[0] = 0xb948da94;
		XKEY->pData[1] = 0xc0a936e2;
		XKEY->pData[2] = 0x2e97da0b;
		XKEY->pData[3] = 0x8b904cf1;
		XKEY->pData[4] = 0x3bf2ab78;
		XKEY->pData[5] = 0x587274d3;
		XKEY->pData[6] = 0xa667cf10;
		XKEY->pData[7] = 0xf0f30814;
		XKEY->Length = 8;
		XKEY->Space = 9;*/

		/*p = 3072, q = 256, SHA-256 test vector*/
		/*XKEY->pData[0] = 0x14dfce52;
		XKEY->pData[1] = 0xf1a369ab;
		XKEY->pData[2] = 0xfa2bb0cd;
		XKEY->pData[3] = 0xc3ca4c8e;
		XKEY->pData[4] = 0xc40e94d7;
		XKEY->pData[5] = 0x47c7ac5b;
		XKEY->pData[6] = 0xd9e9230d;
		XKEY->pData[7] = 0x80f96d39;
		XKEY->Length = 8;
		XKEY->Space = 9;*/

		// ���ڼ���Ű x ����
		ret = Generate_Random(XKEY, pbSrc, dSrcByteLen, KCDSA_Key->KCDSA_x->pData, qLen, KCDSA_Key, HASH);
		KCDSA_Key->KCDSA_x->Length = qLen / 32;
	}

	// x�� ���� ����
	ret = BN_ModInv(BN_Tmp1, KCDSA_Key->KCDSA_x, KCDSA_Key->KCDSA_Q);	GOTO_END;

	// ���ڼ��� ����Ű y ����(Y = G^{X^{-1} mod Q} mod P)
	ret = BN_ModExp(KCDSA_Key->KCDSA_y, KCDSA_Key->KCDSA_G, BN_Tmp1, KCDSA_Key->KCDSA_P);				GOTO_END;

LABEL_END:
	if (BN_Tmp1 != NULL)		DestroyBigNum(BN_Tmp1);
	if (XKEY != NULL)			DestroyBigNum(XKEY);
	return ret;
}

uint32_t KISA_KCDSA_sign(
	KISA_KCDSA			*kcdsa, 
	uint8_t		*MsgDigest, 
	uint32_t		MsgDigestLen,
	uint8_t		*Signature,
	uint32_t		*SignLen,
	uint32_t		HASH,
	uint8_t		*t_omgri,
	uint32_t		omgri_len)
{
	uint8_t	bzTmp[3072 / 8] = { 0, };
	uint8_t	bzTmp1[64];
	uint8_t	*hashTmp;
	uint32_t	i = 0, j = 0, qByteLen = 0, DigestLen = 0;
	uint32_t	ret;
	BIGNUM		*BN_K = NULL, *BN_Tmp1 = NULL, *KCDSA_s = NULL;
	BIGNUM		*KKEY = NULL;
	SHA224_ALG_INFO	SHA224_AlgInfo;
	SHA256_ALG_INFO	SHA256_AlgInfo;

	if (HASH == SHA224)
	{
		for (j = 0; j<SHA256_DIGEST_VALUELEN / 4; j++)
			SHA224_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j<4; j++)
			SHA224_AlgInfo.Count[j] = 0;
		for (j = 0; j<SHA224_DIGEST_BLOCKLEN; j++)
			SHA224_AlgInfo.Buffer[j] = 0;
	}
	else if (HASH == SHA256)
	{
		for (j = 0; j<SHA256_DIGEST_VALUELEN / 4; j++)
			SHA256_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j<4; j++)
			SHA256_AlgInfo.Count[j] = 0;
		for (j = 0; j<SHA256_DIGEST_BLOCKLEN; j++)
			SHA256_AlgInfo.Buffer[j] = 0;
	}
	else {
		ret = CTR_INVALID_ALG_PARAMS;
		GOTO_END;
	}

	if (kcdsa == NULL)	return CTR_INVALID_POINTER;

	if (HASH == SHA224)
		DigestLen = SHA224_DIGEST_VALUELEN;
	else if (HASH == SHA256)
		DigestLen = SHA256_DIGEST_VALUELEN;

	qByteLen = (uint32_t)(DIGITSIZE) * (kcdsa->KCDSA_Q->Length); // Q�� ����Ʈ���� ���̸� ���� 28

	if (SignLen != NULL) { // ������ ���̰� �Է��� �ִ°��
		i = *SignLen;
		if((HASH == SHA224 && qByteLen == 28) || (HASH == SHA256 && qByteLen == 32))
			*SignLen = DigestLen + qByteLen;
		else
			*SignLen = qByteLen + qByteLen;
		ret = CTR_BUFFER_TOO_SMALL;
		if ((i != 0) && (i<*SignLen))			goto LABEL_END;
	}
	if (Signature == NULL)	return CTR_INVALID_POINTER; // ������ NULL�̸� ����

	if (MsgDigest == NULL)	return CTR_INVALID_POINTER; // MsgDigest�� NULL�̸� ����

	ret = CTR_MEMORY_ALLOC_ERROR;
	// P�� ũ�⸸ŭ  K�� Tmp1�� ���� �Ҵ�
	i = kcdsa->KCDSA_P->Length;
	if ((BN_K = CreateBigNum(i + 1)) == NULL)		goto LABEL_END;
	if ((BN_Tmp1 = CreateBigNum(i + 1)) == NULL)	goto LABEL_END;
	// Q�� ũ�⸸ŭ S�� ���� �Ҵ�
	i = kcdsa->KCDSA_Q->Length;
	if ((KCDSA_s = CreateBigNum(i + 1)) == NULL)	goto LABEL_END;
	if ((KKEY = CreateBigNum(i + 1)) == NULL)		goto LABEL_END;

	// step 1. ���� k�� [1, Q-1]���� ���Ƿ� �����Ѵ�.
	srand((unsigned)time(NULL));
	ret = BN_Rand(KKEY, 8 * qByteLen);	GOTO_END;
	
	/*p = 2048, q = 224, SHA-224 test vector*/
	/*KKEY->pData[0] = 0xc1fb7222;
	KKEY->pData[1] = 0x71382b7d;
	KKEY->pData[2] = 0xd33ad7fb;
	KKEY->pData[3] = 0x04ac91d7;
	KKEY->pData[4] = 0x74f4f9db;
	KKEY->pData[5] = 0xd5ee4a09;
	KKEY->pData[6] = 0xb7b75e77;
	KKEY->Length = 7;
	KKEY->Space = 8;*/

	/*p = 2048, q = 224, SHA-256 test vector*/
	/*KKEY->pData[0] = 0xc1fb7222;
	KKEY->pData[1] = 0x71382b7d;
	KKEY->pData[2] = 0xd33ad7fb;
	KKEY->pData[3] = 0x04ac91d7;
	KKEY->pData[4] = 0x74f4f9db;
	KKEY->pData[5] = 0xd5ee4a09;
	KKEY->pData[6] = 0xb7b75e77;
	KKEY->Length = 7;
	KKEY->Space = 8;*/

	/*p = 2048, q = 256, SHA-256 test vector*/
	/*KKEY->pData[0] = 0x9475cf69;
	KKEY->pData[1] = 0x3d053f8a;
	KKEY->pData[2] = 0x9f55d297;
	KKEY->pData[3] = 0xb5ef2d93;
	KKEY->pData[4] = 0x59536696;
	KKEY->pData[5] = 0x4b2a759e;
	KKEY->pData[6] = 0xf737ace8;
	KKEY->pData[7] = 0xb2425ced;
	KKEY->Length = 8;
	KKEY->Space = 9;*/

	/*p = 3072, q = 256, SHA-256 test vector*/
	/*KKEY->pData[0] = 0x80804468;
	KKEY->pData[1] = 0x8dad0082;
	KKEY->pData[2] = 0x726b22c0;
	KKEY->pData[3] = 0x1acaa16c;
	KKEY->pData[4] = 0xe4f6028e;
	KKEY->pData[5] = 0x0383e4e9;
	KKEY->pData[6] = 0xc87ae1f6;
	KKEY->pData[7] = 0xa3d070cb;
	KKEY->Length = 8;
	KKEY->Space = 9;*/

	ret = Generate_Random(KKEY, t_omgri, omgri_len, BN_K->pData, kcdsa->KCDSA_Q->Length * 32, kcdsa, HASH);
	BN_K->Length = kcdsa->KCDSA_Q->Length;
	BN_K->Space = kcdsa->KCDSA_Q->Length + 1;

	// Q�� K�� ���̴� ����.
	// Q�� K�� ũ�⸦ ���ؼ� K�� �� ũ�� Q�� ���ִ� ������ ��.
	if (BN_Cmp(BN_K, kcdsa->KCDSA_Q) >= 0) {
		ret = BN_Sub(BN_K, BN_K, kcdsa->KCDSA_Q);						GOTO_END;
	}

	// step 2. W=G^K mod P�� ����Ѵ�.
	ret = BN_ModExp(BN_Tmp1, kcdsa->KCDSA_G, BN_K, kcdsa->KCDSA_P);		GOTO_END;

	//	step 3. ������ ù �κ� R=h(W)�� ����Ѵ�.
	i = DIGITSIZE * kcdsa->KCDSA_P->Length; // ����Ʈ ���� ����
	ret = BN2OS(BN_Tmp1, i, bzTmp);									GOTO_END;
	j = i;
	if (HASH == SHA224)
	{
		KCDSA_SHA224_Init(&SHA224_AlgInfo);
		KCDSA_SHA224_Update(&SHA224_AlgInfo, bzTmp, j);
		KCDSA_SHA224_Final(&SHA224_AlgInfo, bzTmp);
		memcpy(Signature, bzTmp, SHA224_DIGEST_VALUELEN);
	}
	else if (HASH == SHA256)
	{
		KCDSA_SHA256_Init(&SHA256_AlgInfo);
		KCDSA_SHA256_Update(&SHA256_AlgInfo, bzTmp, j);
		KCDSA_SHA256_Final(&SHA256_AlgInfo, bzTmp);
		if(qByteLen == 28)
			memcpy(Signature, bzTmp+4, SHA224_DIGEST_VALUELEN);
		else
			memcpy(Signature, bzTmp, SHA256_DIGEST_VALUELEN);
	}

	// step 4. Z = Y mod 2^l
	// step 5. h = Hash(Z||M)�� ����Ѵ�.
	hashTmp = (uint8_t*)malloc((64 + MsgDigestLen) * sizeof(uint8_t));
	i = kcdsa->KCDSA_y->Length;
	kcdsa->KCDSA_y->Length = 512 / BitsInDIGIT;
	ret = BN2OS(kcdsa->KCDSA_y, 512 / 8, bzTmp1);		GOTO_END;
	kcdsa->KCDSA_y->Length = i;
	memcpy(hashTmp, bzTmp1, 64);
	memcpy(hashTmp + 64, MsgDigest, MsgDigestLen);

	if (HASH == SHA224)
	{
		for (j = 0; j<SHA256_DIGEST_VALUELEN / 4; j++)
			SHA224_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j<4; j++)
			SHA224_AlgInfo.Count[j] = 0;
		for (j = 0; j<SHA224_DIGEST_BLOCKLEN; j++)
			SHA224_AlgInfo.Buffer[j] = 0;
		KCDSA_SHA224_Init(&SHA224_AlgInfo);
		KCDSA_SHA224_Update(&SHA224_AlgInfo, hashTmp, 64 + MsgDigestLen);
		KCDSA_SHA224_Final(&SHA224_AlgInfo, hashTmp);
	}
	else if (HASH == SHA256)
	{
		for (j = 0; j<SHA256_DIGEST_VALUELEN / 4; j++)
			SHA256_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j<4; j++)
			SHA256_AlgInfo.Count[j] = 0;
		for (j = 0; j<SHA256_DIGEST_BLOCKLEN; j++)
			SHA256_AlgInfo.Buffer[j] = 0;
		KCDSA_SHA256_Init(&SHA256_AlgInfo);
		KCDSA_SHA256_Update(&SHA256_AlgInfo, hashTmp, 64 + MsgDigestLen);
		KCDSA_SHA256_Final(&SHA256_AlgInfo, hashTmp);
	}

	// step 6. E = (R ^ H) mod Q�� ����Ѵ�.
	if ((HASH == SHA224 && qByteLen == 28) || (HASH == SHA256 && qByteLen == 32))
		for (i = 0; i < DigestLen; i++)	bzTmp[i] ^= hashTmp[i];
	else
	{
		for (i = 0; i < qByteLen; i++) bzTmp[i + 4] ^= hashTmp[i + 4];
		for (i = 0; i < qByteLen; i++) bzTmp[i] = bzTmp[i + 4];
	}


	free(hashTmp);

	ret = OS2BN(bzTmp, i, BN_Tmp1);							GOTO_END;
	ret = BN_ModRed(BN_Tmp1, BN_Tmp1, kcdsa->KCDSA_Q);		GOTO_END;
	
	//	step 7. S = X(K-E) mod Q�� ����Ѵ�.
	ret = BN_ModSub(BN_K, BN_K, BN_Tmp1, kcdsa->KCDSA_Q);			GOTO_END;
	ret = BN_ModMul(KCDSA_s, kcdsa->KCDSA_x, BN_K, kcdsa->KCDSA_Q);	GOTO_END;

	ret = BN2OS(KCDSA_s, qByteLen, Signature + qByteLen);			GOTO_END;
	
	ret = CTR_SUCCESS;
LABEL_END:
	if (BN_K != NULL)		DestroyBigNum(BN_K);
	if (BN_Tmp1 != NULL)	DestroyBigNum(BN_Tmp1);
	if (KCDSA_s != NULL)	DestroyBigNum(KCDSA_s);
	if (KKEY != NULL)		DestroyBigNum(KKEY);
	return ret;
}

uint32_t KISA_KCDSA_verify(
	KISA_KCDSA			*kcdsa,
	uint8_t		*MsgDigest,
	uint32_t		MsgDigestLen,
	uint8_t		*Signature,
	uint32_t		SignLen,
	uint32_t		HASH)
{
	uint8_t	bzTmp[3072 / 8] = { 0, };
	uint8_t	bzTmp1[64];
	uint8_t	*hashTmp;
	uint32_t	i = 0, j = 0, qByteLen = 0, DigestLen = 0;
	uint32_t	ret;
	BIGNUM		*BN_Tmp1 = NULL, *BN_Tmp2 = NULL, *BN_Tmp3 = NULL, *KCDSA_s = NULL;
	SHA224_ALG_INFO	SHA224_AlgInfo;
	SHA256_ALG_INFO	SHA256_AlgInfo;

	if (HASH == SHA224)
	{
		for (j = 0; j<SHA256_DIGEST_VALUELEN / 4; j++)
			SHA224_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j<4; j++)
			SHA224_AlgInfo.Count[j] = 0;
		for (j = 0; j<SHA224_DIGEST_BLOCKLEN; j++)
			SHA224_AlgInfo.Buffer[j] = 0;
	}
	else if (HASH == SHA256)
	{
		for (j = 0; j<SHA256_DIGEST_VALUELEN / 4; j++)
			SHA256_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j<4; j++)
			SHA256_AlgInfo.Count[j] = 0;
		for (j = 0; j<SHA256_DIGEST_BLOCKLEN; j++)
			SHA256_AlgInfo.Buffer[j] = 0;
	}
	else {
		ret = CTR_INVALID_ALG_PARAMS;
		GOTO_END;
	}

	if ((kcdsa == NULL) || (MsgDigest == NULL) || (Signature == NULL))
		return CTR_INVALID_POINTER;

	if (HASH == SHA224)
		DigestLen = SHA224_DIGEST_VALUELEN;
	else if (HASH == SHA256)
		DigestLen = SHA256_DIGEST_VALUELEN;

	qByteLen = DIGITSIZE * kcdsa->KCDSA_Q->Length;

	if ((SignLen != DigestLen + qByteLen) && (SignLen != qByteLen + qByteLen))	return CTR_INVALID_SIGNATURE_LEN;

	ret = CTR_MEMORY_ALLOC_ERROR;
	i = kcdsa->KCDSA_P->Length;
	if ((BN_Tmp1 = CreateBigNum(i)) == NULL)	goto LABEL_END;
	if ((BN_Tmp2 = CreateBigNum(i)) == NULL)	goto LABEL_END;
	if ((BN_Tmp3 = CreateBigNum(i)) == NULL)	goto LABEL_END;
	i = kcdsa->KCDSA_Q->Length;
	if ((KCDSA_s = CreateBigNum(i)) == NULL)	goto LABEL_END;


	//KCDSA_s == Signature �� �ϱ�




	memcpy(bzTmp, Signature, qByteLen);
	ret = OS2BN(Signature + qByteLen, qByteLen, KCDSA_s);				GOTO_END;

	ret = CTR_VERIFY_FAIL;
	if (BN_Cmp(KCDSA_s, kcdsa->KCDSA_G) >= 0)			goto LABEL_END;

	// step 1. ���ŵ� ���� {R', S'}�� ���� |R'|=LH, 0 < S' < Q ���� Ȯ���Ѵ�.
	// step 2. Z = Y mod 2^l
	// step 3. h = Hash(Z||M)�� ����Ѵ�.
	hashTmp = (uint8_t*)malloc((64 + MsgDigestLen) * sizeof(uint8_t));
	i = kcdsa->KCDSA_y->Length;
	kcdsa->KCDSA_y->Length = 512 / BitsInDIGIT;
	ret = BN2OS(kcdsa->KCDSA_y, 512 / 8, bzTmp1);		GOTO_END;
	kcdsa->KCDSA_y->Length = i;
	memcpy(hashTmp, bzTmp1, 64);
	memcpy(hashTmp + 64, MsgDigest, MsgDigestLen);

	if (HASH == SHA224)
	{
		KCDSA_SHA224_Init(&SHA224_AlgInfo);
		KCDSA_SHA224_Update(&SHA224_AlgInfo, hashTmp, 64 + MsgDigestLen);
		KCDSA_SHA224_Final(&SHA224_AlgInfo, hashTmp);
	}
	else if (HASH == SHA256)
	{
		KCDSA_SHA256_Init(&SHA256_AlgInfo);
		KCDSA_SHA256_Update(&SHA256_AlgInfo, hashTmp, 64 + MsgDigestLen);
		KCDSA_SHA256_Final(&SHA256_AlgInfo, hashTmp);
	}

	// step 4. E' = (R' ^ H') mod Q�� ����Ѵ�.

	if ((HASH == SHA224 && qByteLen == 28) || (HASH == SHA256 && qByteLen == 32))
		for (i = 0; i < DigestLen; i++)	bzTmp[i] ^= hashTmp[i];
	else
		for (i = 0; i < qByteLen; i++) bzTmp[i] ^= hashTmp[i + 4];

	free(hashTmp);

	ret = OS2BN(bzTmp, i, BN_Tmp1);							GOTO_END;
	ret = BN_ModRed(BN_Tmp1, BN_Tmp1, kcdsa->KCDSA_Q);		GOTO_END;

	// step 5. W' = Y ^ {S'} G ^ {E'} mod P�� ����Ѵ�.
	ret = BN_ModExp(BN_Tmp2, kcdsa->KCDSA_y, KCDSA_s, kcdsa->KCDSA_P);				GOTO_END;
	ret = BN_ModExp(BN_Tmp3, kcdsa->KCDSA_G, BN_Tmp1, kcdsa->KCDSA_P);				GOTO_END;
	ret = BN_ModMul(BN_Tmp1, BN_Tmp2, BN_Tmp3, kcdsa->KCDSA_P);						GOTO_END;

	// step 6. h(W') = R'�� �����ϴ��� Ȯ���Ѵ�.
	i = DIGITSIZE * kcdsa->KCDSA_P->Length;
	ret = BN2OS(BN_Tmp1, i, bzTmp);							GOTO_END; //bzTmp �� �ٲ�
	j = i;
	i = 0;
	//printf("\nsign121212\n\n");			//���� ���� �ٸ� ������ �ٲ�
	//for (int ii = 0; ii < SHA224_DIGEST_VALUELEN; ii++)
	//		printf("%02X %02X %02X: %s\n", bzTmp[ii], Signature[ii], BN_Tmp1->pData[ii], (bzTmp[ii] == Signature[ii]) ? "true" : "false");
	if (HASH == SHA224)
	{
		KCDSA_SHA224_Init(&SHA224_AlgInfo);	
		KCDSA_SHA224_Update(&SHA224_AlgInfo, bzTmp, j);
		KCDSA_SHA224_Final(&SHA224_AlgInfo, bzTmp); //bzTmp�� �ٲ�

		ret = CTR_VERIFY_FAIL;

		/*printf("\nsign1233333333333\n\n");		
		for(int ii = 0; ii < SHA224_DIGEST_VALUELEN; ii++)
				printf("%02X %02X: %s\n", bzTmp[ii], Signature[ii], (bzTmp[ii] == Signature[ii])? "true" : "false");*/
		if (memcmp(bzTmp, Signature, SHA224_DIGEST_VALUELEN) != 0)	//LABEL_END error
			goto LABEL_END;
	}
	else if (HASH == SHA256)
	{
		KCDSA_SHA256_Init(&SHA256_AlgInfo);
		KCDSA_SHA256_Update(&SHA256_AlgInfo, bzTmp, j);
		KCDSA_SHA256_Final(&SHA256_AlgInfo, bzTmp);

		ret = CTR_VERIFY_FAIL;

		if (qByteLen == 28)
		{
			if (memcmp(bzTmp + 4, Signature, qByteLen) != 0)
				goto LABEL_END;
		}
		else
		{
			if (memcmp(bzTmp, Signature, SHA256_DIGEST_VALUELEN) != 0)
				goto LABEL_END;
		}
	}

	ret = CTR_SUCCESS;
LABEL_END:
	if (BN_Tmp1 != NULL)	DestroyBigNum(BN_Tmp1);
	if (BN_Tmp2 != NULL)	DestroyBigNum(BN_Tmp2);
	if (BN_Tmp3 != NULL)	DestroyBigNum(BN_Tmp3);
	if (KCDSA_s != NULL)	DestroyBigNum(KCDSA_s);
	return ret;
}

uint32_t KISA_KCDSA_set_params(KISA_KCDSA *kcdsa, uint32_t *p, int32_t plen, uint32_t *q, int32_t qlen, uint32_t *g, int32_t glen, uint32_t *private_key, int32_t private_keylen, uint32_t *public_key, int32_t public_keylen)
{
	int32_t i;

	for (i = 0; i < plen; i++) {
		kcdsa->KCDSA_P->pData[i] = p[i];
	}
	kcdsa->KCDSA_P->Length = plen;
	kcdsa->KCDSA_P->Space = plen + 1;

	for (i = 0; i < qlen; i++) {
		kcdsa->KCDSA_Q->pData[i] = q[i];
	}
	kcdsa->KCDSA_Q->Length = qlen;
	kcdsa->KCDSA_Q->Space = qlen + 1;

	for (i = 0; i < glen; i++) {
		kcdsa->KCDSA_G->pData[i] = g[i];
	}
	kcdsa->KCDSA_G->Length = glen;
	kcdsa->KCDSA_G->Space = glen + 1;

	for (i = 0; i < private_keylen; i++) {
		kcdsa->KCDSA_x->pData[i] = private_key[i];
	}
	kcdsa->KCDSA_x->Length = private_keylen;
	kcdsa->KCDSA_x->Space = private_keylen + 1;

	for (i = 0; i < public_keylen; i++) {
		kcdsa->KCDSA_y->pData[i] = public_key[i];
	}
	kcdsa->KCDSA_y->Length = public_keylen;
	kcdsa->KCDSA_y->Space = public_keylen + 1;

	return 0;
}
uint32_t KISA_KCDSA_GenerateParameters(
	uint32_t	PrimeBits,
	uint32_t	SubPrimeBits,
	KISA_KCDSA		*kcdsa,
	uint32_t	HASH)
{
	int32_t j;
	uint8_t		bzTmp[3072 / 8 + 4] = { 0, };
	uint8_t		tSeed[256 / 8 + 4] = { 0x00, };
	/*p = 2048, q = 224, SHA-224 test vector*/
	//uint8_t		tSeed[256 / 8 + 4] = { 0xc0, 0x52, 0xa2, 0x76, 0x41, 0x00, 0xf0, 0xf4, 0xec, 0x90, 0x6b, 0x9c, 0x5c, 0x6b, 0x10, 0x6e, 0x34, 0x70, 0xdf, 0xc1, 0x36, 0x9f, 0x12, 0xc0, 0x62, 0xf8, 0x0e, 0xe9 };
	/*p = 2048, q = 224, SHA-256 test vector*/
	//uint8_t		tSeed[256 / 8 + 4] = { 0xe1, 0x75, 0xca, 0xd0, 0xea, 0xcb, 0x74, 0xdd, 0xb4, 0x5f, 0x15, 0xf1, 0xf2, 0x57, 0x22, 0xbf, 0x15, 0x56, 0xef, 0x86, 0x0a, 0x0f, 0xe0, 0x31, 0x71, 0x18, 0x44, 0x9b };
	/*p = 2048, q = 256, SHA-256 test vector*/
	//uint8_t		tSeed[256 / 8 + 4] = { 0xf7, 0x5a, 0xbd, 0xa0, 0x03, 0x2c, 0xe2, 0x18, 0xce, 0x04, 0xba, 0xf0, 0xa6, 0xdc, 0x92, 0xc8, 0x7e, 0xb4, 0x6a, 0xa0, 0x56, 0x8c, 0x42, 0x78, 0x2e, 0x64, 0x4c, 0xc2, 0xb8, 0x2e, 0x24, 0x9a };
	/*p = 3072, q = 256, SHA-256 test vector*/
	//uint8_t		tSeed[256 / 8 + 4] = { 0xb8, 0x56, 0x20, 0x16, 0x38, 0x55, 0xa7, 0xc0, 0x05, 0x76, 0x13, 0xdc, 0xd1, 0xf2, 0xae, 0x61, 0x80, 0xc4, 0x34, 0xd0, 0x98, 0x90, 0xea, 0x70, 0x22, 0x00, 0x83, 0xf2, 0x8d, 0x27, 0x54, 0xad };
	uint32_t		i = 0, Count = 0, k = 0;
	uint32_t		ret, temp_rand = 0;
	uint32_t		g[96];
	/*p = 2048, q = 224, SHA-224 test vector*/
	//uint32_t g[] = { 0x967500f2, 0x4ae06466, 0x8c2eb468, 0xc05a92f8, 0xc314fe16, 0x545cf834, 0x73320013, 0x2024bb80, 0xf8bb047b, 0x66e0db04, 0x629340c6, 0xecd4ec10, 0x046a12e8, 0x806cc64e, 0x59fc0842, 0xc01ad8a8, 0xb5c6285d, 0x3800b9a0, 0x586dd871, 0x9c8c85d0, 0x6e10c0da, 0xceb7b4fa, 0x5ffcb0a8, 0x80cf3ae4, 0x2f30525f, 0x6c1fb75c, 0x376a90e7, 0x6c1af700, 0xf858eca7, 0x12246fc6, 0xba7e782e, 0xf06dbc24, 0xc9888ea8, 0xc031eeba, 0x4568dc7d, 0x582d8950, 0xf9c038b1, 0x764675a0, 0xd85a401d, 0xc8fc9984, 0x462ceac2, 0x5263048e, 0x7354ace8, 0x40d3e2d0, 0x9be0a6db, 0x847b9b84, 0xfb5620bb, 0xc0b23380, 0xf89cd4fc, 0xbe143a82, 0x12609c96, 0x48998c38, 0x5d600a68, 0x00b51688, 0x31982079, 0xf088edf8, 0xca2c4805, 0x4a5e31a0, 0x581ea864, 0xf46c56c0, 0x1e008eaa, 0x9c0f5422, 0x87aca828, 0x8cd78a90 };
	/*p = 2048, q = 224, SHA-256 test vector*/
	//uint32_t g[] = { 0x967500f2, 0x4ae06466, 0x8c2eb468, 0xc05a92f8, 0xc314fe16, 0x545cf834, 0x73320013, 0x2024bb80, 0xf8bb047b, 0x66e0db04, 0x629340c6, 0xecd4ec10, 0x046a12e8, 0x806cc64e, 0x59fc0842, 0xc01ad8a8, 0xb5c6285d, 0x3800b9a0, 0x586dd871, 0x9c8c85d0, 0x6e10c0da, 0xceb7b4fa, 0x5ffcb0a8, 0x80cf3ae4, 0x2f30525f, 0x6c1fb75c, 0x376a90e7, 0x6c1af700, 0xf858eca7, 0x12246fc6, 0xba7e782e, 0xf06dbc24, 0xc9888ea8, 0xc031eeba, 0x4568dc7d, 0x582d8950, 0xf9c038b1, 0x764675a0, 0xd85a401d, 0xc8fc9984, 0x462ceac2, 0x5263048e, 0x7354ace8, 0x40d3e2d0, 0x9be0a6db, 0x847b9b84, 0xfb5620bb, 0xc0b23380, 0xf89cd4fc, 0xbe143a82, 0x12609c96, 0x48998c38, 0x5d600a68, 0x00b51688, 0x31982079, 0xf088edf8, 0xca2c4805, 0x4a5e31a0, 0x581ea864, 0xf46c56c0, 0x1e008eaa, 0x9c0f5422, 0x87aca828, 0x8cd78a90 };
	/*p = 2048, q = 256, SHA-256 test vector*/
	//uint32_t g[] = { 0xe8b24a24, 0xac387aec, 0x8aec08c2, 0x84c12e6c, 0xbe1cd1b6, 0x38463461, 0x6010f000, 0x104068c0, 0x7f5aac40, 0x7c9ff2d4, 0xf4a43f0a, 0x0654781e, 0xd86e34e8, 0xe64c5694, 0xcd8e849d, 0x747db63c, 0x185064a8, 0xd408d05c, 0x4c5b78d0, 0x8a941bd2, 0x1bc25c5b, 0x880abc78, 0x3c18123c, 0x29bc289a, 0x04f91eac, 0x2edc49e6, 0x10300019, 0xc028c020, 0x72e0c0d0, 0x37ca6c57, 0xbc97a2d4, 0x04e877fa, 0x9ea41876, 0xf846c4c8, 0xd68ce56e, 0x45be04d5, 0x308426fc, 0xe8703c38, 0x2c183074, 0x0cd3e504, 0x8594d375, 0x9a159c53, 0x286246d8, 0x76f8aa1e, 0x2147d472, 0x8431e7ec, 0x789cc107, 0xe8e0e010, 0x2040c840, 0x09042f30, 0xefb92ccf, 0xfc8fe9d4, 0xda48e662, 0x36d2b8ce, 0x942d4aa8, 0xebccdd2b, 0xbd2b840d, 0x101cac10, 0x3c901434, 0x8bc4908c, 0xcc4bad84, 0xcd948b7d, 0xb256284b, 0xcfbaee38 };
	/*p = 3072, q = 256, SHA-256 test vector*/
	//uint32_t g[] = { 0xf042e385, 0x287a8c5e, 0x0080c0c0, 0x50004040, 0x3f1d3060, 0x4725c321, 0x9cecba29, 0x2c686420, 0xbcf8f4b0, 0x2fc92360, 0x47e13b55, 0x844c536d, 0xf0e8a018, 0x448840b8, 0x2c5238de, 0xdf354b21, 0x92165a5e, 0x4c86cace, 0x16f82458, 0x0ef092f4, 0xb8e88aec, 0x38787838, 0xb8f8f8b8, 0x80275543, 0x882684a2, 0xa06c8caa, 0x76724894, 0xa502be3a, 0x7da7913b, 0x9af46913, 0x28b0f800, 0x88105860, 0xf1d71cc0, 0xd9bf65cb, 0x56a74db3, 0x488c9002, 0xb8fc00c4, 0x94063834, 0xcd6fd1f3, 0xc090c9eb, 0x00d060b0, 0x0a8040c0, 0xd200ee9c, 0x9ac8b664, 0xc24e9abb, 0x129eeaf6, 0xd00d3a46, 0xa8923ca6, 0x006a147e, 0x5c3810a8, 0xbc040cd4, 0x73e91f34, 0x16bcc7bd, 0xe4b80a30, 0x14e87cd0, 0x7302ac00, 0x2b5d4f01, 0xe31507b9, 0x60f04071, 0xa0308090, 0xbc7ac0d0, 0x7b6917be, 0xae31df4d, 0xbedab652, 0xc4101c62, 0x56d00a38, 0xee68a29c, 0x0ce44f79, 0x2c049cf4, 0xad24bc14, 0x88beb46a, 0x30665c12, 0x027498ba, 0x32c61a2e, 0x29eb4a5e, 0x5263e527, 0x0afcae20, 0x60806000, 0xd0806000, 0x3dbbf940, 0xc543817f, 0x38140907, 0x4824c01c, 0x5834d02c, 0xe923d05a, 0x81bbb56f, 0x3ca4cc07, 0xc860b894, 0x9a60d8f0, 0x02c84e94, 0xd5cb81fc, 0x062a0eb2, 0xf61afea2, 0x28aaec4c, 0xa0226466, 0xcafddcde };
	BIGNUM		*BN_Tmp1 = NULL, *BN_Tmp2 = NULL;
	SHA224_ALG_INFO	SHA224_AlgInfo;
	SHA256_ALG_INFO	SHA256_AlgInfo;
	BIGNUM *KCDSA_J = NULL;

	if (kcdsa == NULL)	return CTR_INVALID_POINTER;

	if ((PrimeBits < 2048) || (PrimeBits > 3072) || (PrimeBits % 256))
		return CTR_INVALID_ALG_PARAMS;
	if ((SubPrimeBits < 224) || (SubPrimeBits > 256) || (SubPrimeBits % 32))
		return CTR_INVALID_ALG_PARAMS;

	ret = CTR_MEMORY_ALLOC_ERROR;
	if ((BN_Tmp1 = CreateBigNum(PrimeBits / 32 + 1)) == NULL)			goto LABEL_END;
	if ((BN_Tmp2 = CreateBigNum(PrimeBits / 32 + 1)) == NULL)			goto LABEL_END;

	for (j = 0; j < (int32_t)(PrimeBits / 32 + 2); j++)
	{
		BN_Tmp1->pData[j] = 0;
		BN_Tmp2->pData[j] = 0;
	}

	if (HASH == SHA224)
	{
		for (j = 0; j < SHA256_DIGEST_VALUELEN / 4; j++)
			SHA224_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j < 4; j++)
			SHA224_AlgInfo.Count[j] = 0;
		for (j = 0; j < SHA224_DIGEST_BLOCKLEN; j++)
			SHA224_AlgInfo.Buffer[j] = 0;
	}
	else if (HASH == SHA256)
	{
		for (j = 0; j < SHA256_DIGEST_VALUELEN / 4; j++)
			SHA256_AlgInfo.ChainVar[j] = 0;
		for (j = 0; j < 4; j++)
			SHA256_AlgInfo.Count[j] = 0;
		for (j = 0; j < SHA256_DIGEST_BLOCKLEN; j++)
			SHA256_AlgInfo.Buffer[j] = 0;
	}
	else
	{
		ret = CTR_INVALID_ALG_PARAMS;
		GOTO_END;
	}

	// �Ҽ� �� (P, Q) ����
LABEL_Start:
	for (;;) {
		// Step 1. ��Ʈ ���� |Q|�� ����Ʈ �� Seed�� �����Ѵ�.
		srand((unsigned)time(NULL) * k++);
		for (j = 0; j < (signed)(SubPrimeBits / 8); j++) {
			tSeed[j] = rand();
		}

		kcdsa->SeedLen = SubPrimeBits / 8;

		// Step 2. Seed�� �Ϲ��� �Լ��� �Է����� �Ͽ� ��Ʈ ���̰� n = (|P|-|Q|-4)�� ���� U�� �����Ѵ�. (U �� PPGF(Seed, n))
		if (HASH == SHA224)
		{
			ret = KCDSA_PRNG_SHA_224(&SHA224_AlgInfo, tSeed, kcdsa->SeedLen, bzTmp, PrimeBits - SubPrimeBits - 4);			GOTO_END;
		}
		else
		{
			ret = KCDSA_PRNG_SHA_256(&SHA256_AlgInfo, tSeed, kcdsa->SeedLen, bzTmp, PrimeBits - SubPrimeBits - 4);			GOTO_END;
		}

		ret = OS2BN(bzTmp, (PrimeBits - SubPrimeBits) / 8, BN_Tmp1);	GOTO_END;

		// Step 3. U�� ������ 4 ��Ʈ '1000'�� ���̰� ������ ��Ʈ�� 1�� ����� �̸� J�� �д�.(J �� 2^|P|-|Q|-1 �� U �� 1)
		SetBitDIGIT(BN_Tmp1->pData, PrimeBits - SubPrimeBits - 1);
		SetBitDIGIT(BN_Tmp1->pData, 0);

		// Step 4. ���� �Ҽ� ���� �˰������� J�� �����Ͽ� �Ҽ��� �ƴϸ� Step 1�� �̵�
		if (MillerRabin(BN_Tmp1) != CTR_SUCCESS)
			goto LABEL_Start;
		break;
	}

	if ((KCDSA_J = CreateBigNum(PrimeBits / 32 + 1)) == NULL);	GOTO_END;

	for (j = BN_Tmp1->Length - 1; j >= 0; j--)
		KCDSA_J->pData[j] = BN_Tmp1->pData[j];
	KCDSA_J->Length = BN_Tmp1->Length;
	KCDSA_J->Space = BN_Tmp1->Space;

	// Step 5, 6. Count�� 0���� �ΰ� Count�� 1�� ������Ų��.
	for (Count = 1; Count < (1 << 24); Count++) {
		// Step 7. Count > 2^24�̸� �ܰ� 1�� �̵�
		if (Count == (1 << 24))	goto LABEL_Start;

		// Step 8. Seed�� Count�� ������ ���� �Ϲ��� �Լ� PPGF�� �Է����� �Ͽ� ��Ʈ ���̰� |Q|�� ���� U�� �����Ѵ�. (U �� PPGF(Seed||Count, |Q|))
		BIG_W2B(Count, (&tSeed[kcdsa->SeedLen]));
		tSeed[kcdsa->SeedLen] = 0;

		if (HASH == SHA224)	{
			ret = KCDSA_PRNG_SHA_224(&SHA224_AlgInfo, tSeed, kcdsa->SeedLen + 4, bzTmp, SubPrimeBits);			GOTO_END;
		} else {
			ret = KCDSA_PRNG_SHA_256(&SHA256_AlgInfo, tSeed, kcdsa->SeedLen + 4, bzTmp, SubPrimeBits);			GOTO_END;
		}

		ret = OS2BN(bzTmp, SubPrimeBits / 8, kcdsa->KCDSA_Q);	GOTO_END;

		// Step 9. U�� �ֻ��� �� ������ ��Ʈ�� 1�� ����� �̸� Q�� �д�. (Q �� 2^|Q|-1 �� U �� 1)
		SetBitDIGIT(kcdsa->KCDSA_Q->pData, SubPrimeBits - 1);
		SetBitDIGIT(kcdsa->KCDSA_Q->pData, 0);

		// Step 10. P �� (2J|Q| + 1)�� ��Ʈ ���̰� |P|���� ��� �ܰ� 6���� �̵�
		ret = BN_Mul(kcdsa->KCDSA_P, BN_Tmp1, kcdsa->KCDSA_Q);						GOTO_END;
		if (CheckBitDIGIT(kcdsa->KCDSA_P->pData, PrimeBits - 1))
			continue;
		ret = BN_SHL(kcdsa->KCDSA_P, kcdsa->KCDSA_P, 1);					GOTO_END;
		SetBitDIGIT(kcdsa->KCDSA_P->pData, 0);

		// Step 11. ���� �Ҽ� ���� �˰������� Q�� �����Ͽ� �Ҽ��� �ƴϸ� �ܰ� 6���� �̵�
		if (MillerRabin(kcdsa->KCDSA_Q) != CTR_SUCCESS)	continue;

		// Step 12. ���� �Ҽ� ���� �˰������� P�� �����Ͽ� �Ҽ��� �ƴϸ� �ܰ� 6���� �̵�
		if (MillerRabin(kcdsa->KCDSA_P) == CTR_SUCCESS)	break;
	}

	// Step 13. �Ҽ� P, Q, J�� ���� �� Seed, Count�� ����Ѵ�.
	kcdsa->Count = Count;

	ret = BN_SHL(KCDSA_J, KCDSA_J, 1);								GOTO_END;

	// ������ g ����
	for (;;) {
		// Step 1. p���� ���� ������ �� h�� �����Ѵ�.
		for (i = 0; i < PrimeBits / 8; i++)
			((uint8_t*)g)[i] = rand();

		for (i = 0; i < PrimeBits / 32; i++)
			BN_Tmp2->pData[i] = g[i];
		BN_Tmp2->Length = PrimeBits / 32;
		BN_Tmp2->Space = PrimeBits / 32 + 1;

		// Step 2. G �� h ^ 2J mod P�� ����Ѵ�
		ret = BN_ModExp(kcdsa->KCDSA_G, BN_Tmp2, KCDSA_J, kcdsa->KCDSA_P);						GOTO_END;

		// Step 3. G = 1�̸� �ܰ� 1�� ����.
		if (BN_Cmp(kcdsa->KCDSA_G, &BN_One) != 0)
			break;
	}
	ret = CTR_SUCCESS;
LABEL_END:
	if (BN_Tmp1 != NULL)	DestroyBigNum(BN_Tmp1);
	if (BN_Tmp2 != NULL)	DestroyBigNum(BN_Tmp2);
	if (KCDSA_J != NULL)	DestroyBigNum(KCDSA_J);
	return ret;
}

uint32_t KISA_KCDSA_DestroyObject(KISA_KCDSA	**kcdsa)
{
	KISA_KCDSA	*BN_Key = *kcdsa;

	if (kcdsa == NULL)	return CTR_INVALID_POINTER;
	if (*kcdsa == NULL)	return CTR_SUCCESS;

	if (BN_Key->KCDSA_P != NULL)		DestroyBigNum(BN_Key->KCDSA_P);
	if (BN_Key->KCDSA_Q != NULL)		DestroyBigNum(BN_Key->KCDSA_Q);
	if (BN_Key->KCDSA_G != NULL)		DestroyBigNum(BN_Key->KCDSA_G);
	if (BN_Key->KCDSA_x != NULL)		DestroyBigNum(BN_Key->KCDSA_x);
	if (BN_Key->KCDSA_y != NULL)		DestroyBigNum(BN_Key->KCDSA_y);

	memset((uint8_t *)BN_Key, 0, sizeof(KISA_KCDSA));
	free(BN_Key);
	*kcdsa = NULL;

	return CTR_SUCCESS;
}