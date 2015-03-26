using System.Configuration;

namespace BusinessApplication.Core
{
    public class BusinessService
    {
        private readonly string _databaseConnectionString =
            ConfigurationManager.ConnectionStrings["MyConnectionString"].ConnectionString;

        private readonly string _webServiceAddress = ConfigurationManager.AppSettings["MyWebServiceAddress"];
        private readonly LoggingDataSink _loggingDataSink;

        private DataAccessComponent _dataAccessComponent;
        private WebServiceProxy _webServiceProxy;
        private LoggingComponent _loggingComponent;
        

        public BusinessService()
        {
            _loggingDataSink = new LoggingDataSink();
            _loggingComponent = new LoggingComponent(_loggingDataSink);
            _webServiceProxy = new WebServiceProxy(_webServiceAddress);
            _dataAccessComponent = new DataAccessComponent(_databaseConnectionString);
        }
    }

}
