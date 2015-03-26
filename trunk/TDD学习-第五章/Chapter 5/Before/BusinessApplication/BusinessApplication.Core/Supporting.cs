using System;

namespace BusinessApplication.Core
{
    public interface ILoggingComponent
    {
    }

    public interface IWebServiceProxy
    {
    }

    public interface IDataAccessComponent
    {
    }

    internal class LoggingDataSink
    {
    }

    internal class LoggingComponent : ILoggingComponent
    {
        public LoggingComponent(LoggingDataSink loggingDataSink)
        {
            throw new NotImplementedException();
        }
    }

    internal class WebServiceProxy : IWebServiceProxy
    {
        public WebServiceProxy(string webServiceAddress)
        {
            throw new NotImplementedException();
        }
    }

    internal class DataAccessComponent : IDataAccessComponent
    {
        public DataAccessComponent(string connectionString)
        {

        }
    }

}