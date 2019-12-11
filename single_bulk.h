#ifndef SINGLE_BULK_H
#define SINGLE_BULK_H
//------------------------------------------------------------------------
#include <vector>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include <list>
//------------------------------------------------------------------------
#include "bulk_manager.h"
#include "misc.h"
//------------------------------------------------------------------------

class TSingleBulk : public TBulkObserver
{
private:

  /*! Stores not-manage commands */
  /*! std::vector<std::string> Commands */
  std::vector<std::string> Commands;

  /*! \brief Maximal size of fixed bulk */
  /*! std::size_t N */
  std::size_t N;

  /*! \brief Stores first command time in this bulk */
  /*! std::chrono::time_point<std::chrono::system_clock> */
  time_t FirstCmdTime;

  /*! \brief Pointer on global list of bulk's strings */
  /*! std::list<TElem>* BulkList */
  std::list<TElem>* BulkList;

  /*! \brief Pointer to global condition variable */
  /*! std::condition_variable* CondVar */
  std::condition_variable* CondVar;

  /*! \brief Pointer to global mutex */
  /*! std::mutex Mutex */
  std::mutex* Mtx;

  /*! \brief Moves commands from vector to string and cleans vector */
  /*! std::string CmdsToStr() */
  std::string CmdsToStr();

public:

  /*! \brief Default constructor is used in child class */
  /*! TSingleBulk() */
  TSingleBulk() {N = 0;}

  /*! \brief Constructor sets default fields */
  /*! TFixedBulk(TBulkManager* Manager, EBulkType Type, std::list<TElem>* BulkList, std::mutex* Mutex, std::condition_variable* CondVar, std::size_t N) */
  TSingleBulk(TBulkManager* Manager, EBulkType Type, std::list<TElem>* BulkList, std::mutex* Mtx, std::condition_variable* CondVar, std::size_t N = 0);

  /*! \brief Adds cmd to commands-vector */
  /*! void AddCmd(std::string& Cmd) */
  void AddCmd(std::string& Cmd) override;

  /*! \brief Clears commands-vector */
  /*! void ClearCmds() */
  void ClearCmds();

  /*! \brief Print bulk in std::cout and file, clears command-vector */
  /*! void ReleaseBulk(bool NeedToPrint = true) */
  void ReleaseBulk(bool NeedToPrint = true) override;
};
//------------------------------------------------------------------------

#endif // SINGLE_BULK_H
