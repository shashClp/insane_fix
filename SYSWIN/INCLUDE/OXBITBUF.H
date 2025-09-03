// ==========================================================================
// 							Class Specification : COXBitBuffer
// ==========================================================================

// Header file : bitbuff.h

// Source : Periphere NV (Eddy Kestens)
// Creation Date : 	   2nd November 1995
// Last Modification : 2nd November 1995

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO  Needs a resource (template)

//	NO	Persistent objects (saveable on disk)
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :
//	This class allows bitwase access to a buffer of bytes

// Remark:
//

// Prerequisites (necessary conditions):
//

/////////////////////////////////////////////////////////////////////////////
#ifndef __BITBUFF_H__
#define __BITBUFF_H__

typedef unsigned char UCHAR;
typedef unsigned char* LPBYTE;

class COXBitBuffer
{
// Data members -------------------------------------------------------------
public:

protected:
	LPBYTE        m_pBuffer;
	unsigned char m_ucMask;
  long          m_cByte;
  int           m_nRack;
  long          m_nLastByte;

private:

// Member functions ---------------------------------------------------------
public:
	COXBitBuffer(LPBYTE pBuffer);
	// --- In  : pBuffer : The buffer from which the bits will be read or written
	// --- Out :
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

  COXBitBuffer(LPBYTE pBuffer, int nInLength );
  // --- In  : pBuffer : The buffer from which the bits will be read or written
	//         : InLength: sets the length of PBuffer to detect last byte
	// --- Out :
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

	void OutputBit(int bit);
	// --- In       : bit, bit to be written
	// --- Out      :
	// --- Returns  :
	// --- Effect   : writing a bit to the outputbuffer (compressing)


	void OutputBits(int code, int count);
	// --- In       : code    : sequence of bits being written
	//				  count   : number of bits being written
	// ---Out       :
	// ---Returns   :
	// ---Effect    : compressing count bits to the outputbuffer

	void OutputBitsEOS();
	// ---In        : code    : sequence of null bits to mark end of buffer
	//				: count   : END OF STREAM, 12 bits
	// ---Out		:
	// ---Returns   :
	// ---Effect    : terminating with EOS zero bits to mark end of buffer

	int InputBit();
    // ---In 		:
    // ---Out		:
    // ---Returns	: bit from buffer
    // ---Effect	: reading a bit from the input buffer for expanding

	unsigned long InputBits(int bit_count);
	// ---In		: bit_count : number of bit being read
	// ---Out		:
	// ---Returns   : multiple bits read from buffer
	// ---Effect	: reading multiple bits from the inputbuffer for expanding		:

  long GetCompressedLength() { return m_cByte; };
	// ---In		:
	// ---Out		:
	// ---Returns	: m_cByte : number of compressed bytes
	// ---Effect	: reach for protected member variable

	virtual ~COXBitBuffer();
	// --- In  :
	// --- Out :
	// --- Returns :
	// --- Effect : Destructor of object

protected:

private:

};

#endif
// ==========================================================================
