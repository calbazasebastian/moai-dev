// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USLUAOBJECT_H
#define	USLUAOBJECT_H

#include <uslscore/STLString.h>
#include <uslscore/USLuaRef.h>
#include <uslscore/USObject.h>

class USLuaClass;
class USLuaSerializer;
class USLuaState;
class USLuaStateHandle;

//================================================================//
// USLuaLocal
//================================================================//
class USLuaLocal {
private:

	friend class USLuaObject;
	int mRef;

public:
	
	//----------------------------------------------------------------//
			USLuaLocal		();
			~USLuaLocal		();
	
	//----------------------------------------------------------------//
	inline operator bool () {
		return ( this->mRef != LUA_NOREF );
	}
};

//================================================================//
// USLuaObject
//================================================================//
class USLuaObject :
	public virtual USObject {
private:

	USLuaLocal		mContain;

protected:

	
	USLuaRef		mInstanceTable;		// ref to instance table (weak for factory class instances; strong for singletons)
	USLuaRef		mUserdata;			// ref to userdata (weak)
	

	//----------------------------------------------------------------//
	static int				_gc						( lua_State* L );
	static int				_getClass				( lua_State* L );
	static int				_getClassName			( lua_State* L );
	static int				_index					( lua_State* L );
	static int				_newindex				( lua_State* L );
	static int				_tombstone				( lua_State* L );
	static int				_tostring				( lua_State* L );

	//----------------------------------------------------------------//
	void					ClearLocal			( USLuaLocal& ref );
	void					OnRelease			( u32 refCount );
	void					OnRetain			( u32 refCount );
	bool					PushLocal			( USLuaState& state, USLuaLocal& ref );
	void					PushMemberTable		( USLuaState& state );
	void					SetLocal			( USLuaState& state, int idx, USLuaLocal& ref );
	void					SetMemberTable		( USLuaState& state, int idx );

public:

	friend class USLuaClass;
	friend class USLuaSerializer;

	//----------------------------------------------------------------//
	void					BindToLuaWithTable		( USLuaState& state ); // push table at top of stack!
	virtual USLuaClass*		GetLuaClass				();
	USLuaStateHandle		GetSelf					();
	bool					IsBound					();
	void					LockToRefCount			();
	void					LuaRelease				( USLuaObject& object );
	void					LuaRetain				( USLuaObject& object );
	void					LuaUnbind				();
	void					PushLuaClassTable		( USLuaState& state );
	void					PushLuaUserdata			( USLuaState& state );
	virtual void			RegisterLuaClass		( USLuaState& state );
	virtual void			RegisterLuaFuncs		( USLuaState& state );
	static void             ReportLeaks				( FILE *f, bool clearAfter );
	virtual	void			SerializeIn				( USLuaState& state, USLuaSerializer& serializer );
	virtual	void			SerializeOut			( USLuaState& state, USLuaSerializer& serializer );
							USLuaObject				();
	virtual					~USLuaObject			();
};

//================================================================//
// USLuaClass
//================================================================//
class USLuaClass :
	public USObject {
protected:

	USLuaRef	mClassTable;		// global factory class for type
	USLuaRef	mInterfaceTable;	// interface shared by all instances of type

	//----------------------------------------------------------------//
	void				InitLuaFactoryClass			( USLuaObject& data, USLuaState& state );
	void				InitLuaSingletonClass		( USLuaObject& data, USLuaState& state );
	void				PushInterfaceTable			( USLuaState& state );
	virtual void		RegisterLuaClass			( USLuaState& state ) = 0;

public:

	friend class USLuaObject;

	//----------------------------------------------------------------//
	virtual USLuaObject*	GetSingleton			();
	bool					IsSingleton				();
	virtual void			Register				() = 0;
							USLuaClass				();
	virtual					~USLuaClass				();
};

#endif
