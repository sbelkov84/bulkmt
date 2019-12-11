#ifndef BULK_MANAGER_H
#define BULK_MANAGER_H
//------------------------------------------------------------------------
#include <map>
#include <string>
#include <stdlib.h>
//------------------------------------------------------------------------

enum EBulkType
{
  eBulkTypeFixed
 ,eBulkTypeDynamic
};
//------------------------------------------------------------------------

struct TBulkObserver
{
  /*! \brief Interface add-command in bulk method */
  /*! virtual void AddCmd(std::string& Cmd) */
  virtual void AddCmd(std::string& Cmd) = 0;

  /*! \brief Interface release-command from bulk method */
  /*! virtual void ReleaseBulk(bool NeedToPrint = true) */
  virtual void ReleaseBulk(bool NeedToPrint = true) = 0;
};
//------------------------------------------------------------------------

struct TBulkManager
{
  /*! \brief Container for pointers on subscribers */
  /*! std::map<TBulkObserver*> BulkSubs */
  std::map<EBulkType, TBulkObserver*> BulkSubs;

  /*! \brief Constructor. Creates directory for log-files */
  /*! TBulkManager() */
  TBulkManager();

  /*! \brief Stores index of active bulk */
  /*! char ActiveBulkIndex */
  char ActiveBulkIndex;

  /*! \brief Quantity of opening curly brackets */
  /*! std::size_t OpeningCount */
  std::size_t OpeningCount;

  /*! \brief Quantity of closing curly brackets */
  /*! std::size_t ClosingCount */
  std::size_t ClosingCount;

  /*! \brief Returns 'true' if OpeningCount is equal to ClosingCount */
  /*! bool IsClosed() */
  bool IsDynClosed();

  /*! \brief Sets curly brackets counters to zero */
  /*! void SetBrackCountersToZero() */
  void SetBrackCountersToZero();

  /*! Subscribes to BulkManager */
  /*! void Subscribe(TBulkObserver* Bulk) */
  void Subscribe(TBulkObserver* Bulk, EBulkType Type)
  {
    BulkSubs[Type] = Bulk;
  }

  /*! \brief Primary command processing and after that calling necessary BulkSubs command-processing */
  /*! void ExecCmd(std::string Cmd) */
  void ExecCmd(std::string Cmd);
};
//------------------------------------------------------------------------

#endif // BULK_MANAGER_H
