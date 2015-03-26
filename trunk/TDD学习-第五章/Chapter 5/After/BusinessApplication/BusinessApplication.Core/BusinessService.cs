using System.Configuration;
using Ninject;

namespace BusinessApplication.Core
{
    public class BusinessService 
    {
        private ILoggingComponent _loggingComponent;
        private IWebServiceProxy _webServiceProxy;
        private IDataAccessComponent _dataAccessComponent;

        public BusinessService(ILoggingComponent loggingComponent, IWebServiceProxy webServiceProxy, IDataAccessComponent dataAccessComponent)
        {
            _loggingComponent = loggingComponent;
            _webServiceProxy = webServiceProxy;
            _dataAccessComponent = dataAccessComponent;
        }
    }

}
