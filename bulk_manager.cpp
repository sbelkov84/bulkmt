//------------------------------------------------------------------------
#include "bulk_manager.h"
//------------------------------------------------------------------------
//#include <boost/filesystem.hpp>
#include <filesystem>
#include <iostream>
//------------------------------------------------------------------------

TBulkManager::TBulkManager()
{
  /*boost::filesystem::path Path{L"./logs"};
  boost::filesystem::create_directory(Path);*/
  std::filesystem::create_directory("./logs");
  SetBrackCountersToZero();
}
//------------------------------------------------------------------------

bool TBulkManager::IsDynClosed()
{
  return OpeningCount == ClosingCount;
}
//------------------------------------------------------------------------

void TBulkManager::SetBrackCountersToZero()
{
  OpeningCount = 0;
  ClosingCount = 0;
}
//------------------------------------------------------------------------

void TBulkManager::ExecCmd(std::string Cmd)
{
  if (Cmd == "{")
  {
    BulkSubs[eBulkTypeFixed]->ReleaseBulk();
    ++OpeningCount;
  }
  else if (Cmd == "}")
  {
    ++ClosingCount;
    if (IsDynClosed())
    {
      BulkSubs[eBulkTypeDynamic]->ReleaseBulk();
    }
  }
  else if (Cmd.empty())
  {
    SetBrackCountersToZero();
    BulkSubs[eBulkTypeFixed]->ReleaseBulk();
    BulkSubs[eBulkTypeDynamic]->ReleaseBulk(IsDynClosed());
  }
  else
  {
    if (IsDynClosed())
      BulkSubs[eBulkTypeFixed]->AddCmd(Cmd);
    else
      BulkSubs[eBulkTypeDynamic]->AddCmd(Cmd);
  }
}
//------------------------------------------------------------------------
