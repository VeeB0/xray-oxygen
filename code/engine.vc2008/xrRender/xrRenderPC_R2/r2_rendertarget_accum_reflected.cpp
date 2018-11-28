#include "stdafx.h"

void CRenderTarget::accum_reflected(light* L)
{
	phase_accumulator();
	RImplementation.stats.l_visible++;

	// *** assume accumulator setted up ***
	// *****************************	Mask by stencil		*************************************
	ref_shader		shader = s_accum_reflected;

	bool	bIntersect = false; //enable_scissor(L);
	L->xform_calc();
	RCache.set_xform_world(L->m_xform);
	RCache.set_xform_view((Device.mView));
	RCache.set_xform_project((Device.mProject));
	bIntersect = enable_scissor(L);
	enable_dbt_bounds(L);

	// *****************************	Minimize overdraw	*************************************
	// Select shader (front or back-faces), *** back, if intersect near plane
	RCache.set_ColorWriteEnable();
	RCache.set_CullMode(bIntersect ? CULL_CW : CULL_CCW);		// back or front

	// 2D texgen (texture adjustment matrix)
	Matrix4x4			m_Texgen;
	{
		float	_w = float(Device.dwWidth);
		float	_h = float(Device.dwHeight);
		float	o_w = (.5f / _w);
		float	o_h = (.5f / _h);
		Matrix4x4			m_TexelAdjust =
		{
			0.5f,				0.0f,				0.0f,			0.0f,
			0.0f,				-0.5f,				0.0f,			0.0f,
			0.0f,				0.0f,				1.0f,			0.0f,
			0.5f + o_w,			0.5f + o_h,			0.0f,			1.0f
		};
		m_Texgen.Multiply(RCache.xforms.m_wvp, m_TexelAdjust);
	}

	// Common constants
	Fvector		L_dir, L_clr, L_pos;	float L_spec;
	L_clr.set(L->color.r, L->color.g, L->color.b);
	L_spec = u_diffuse2s(L_clr);
	Device.mView.TransformTiny(L_pos, L->position);
	Device.mView.TransformDir(L_dir, L->direction);
	L_dir.normalize();

	{
		// Lighting
		RCache.set_Shader(shader);

		// Constants
		RCache.set_c("Ldynamic_pos", L_pos.x, L_pos.y, L_pos.z, 1 / (L->range*L->range));
		RCache.set_c("Ldynamic_color", L_clr.x, L_clr.y, L_clr.z, L_spec);
		RCache.set_c("direction", L_dir.x, L_dir.y, L_dir.z, 0);
		RCache.set_c("m_texgen", m_Texgen);

		RCache.set_Stencil(TRUE, D3DCMP_LESSEQUAL, 0x01, 0xff, 0x00, D3DSTENCILOP_KEEP, D3DSTENCILOP_KEEP, D3DSTENCILOP_KEEP);
		draw_volume(L);
	}

	// blend-copy
	if (!RImplementation.o.fp16_blend) 
	{
		u_setrt(rt_Accumulator, NULL, NULL, HW.pBaseZB);
		RCache.set_Element(s_accum_mask->E[SE_MASK_ACCUM_VOL]);
		RCache.set_c("m_texgen", m_Texgen);
		draw_volume(L);
	}

	// 
	u_DBT_disable();
}
