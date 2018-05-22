#ifndef PHJOINT_DESTROY_INFO_H
#define PHJOINT_DESTROY_INFO_H

#include "../../3rd-party/ode/include/ode/common.h"
class CPHJointDestroyInfo
{
	friend class CPHShellSplitterHolder;
	friend class CPHShell;
	dJointFeedback m_joint_feedback;
	float m_sq_break_force;
	float m_sq_break_torque;
	u16 m_end_element;
	u16 m_end_joint;
	bool m_breaked;
public:
	CPHJointDestroyInfo(float break_force, float break_torque);
	inline dJointFeedback*		JointFeedback() { return &m_joint_feedback; }

	inline	bool Breaked() { return m_breaked; };
	bool Update();
};

#endif PHJOINT_DESTROY_INFO_H