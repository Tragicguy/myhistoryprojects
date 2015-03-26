using System;
using System.Diagnostics;
using BusinessApplication.Core;
using Moq;
using Ninject;
using NUnit.Framework;

namespace BusinessApplication.UnitTests
{
    [TestFixture]
    public class PersonServiceTests
    {
        [Test]
        public void ShouldBeAbleToCallPersonServiceAndGetPerson()
        {
            var expected = new Person {Id = 1, FirstName = "John", LastName = "Doe"};
            var personReposityMock = new Mock<IPersonRepository>();
            personReposityMock
                .Setup(pr => pr.GetPerson(1))
                .Returns(new Person {Id = 1, FirstName = "Bob", LastName = "Smith"});
            var personService = new PersonService(personReposityMock.Object);
            var actual = personService.GetPerson(expected.Id);

            Assert.AreEqual(expected.Id, actual.Id);
            Assert.AreEqual(expected.FirstName, actual.FirstName);
            Assert.AreEqual(expected.LastName, actual.LastName);            
        }
    }
}