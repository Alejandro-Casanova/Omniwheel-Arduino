/*
 * array_utils.h
 *
 *  Created on: Nov 14, 2018
 *      Author: pasto
 */

#ifndef ARRAY_UTILS_H_
#define ARRAY_UTILS_H_

// copy B on A
inline void AUmap_uint8(uint8_t* A, uint8_t* B, uint8_t lenght)
{
  	for(int i = 0; i < lenght; i++)
		A[i] = B[i];
}
inline void AUmap_uint8_inv(uint8_t* A, uint8_t* B, uint8_t lenght)
{
  	for(int i = 0; i < lenght; i++)
		A[i] = B[lenght-i];
}
inline void AUmap_uint8uint32(int32_t* A, uint8_t* B, uint8_t lenght)
{
  	for(int i = 0; i < lenght; i++)
		A[i] = (int32_t)B[i];
}
void AUmap_charNum2Uint32(int32_t* A, uint8_t* B, uint8_t lenght)
{
  	for(int i = 0; i < lenght; i++)
		A[i] = (int32_t)B[i]-48;
}

// maps zeros to the arraay
inline void AUzero_uint8(uint8_t* A, uint8_t lenght)
{
	for(int i = 0; i < lenght; i++)
		A[i] = 0;
}
void AUsero_int16(int16_t* A, uint8_t lenght)
{
  	for(int i = 0; i < lenght; i++)
		A[i] = 0;
}




uint16_t AU_uint8_to_uint16(uint8_t A, uint8_t B)
{
	uint16_t t1, t2, t3;
	t1 = (uint16_t)A;
	t2 = (uint16_t)B;
	t3 = t2 + (t1<<8);
	return t3;
}

#endif /* ARRAY_UTILS_H_ */
