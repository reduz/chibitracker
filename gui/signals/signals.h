
//
// C++ Interface: signals
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PIGUISIGNALS_H
#define PIGUISIGNALS_H

#ifndef GUI_EXTERNAL_SIGNAL

#include "base/defs.h"


namespace GUI {

typedef void (*_EmptyFunc)();

template<class T>
struct _UnionMethod {

	union Same {
		
		_EmptyFunc ef;
		T t;
	} same;
};
		
template<class T>
_EmptyFunc get_method_ptr(T p_method) {
	
	_UnionMethod<T> u;
	u.same.t=p_method;
	return u.same.ef;
}

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/



class SignalBase;


class SignalTarget {

	struct Connection {

		int refcount; //amount of times this signal is connected to methods of this class!
		
		
		SignalBase * signal;

		Connection *next;

		Connection() { next=0; signal=0; refcount=0; }
	};

	// using underscore because some older compilers will complain
	
	Connection *_conn_list; //signals connected are referenced here, so as you see, it's very lightweight
	
	friend class SignalBase;
	bool _register_signal( SignalBase *p_signal ); ///< return true if signal already exists, to warn and avoid connecting twice.
	void _clear_signal( SignalBase *p_signal );
public:
	
		
	SignalTarget();
	~SignalTarget();
};



/* 
	NtoN: 0to1
	N: 1
*/



/*
	examples
	n: 0
	N: 1
*/
	




template< class P1, class P2, class P3,class P4, class P5, class P6 > 
class Method6 {
public:

	typedef void (Method6::*LocalMethod)(P1,P2,P3,P4,P5,P6);
	typedef void (*CallFuncPtr)(void *,LocalMethod,P1,P2,P3,P4,P5,P6);

private:


	/* the clever hack that does the magic */
	template<class T>
	static void call_func_template(void *p_object_ptr,LocalMethod p_m,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5, P6 p6) {
		
		T *t = static_cast<T*>(p_object_ptr);
		void (T::*obj_method)(P1,P2,P3,P4,P5,P6)=reinterpret_cast<void (T::*)(P1,P2,P3,P4,P5,P6)>(p_m);
		return (t->*obj_method)(p1,p2,p3,p4,p5,p6);
	}

	SignalTarget *instance; 
	CallFuncPtr call_func;
	LocalMethod method;
	void *object;
public:

	SignalTarget *get_instance() const { return instance; }
	_EmptyFunc get_method() const { return get_method_ptr(method); }
	bool is_empty() { return object==0; }
	
	bool operator==(const Method6& p_method) const { return ( get_instance()==p_method.get_instance() && get_method()==p_method.get_method()); }
	bool operator!=(const Method6& p_method) const { return !( *this==p_method ); }
	
	void call(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6) {

		if (!object)
			return;
		call_func(object,method,p1,p2,p3,p4,p5,p6);

	};

	bool is_empty() const  { return (object==0); }
	void clear() { object=0; }

	template<class T>
	void set( T* p_instance,void (T::*p_method)(P1,P2,P3,P4,P5,P6) ) {

		// real stuff used to call
		
		instance=p_instance;
		object=p_instance;
		method=reinterpret_cast<LocalMethod>(p_method);
		call_func=&call_func_template<T>;
	}

	template<class T>
	Method6( T* p_instance,void (T::*p_method)(P1,P2,P3,P4,P5,P6)) {
		set( p_instance,p_method );
	}

	Method6() { object=0; call_func=0; instance=0; }
};


/*
	examples
	N: 2
	BindN: 3
	NtoN: 2to3
*/
	


template<class P1,class P2,class P3,class P4,class P5>
struct Bind5to6_Base {
	
	virtual SignalTarget *get_instance() const=0;
	virtual _EmptyFunc get_method()const =0;
	virtual void call(P1,P2,P3,P4,P5)=0;
	virtual Bind5to6_Base * copy() const=0;
	virtual ~Bind5to6_Base() {}
};


template<class P1,class P2,class P3,class P4,class P5,class P6>
struct Bind5to6 : public Bind5to6_Base<P1,P2,P3,P4,P5> {

	P6 data;
	Method6<P1,P2,P3,P4,P5,P6> method;

	_EmptyFunc get_method() const { return method.get_method(); }
	SignalTarget *get_instance() const { return method.get_instance(); }
	
