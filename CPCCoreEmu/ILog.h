#pragma once

class ILog
{
public :

   virtual void WriteLog ( const char* pLog ) = 0;
   virtual void WriteLogByte ( unsigned char pNumber ) = 0;
   virtual void WriteLogShort ( unsigned short pNumber ) = 0;
   virtual void WriteLog ( unsigned int pNumber ) = 0;
   virtual void EndOfLine () = 0;

};
