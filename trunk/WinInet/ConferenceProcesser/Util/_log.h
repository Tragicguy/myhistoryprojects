/************************************
  REVISION LOG ENTRY
  Revision By: YCW
  Revised on 2003-11-1
  Comments: Logging facilities
 ************************************/

#ifndef ___log_h__
#define ___log_h__

#include <fstream>
#include <string>
#include <ctime>
using namespace std;

#include <stdarg.h>
#pragma warning(disable:4996)
namespace extension
{
/*
May 29 2000
Used for event logging
*/
	class event_log
	{
	public:
		// Event types
		typedef enum event_type
		{
			event_type_error,
			event_type_warning,
			event_type_information
		} event_type;

		event_log(char* szFile, bool bLogging = true);

		virtual ~event_log();

		void log_info(const char* szFormat, ...);

		void log_warning(const char* szFormat, ...);

		void log_error(const char* szFormat, ...);

		void log_event(event_type evtType, const char* szFormat, ...);

		void enable_logging(bool bLogging);

	private:
		void vlog_event(event_type evtType, const char* szFormat, va_list arg_list);

	private:
		// Flag used to enable or disable logging
		bool m_bLogging;
		// Name of the log file
		string m_strFileName;

		const char* event_type_to_string(event_type evtType);
		void del_hugelog(char* szFile);
		
	};
} // Namespace alx

#endif // ___log_h__
