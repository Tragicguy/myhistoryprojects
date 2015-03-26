#pragma once


class SeagleAES
{
public:
	SeagleAES(int keySize, byte* keyBytes);
	~SeagleAES(void);

private:
	int Nb;         // block size in 32-bit words.  Always 4 for AES.  (128 bits).
    int Nk;         // key size in 32-bit words.  4, 6, 8.  (128, 192, 256 bits).
    int Nr;         // number of rounds. 10, 12, 14.

    byte* key;     // the seed key. size will be 4 * keySize from ctor.
    
    byte** w;      // key schedule array. 
	short SizeofW;

    byte** State;  // State matrix
	short SizeofState;
	
private:
	void SetNbNkNr(int keySize);
	void KeyExpansion();

	byte**	AllocateArray(short iDemention0,short iDemention1);
    void	SetKeyExpansionValue(byte** KeySchedule,byte* Key,short row, short index1, short index2); 
	byte*	SubWord(byte word[]);
	byte*	RotWord(byte word[]);
	void	AddRoundKey(int round);
	void	SubBytes();
	void	InvSubBytes();
	void	ShiftRows();
	void	InvShiftRows();
	void	MixColumns();
	void	InvMixColumns();

	static byte gfmultby01(byte b);
	static byte gfmultby02(byte b);
	static byte gfmultby03(byte b);
	static byte gfmultby09(byte b);
	static byte gfmultby0b(byte b);
	static byte gfmultby0d(byte b);
	static byte gfmultby0e(byte b);
public:
	void	Cipher(byte* input, byte* output);  // encipher 16-bit input
	void	InvCipher(byte* input, byte* output);// decipher 16-bit input
};
