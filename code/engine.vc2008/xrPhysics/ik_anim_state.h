#pragma once
class	CBlend;
class	ENGINE_API	motion_marks;
class	IKinematicsAnimated;
class	XRPHYSICS_API ik_anim_state
{
	bool is_step;
	bool do_glue;
	bool is_idle;
	bool is_blending;
	const CBlend	*current_blend;
public:
	ik_anim_state() : is_step(false), do_glue(true), is_idle(false), is_blending(false), current_blend(0) {};
	void update(IKinematicsAnimated *K, const	CBlend *b, u16 interval);
	inline bool step() const { return is_step; }
	inline bool glue() const { return do_glue; }
	inline bool idle() const { return is_idle; }
	inline bool blending() const { return	is_blending; }
	inline bool auto_unstuck() const { return true /*|| idle() || blending()*/; }
	static bool time_step_begin(IKinematicsAnimated *K, const CBlend& B, u16 limb_id, float &time);
};