	void call(P1 p_data1,P2 p_data2,P3 p_data3,P4 p_data4,P5 p_data5) { method.call(p_data1,p_data2,p_data3,p_data4,p_data5, data); }
	Bind5to6_Base<P1,P2,P3,P4,P5> * copy() const { 
		Bind5to6<P1,P2,P3,P4,P5,P6> *_copy = new Bind5to6<P1,P2,P3,P4,P5,P6>(method, data);
		*_copy=*this;
		return _copy; 
	}
	
	Bind5to6(const Method6<P1,P2,P3,P4,P5,P6>& p_method, P6 p_data) {

		data = p_data;
		method=p_method;
	}
};

template< class P1,class P2,class P3,class P4,class P5 >
class Method5 {
public:

	typedef void (Method5::*LocalMethod)(P1,P2,P3,P4,P5);
	typedef void (*CallFuncPtr)(void *,LocalMethod,P1,P2,P3,P4,P5);

private:


	/* the clever hack that does the magic */
	template<class T>
	static void call_func_template(void *p_object_ptr,LocalMethod p_m,P1 p1,P2 p2,P3 p3,P4 p4,P5 p5) {
		
		T *t = static_cast<T*>(p_object_ptr);
		void (T::*obj_method)(P1,P2,P3,P4,P5)=reinterpret_cast<void (T::*)(P1,P2,P3,P4,P5)>(p_m);
		return (t->*obj_method)(p1,p2,p3,p4,p5);
	}

	SignalTarget *instance; 
	CallFuncPtr call_func;
	LocalMethod method;
	void *object;
	Bind5to6_Base<P1,P2,P3,P4,P5> *bindobj;
public:

	SignalTarget *get_instance() const { return bindobj?bindobj->get_instance():instance; }
	_EmptyFunc get_method() const { return bindobj?bindobj->get_method():get_method_ptr(method); }
	bool is_empty() { return object==0 && bindobj==0; }

	bool operator==(const Method5& p_method) const { return ( get_instance()==p_method.get_instance() && get_method()==p_method.get_method()); }
	bool operator!=(const Method5& p_method) const { return !( *this==p_method ); }
	
	void call(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5) {

		if (bindobj)
			bindobj->call(p1,p2,p3,p4,p5);
		else if (object) {
			call_func(object,method,p1,p2,p3,p4,p5);
		}

	};

	bool is_empty() const  { return (object==0 && bindobj==0); }
	void clear() { object=0; if (bindobj) delete bindobj; bindobj=0; }

	template<class T>
	void set( T* p_instance,void (T::*p_method)(P1,P2,P3,P4,P5) ) {

		// real stuff used to call
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=p_instance;
		object=p_instance;
		method=reinterpret_cast<LocalMethod>(p_method);
		call_func=&call_func_template<T>;
	}

	template<class TBind>
	void bind(const Method6<P1,P2,P3,P4,P5, TBind>& p_method, TBind p_data) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=0;
		object=0;
		method=0;
		bindobj = new Bind5to6<P1,P2,P3,P4,P5, TBind>(p_method, p_data);
	}

	template<class T>
	Method5( T* p_instance,void (T::*p_method)(P1,P2,P3,P4,P5)) {
		bindobj = 0;
		set( p_instance,p_method );
	}

	template<class TBind>
	Method5(const Method6<P1,P2,P3,P4,P5,TBind>& p_method, TBind p_data) {
		bindobj=0; object=0; call_func=0; instance=0;
		bind( p_method, p_data );
		
	}

	void operator=(const Method5<P1,P2,P3,P4,P5>& p_method) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
		
	}

	Method5(const Method5<P1,P2,P3,P4,P5>& p_method) {
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
	}	

	Method5() { bindobj=0; object=0; call_func=0; instance=0; }

	~Method5() { if (bindobj) delete bindobj; }
};



template<class P1,class P2,class P3,class P4>
struct Bind4to5_Base {
	
	virtual SignalTarget *get_instance() const=0;
	virtual _EmptyFunc get_method()const =0;
	virtual void call(P1,P2,P3,P4)=0;
	virtual Bind4to5_Base * copy() const=0;
	virtual ~Bind4to5_Base() {}
};


template<class P1,class P2,class P3,class P4,class P5>
struct Bind4to5 : public Bind4to5_Base<P1,P2,P3,P4> {

	P5 data;
	Method5<P1,P2,P3,P4,P5> method;

	_EmptyFunc get_method() const { return method.get_method(); }
	SignalTarget *get_instance() const { return method.get_instance(); }
	
