//------------------------------------------------------------------------
#include "single_bulk.h"
//------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <fstream>
//------------------------------------------------------------------------
std::mutex Mtx;
std::condition_variable CondVar;
std::list<TElem> ListOfBulks;
//------------------------------------------------------------------------

/*! \brief Main counter */
/*! static TMainCounter MainCounter */
TMainCounter MainCounter;

/*! \brief Log counter */
/*! static TSimpleCounter LogCounter */
TSimpleCounter LogCounter;

/*! \brief File1 counter */
/*! static TSimpleCounter File1Counter */
TSimpleCounter File1Counter;

/*! \brief File2 counter */
/*! static TSimpleCounter File2Counter */
TSimpleCounter File2Counter;
//------------------------------------------------------------------------

void PrintInConsole(std::list<TElem>& List, TSimpleCounter& Counter)
{
  std::string Output;
  std::size_t CmdsQty;
  std::unique_lock<std::mutex> Lk(Mtx);
  CondVar.wait(Lk, [&List](){return !List.empty();});
  auto Elem = List.begin();
  while (Elem != List.end() && (*Elem).WasConsoled)
  {
    ++Elem;
  }
  if (Elem != List.end())
  {
    Output = (*Elem).Output;
    CmdsQty = (*Elem).CmdsQty;
    (*Elem).WasConsoled = true;
    if ((*Elem).WasFiled)
      List.erase(Elem);
  }
  Lk.unlock();
  //---
  std::cout << Output << std::endl;
  ++Counter.BulkQty;
  Counter.CmdQty += CmdsQty;
}
//------------------------------------------------------------------------

void PrintInFile(std::list<TElem>& List, TSimpleCounter& Counter)
{
  std::string Output, Time;
  std::size_t CmdsQty;
  std::unique_lock<std::mutex> Lk(Mtx);
  CondVar.wait(Lk, [&List](){return !List.empty();});
  auto Elem = List.begin();
  while (Elem != List.end() && (*Elem).WasFiled)
  {
    ++Elem;
  }
  if (Elem != List.end())
  {
    Output = (*Elem).Output;
    Time = (*Elem).Time;
    CmdsQty = (*Elem).CmdsQty;
    (*Elem).WasFiled = true;
    if ((*Elem).WasConsoled)
      List.erase(Elem);
  }
  Lk.unlock();
  //---
  std::string FileName = "./logs/bulk";
  FileName += Time;
  FileName += ".log";
  std::ofstream OutFile(FileName, std::ios::binary);
  OutFile << Output;  
  OutFile.close();
  ++Counter.BulkQty;
  Counter.CmdQty += CmdsQty;
}
//------------------------------------------------------------------------

int main(int argc, char** argv)
{
  std::string BulkSize;
  if (argc > 1)
    BulkSize.append(argv[1]);

  /*! \brief Maximal size of fixed bulk */
  /*! std::size_t N */
  std::size_t N = 0;
  
  try 
  {
    N = stoi(BulkSize); 
  }
  catch (...)
  {
    std::cout << "Please set BulkSize cmd parameter greater than 0" << std::endl;
    return 1;
  }

  if (N <= 0)
  {
    std::cout << "Please set BulkSize cmd parameter greater than 0" << std::endl;
    return 1;
  }

  TBulkManager BulkManager;
  TSingleBulk FixedBulk(&BulkManager, eBulkTypeFixed, &ListOfBulks, &Mtx, &CondVar, N);
  TSingleBulk DynamicBulk(&BulkManager, eBulkTypeDynamic, &ListOfBulks, &Mtx, &CondVar);

  std::thread Log(PrintInConsole, std::ref(ListOfBulks), std::ref(LogCounter));
  std::thread File1(PrintInFile, std::ref(ListOfBulks), std::ref(File1Counter));
  std::thread File2(PrintInFile, std::ref(ListOfBulks), std::ref(File2Counter));

  std::string Cmd;
  while (std::getline(std::cin, Cmd))
  {
    ++MainCounter.LineQty;
    if (Cmd != "{" && Cmd != "}" && !Cmd.empty())
      ++MainCounter.CmdQty;

    BulkManager.ExecCmd(Cmd);
  }

  Log.join();
  File1.join();
  File2.join();

  std::cout << "Main:  Lines = " << MainCounter.LineQty << " Cmds = " << MainCounter.CmdQty << " Bulks = " << LogCounter.BulkQty << std::endl;
  std::cout << "Log:   Bulks = " << LogCounter.BulkQty << " Cmds = " << LogCounter.CmdQty << std::endl;
  std::cout << "File1: Bulks = " << File1Counter.BulkQty << " Cmds = " << File1Counter.CmdQty << std::endl;
  std::cout << "File2: Bulks = " << File2Counter.BulkQty << " Cmds = " << File2Counter.CmdQty << std::endl;

  return 0;
}
