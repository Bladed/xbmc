#pragma once
//#include "windows.h"

/*extern "C"
{
#include "cmyth/cmyth.h"
#include "refmem/refmem.h"
}*/

//#include <string>
#include <vector>
#include <map>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include "utils/StdString.h"
#include "libcmyth.h"
#include "thread.h"

template < class T > class MythPointer;
template < class T > class MythPointerThreadSafe;

enum TimerType {
  NotRecording	  = 0,
  SingleRecord    = 1,
  TimeslotRecord	= 2,
  ChannelRecord   = 3,
  AllRecord	      = 4,
  WeekslotRecord	= 5,
  FindOneRecord	  = 6,
  OverrideRecord	= 7,
  DontRecord	    = 8,
  FindDailyRecord	= 9,
  FindWeeklyRecord= 10
};

class MythChannel;
class MythRecorder;
class MythProgramInfo;
class MythEventHandler;
typedef std::vector< MythChannel > MythChannelList;

class MythRecorder 
{
public:
  MythRecorder();
  MythRecorder(cmyth_recorder_t cmyth_recorder);
  bool SpawnLiveTV(MythChannel &channel);
  bool LiveTVChainUpdate(CStdString chainID);
  bool IsNull();
  bool IsRecording();
  int ID();
  bool CheckChannel(MythChannel &channel);
  bool SetChannel(MythChannel &channel);
  int ReadLiveTV(void* buffer,long long length);
  MythProgramInfo GetCurrentProgram();
  long long LiveTVSeek(long long offset, int whence);
  long long LiveTVDuration();
  bool Stop();
private:
  boost::shared_ptr< MythPointerThreadSafe< cmyth_recorder_t > > m_recorder_t;
  static void prog_update_callback(cmyth_proginfo_t prog);
  boost::shared_ptr< int > livechainupdated;
  
};

class MythSignal
{
  friend class MythEventHandler;
public:
    MythSignal();
    CStdString  AdapterStatus();     /*!< @brief (optional) status of the adapter that's being used */
    int    SNR();                       /*!< @brief (optional) signal/noise ratio */
    int    Signal();                    /*!< @brief (optional) signal strength */
    long   BER();                       /*!< @brief (optional) bit error rate */
    long   UNC();                       /*!< @brief (optional) uncorrected blocks */
private:
    CStdString  m_AdapterStatus;     /*!< @brief (optional) status of the adapter that's being used */
    int    m_SNR;                       /*!< @brief (optional) signal/noise ratio */
    int    m_Signal;                    /*!< @brief (optional) signal strength */
    long   m_BER;                       /*!< @brief (optional) bit error rate */
    long   m_UNC;                       /*!< @brief (optional) uncorrected blocks */
};

class MythEventHandler 
{
public:
  MythEventHandler();
  MythEventHandler(CStdString,unsigned short port);
  void SetRecorder(MythRecorder &rec);
  MythSignal GetSignal();
  void Stop();
  void PreventLiveChainUpdate();
  void AllowLiveChainUpdate();
private:
  class ImpMythEventHandler;
  boost::shared_ptr< ImpMythEventHandler > m_imp;

};

typedef cmyth_program_t MythProgram;

class MythTimer
{
public:
  MythTimer();
  MythTimer(cmyth_timer_t cmyth_timer);
  int RecordID();
  int ChanID();
  time_t StartTime();
  time_t EndTime();
  CStdString Title();
  CStdString Description();
  int Type();
  CStdString Category();
  bool IsNull();
private:
  boost::shared_ptr< MythPointer< cmyth_timer_t > > m_timer_t;  
};

typedef std::pair< CStdString, std::vector< int > > MythChannelGroup;