	void call(P1 p_data1,P2 p_data2,P3 p_data3,P4 p_data4) { method.call(p_data1,p_data2,p_data3,p_data4, data); }
	Bind4to5_Base<P1,P2,P3,P4> * copy() const { 
		Bind4to5<P1,P2,P3,P4,P5> *_copy = new Bind4to5<P1,P2,P3,P4,P5>(method, data);
		*_copy=*this;
		return _copy; 
	}
	
	Bind4to5(const Method5<P1,P2,P3,P4,P5>& p_method, P5 p_data) {

		data = p_data;
		method=p_method;
	}
};

template< class P1,class P2,class P3,class P4 >
class Method4 {
public:

	typedef void (Method4::*LocalMethod)(P1,P2,P3,P4);
	typedef void (*CallFuncPtr)(void *,LocalMethod,P1,P2,P3,P4);

private:


	/* the clever hack that does the magic */
	template<class T>
	static void call_func_template(void *p_object_ptr,LocalMethod p_m,P1 p1,P2 p2,P3 p3,P4 p4) {
		
		T *t = static_cast<T*>(p_object_ptr);
		void (T::*obj_method)(P1,P2,P3,P4)=reinterpret_cast<void (T::*)(P1,P2,P3,P4)>(p_m);
		return (t->*obj_method)(p1,p2,p3,p4);
	}

	SignalTarget *instance; 
	CallFuncPtr call_func;
	LocalMethod method;
	void *object;
	Bind4to5_Base<P1,P2,P3,P4> *bindobj;
public:

	SignalTarget *get_instance() const { return bindobj?bindobj->get_instance():instance; }
	_EmptyFunc get_method() const { return bindobj?bindobj->get_method():get_method_ptr(method); }
	bool is_empty() { return object==0 && bindobj==0; }

	bool operator==(const Method4& p_method) const { return ( get_instance()==p_method.get_instance() && get_method()==p_method.get_method()); }
	bool operator!=(const Method4& p_method) const { return !( *this==p_method ); }
	
	void call(P1 p1,P2 p2,P3 p3,P4 p4) {

		if (bindobj)
			bindobj->call(p1,p2,p3,p4);
		else if (object) {
			call_func(object,method,p1,p2,p3,p4);
		}

	};

	bool is_empty() const  { return (object==0 && bindobj==0); }
	void clear() { object=0; if (bindobj) delete bindobj; bindobj=0; }

	template<class T>
	void set( T* p_instance,void (T::*p_method)(P1,P2,P3,P4) ) {

		// real stuff used to call
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=p_instance;
		object=p_instance;
		method=reinterpret_cast<LocalMethod>(p_method);
		call_func=&call_func_template<T>;
	}

	template<class TBind>
	void bind(const Method5<P1,P2,P3,P4, TBind>& p_method, TBind p_data) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=0;
		object=0;
		method=0;
		bindobj = new Bind4to5<P1,P2,P3,P4, TBind>(p_method, p_data);
	}

	template<class T>
	Method4( T* p_instance,void (T::*p_method)(P1,P2,P3,P4)) {
		bindobj = 0;
		set( p_instance,p_method );
	}

	template<class TBind>
	Method4(const Method5<P1,P2,P3,P4,TBind>& p_method, TBind p_data) {
		bindobj=0; object=0; call_func=0; instance=0;
		bind( p_method, p_data );
		
	}

	void operator=(const Method4<P1,P2,P3,P4>& p_method) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
		
	}

	Method4(const Method4<P1,P2,P3,P4>& p_method) {
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
	}	

	Method4() { bindobj=0; object=0; call_func=0; instance=0; }

	~Method4() { if (bindobj) delete bindobj; }
};



template<class P1,class P2,class P3>
struct Bind3to4_Base {
	
	virtual SignalTarget *get_instance() const=0;
	virtual _EmptyFunc get_method()const =0;
	virtual void call(P1,P2,P3)=0;
	virtual Bind3to4_Base * copy() const=0;
	virtual ~Bind3to4_Base() {}
};


template<class P1,class P2,class P3,class P4>
struct Bind3to4 : public Bind3to4_Base<P1,P2,P3> {

	P4 data;
	Method4<P1,P2,P3,P4> method;

	_EmptyFunc get_method() const { return method.get_method(); }
	SignalTarget *get_instance() const { return method.get_instance(); }
	
