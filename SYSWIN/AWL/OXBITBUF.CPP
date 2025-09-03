// ==========================================================================
// 						Class Implementation : COXBitBuffer
// ==========================================================================

// Source file :bitbuff.cpp

// Source : Periphere NV (Eddy Kestens)
// Creation Date : 	   2nd November 1995
// Last Modification : 2nd November 1995

// //////////////////////////////////////////////////////////////////////////

#include "oxbitbuf.h" // class specification
#include <iostream>
#include <fstream>

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:
	// LPBYTE m_pBuffer;
	// --- The buffer ow which bits are written or read

// private:

// Member functions ---------------------------------------------------------
// public:

COXBitBuffer::COXBitBuffer(LPBYTE pBuffer)
	{
	m_pBuffer = pBuffer;
 	m_ucMask = 0x80;
 	m_cByte = 0; 		// index gelezen byte
 	m_nRack = 0;
	}

COXBitBuffer::COXBitBuffer(LPBYTE pBuffer, int nInLength)
	{
	m_pBuffer = pBuffer;
 	m_nLastByte = nInLength;
 	m_ucMask = 0x80;
 	m_cByte = 0; 		// index gelezen byte
 	m_nRack = 0;
	}

void COXBitBuffer::OutputBit(int bit)
	{
    if ( bit )
    	m_nRack |= m_ucMask;

    m_ucMask >>= 1;
    if ( m_ucMask == 0 )
    	{
    	m_pBuffer[m_cByte] = (UCHAR)m_nRack;
		m_cByte++;
		m_nRack = 0;
		m_ucMask = 0x80;
    	}
	}

void COXBitBuffer::OutputBits(int code, int count)
	{
	int test = 0;
    unsigned long mask;
    mask = 1L << ( count - 1 );
    while ( mask != 0)
    	{
        if ( mask & code )
            m_nRack |= m_ucMask;
        m_ucMask >>= 1;
        if ( m_ucMask == 0 )
        	{
        	m_pBuffer[m_cByte] = (UCHAR)m_nRack;
	    	m_cByte++;
	    	m_nRack = 0;
            m_ucMask = 0x80;
        	}
        mask >>= 1;
	    }
    if (m_ucMask == 0x01)
    test++;
	}

void COXBitBuffer::OutputBitsEOS()
	{
	if ( m_ucMask == 0x80)
		return;
    while ( m_ucMask != 0)
    	{
	    m_ucMask >>= 1;
    	if ( m_ucMask == 0 )
    		{
    		m_pBuffer[m_cByte] = (UCHAR)m_nRack;
    		m_cByte++;
    		}
    	}
	}

int	COXBitBuffer::InputBit()
	{
    int value;
    if (m_ucMask == 0x80)
    	{
    	m_nRack = m_pBuffer[m_cByte];
        m_cByte++;
        }
    value = m_nRack & m_ucMask;
	m_ucMask >>= 1;
    if ( m_ucMask == 0 )
		m_ucMask = 0x80;
    return( value ? 1 : 0 );
	}

unsigned long COXBitBuffer::InputBits(int bit_count)
	{
    unsigned long mask;
    unsigned long return_value;
    mask = 1L << ( bit_count - 1 );
    return_value = 0;
    while ( mask != 0)
    	{
		if ( m_ucMask == 0x80 )
			if (m_cByte < m_nLastByte )
				{
				m_nRack = m_pBuffer[m_cByte];
	    		m_cByte++;
	    		}
			else
				return 0;
		if ( m_nRack & m_ucMask )
            return_value |= mask;
        mask >>= 1;
        m_ucMask >>= 1;
        if ( m_ucMask == 0 )
            if ( m_nLastByte == m_cByte )
            	return 0;
        	else
          		m_ucMask = 0x80;
    	}
    return( return_value );
	}

COXBitBuffer::~COXBitBuffer()
	{
	}

// protected:

// private:

// ==========================================================================
