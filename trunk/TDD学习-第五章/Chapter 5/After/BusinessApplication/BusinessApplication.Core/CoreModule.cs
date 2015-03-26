using System;
using System.Configuration;
using Ninject.Activation;
using Ninject.Modules;

namespace BusinessApplication.Core
{
    public class CoreModule : NinjectModule
    {
        public override void Load()
        {
            Bind<BusinessService>().ToSelf();
            Bind<ILoggingDataSink>().To<LoggingDataSink>();
            Bind<ILoggingComponent>().To<LoggingComponent>();
            Bind<IDataAccessComponent>().ToProvider(new DataAccessComponentProvider());
            Bind<IWebServiceProxy>().ToProvider(new WebServiceProxyComponentProvider());

            Bind<IPersonRepository>().To<PersonRepository>();
            Bind<IPersonService>().To<PersonService>();
        }
    }

    public class WebServiceProxyComponentProvider : Provider<IWebServiceProxy>
    {
        protected override IWebServiceProxy CreateInstance(IContext context)
        {
            var webServiceAddress = ConfigurationManager.AppSettings["MyWebServiceAddress"];
            return new WebServiceProxy(webServiceAddress);
        }
    }

    public class DataAccessComponentProvider : Provider<IDataAccessComponent>
    {
        protected override IDataAccessComponent CreateInstance(IContext context)
        {
            var databaseConnectionString = 
                ConfigurationManager.ConnectionStrings["MyConnectionString"].ConnectionString;
            return new DataAccessComponent(databaseConnectionString);
        }
    }
}