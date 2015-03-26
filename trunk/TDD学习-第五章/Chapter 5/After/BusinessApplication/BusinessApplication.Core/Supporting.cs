using System;

namespace BusinessApplication.Core
{
    public interface ILoggingDataSink
    {        
    }

    public interface ILoggingComponent
    {
    }

    public interface IWebServiceProxy
    {
    }

    public interface IDataAccessComponent
    {
    }

    internal class LoggingDataSink : ILoggingDataSink
    {
    }

    internal class LoggingComponent : ILoggingComponent
    {
        public LoggingComponent(ILoggingDataSink loggingDataSink)
        {
            
        }
    }

    internal class WebServiceProxy : IWebServiceProxy
    {
        public WebServiceProxy(string webServiceAddress)
        {
            
        }
    }

    internal class DataAccessComponent : IDataAccessComponent
    {
        public DataAccessComponent(string connectionString)
        {

        }
    }    
}