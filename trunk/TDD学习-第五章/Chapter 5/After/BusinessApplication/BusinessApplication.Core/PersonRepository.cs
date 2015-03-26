using System.Collections.Generic;
using System.Linq;

namespace BusinessApplication.Core
{
    public interface IPersonRepository
    {
        Person GetPerson(int personId);
    }

    public class PersonRepository : IPersonRepository
    {
        private readonly IList<Person> _personList;

        public Person GetPerson(int personId)
        {
            return _personList.Where(person => person.Id == personId).FirstOrDefault();
        }

        public PersonRepository()
        {
            _personList = new List<Person>
                              {
                                  new Person {Id = 1, FirstName = "John", LastName = "Doe"},
                                  new Person {Id = 2, FirstName = "Richard", LastName = "Roe"},
                                  new Person {Id = 1, FirstName = "Amy", LastName = "Adams"}
                              };
        }
    }
}