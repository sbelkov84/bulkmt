//------------------------------------------------------------------------
#include "single_bulk.h"
//------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <thread>
#include <list>
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
    (*Elem).WasConsoled = true;
    if ((*Elem).WasFiled)
      List.erase(Elem);
  }
  Lk.unlock();
  //---
  std::cout << Output << std::endl;
}
//------------------------------------------------------------------------

void PrintInFile(std::list<TElem>& List, TSimpleCounter& Counter)
{
  std::string Output, Time;
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
  std::thread File1(PrintInConsole, std::ref(ListOfBulks), std::ref(File1Counter));
  std::thread File2(PrintInConsole, std::ref(ListOfBulks), std::ref(File2Counter));

  std::string Cmd;
  while (std::getline(std::cin, Cmd))
  {
    BulkManager.ExecCmd(Cmd);
  }

  Log.join();
  File1.join();
  File2.join();

  return 0;
}
