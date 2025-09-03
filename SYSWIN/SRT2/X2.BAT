@set INCLUDE=c:\PROGRAMA\WATCOM\H;C:\EUSKAL;c:\programa\code\awl
@wpp386 %1.cpp -5r -zq -oneatxi -s -fp6 -fpi87 -zp=8
@wlink sys pmodew f %1
