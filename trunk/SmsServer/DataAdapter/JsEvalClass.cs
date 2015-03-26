using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.JScript;
using System.CodeDom.Compiler;
using System.Reflection;  
namespace SmsServer.DataAdapter
{
    public class JsEvalClass
    {
        private object _Evaluator = null;
        private Type _EvaluatorType = null;
        private readonly string _JscriptCode =
        @"package JsEvalClass{class JsEvalClass{public function Eval(expr){return eval(expr,'unsafe');}}}";
        
        public JsEvalClass()
        {
            InitJscriptEngine();
        }
        /// <summary>
        /// 执行JS脚本
        /// </summary>
        /// <param name="strCode"></param>
        /// <returns></returns>
        public object RunJscriptCode(string strCode)
        {
            return _EvaluatorType.InvokeMember(
              "Eval",
              BindingFlags.InvokeMethod,
              null,
              _Evaluator,
              new object[] { strCode }
             );
        }
        /// <summary>
        /// 初始化脚本引擎
        /// </summary>
        private void InitJscriptEngine()
        {
            JScriptCodeProvider compiler = new JScriptCodeProvider();
            CompilerParameters parameters = new CompilerParameters();
            parameters.GenerateInMemory = true;
            CompilerResults results;
            results = compiler.CompileAssemblyFromSource(parameters, _JscriptCode);
            Assembly assembly = results.CompiledAssembly;
            _EvaluatorType = assembly.GetType("JsEvalClass.JsEvalClass");
            _Evaluator = Activator.CreateInstance(_EvaluatorType);
        }
    }
}
