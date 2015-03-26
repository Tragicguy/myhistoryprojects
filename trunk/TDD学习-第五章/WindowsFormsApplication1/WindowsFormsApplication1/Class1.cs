using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WindowsFormsApplication1
{
    public interface ITestInterface
    {
        string GetString();
    }
    public class Class1 : ITestInterface
    {
        public string GetString()
        {
            return "hello world";
        }
    }
}
