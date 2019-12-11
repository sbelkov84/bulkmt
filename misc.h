#ifndef MISC_H
#define MISC_H
//------------------------------------------------------------------------

struct TSimpleCounter 
{
  
  unsigned int BulkQty;

  unsigned int CmdQty; 

  TSimpleCounter() : BulkQty(0), CmdQty(0) {}
};
//------------------------------------------------------------------------

struct TMainCounter : TSimpleCounter
{
  
  unsigned int LineQty;

  TMainCounter() : LineQty(0) {}
};
//------------------------------------------------------------------------

struct TElem
{
  bool WasConsoled;

  bool WasFiled;
 
  std::string Output;

  std::string Time;

  TElem(std::string& Output, std::string& Time)
  {
    WasConsoled = false;
    WasFiled = false;
    this->Output = Output;
    this->Time = Time;
  }
};
//------------------------------------------------------------------------

#endif // MISC_H