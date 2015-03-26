namespace BusinessApplication.Core
{
    public interface IPersonService
    {
        Person GetPerson (int personId);
    }

    public class PersonService : IPersonService
    {
        private readonly IPersonRepository _personRepository;

        public PersonService(IPersonRepository personRepository)
        {
            _personRepository = personRepository;
        }
        public Person GetPerson (int personId)
        {
            return _personRepository.GetPerson(personId);
        }        
    }
}