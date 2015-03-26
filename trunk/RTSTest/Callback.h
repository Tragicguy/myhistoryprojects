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

#endif