	void call(P1 p_data1,P2 p_data2,P3 p_data3) { method.call(p_data1,p_data2,p_data3, data); }
	Bind3to4_Base<P1,P2,P3> * copy() const { 
		Bind3to4<P1,P2,P3,P4> *_copy = new Bind3to4<P1,P2,P3,P4>(method, data);
		*_copy=*this;
		return _copy; 
	}
	
	Bind3to4(const Method4<P1,P2,P3,P4>& p_method, P4 p_data) {

		data = p_data;
		method=p_method;
	}
};

template< class P1,class P2,class P3 >
class Method3 {
public:

	typedef void (Method3::*LocalMethod)(P1,P2,P3);
	typedef void (*CallFuncPtr)(void *,LocalMethod,P1,P2,P3);

private:


	/* the clever hack that does the magic */
	template<class T>
	static void call_func_template(void *p_object_ptr,LocalMethod p_m,P1 p1,P2 p2,P3 p3) {
		
		T *t = static_cast<T*>(p_object_ptr);
		void (T::*obj_method)(P1,P2,P3)=reinterpret_cast<void (T::*)(P1,P2,P3)>(p_m);
		return (t->*obj_method)(p1,p2,p3);
	}

	SignalTarget *instance; 
	CallFuncPtr call_func;
	LocalMethod method;
	void *object;
	Bind3to4_Base<P1,P2,P3> *bindobj;
public:

	SignalTarget *get_instance() const { return bindobj?bindobj->get_instance():instance; }
	_EmptyFunc get_method() const { return bindobj?bindobj->get_method():get_method_ptr(method); }
	bool is_empty() { return object==0 && bindobj==0; }

	bool operator==(const Method3& p_method) const { return ( get_instance()==p_method.get_instance() && get_method()==p_method.get_method()); }
	bool operator!=(const Method3& p_method) const { return !( *this==p_method ); }
	
	void call(P1 p1,P2 p2,P3 p3) {

		if (bindobj)
			bindobj->call(p1,p2,p3);
		else if (object) {
			call_func(object,method,p1,p2,p3);
		}

	};

	bool is_empty() const  { return (object==0 && bindobj==0); }
	void clear() { object=0; if (bindobj) delete bindobj; bindobj=0; }

	template<class T>
	void set( T* p_instance,void (T::*p_method)(P1,P2,P3) ) {

		// real stuff used to call
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=p_instance;
		object=p_instance;
		method=reinterpret_cast<LocalMethod>(p_method);
		call_func=&call_func_template<T>;
	}

	template<class TBind>
	void bind(const Method4<P1,P2,P3, TBind>& p_method, TBind p_data) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=0;
		object=0;
		method=0;
		bindobj = new Bind3to4<P1,P2,P3, TBind>(p_method, p_data);
	}

	template<class T>
	Method3( T* p_instance,void (T::*p_method)(P1,P2,P3)) {
		bindobj = 0;
		set( p_instance,p_method );
	}

	template<class TBind>
	Method3(const Method4<P1,P2,P3,TBind>& p_method, TBind p_data) {
		bindobj=0; object=0; call_func=0; instance=0;
		bind( p_method, p_data );
		
	}

	void operator=(const Method3<P1,P2,P3>& p_method) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
		
	}

	Method3(const Method3<P1,P2,P3>& p_method) {
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
	}	

	Method3() { bindobj=0; object=0; call_func=0; instance=0; }

	~Method3() { if (bindobj) delete bindobj; }
};



template<class P1,class P2>
struct Bind2to3_Base {
	
	virtual SignalTarget *get_instance() const=0;
	virtual _EmptyFunc get_method()const =0;
	virtual void call(P1,P2)=0;
	virtual Bind2to3_Base * copy() const=0;
	virtual ~Bind2to3_Base() {}
};


template<class P1,class P2,class P3>
struct Bind2to3 : public Bind2to3_Base<P1,P2> {

	P3 data;
	Method3<P1,P2,P3> method;

	_EmptyFunc get_method() const { return method.get_method(); }
	SignalTarget *get_instance() const { return method.get_instance(); }
	
	void call(P1 p_data1,P2 p_data2) { method.call(p_data1,p_data2, data); }
	Bind2to3_Base<P1,P2> * copy() const { 
		Bind2to3<P1,P2,P3> *_copy = new Bind2to3<P1,P2,P3>(method, data);
		*_copy=*this;
		return _copy; 
	}
	
	Bind2to3(const Method3<P1,P2,P3>& p_method, P3 p_data) {

		data = p_data;
		method=p_method;
	}
};