class MythDatabase
{
public:
  MythDatabase();
  MythDatabase(CStdString server,CStdString database,CStdString user,CStdString password);
  bool TestConnection(CStdString &msg);
  std::map< int , MythChannel > ChannelList();
  std::vector< MythProgram > GetGuide(time_t starttime, time_t endtime);
  std::vector< MythTimer > GetTimers();
  int AddTimer(int chanid,CStdString channame, CStdString description, time_t starttime, time_t endtime,CStdString title,CStdString category,TimerType type);
  bool DeleteTimer(int recordid);
  bool UpdateTimer(int recordid,int chanid,CStdString channame,CStdString description, time_t starttime, time_t endtime,CStdString title,CStdString category, TimerType type);
  boost::unordered_map< CStdString, std::vector< int > > GetChannelGroups();
  std::map< int, std::vector< int > > SourceList();
  bool IsNull();
private:
  boost::shared_ptr< MythPointerThreadSafe< cmyth_database_t > > m_database_t;
};

class MythChannel
{
public:
  MythChannel();
  MythChannel(cmyth_channel_t cmyth_channel,bool isRadio);
  int ID();
  int Number();
  int SourceID();
  CStdString Name();
  CStdString Icon();
  bool Visible();
  bool IsRadio();
  bool IsNull();
private:
  boost::shared_ptr< MythPointer< cmyth_channel_t > > m_channel_t;
  bool m_radio;
};

class MythProgramInfo 
{
  friend class MythConnection;
public:
  MythProgramInfo();
  MythProgramInfo(cmyth_proginfo_t cmyth_proginfo);
  CStdString ProgramID();
  CStdString Title();
  CStdString Path();
  CStdString Description();
  CStdString ChannelName();
  int ChannelID();
  time_t RecStart();
  int Duration();
  CStdString Category();
  CStdString RecordingGroup();
  long long uid();
  bool IsNull();
private:
  boost::shared_ptr< MythPointer< cmyth_proginfo_t > > m_proginfo_t;
};


class MythTimestamp
{
public:
  MythTimestamp();
  MythTimestamp(cmyth_timestamp_t cmyth_timestamp);
  MythTimestamp(CStdString time,bool datetime);
  MythTimestamp(time_t time);
  bool operator==(const MythTimestamp &other);
  bool operator!=(const MythTimestamp &other){return !(*this == other);}
  bool operator>(const MythTimestamp &other);
  bool operator>=(const MythTimestamp &other){return (*this == other||*this > other);}
  bool operator<(const MythTimestamp &other);
  bool operator<=(const MythTimestamp &other){return (*this == other||*this < other);}
  time_t UnixTime();
  CStdString String();
  CStdString Isostring();
  CStdString Displaystring(bool use12hClock);
  bool IsNull();
private:
  boost::shared_ptr< MythPointer< cmyth_timestamp_t > > m_timestamp_t;
};

class MythFile 
{
public:
  MythFile();
  MythFile(cmyth_file_t myth_file);
  bool IsNull();
  int Read(void* buffer,long long length);
  long long Seek(long long offset, int whence);
  long long Duration();

private:
  boost::shared_ptr< MythPointer< cmyth_file_t > > m_file_t; 
};

class MythConnection 
{
public:
  MythConnection();
  MythConnection(CStdString server,unsigned short port);
  MythRecorder GetFreeRecorder();
  MythRecorder GetRecorder(int n);
  MythEventHandler CreateEventHandler();
  boost::unordered_map< CStdString, MythProgramInfo > GetRecordedPrograms();
  boost::unordered_map< CStdString, MythProgramInfo > GetPendingPrograms();
  boost::unordered_map< CStdString, MythProgramInfo > GetScheduledPrograms();
  bool DeleteRecording(MythProgramInfo &recording);
  bool IsConnected();
  CStdString GetServer();
  int GetProtocolVersion();
  bool GetDriveSpace(long long &total,long long &used);
  bool UpdateSchedules(int id);
  MythFile ConnectFile(MythProgramInfo &recording);
  bool IsNull();
private:
  boost::shared_ptr< MythPointer< cmyth_conn_t > > m_conn_t;
  CStdString m_server;
  unsigned short m_port;
};

