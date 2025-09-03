#ifndef _CVIDEO_H_
#define _CVIDEO_H_


//#define BYTE unsigned char

#define RAW_X 160
#define RAW_Y  50
#define COM_X  80
#define COM_Y  50
#define DEC_X 120
#define DEC_Y  50

extern long ZMRand();

class CVideo
{
  public:

    CVideo();
    ~CVideo();

    bool Load(char *filename);
	bool LoadPal(char *filename);
    char* RenderAlpha();
    void RenderRGB(unsigned long* dstRGB);
	void RenderRGBPal(unsigned long* dstRGB);
    int GetActFrame() {return m_iActFrame;}
    int GetTotalFrames() {return m_iTotalFrames;}
    void SetNoise(int noise) {m_iNoiseLevel = noise;} //1,3,7,15,31...
    void SetInterlace(bool interlace) {m_iInterlace = interlace?2:1;}
	void SetRewind(bool rewind) {m_bRewind = rewind;}

    typedef unsigned char tRFrame[RAW_Y][RAW_X]; //Raw Frame
    typedef unsigned char tCFrame[COM_Y][COM_X]; //Compressed Frame
    typedef unsigned char tUFrame[DEC_Y][DEC_X]; //Uncompressed Frame
    typedef tCFrame tMovie[];


  private:

    char* DecodeFrame();
    void Decode(char *ActFrame,tRFrame LastFrame,tRFrame RDest);
    void RFrame2Video(tRFrame Orig,char* Dest);
    bool LoadCompMovie(char *fname);
    void CloseMovie();        

  protected:

    static unsigned char CODEC[64];    
    static unsigned char DECODEC[16];

    bool m_bLoaded;
    tRFrame tempRFrame;
    int m_iTotalFrames;
    int m_iActFrame;
    int m_iNoiseLevel;
    int m_iInterlace;
	bool m_bRewind;
    char *m_pMovie;
    char *m_pMoviePtr;
    char *m_pBuffer;
    long *m_pBufferRGB;
	long m_aPal[256];
};

#endif //_CVIDEO_H_
/*
----------------------------------------------------------------------
-- Compressior Algorithm:
--
-- DifID (4-bit nibbles)
-- ---------------------
--
--  0000 - No change.
--  0001 -  +1 , -63
--  0010 -  +2 , -62
--  0011 -  +4 , -60
--  0100 -  +8 , -56
--  0101 - +16 , -48
--  0110 - +24 , -40
--  0111 - +32 , -32
--  1000 - +40 , -24
--  1001 - +48 , -16
--  1010 - +56 ,  -8
--  1011 - +60 ,  -4
--  1100 - +62 ,  -2
--  1101 - +63 ,  -1
--  1110 -  --> Crt JMP: Next Nibble: [#pixels_to_jump]
--  1111 -  --> Crt RLE: Next 2 Nibbles: [DifID][#pixels_to_repeat]
--
--  *If the first BYTE of the frame is FFh then the frame is uncompressed.
--  *Palette is always the same [0=black...63=white]
--
----------------------------------------------------------------------
*/

