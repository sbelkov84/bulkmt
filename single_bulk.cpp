//------------------------------------------------------------------------
#include "single_bulk.h"
//------------------------------------------------------------------------
#include <iostream>
//------------------------------------------------------------------------

TSingleBulk::TSingleBulk(TBulkManager* Manager, EBulkType Type, std::list<TElem>* BulkList, std::mutex* Mtx, std::condition_variable* CondVar, std::size_t N)
{
  Manager->Subscribe(this, Type);
  this->BulkList = BulkList;
  this->Mtx = Mtx;
  this->CondVar = CondVar;
  this->N = N;
}
//------------------------------------------------------------------------

void TSingleBulk::AddCmd(std::string& Cmd)
{
  if (Commands.empty())
    FirstCmdTime = std::time(nullptr);

  Commands.push_back(Cmd);
  if (N > 0 && Commands.size() == N)
    ReleaseBulk();
}
//------------------------------------------------------------------------

void TSingleBulk::ClearCmds()
{
  Commands.resize(0);
}
//------------------------------------------------------------------------

std::string TSingleBulk::CmdsToStr()
{
  std::string Res = "bulk: ";
  for (auto It = Commands.begin(); It != Commands.end(); ++It)
  {
    Res += *It;
    Res += " ";
  }
  ClearCmds();
  //---
  return Res;
}
//------------------------------------------------------------------------

void TSingleBulk::ReleaseBulk(bool NeedToPrint)
{
  if (NeedToPrint)
  {
    Mtx->lock();
    std::string Out = std::move(CmdsToStr());
    std::string Time = std::move(std::to_string(FirstCmdTime));
    BulkList->emplace_back(TElem(Out, Time));
    Mtx->unlock();
    CondVar->notify_all();
  }
  //---
  ClearCmds();
}
//------------------------------------------------------------------------