template< class P1,class P2 >
class Method2 {
public:

	typedef void (Method2::*LocalMethod)(P1,P2);
	typedef void (*CallFuncPtr)(void *,LocalMethod,P1,P2);

private:


	/* the clever hack that does the magic */
	template<class T>
	static void call_func_template(void *p_object_ptr,LocalMethod p_m,P1 p1,P2 p2) {
		
		T *t = static_cast<T*>(p_object_ptr);
		void (T::*obj_method)(P1,P2)=reinterpret_cast<void (T::*)(P1,P2)>(p_m);
		return (t->*obj_method)(p1,p2);
	}

	SignalTarget *instance; 
	CallFuncPtr call_func;
	LocalMethod method;
	void *object;
	Bind2to3_Base<P1,P2> *bindobj;
public:

	SignalTarget *get_instance() const { return bindobj?bindobj->get_instance():instance; }
	_EmptyFunc get_method() const { return bindobj?bindobj->get_method():get_method_ptr(method); }
	bool is_empty() { return object==0 && bindobj==0; }

	bool operator==(const Method2& p_method) const { return ( get_instance()==p_method.get_instance() && get_method()==p_method.get_method()); }
	bool operator!=(const Method2& p_method) const { return !( *this==p_method ); }
	
	void call(P1 p1,P2 p2) {

		if (bindobj)
			bindobj->call(p1,p2);
		else if (object) {
			call_func(object,method,p1,p2);
		}

	};

	bool is_empty() const  { return (object==0 && bindobj==0); }
	void clear() { object=0; if (bindobj) delete bindobj; bindobj=0; }

	template<class T>
	void set( T* p_instance,void (T::*p_method)(P1,P2) ) {

		// real stuff used to call
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=p_instance;
		object=p_instance;
		method=reinterpret_cast<LocalMethod>(p_method);
		call_func=&call_func_template<T>;
	}

	template<class TBind>
	void bind(const Method3<P1,P2, TBind>& p_method, TBind p_data) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=0;
		object=0;
		method=0;
		bindobj = new Bind2to3<P1,P2, TBind>(p_method, p_data);
	}

	template<class T>
	Method2( T* p_instance,void (T::*p_method)(P1,P2)) {
		bindobj = 0;
		set( p_instance,p_method );
	}

	template<class TBind>
	Method2(const Method3<P1,P2,TBind>& p_method, TBind p_data) {
		bindobj=0; object=0; call_func=0; instance=0;
		bind( p_method, p_data );
		
	}

	void operator=(const Method2<P1,P2>& p_method) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
		
	}

	Method2(const Method2<P1,P2>& p_method) {
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
	}	

	Method2() { bindobj=0; object=0; call_func=0; instance=0; }

	~Method2() { if (bindobj) delete bindobj; }
};



template<class P1>
struct Bind1to2_Base {
	
	virtual SignalTarget *get_instance() const=0;
	virtual _EmptyFunc get_method()const =0;
	virtual void call(P1)=0;
	virtual Bind1to2_Base * copy() const=0;
	virtual ~Bind1to2_Base() {}
};


template<class P1,class P2>
struct Bind1to2 : public Bind1to2_Base<P1> {

	P2 data;
	Method2<P1,P2> method;

	_EmptyFunc get_method() const { return method.get_method(); }
	SignalTarget *get_instance() const { return method.get_instance(); }
	
	void call(P1 p_data1) { method.call(p_data1, data); }
	Bind1to2_Base<P1> * copy() const { 
		Bind1to2<P1,P2> *_copy = new Bind1to2<P1,P2>(method, data);
		*_copy=*this;
		return _copy; 
	}
	
	Bind1to2(const Method2<P1,P2>& p_method, P2 p_data) {

		data = p_data;
		method=p_method;
	}
};

template< class P1 >
class Method1 {
public:

	typedef void (Method1::*LocalMethod)(P1);
	typedef void (*CallFuncPtr)(void *,LocalMethod,P1);

private:


	/* the clever hack that does the magic */
	template<class T>
	static void call_func_template(void *p_object_ptr,LocalMethod p_m,P1 p1) {
		
		T *t = static_cast<T*>(p_object_ptr);
		void (T::*obj_method)(P1)=reinterpret_cast<void (T::*)(P1)>(p_m);
		return (t->*obj_method)(p1);
	}

