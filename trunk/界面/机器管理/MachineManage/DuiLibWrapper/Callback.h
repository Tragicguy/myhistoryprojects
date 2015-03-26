///////////////////////////////////////////////////
//	作者：潘鹏
//	时间：2010-11-29
//	功能：回调模板化
///////////////////////////////////////////////////
//
//	使用方法
/*
#include "stdio.h"
#include "Callback.h"

class A
{
public:
	bool A_test(int a)
	{
		if (a > 0)
		{
			printf("%d > 0\n", a);
			return true;
		}

		printf("%d < 0\n", a);
		return false;
	}
};

class B
{
public:
	Callback<bool, int> m_callback;
public:
	void B_test(int a)
	{
		try
		{
			m_callback.call(a);
		}
		catch(...){}
	}
};

void main()
{
	A a;
	B b;

	CallbackObject<bool, int>* pCallback = b.m_callback.BindCallback(&a, &A::A_test);
	
	pCallback->call(3);

	b.B_test(2);

	delete pCallback;
}*/

#ifndef _PPCALLBACK_
#define _PPCALLBACK_

template<typename R, typename V>class CallbackObject
{
public:
	virtual ~CallbackObject(){}
	virtual R call(V val) = 0;
};

template<typename R, typename V>class CallbackFunction : public CallbackObject<R, V>
{
public:
	explicit CallbackFunction(R(*function)(V)){ m_function = function; }
	virtual R call(V val){ return (*m_function)(val); }
protected:
	R		(*m_function)(V);
};

class CallbackException
{
};

template<typename R, typename O, typename V>class CallbackMemberFunction : public CallbackObject<R, V>
{
public:
	explicit CallbackMemberFunction(O* pObject, R(O::*function)(V))
	{ 
		m_pObject = pObject; 
		m_function = function; 
	}
	virtual R call(V val)
	{ 
		if (m_pObject == 0)
			throw CallbackException();
		return (m_pObject->*m_function)(val);
	}
protected:
	O*		m_pObject;
	R		(O::*m_function)(V);
};

template<typename R, typename V>class Callback
{
protected:
	CallbackObject<R, V>*	m_pCallbackObject;
public:
	Callback(){	m_pCallbackObject = 0; }
	~Callback()
	{ 
		if (m_pCallbackObject)
		{
			delete m_pCallbackObject;
			m_pCallbackObject = 0;
		}
	}

	template<typename R, typename V>CallbackObject<R, V>* BindCallback(R(*function)(V))
	{
		if (m_pCallbackObject)
		{
			delete m_pCallbackObject;
			m_pCallbackObject = 0;
		}
		m_pCallbackObject = new CallbackFunction<R, V>(function);
		return m_pCallbackObject;
	}

	template<typename R, typename O, typename V>CallbackObject<R, V>* BindCallback(O* pObject, R(O::*function)(V))
	{
		if (m_pCallbackObject)
		{
			delete m_pCallbackObject;
			m_pCallbackObject = 0;
		}
		m_pCallbackObject = new CallbackMemberFunction<R, O, V>(pObject, function);
		return m_pCallbackObject;
	}

	virtual R call(V val)
	{
		if (m_pCallbackObject == 0)
			throw CallbackException();
		return m_pCallbackObject->call(val);
	}

	bool IsEnableCallback(){ return m_pCallbackObject != 0; }
};

/*******************************************************/
//以下是扩展，可以支持多参数调用 
template<typename R, typename V1, typename V2, typename V3, typename V4, typename V5>
class CallbackObjectEx
{
public:
	virtual ~CallbackObjectEx(){}
	virtual R call(V1 val1, V2 val2, V3 val3, V4 val4, V5 val5) = 0;
};

template<typename R, typename V1, typename V2, typename V3, typename V4, typename V5>
class CallbackFunctionEx : public CallbackObjectEx<R, V1, V2, V3, V4, V5>
{
public:
	explicit CallbackFunctionEx(R(*function)(V1, V2, V3, V4, V5)){ m_function = function; }
	virtual R call(V1 val1, V2 val2, V3 val3, V4 val4, V5 val5){ return (*m_function)(val1, val2, val3, val4, val5); }
protected:
	R		(*m_function)(V1, V2, V3, V4, V5);
};

template<typename R, typename O, typename V1, typename V2, typename V3, typename V4, typename V5>
class CallbackMemberFunctionEx : public CallbackObjectEx<R, V1, V2, V3, V4, V5>
{
public:
	explicit CallbackMemberFunctionEx(O* pObject, R(O::*function)(V1, V2, V3, V4, V5))
	{ 
		m_pObject = pObject; 
		m_function = function; 
	}
	virtual R call(V1 val1, V2 val2, V3 val3, V4 val4, V5 val5)
	{ 
		if (m_pObject == 0)
			throw CallbackException();
		return (m_pObject->*m_function)(val1, val2, val3, val4, val5);
	}
protected:
	O*		m_pObject;
	R		(O::*m_function)(V1, V2, V3, V4, V5);
};

template<typename R, typename V1, typename V2, typename V3, typename V4, typename V5>
class CallbackEx
{
protected:
	CallbackObjectEx<R, V1, V2, V3, V4, V5>*	m_pCallbackObject;
public:
	CallbackEx() : m_pCallbackObject(NULL)
	{
		m_pCallbackObject = NULL;
	}
	~CallbackEx()
	{ 
		if (m_pCallbackObject)
		{
			delete m_pCallbackObject;
			m_pCallbackObject = 0;
		}
	}
	void init()
	{
		m_pCallbackObject = NULL;
	}

	template<typename R, typename V1, typename V2, typename V3, typename V4, typename V5>
	CallbackObjectEx<R, V1, V2, V3, V4, V5>* BindCallback(R(*function)(V1, V2, V3, V4, V5))
	{
		if (m_pCallbackObject)
		{
			delete m_pCallbackObject;
			m_pCallbackObject = 0;
		}
		m_pCallbackObject = new CallbackFunctionEx<R, V1, V2, V3, V4, V5>(function);
		return m_pCallbackObject;
	}

	template<typename R, typename O, typename V1, typename V2, typename V3, typename V4, typename V5>
	CallbackObjectEx<R, V1, V2, V3, V4, V5>* BindCallback(O* pObject, R(O::*function)(V1, V2, V3, V4, V5))
	{
		if (m_pCallbackObject != NULL)
		{
			delete m_pCallbackObject;
			m_pCallbackObject = 0;
		}
		m_pCallbackObject = new CallbackMemberFunctionEx<R, O, V1, V2, V3, V4, V5>(pObject, function);
		return m_pCallbackObject;
	}

	R call(V1 val1, V2 val2, V3 val3, V4 val4, V5 val5)
	{
		if (m_pCallbackObject == 0)
			throw CallbackException();
		return m_pCallbackObject->call(val1, val2, val3, val4, val5);
	}

	bool IsEnableCallback(){ return m_pCallbackObject != 0; }
};
#endif