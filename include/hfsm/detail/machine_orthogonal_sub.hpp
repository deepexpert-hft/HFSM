namespace hfsm {
namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <StateID, ForkID, ForkID, typename, ShortIndex, typename...>
struct _OS;

//------------------------------------------------------------------------------

template <StateID NInitialID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial,
		  typename... TRemaining>
struct _OS<NInitialID, NCompoID, NOrthoID, TArgs, NIndex, TInitial, TRemaining...> {
	static constexpr StateID   INITIAL_ID	= NInitialID;
	static constexpr StateID   COMPO_ID		= NCompoID;
	static constexpr StateID   ORTHO_ID		= NOrthoID;
	static constexpr LongIndex PRONG_INDEX	= NIndex;

	using Args				= TArgs;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateParents		= Array<Parent, StateList::SIZE>;
	using PlanControl		= PlanControlT<Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using FullControl		= FullControlT<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using Initial			= typename WrapMaterial<INITIAL_ID, COMPO_ID, ORTHO_ID, Args, TInitial>::Type;
	using InitialForward	= typename WrapForward<TInitial>::Type;
	using Remaining			= _OS<INITIAL_ID + InitialForward::STATE_COUNT,
								  COMPO_ID + InitialForward::COMPOSITE_COUNT,
								  ORTHO_ID + InitialForward::ORTHOGONAL_COUNT,
								  Args, PRONG_INDEX + 1, TRemaining...>;
	using Forward			= _OSF<TInitial, TRemaining...>;

	_OS(StateParents& stateParents,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void	  wideForwardGuard		(const ShortIndex prong,
										 FullControl& control);

	inline void	  wideForwardGuard		(FullControl& control);
	inline void	  wideGuard				(FullControl& control);

	inline void	  wideEnterInitial		(PlanControl& control);
	inline void	  wideEnter				(PlanControl& control);

	inline Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const TEvent& event,
										 FullControl& control);

	inline void   wideExit				(PlanControl& control);

	inline void   wideForwardRequest	(const ShortIndex prong, const TransitionType transition);
	inline void   wideRequestRemain		();
	inline void   wideRequestRestart	();
	inline void   wideRequestResume		();
	inline void   wideChangeToRequested	(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT  + Remaining::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
	Remaining remaining;
};

//------------------------------------------------------------------------------

template <StateID NInitialID,
		  ForkID NCompoID,
		  ForkID NOrthoID,
		  typename TArgs,
		  ShortIndex NIndex,
		  typename TInitial>
struct _OS<NInitialID, NCompoID, NOrthoID, TArgs, NIndex, TInitial> {
	static constexpr StateID   INITIAL_ID	= NInitialID;
	static constexpr StateID   COMPO_ID		= NCompoID;
	static constexpr StateID   ORTHO_ID		= NOrthoID;
	static constexpr LongIndex PRONG_INDEX	= NIndex;

	using Args				= TArgs;

	using Context			= typename Args::Context;
	using Config			= typename Args::Config;
	using StateList			= typename Args::StateList;
	using PayloadList		= typename Args::PayloadList;

	using StateParents		= Array<Parent, StateList::SIZE>;
	using PlanControl		= PlanControlT<Context, StateList, Args::FORK_COUNT, Args::PLAN_CAPACITY>;
	using Transition		= TransitionT<PayloadList>;
	using TransitionType	= typename Transition::Type;
	using FullControl		= FullControlT<Context, StateList, Args::FORK_COUNT, PayloadList, Args::PLAN_CAPACITY>;

	using Initial			= typename WrapMaterial<INITIAL_ID, COMPO_ID, ORTHO_ID, Args, TInitial>::Type;
	using Forward			= _OSF<TInitial>;

	_OS(StateParents& stateParents,
		const ShortIndex fork,
		Parents& forkParents,
		ForkPointers& forkPointers);

	inline void   wideForwardGuard		(const ShortIndex prong,
										 FullControl& control);

	inline void   wideForwardGuard		(FullControl& control);
	inline void   wideGuard				(FullControl& control);

	inline void   wideEnterInitial		(PlanControl& control);
	inline void   wideEnter				(PlanControl& control);

	inline Status wideUpdate			(FullControl& control);

	template <typename TEvent>
	inline void   wideReact				(const TEvent& event,
										 FullControl& control);

	inline void   wideExit				(PlanControl& control);

	inline void   wideForwardRequest	(const ShortIndex prong, const TransitionType transition);
	inline void   wideRequestRemain		();
	inline void   wideRequestRestart	();
	inline void   wideRequestResume		();
	inline void   wideChangeToRequested	(PlanControl& control);

#ifdef HFSM_ENABLE_STRUCTURE_REPORT
	static constexpr LongIndex NAME_COUNT	 = Initial::NAME_COUNT;

	void wideGetNames(const LongIndex parent,
					  const ShortIndex depth,
					  StructureStateInfos& stateInfos) const;
#endif

	Initial initial;
};

////////////////////////////////////////////////////////////////////////////////

}
}

#include "machine_orthogonal_sub_1.inl"
#include "machine_orthogonal_sub_2.inl"