	SignalTarget *instance; 
	CallFuncPtr call_func;
	LocalMethod method;
	void *object;
	Bind1to2_Base<P1> *bindobj;
public:

	SignalTarget *get_instance() const { return bindobj?bindobj->get_instance():instance; }
	_EmptyFunc get_method() const { return bindobj?bindobj->get_method():get_method_ptr(method); }
	bool is_empty() { return object==0 && bindobj==0; }

	bool operator==(const Method1& p_method) const { return ( get_instance()==p_method.get_instance() && get_method()==p_method.get_method()); }
	bool operator!=(const Method1& p_method) const { return !( *this==p_method ); }
	
	void call(P1 p1) {

		if (bindobj)
			bindobj->call(p1);
		else if (object) {
			call_func(object,method,p1);
		}

	};

	bool is_empty() const  { return (object==0 && bindobj==0); }
	void clear() { object=0; if (bindobj) delete bindobj; bindobj=0; }

	template<class T>
	void set( T* p_instance,void (T::*p_method)(P1) ) {

		// real stuff used to call
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=p_instance;
		object=p_instance;
		method=reinterpret_cast<LocalMethod>(p_method);
		call_func=&call_func_template<T>;
	}

	template<class TBind>
	void bind(const Method2<P1, TBind>& p_method, TBind p_data) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=0;
		object=0;
		method=0;
		bindobj = new Bind1to2<P1, TBind>(p_method, p_data);
	}

	template<class T>
	Method1( T* p_instance,void (T::*p_method)(P1)) {
		bindobj = 0;
		set( p_instance,p_method );
	}

	template<class TBind>
	Method1(const Method2<P1,TBind>& p_method, TBind p_data) {
		bindobj=0; object=0; call_func=0; instance=0;
		bind( p_method, p_data );
		
	}

	void operator=(const Method1<P1>& p_method) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
		
	}

	Method1(const Method1<P1>& p_method) {
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
	}	

	Method1() { bindobj=0; object=0; call_func=0; instance=0; }

	~Method1() { if (bindobj) delete bindobj; }
};



/**
 * -Simple Method Pointer (no parameters)
 * usage:
 *
 * Method my_method_ptr;
 *
 * -Connect to a method "desired_method", where instance inherits from SignalTarget
 *
 * my_method_ptr.set( instance , &Class::desired_method );
 *
 * or alternatively:
 *
 * my_method_ptr = MethodPtr<ClassName>( instance , &ClassName::desired_method );
 * 
 * -Call the method
 *
 * my_method_ptr.call();
 *
 * NOTE: Return type must be always void
 */

/** Bind Method1 to Method0 **/

struct Bind0to1_Base {
	
	virtual SignalTarget *get_instance() const=0;
	virtual _EmptyFunc get_method()const =0;
	virtual void call()=0;
	virtual Bind0to1_Base * copy() const=0;
	virtual ~Bind0to1_Base() {}
};

template<class P1>
struct Bind0to1 : public Bind0to1_Base {
	
	P1 data1;
	Method1<P1> method;

	_EmptyFunc get_method() const { return method.get_method(); }
	SignalTarget *get_instance() const { return method.get_instance(); }
	
	void call() { method.call(data1); }
	Bind0to1_Base * copy() const { 
		Bind0to1<P1> *_copy = new Bind0to1<P1>(method,data1);
		*_copy=*this;
		return _copy; 
	}
	
	Bind0to1(const Method1<P1>& p_method1, P1 p_data1) {data1=p_data1; method=p_method1; }
};

class Method {
public:

	typedef void (Method::*LocalMethod)();
	typedef void (*CallFuncPtr)(void *,LocalMethod);

private:


	/* the clever hack that does the magic */
	template<class T>
	static void call_func_template(void *p_object_ptr,LocalMethod p_m) {
		
		T *t = static_cast<T*>(p_object_ptr);
		void (T::*obj_method)()=reinterpret_cast<void (T::*)()>(p_m);
		return (t->*obj_method)();
	}

	SignalTarget *instance; 
	CallFuncPtr call_func;
	LocalMethod method;
	void *object;
	Bind0to1_Base *bindobj;
public:

	SignalTarget *get_instance() const { return bindobj?bindobj->get_instance():instance; }
	_EmptyFunc get_method() const { return bindobj?bindobj->get_method():get_method_ptr(method); }
	bool is_empty() { return object==0 && bindobj==0; }
	
