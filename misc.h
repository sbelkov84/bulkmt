#ifndef MISC_H
#define MISC_H
//------------------------------------------------------------------------

struct TSimpleCounter 
{
  
  std::size_t BulkQty;

  std::size_t CmdQty; 

  TSimpleCounter() : BulkQty(0), CmdQty(0) {}
};
//------------------------------------------------------------------------

struct TMainCounter : TSimpleCounter
{
  
  std::size_t LineQty;

  TMainCounter() : LineQty(0) {}
};
//------------------------------------------------------------------------

struct TElem
{
  bool WasConsoled;

  bool WasFiled;
 
  std::string Output;

  std::string Time;

  std::size_t CmdsQty;

  TElem(std::string& Output, std::string& Time, std::size_t CmdsQty)
  {
    WasConsoled = false;
    WasFiled = false;
    this->Output = Output;
    this->Time = Time;
    this->CmdsQty = CmdsQty;
  }
};
//------------------------------------------------------------------------

#endif // MISC_H