using BusinessApplication.Core;
using Ninject;
using NUnit.Framework;

namespace BusinessApplication.UnitTests
{
    [TestFixture]
    public class BusinessServiceTests
    {
        [Test]
        public void ShouldBeAbleToGetBusinessServiceFromNinject()
        {
            BusinessService actual;
            var kernel = new StandardKernel(new CoreModule());
            actual = kernel.Get<BusinessService>();

            Assert.IsNotNull(actual);
        }
    }
}