	bool operator==(const Method& p_method) const { return get_method()==p_method.get_method() && get_instance()==p_method.get_instance();}
	bool operator!=(const Method& p_method) const { return !( *this==p_method ); }
	
	void call() {
		if (bindobj)
			bindobj->call();
		else if (object) {
			
			call_func(object,method);
		}

	};

	bool is_empty() const  { return (object==0 && bindobj==0); }
	void clear() { object=0; if (bindobj) delete bindobj; bindobj=0; }

	template<class T>
	void set( T* p_instance,void (T::*p_method)() ) {

		// real stuff used to call
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=p_instance;
		object=p_instance;
		method=reinterpret_cast<LocalMethod>(p_method);
		call_func=&call_func_template<T>;
	}

	template<class P1>
	void bind(const Method1<P1>& p_method1, P1 p_data1) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		instance=0;
		object=0;
		method=0;
		bindobj = new Bind0to1<P1>(p_method1,p_data1);		
	}
	
	template<class T>
	Method( T* p_instance,void (T::*p_method)()) {
		bindobj=0;
		set( p_instance,p_method );
		
	}

	template<class P1>
	Method(const Method1<P1>& p_method1, P1 p_data1) {
		bindobj=0; object=0; call_func=0; instance=0;
		bind( p_method1,p_data1 );
		
	}
	
	void operator=(const Method& p_method) {
		
		if (bindobj)
			delete bindobj;
		bindobj=0;
		
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
		
	}
	Method() { bindobj=0; object=0; call_func=0; instance=0; }
	Method(const Method& p_method) {
		if (p_method.bindobj) {
			
			bindobj=p_method.bindobj->copy();
			object=0; call_func=0; instance=0;
			
		} else {
			
			object=p_method.object;
			call_func=p_method.call_func;
			instance=p_method.instance;
			method = p_method.method;
			bindobj=0;
		}
	}	
	
	~Method() { if (bindobj) delete bindobj; }
};


/**
 * SignalTarget:
 *
 * Signals can only be connected to objects inheriting from this class.
 * (normal or multiple inheritance is fine)
 * This is so, when the object dies, signals pointing to it are cleaned up,
 * so it becomes impossible (or at least very hard) to call a no longer existing instance.
 */


class SignalBase {
protected:
friend class SignalTarget;

	virtual void remove_target( SignalTarget *p_target )=0;

	bool register_at_target( SignalTarget *p_target ); //return true if failed! (signal already connected to
	void remove_from_target( SignalTarget *p_target );
	
	virtual ~SignalBase() {};

};


/*
	signal_call
	N: 5
*/





template<class M=Method>
class Signal : public SignalBase {

	struct Connection {

		SignalTarget *instance;
		Connection *next;
		M method;
		
		bool removed; //workaround to avoid a complex situation (target is deleted while in call)

		Connection() { next=0; removed=false; instance=0; }
	};


	Connection *conn_list;
	
	int call_ref; // each time we call, increments by one, then decrements by one
	bool remotion_on_call; //workaround to avoid a complex situation (target is deleted while in call)

	virtual void remove_target( SignalTarget *p_target ) {

		Connection *c = conn_list;
				
		while (c) {

			
			if (c->instance==p_target) {
				
				c->removed=true;
			}
			
			c=c->next;
			
		}
		
		//make sure we are not inside a call. if we are, the call will erase this later
		if (call_ref==0) 
			remove_pending();
		else
			remotion_on_call=true;
		
	}
	
	void remove_pending() {

		Connection *c = conn_list;
		Connection **cp = &conn_list;
				
		while (c) {

			
			if (c->removed) {

				Connection *aux=c;
				*cp=c->next;
				c=c->next;

				remove_from_target( aux->instance );
				delete aux; //good bye connection

				continue;
				
			}

			cp=&c->next;
			c=c->next;
			
		}
		
		remotion_on_call=false;
	}
	
	/// for now, you can't really copy a signal to another 
	Signal(const Signal& p_cc);
	void operator=(const Signal& p_cc);
public:
	
	
	template<class T, class U>
	void connect( T* p_instance, U p_method ) {
		
		connect( M(p_instance,p_method) );
	}
	
