////////////////////////////////////////////////////////////
//
//    Created:   March 2018
//    Copyright: CCP 2018
//

#pragma once

#include "ITr2Controller.h"

BLUE_DECLARE( Tr2ControllerFloatVariable );
BLUE_DECLARE_VECTOR( Tr2ControllerFloatVariable );
BLUE_DECLARE( Tr2StateMachine );
BLUE_DECLARE_VECTOR( Tr2StateMachine );
BLUE_DECLARE_INTERFACE( ITr2Updateable );


BLUE_CLASS( Tr2Controller ):
	public ITr2Controller,
	public IListNotify
{
public:
	Tr2Controller( IRoot* lockobj = nullptr );

	EXPOSE_TO_BLUE();

	virtual void OnListModified( long event, ssize_t key, ssize_t key2, IRoot* value, const IList* list );

	virtual void Link( IRoot& owner );
	virtual void Unlink();

	virtual void Start();
	virtual void Stop();
	virtual void Update();

	virtual void SetVariable( const char* name, float value );

	IRoot* GetOwner() const;
	Tr2ControllerFloatVariable* GetVariableByName( const char* name ) const;

	const PTr2ControllerFloatVariableVector& GetVariables() const;
	void GetBindingPathRoots( std::unordered_map<std::string, IRoot*>& variables ) const;

	void RegisterUpdateable( ITr2Updateable& updateable );
	void UnRegisterUpdateable( ITr2Updateable& updateable );
private:
	std::string m_name;
	PTr2StateMachineVector m_stateMachines;
	PTr2ControllerFloatVariableVector m_variables;
	
	TrackableStdSet<ITr2UpdateablePtr> m_updateables;

	IRoot* m_owner;
	bool m_isActive;
	bool m_isShared;
};

TYPEDEF_BLUECLASS( Tr2Controller );