	void connect( const M& p_method ) {
	
		M m=p_method;
		
		if (call_ref>0) //cant connect while call-ref-ing
			return;
		
		Connection *c = conn_list;
		Connection *last=0;

		/* Must check beforehand if we have this method */
		
		while (c) {
			
			if (c->method==m) {
				return; // already connected there! dont do it again
			}

			if (!c->next)
				last=c;
			c=c->next;
		}

		register_at_target( m.get_instance() );
			//instance already has us? do we have that method?


		c = new Connection;
		c->method=m;
		c->instance=m.get_instance();
		if (conn_list && last) {
			
			last->next=c;
		} else {
			c->next=conn_list;
			conn_list=c;
		}

	}

	
	template<class T,class U>
	void disconnect(T p_instance, U p_method_ptr ) {

		disconnect( M( p_instance, p_method_ptr ) );
	}
	
	void disconnect( const M& p_method ) {

		Connection *c=conn_list;
				
		while (c) {

			
			if (c->method==p_method) {

				
				c->removed=true;
				break;
			}
			
			c=c->next;
		}
						
		// unless inside a call, cleanup! otherwise autocleanup will happen later
		
		if (call_ref==0) 
			remove_pending();
		else
			remotion_on_call=true;
		
	}
	
	/* You can only call the right method! using the wrong one will cause a compiler error! */
	
	void call() {
		
		call_ref++;
		
		Connection *c = conn_list;
		while (c) {
			
			if (!c->removed) //removed in a previous call
				c->method.call();
			c=c->next;
		}	
		call_ref--;
		
		if (call_ref==0 && remotion_on_call)
			remove_pending();
			
	}

	template<class P1>
	void call(P1 p1) {

		call_ref++;

		Connection *c = conn_list;


		while (c) {


			if (!c->removed) //removed in a previous call
				c->method.call(p1);
			c=c->next;
		}		


		call_ref--;

		if (call_ref==0 && remotion_on_call)
			remove_pending();

	}

	template<class P1,class P2>
	void call(P1 p1,P2 p2) {

		call_ref++;

		Connection *c = conn_list;

		while (c) {

			if (!c->removed) //removed in a previous call
				c->method.call(p1,p2);
			c=c->next;
		}		

		call_ref--;

		if (call_ref==0 && remotion_on_call)
			remove_pending();

	}

	template<class P1,class P2,class P3>
	void call(P1 p1,P2 p2,P3 p3) {

		call_ref++;

		Connection *c = conn_list;

		while (c) {

			if (!c->removed) //removed in a previous call
				c->method.call(p1,p2,p3);
			c=c->next;
		}		

		call_ref--;

		if (call_ref==0 && remotion_on_call)
			remove_pending();

	}

	template<class P1,class P2,class P3,class P4>
	void call(P1 p1,P2 p2,P3 p3,P4 p4) {

		call_ref++;

		Connection *c = conn_list;

		while (c) {

			if (!c->removed) //removed in a previous call
				c->method.call(p1,p2,p3,p4);
			c=c->next;
		}		

		call_ref--;

		if (call_ref==0 && remotion_on_call)
			remove_pending();

	}

		template<class P1,class P2,class P3,class P4,class P5>
	void call(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5) {

		call_ref++;

		Connection *c = conn_list;

		while (c) {

			if (!c->removed) //removed in a previous call
				c->method.call(p1,p2,p3,p4,p5);
			c=c->next;
		}		

		call_ref--;

		if (call_ref==0 && remotion_on_call)
			remove_pending();

	}

	template<class P1,class P2,class P3,class P4,class P5,class P6>
	void call(P1 p1,P2 p2,P3 p3,P4 p4,P5 p5,P6 p6) {

		call_ref++;

		Connection *c = conn_list;

		while (c) {

			if (!c->removed) //removed in a previous call
				c->method.call(p1,p2,p3,p4,p5,p6);
			c=c->next;
		}		

		call_ref--;

		if (call_ref==0 && remotion_on_call)
			remove_pending();

	}

	void clear() {

		if (call_ref>0) {
			
			PRINT_ERROR(" DO NOT CLEAR A SIGNAL IN THE MIDDLE OF A CALL() ");
			
		}
		
		Connection *c = conn_list;
				
		while (c) {
			
			c->removed=true;
			c=c->next;
		}
		remove_pending();
	};
	
	Signal() {

		conn_list=0;
		call_ref=0; 
		remotion_on_call=false;		
	}
	
	
	~Signal() {
		
		if (call_ref>0) {
			
			PRINT_ERROR(" DO NOT DELETE A SIGNAL IN THE MIDDLE OF A CALL() ");
			
		}

		clear();
		
	};

};



}
		
#else

#include GUI_EXTERNAL_SIGNAL

#endif		
#endif



