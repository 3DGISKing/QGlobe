// Copyright (C) 2002-2008 Nikolaus Gebhardt
// This file is part of the "Geo Engine".
// For conditions of distribution and use, see copyright notice in Geo.h

#ifndef __C_OPEN_GL_FEATURE_MAP_H_INCLUDED__
#define __C_OPEN_GL_FEATURE_MAP_H_INCLUDED__

#include "geoCompileConfig.h"
#ifdef _GEO_COMPILE_WITH_OPENGL_

#include "EDriverFeatures.h"
#include "geoTypes.h"
#include "os.h"

#if defined(_GEO_WINDOWS_API_)
	// include windows headers for HWND
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <GL/gl.h>
	#include "glext.h"
#ifdef _MSC_VER
	#pragma comment(lib, "OpenGL32.lib")
#endif
#elif defined(_GEO_USE_OSX_DEVICE_)
	//kimyi : Use QT
	//#include "CGeoDeviceMacOSX.h"
	#if defined(_GEO_OPENGL_USE_EXTPOINTER_)
		#define GL_GLEXT_LEGACY 1
	#endif
	#include <OpenGL/gl.h>
	#if defined(_GEO_OPENGL_USE_EXTPOINTER_)
		#include "glext.h"
	#endif
#elif defined(_GEO_USE_SDL_DEVICE_)
	#if defined(_GEO_OPENGL_USE_EXTPOINTER_)
		#define GL_GLEXT_LEGACY 1
		#define GLX_GLXEXT_LEGACY 1
	#else
		#define GL_GLEXT_PROTOTYPES 1
		#define GLX_GLXEXT_PROTOTYPES 1
	#endif
	#define NO_SDL_GLEXT
	#include <SDL/SDL_video.h>
	#include <SDL/SDL_opengl.h>
	#include "glext.h"
#else
	#if defined(_GEO_OPENGL_USE_EXTPOINTER_)
		#define GL_GLEXT_LEGACY 1
		#define GLX_GLXEXT_LEGACY 1
	#else
		#define GL_GLEXT_PROTOTYPES 1
		#define GLX_GLXEXT_PROTOTYPES 1
	#endif
	#include <GL/gl.h>
	#include <GL/glx.h>
	#if defined(_GEO_OPENGL_USE_EXTPOINTER_)
	#include "glext.h"
	#undef GLX_ARB_get_proc_address // avoid problems with local glxext.h
	#include "glxext.h"
	#endif
#endif


namespace geo
{
namespace video
{

static const char* const OpenGLFeatureStrings[] = {
	"GL_3DFX_multisample",
	"GL_3DFX_tbuffer",
	"GL_3DFX_texture_compression_FXT1",
	"GL_APPLE_client_storage",
	"GL_APPLE_element_array",
	"GL_APPLE_fence",
	"GL_APPLE_flush_buffer_range",
	"GL_APPLE_specular_vector",
	"GL_APPLE_transform_hint",
	"GL_APPLE_vertex_array_object",
	"GL_APPLE_vertex_array_range",
	"GL_APPLE_ycbcr_422",
	"GL_ARB_color_buffer_float",
	"GL_ARB_depth_buffer_float",
	"GL_ARB_depth_texture",
	"GL_ARB_draw_buffers",
	"GL_ARB_draw_instanced",
	"GL_ARB_fragment_program",
	"GL_ARB_fragment_program_shadow",
	"GL_ARB_fragment_shader",
	"GL_ARB_framebuffer_object",
	"GL_ARB_framebuffer_sRGB",
	"GL_ARB_geometry_shader4",
	"GL_ARB_half_float_pixel",
	"GL_ARB_half_float_vertex",
	"GL_ARB_imaging",
	"GL_ARB_instanced_arrays",
	"GL_ARB_map_buffer_range",
	"GL_ARB_matrix_palette",
	"GL_ARB_multisample",
	"GL_ARB_multitexture",
	"GL_ARB_occlusion_query",
	"GL_ARB_pixel_buffer_object",
	"GL_ARB_point_parameters",
	"GL_ARB_point_sprite",
	"GL_ARB_shader_objects",
	"GL_ARB_shading_language_100",
	"GL_ARB_shadow",
	"GL_ARB_shadow_ambient",
	"GL_ARB_texture_border_clamp",
	"GL_ARB_texture_buffer_object",
	"GL_ARB_texture_compression",
	"GL_ARB_texture_compression_rgtc",
	"GL_ARB_texture_cube_map",
	"GL_ARB_texture_env_add",
	"GL_ARB_texture_env_combine",
	"GL_ARB_texture_env_crossbar",
	"GL_ARB_texture_env_dot3",
	"GL_ARB_texture_float",
	"GL_ARB_texture_mirrored_repeat",
	"GL_ARB_texture_non_power_of_two",
	"GL_ARB_texture_rectangle",
	"GL_ARB_texture_rg",
	"GL_ARB_transpose_matrix",
	"GL_ARB_vertex_array_object",
	"GL_ARB_vertex_blend",
	"GL_ARB_vertex_buffer_object",
	"GL_ARB_vertex_program",
	"GL_ARB_vertex_shader",
	"GL_ARB_window_pos",
	"GL_ATI_draw_buffers",
	"GL_ATI_element_array",
	"GL_ATI_envmap_bumpmap",
	"GL_ATI_fragment_shader",
	"GL_ATI_map_object_buffer",
	"GL_ATI_pixel_format_float",
	"GL_ATI_pn_triangles",
	"GL_ATI_separate_stencil",
	"GL_ATI_text_fragment_shader",
	"GL_ATI_texture_env_combine3",
	"GL_ATI_texture_float",
	"GL_ATI_texture_mirror_once",
	"GL_ATI_vertex_array_object",
	"GL_ATI_vertex_attrib_array_object",
	"GL_ATI_vertex_streams",
	"GL_EXT_422_pixels",
	"GL_EXT_abgr",
	"GL_EXT_bgra",
	"GL_EXT_bindable_uniform",
	"GL_EXT_blend_color",
	"GL_EXT_blend_equation_separate",
	"GL_EXT_blend_func_separate",
	"GL_EXT_blend_logic_op",
	"GL_EXT_blend_minmax",
	"GL_EXT_blend_subtract",
	"GL_EXT_clip_volume_hint",
	"GL_EXT_cmyka",
	"GL_EXT_color_subtable",
	"GL_EXT_compiled_vertex_array",
	"GL_EXT_convolution",
	"GL_EXT_coordinate_frame",
	"GL_EXT_copy_texture",
	"GL_EXT_cull_vertex",
	"GL_EXT_depth_bounds_test",
	"GL_EXT_direct_state_access",
	"GL_EXT_draw_buffers2",
	"GL_EXT_draw_instanced",
	"GL_EXT_draw_range_elements",
	"GL_EXT_fog_coord",
	"GL_EXT_framebuffer_blit",
	"GL_EXT_framebuffer_multisample",
	"GL_EXT_framebuffer_object",
	"GL_EXT_framebuffer_sRGB",
	"GL_EXT_geometry_shader4",
	"GL_EXT_gpu_program_parameters",
	"GL_EXT_gpu_shader4",
	"GL_EXT_histogram",
	"GL_EXT_index_array_formats",
	"GL_EXT_index_func",
	"GL_EXT_index_material",
	"GL_EXT_index_texture",
	"GL_EXT_light_texture",
	"GL_EXT_misc_attribute",
	"GL_EXT_multi_draw_arrays",
	"GL_EXT_multisample",
	"GL_EXT_packed_depth_stencil",
	"GL_EXT_packed_float",
	"GL_EXT_packed_pixels",
	"GL_EXT_paletted_texture",
	"GL_EXT_pixel_buffer_object",
	"GL_EXT_pixel_transform",
	"GL_EXT_pixel_transform_color_table",
	"GL_EXT_point_parameters",
	"GL_EXT_polygon_offset",
	"GL_EXT_rescale_normal",
	"GL_EXT_secondary_color",
	"GL_EXT_separate_specular_color",
	"GL_EXT_shadow_funcs",
	"GL_EXT_shared_texture_palette",
	"GL_EXT_stencil_clear_tag",
	"GL_EXT_stencil_two_side",
	"GL_EXT_stencil_wrap",
	"GL_EXT_subtexture",
	"GL_EXT_texture",
	"GL_EXT_texture3D",
	"GL_EXT_texture_array",
	"GL_EXT_texture_buffer_object",
	"GL_EXT_texture_compression_latc",
	"GL_EXT_texture_compression_rgtc",
	"GL_EXT_texture_compression_s3tc",
	"GL_EXT_texture_cube_map",
	"GL_EXT_texture_env_add",
	"GL_EXT_texture_env_combine",
	"GL_EXT_texture_env_dot3",
	"GL_EXT_texture_filter_anisotropic",
	"GL_EXT_texture_integer",
	"GL_EXT_texture_lod_bias",
	"GL_EXT_texture_mirror_clamp",
	"GL_EXT_texture_object",
	"GL_EXT_texture_perturb_normal",
	"GL_EXT_texture_shared_exponent",
	"GL_EXT_texture_sRGB",
	"GL_EXT_timer_query",
	"GL_EXT_transform_feedback",
	"GL_EXT_vertex_array",
	"GL_EXT_vertex_array_bgra",
	"GL_EXT_vertex_shader",
	"GL_EXT_vertex_weighting",
	"GL_FfdMaskSGIX",
	"GL_GREMEDY_frame_terminator",
	"GL_GREMEDY_string_marker",
	"GL_HP_convolution_border_modes",
	"GL_HP_image_transform",
	"GL_HP_occlusion_test",
	"GL_HP_texture_lighting",
	"GL_IBM_cull_vertex",
	"GL_IBM_multimode_draw_arrays",
	"GL_IBM_rasterpos_clip",
	"GL_IBM_texture_mirrored_repeat",
	"GL_IBM_vertex_array_lists",
	"GL_INGR_blend_func_separate",
	"GL_INGR_color_clamp",
	"GL_INGR_interlace_read",
	"GL_INGR_palette_buffer",
	"GL_INTEL_parallel_arrays",
	"GL_INTEL_texture_scissor",
	"GL_MESA_pack_invert",
	"GL_MESA_resize_buffers",
	"GL_MESA_window_pos",
	"GL_MESAX_texture_stack",
	"GL_MESA_ycbcr_texture",
	"GL_NV_blend_square",
	"GL_NV_conditional_render",
	"GL_NV_copy_depth_to_color",
	"GL_NV_depth_buffer_float",
	"GL_NV_depth_clamp",
	"GL_NV_evaluators",
	"GL_NV_fence",
	"GL_NV_float_buffer",
	"GL_NV_fog_distance",
	"GL_NV_fragment_program",
	"GL_NV_fragment_program2",
	"GL_NV_fragment_program4",
	"GL_NV_fragment_program_option",
	"GL_NV_framebuffer_multisample_coverage",
	"GL_NV_geometry_program4",
	"GL_NV_geometry_shader4",
	"GL_NV_gpu_program4",
	"GL_NV_half_float",
	"GL_NV_light_max_exponent",
	"GL_NV_multisample_filter_hint",
	"GL_NV_occlusion_query",
	"GL_NV_packed_depth_stencil",
	"GL_NV_parameter_buffer_object",
	"GL_NV_pixel_data_range",
	"GL_NV_point_sprite",
	"GL_NV_present_video",
	"GL_NV_primitive_restart",
	"GL_NV_register_combiners",
	"GL_NV_register_combiners2",
	"GL_NV_texgen_emboss",
	"GL_NV_texgen_reflection",
	"GL_NV_texture_compression_vtc",
	"GL_NV_texture_env_combine4",
	"GL_NV_texture_expand_normal",
	"GL_NV_texture_rectangle",
	"GL_NV_texture_shader",
	"GL_NV_texture_shader2",
	"GL_NV_texture_shader3",
	"GL_NV_transform_feedback",
	"GL_NV_vertex_array_range",
	"GL_NV_vertex_array_range2",
	"GL_NV_vertex_program",
	"GL_NV_vertex_program1_1",
	"GL_NV_vertex_program2",
	"GL_NV_vertex_program2_option",
	"GL_NV_vertex_program3",
	"GL_NV_vertex_program4",
	"GL_OES_read_format",
	"GL_OML_interlace",
	"GL_OML_resample",
	"GL_OML_subsample",
	"GL_PGI_misc_hints",
	"GL_PGI_vertex_hints",
	"GL_REND_screen_coordinates",
	"GL_S3_s3tc",
	"GL_SGI_color_matrix",
	"GL_SGI_color_table",
	"GL_SGI_depth_pass_instrument",
	"GL_SGIS_detail_texture",
	"GL_SGIS_fog_function",
	"GL_SGIS_generate_mipmap",
	"GL_SGIS_multisample",
	"GL_SGIS_pixel_texture",
	"GL_SGIS_point_line_texgen",
	"GL_SGIS_point_parameters",
	"GL_SGIS_sharpen_texture",
	"GL_SGIS_texture4D",
	"GL_SGIS_texture_border_clamp",
	"GL_SGIS_texture_color_mask",
	"GL_SGIS_texture_edge_clamp",
	"GL_SGIS_texture_filter4",
	"GL_SGIS_texture_lod",
	"GL_SGIS_texture_select",
	"GL_SGI_texture_color_table",
	"GL_SGIX_async",
	"GL_SGIX_async_histogram",
	"GL_SGIX_async_pixel",
	"GL_SGIX_blend_alpha_minmax",
	"GL_SGIX_calligraphic_fragment",
	"GL_SGIX_clipmap",
	"GL_SGIX_convolution_accuracy",
	"GL_SGIX_depth_pass_instrument",
	"GL_SGIX_depth_texture",
	"GL_SGIX_flush_raster",
	"GL_SGIX_fog_offset",
	"GL_SGIX_fog_scale",
	"GL_SGIX_fragment_lighting",
	"GL_SGIX_framezoom",
	"GL_SGIX_igloo_interface",
	"GL_SGIX_impact_pixel_texture",
	"GL_SGIX_instruments",
	"GL_SGIX_interlace",
	"GL_SGIX_ir_instrument1",
	"GL_SGIX_list_priority",
	"GL_SGIX_pixel_texture",
	"GL_SGIX_pixel_tiles",
	"GL_SGIX_polynomial_ffd",
	"GL_SGIX_reference_plane",
	"GL_SGIX_resample",
	"GL_SGIX_scalebias_hint",
	"GL_SGIX_shadow",
	"GL_SGIX_shadow_ambient",
	"GL_SGIX_sprite",
	"GL_SGIX_subsample",
	"GL_SGIX_tag_sample_buffer",
	"GL_SGIX_texture_add_env",
	"GL_SGIX_texture_coordinate_clamp",
	"GL_SGIX_texture_lod_bias",
	"GL_SGIX_texture_multi_buffer",
	"GL_SGIX_texture_scale_bias",
	"GL_SGIX_texture_select",
	"GL_SGIX_vertex_preclip",
	"GL_SGIX_ycrcb",
	"GL_SGIX_ycrcba",
	"GL_SGIX_ycrcb_subsample",
	"GL_SUN_convolution_border_modes",
	"GL_SUN_global_alpha",
	"GL_SUN_mesh_array",
	"GL_SUN_slice_accum",
	"GL_SUN_triangle_list",
	"GL_SUN_vertex",
	"GL_SUNX_constant_data",
	"GL_WIN_phong_shading",
	"GL_WIN_specular_fog"
};


class COpenGLExtensionHandler
{
	public:
	enum EOpenGLFeatures {
		GEO_3DFX_multisample = 0,
		GEO_3DFX_tbuffer,
		GEO_3DFX_texture_compression_FXT1,
		GEO_APPLE_client_storage,
		GEO_APPLE_element_array,
		GEO_APPLE_fence,
		GEO_APPLE_flush_buffer_range,
		GEO_APPLE_specular_vector,
		GEO_APPLE_transform_hint,
		GEO_APPLE_vertex_array_object,
		GEO_APPLE_vertex_array_range,
		GEO_APPLE_ycbcr_422,
		GEO_ARB_color_buffer_float,
		GEO_ARB_depth_buffer_float,
		GEO_ARB_depth_texture,
		GEO_ARB_draw_buffers,
		GEO_ARB_draw_instanced,
		GEO_ARB_fragment_program,
		GEO_ARB_fragment_program_shadow,
		GEO_ARB_fragment_shader,
		GEO_ARB_framebuffer_object,
		GEO_ARB_framebuffer_sRGB,
		GEO_ARB_geometry_shader4,
		GEO_ARB_half_float_pixel,
		GEO_ARB_half_float_vertex,
		GEO_ARB_imaging,
		GEO_ARB_instanced_arrays,
		GEO_ARB_map_buffer_range,
		GEO_ARB_matrix_palette,
		GEO_ARB_multisample,
		GEO_ARB_multitexture,
		GEO_ARB_occlusion_query,
		GEO_ARB_pixel_buffer_object,
		GEO_ARB_point_parameters,
		GEO_ARB_point_sprite,
		GEO_ARB_shader_objects,
		GEO_ARB_shading_language_100,
		GEO_ARB_shadow,
		GEO_ARB_shadow_ambient,
		GEO_ARB_texture_border_clamp,
		GEO_ARB_texture_buffer_object,
		GEO_ARB_texture_compression,
		GEO_ARB_texture_compression_rgtc,
		GEO_ARB_texture_cube_map,
		GEO_ARB_texture_env_add,
		GEO_ARB_texture_env_combine,
		GEO_ARB_texture_env_crossbar,
		GEO_ARB_texture_env_dot3,
		GEO_ARB_texture_float,
		GEO_ARB_texture_mirrored_repeat,
		GEO_ARB_texture_non_power_of_two,
		GEO_ARB_texture_rectangle,
		GEO_ARB_texture_rg,
		GEO_ARB_transpose_matrix,
		GEO_ARB_vertex_array_object,
		GEO_ARB_vertex_blend,
		GEO_ARB_vertex_buffer_object,
		GEO_ARB_vertex_program,
		GEO_ARB_vertex_shader,
		GEO_ARB_window_pos,
		GEO_ATI_draw_buffers,
		GEO_ATI_element_array,
		GEO_ATI_envmap_bumpmap,
		GEO_ATI_fragment_shader,
		GEO_ATI_map_object_buffer,
		GEO_ATI_pixel_format_float,
		GEO_ATI_pn_triangles,
		GEO_ATI_separate_stencil,
		GEO_ATI_text_fragment_shader,
		GEO_ATI_texture_env_combine3,
		GEO_ATI_texture_float,
		GEO_ATI_texture_mirror_once,
		GEO_ATI_vertex_array_object,
		GEO_ATI_vertex_attrib_array_object,
		GEO_ATI_vertex_streams,
		GEO_EXT_422_pixels,
		GEO_EXT_abgr,
		GEO_EXT_bgra,
		GEO_EXT_bindable_uniform,
		GEO_EXT_blend_color,
		GEO_EXT_blend_equation_separate,
		GEO_EXT_blend_func_separate,
		GEO_EXT_blend_logic_op,
		GEO_EXT_blend_minmax,
		GEO_EXT_blend_subtract,
		GEO_EXT_clip_volume_hint,
		GEO_EXT_cmyka,
		GEO_EXT_color_subtable,
		GEO_EXT_compiled_vertex_array,
		GEO_EXT_convolution,
		GEO_EXT_coordinate_frame,
		GEO_EXT_copy_texture,
		GEO_EXT_cull_vertex,
		GEO_EXT_depth_bounds_test,
		GEO_EXT_direct_state_access,
		GEO_EXT_draw_buffers2,
		GEO_EXT_draw_instanced,
		GEO_EXT_draw_range_elements,
		GEO_EXT_fog_coord,
		GEO_EXT_framebuffer_blit,
		GEO_EXT_framebuffer_multisample,
		GEO_EXT_framebuffer_object,
		GEO_EXT_framebuffer_sRGB,
		GEO_EXT_geometry_shader4,
		GEO_EXT_gpu_program_parameters,
		GEO_EXT_gpu_shader4,
		GEO_EXT_histogram,
		GEO_EXT_index_array_formats,
		GEO_EXT_index_func,
		GEO_EXT_index_material,
		GEO_EXT_index_texture,
		GEO_EXT_light_texture,
		GEO_EXT_misc_attribute,
		GEO_EXT_multi_draw_arrays,
		GEO_EXT_multisample,
		GEO_EXT_packed_depth_stencil,
		GEO_EXT_packed_float,
		GEO_EXT_packed_pixels,
		GEO_EXT_paletted_texture,
		GEO_EXT_pixel_buffer_object,
		GEO_EXT_pixel_transform,
		GEO_EXT_pixel_transform_color_table,
		GEO_EXT_point_parameters,
		GEO_EXT_polygon_offset,
		GEO_EXT_rescale_normal,
		GEO_EXT_secondary_color,
		GEO_EXT_separate_specular_color,
		GEO_EXT_shadow_funcs,
		GEO_EXT_shared_texture_palette,
		GEO_EXT_stencil_clear_tag,
		GEO_EXT_stencil_two_side,
		GEO_EXT_stencil_wrap,
		GEO_EXT_subtexture,
		GEO_EXT_texture,
		GEO_EXT_texture3D,
		GEO_EXT_texture_array,
		GEO_EXT_texture_buffer_object,
		GEO_EXT_texture_compression_latc,
		GEO_EXT_texture_compression_rgtc,
		GEO_EXT_texture_compression_s3tc,
		GEO_EXT_texture_cube_map,
		GEO_EXT_texture_env_add,
		GEO_EXT_texture_env_combine,
		GEO_EXT_texture_env_dot3,
		GEO_EXT_texture_filter_anisotropic,
		GEO_EXT_texture_integer,
		GEO_EXT_texture_lod_bias,
		GEO_EXT_texture_mirror_clamp,
		GEO_EXT_texture_object,
		GEO_EXT_texture_perturb_normal,
		GEO_EXT_texture_shared_exponent,
		GEO_EXT_texture_sRGB,
		GEO_EXT_timer_query,
		GEO_EXT_transform_feedback,
		GEO_EXT_vertex_array,
		GEO_EXT_vertex_array_bgra,
		GEO_EXT_vertex_shader,
		GEO_EXT_vertex_weighting,
		GEO_FfdMaskSGIX,
		GEO_GREMEDY_frame_terminator,
		GEO_GREMEDY_string_marker,
		GEO_HP_convolution_border_modes,
		GEO_HP_image_transform,
		GEO_HP_occlusion_test,
		GEO_HP_texture_lighting,
		GEO_IBM_cull_vertex,
		GEO_IBM_multimode_draw_arrays,
		GEO_IBM_rasterpos_clip,
		GEO_IBM_texture_mirrored_repeat,
		GEO_IBM_vertex_array_lists,
		GEO_INGR_blend_func_separate,
		GEO_INGR_color_clamp,
		GEO_INGR_interlace_read,
		GEO_INGR_palette_buffer,
		GEO_INTEL_parallel_arrays,
		GEO_INTEL_texture_scissor,
		GEO_MESA_pack_invert,
		GEO_MESA_resize_buffers,
		GEO_MESA_window_pos,
		GEO_MESAX_texture_stack,
		GEO_MESA_ycbcr_texture,
		GEO_NV_blend_square,
		GEO_NV_conditional_render,
		GEO_NV_copy_depth_to_color,
		GEO_NV_depth_buffer_float,
		GEO_NV_depth_clamp,
		GEO_NV_evaluators,
		GEO_NV_fence,
		GEO_NV_float_buffer,
		GEO_NV_fog_distance,
		GEO_NV_fragment_program,
		GEO_NV_fragment_program2,
		GEO_NV_fragment_program4,
		GEO_NV_fragment_program_option,
		GEO_NV_framebuffer_multisample_coverage,
		GEO_NV_geometry_program4,
		GEO_NV_geometry_shader4,
		GEO_NV_gpu_program4,
		GEO_NV_half_float,
		GEO_NV_light_max_exponent,
		GEO_NV_multisample_filter_hint,
		GEO_NV_occlusion_query,
		GEO_NV_packed_depth_stencil,
		GEO_NV_parameter_buffer_object,
		GEO_NV_pixel_data_range,
		GEO_NV_point_sprite,
		GEO_NV_present_video,
		GEO_NV_primitive_restart,
		GEO_NV_register_combiners,
		GEO_NV_register_combiners2,
		GEO_NV_texgen_emboss,
		GEO_NV_texgen_reflection,
		GEO_NV_texture_compression_vtc,
		GEO_NV_texture_env_combine4,
		GEO_NV_texture_expand_normal,
		GEO_NV_texture_rectangle,
		GEO_NV_texture_shader,
		GEO_NV_texture_shader2,
		GEO_NV_texture_shader3,
		GEO_NV_transform_feedback,
		GEO_NV_vertex_array_range,
		GEO_NV_vertex_array_range2,
		GEO_NV_vertex_program,
		GEO_NV_vertex_program1_1,
		GEO_NV_vertex_program2,
		GEO_NV_vertex_program2_option,
		GEO_NV_vertex_program3,
		GEO_NV_vertex_program4,
		GEO_OES_read_format,
		GEO_OML_interlace,
		GEO_OML_resample,
		GEO_OML_subsample,
		GEO_PGI_misc_hints,
		GEO_PGI_vertex_hints,
		GEO_REND_screen_coordinates,
		GEO_S3_s3tc,
		GEO_SGI_color_matrix,
		GEO_SGI_color_table,
		GEO_SGI_depth_pass_instrument,
		GEO_SGIS_detail_texture,
		GEO_SGIS_fog_function,
		GEO_SGIS_generate_mipmap,
		GEO_SGIS_multisample,
		GEO_SGIS_pixel_texture,
		GEO_SGIS_point_line_texgen,
		GEO_SGIS_point_parameters,
		GEO_SGIS_sharpen_texture,
		GEO_SGIS_texture4D,
		GEO_SGIS_texture_border_clamp,
		GEO_SGIS_texture_color_mask,
		GEO_SGIS_texture_edge_clamp,
		GEO_SGIS_texture_filter4,
		GEO_SGIS_texture_lod,
		GEO_SGIS_texture_select,
		GEO_SGI_texture_color_table,
		GEO_SGIX_async,
		GEO_SGIX_async_histogram,
		GEO_SGIX_async_pixel,
		GEO_SGIX_blend_alpha_minmax,
		GEO_SGIX_calligraphic_fragment,
		GEO_SGIX_clipmap,
		GEO_SGIX_convolution_accuracy,
		GEO_SGIX_depth_pass_instrument,
		GEO_SGIX_depth_texture,
		GEO_SGIX_flush_raster,
		GEO_SGIX_fog_offset,
		GEO_SGIX_fog_scale,
		GEO_SGIX_fragment_lighting,
		GEO_SGIX_framezoom,
		GEO_SGIX_igloo_interface,
		GEO_SGIX_impact_pixel_texture,
		GEO_SGIX_instruments,
		GEO_SGIX_interlace,
		GEO_SGIX_ir_instrument1,
		GEO_SGIX_list_priority,
		GEO_SGIX_pixel_texture,
		GEO_SGIX_pixel_tiles,
		GEO_SGIX_polynomial_ffd,
		GEO_SGIX_reference_plane,
		GEO_SGIX_resample,
		GEO_SGIX_scalebias_hint,
		GEO_SGIX_shadow,
		GEO_SGIX_shadow_ambient,
		GEO_SGIX_sprite,
		GEO_SGIX_subsample,
		GEO_SGIX_tag_sample_buffer,
		GEO_SGIX_texture_add_env,
		GEO_SGIX_texture_coordinate_clamp,
		GEO_SGIX_texture_lod_bias,
		GEO_SGIX_texture_multi_buffer,
		GEO_SGIX_texture_scale_bias,
		GEO_SGIX_texture_select,
		GEO_SGIX_vertex_preclip,
		GEO_SGIX_ycrcb,
		GEO_SGIX_ycrcba,
		GEO_SGIX_ycrcb_subsample,
		GEO_SUN_convolution_border_modes,
		GEO_SUN_global_alpha,
		GEO_SUN_mesh_array,
		GEO_SUN_slice_accum,
		GEO_SUN_triangle_list,
		GEO_SUN_vertex,
		GEO_SUNX_constant_data,
		GEO_WIN_phong_shading,
		GEO_WIN_specular_fog,
		GEO_OpenGL_Feature_Count
	};


	// constructor
	COpenGLExtensionHandler();

	// deferred initialization
	void initExtensions(bool stencilBuffer);

	//! queries the features of the driver, returns true if feature is available
	bool queryFeature(E_VIDEO_DRIVER_FEATURE feature) const;

	//! queries the features of the driver, returns true if feature is available
	bool queryOpenGLFeature(EOpenGLFeatures feature) const
	{
		return FeatureAvailable[feature];
	}

	//! show all features with availablity
	void dump() const;

	// Some variables for properties
	bool StencilBuffer;
	bool MultiTextureExtension;
	bool MultiSamplingExtension;
	bool AnisotropyExtension;
	bool TextureCompressionExtension;

	// Some non-boolean properties
	//! Maxmimum texture layers supported by the fixed pipeline
	u32 MaxTextureUnits;
	//! Maximum hardware lights supported
	GLint MaxLights;
	//! Optimal number of indices per meshbuffer
	GLint MaxIndices;
	//! Maximal Anisotropy
	f32 MaxAnisotropy;
	//! Number of user clipplanes
	u32 MaxUserClipPlanes;

	//! OpenGL version as Integer: 100*Major+Minor, i.e. 2.1 becomes 201
	u32 Version;
	//! GLSL version as Integer: 100*Major+Minor
	u32 ShaderLanguageVersion;

	// public access to the (loaded) extensions.
	// general functions
	void extGlActiveTexture(GLenum texture);
	void extGlClientActiveTexture(GLenum texture);
	void extGlPointParameterf(GLint loc, GLfloat f);
	void extGlPointParameterfv(GLint loc, const GLfloat *v);
	void extGlStencilFuncSeparate (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
	void extGlStencilOpSeparate (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
	void extGlCompressedTexImage2D(GLenum target, GLint level,
		GLenum internalformat, GLsizei width, GLsizei height,
		GLint border, GLsizei imageSize, const void* data);

	// shader programming
	void extGlGenPrograms(GLsizei n, GLuint *programs);
	void extGlBindProgram(GLenum target, GLuint program);
	void extGlProgramString(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
	void extGlDeletePrograms(GLsizei n, const GLuint *programs);
	void extGlProgramLocalParameter4fv(GLenum, GLuint, const GLfloat *);
	GLhandleARB extGlCreateShaderObject(GLenum shaderType);
	void extGlShaderSource(GLhandleARB shader, int numOfStrings, const char **strings, int *lenOfStrings);
	void extGlCompileShader(GLhandleARB shader);
	GLhandleARB extGlCreateProgramObject(void);
	void extGlAttachObject(GLhandleARB program, GLhandleARB shader);
	void extGlLinkProgram(GLhandleARB program);
	void extGlUseProgramObject(GLhandleARB prog);
	void extGlDeleteObject(GLhandleARB object);
	void extGlGetInfoLog(GLhandleARB object, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
	void extGlGetObjectParameteriv(GLhandleARB object, GLenum type, int *param);
	GLint extGlGetUniformLocation(GLhandleARB program, const char *name);
	void extGlUniform4fv(GLint location, GLsizei count, const GLfloat *v);
	void extGlUniform1iv(GLint loc, GLsizei count, const GLint *v);
	void extGlUniform1fv(GLint loc, GLsizei count, const GLfloat *v);
	void extGlUniform2fv(GLint loc, GLsizei count, const GLfloat *v);
	void extGlUniform3fv(GLint loc, GLsizei count, const GLfloat *v);
	void extGlUniformMatrix2fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v);
	void extGlUniformMatrix3fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v);
	void extGlUniformMatrix4fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v);
	void extGlGetActiveUniform(GLhandleARB program, GLuint index, GLsizei maxlength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);

	// framebuffer objects
	void extGlBindFramebuffer(GLenum target, GLuint framebuffer);
	void extGlDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
	void extGlGenFramebuffers(GLsizei n, GLuint *framebuffers);
	GLenum extGlCheckFramebufferStatus(GLenum target);
	void extGlFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	void extGlBindRenderbuffer(GLenum target, GLuint renderbuffer);
	void extGlDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
	void extGlGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
	void extGlRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	void extGlFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	void extGlActiveStencilFace(GLenum face);

	// vertex buffer object
	void extGlGenBuffers(GLsizei n, GLuint *buffers);
	void extGlBindBuffer(GLenum target, GLuint buffer);
	void extGlBufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
	void extGlDeleteBuffers(GLsizei n, const GLuint *buffers);
	void extGlBufferSubData (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
	void extGlGetBufferSubData (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
	void *extGlMapBuffer (GLenum target, GLenum access);
	GLboolean extGlUnmapBuffer (GLenum target);
	GLboolean extGlIsBuffer (GLuint buffer);
	void extGlGetBufferParameteriv (GLenum target, GLenum pname, GLint *params);
	void extGlGetBufferPointerv (GLenum target, GLenum pname, GLvoid **params);


	protected:
	// the global feature array
	bool FeatureAvailable[GEO_OpenGL_Feature_Count];

	#if defined(_GEO_OPENGL_USE_EXTPOINTER_)
		PFNGLACTIVETEXTUREARBPROC pGlActiveTextureARB;
		PFNGLCLIENTACTIVETEXTUREARBPROC	pGlClientActiveTextureARB;
		PFNGLGENPROGRAMSARBPROC pGlGenProgramsARB;
		PFNGLBINDPROGRAMARBPROC pGlBindProgramARB;
		PFNGLPROGRAMSTRINGARBPROC pGlProgramStringARB;
		PFNGLDELETEPROGRAMSNVPROC pGlDeleteProgramsARB;
		PFNGLPROGRAMLOCALPARAMETER4FVARBPROC pGlProgramLocalParameter4fvARB;
		PFNGLCREATESHADEROBJECTARBPROC pGlCreateShaderObjectARB;
		PFNGLSHADERSOURCEARBPROC pGlShaderSourceARB;
		PFNGLCOMPILESHADERARBPROC pGlCompileShaderARB;
		PFNGLCREATEPROGRAMOBJECTARBPROC pGlCreateProgramObjectARB;
		PFNGLATTACHOBJECTARBPROC pGlAttachObjectARB;
		PFNGLLINKPROGRAMARBPROC pGlLinkProgramARB;
		PFNGLUSEPROGRAMOBJECTARBPROC pGlUseProgramObjectARB;
		PFNGLDELETEOBJECTARBPROC pGlDeleteObjectARB;
		PFNGLGETINFOLOGARBPROC pGlGetInfoLogARB;
		PFNGLGETOBJECTPARAMETERIVARBPROC pGlGetObjectParameterivARB;
		PFNGLGETUNIFORMLOCATIONARBPROC pGlGetUniformLocationARB;
		PFNGLUNIFORM1IVARBPROC pGlUniform1ivARB;
		PFNGLUNIFORM1FVARBPROC pGlUniform1fvARB;
		PFNGLUNIFORM2FVARBPROC pGlUniform2fvARB;
		PFNGLUNIFORM3FVARBPROC pGlUniform3fvARB;
		PFNGLUNIFORM4FVARBPROC pGlUniform4fvARB;
		PFNGLUNIFORMMATRIX2FVARBPROC pGlUniformMatrix2fvARB;
		PFNGLUNIFORMMATRIX3FVARBPROC pGlUniformMatrix3fvARB;
		PFNGLUNIFORMMATRIX4FVARBPROC pGlUniformMatrix4fvARB;
		PFNGLGETACTIVEUNIFORMARBPROC pGlGetActiveUniformARB;
		PFNGLPOINTPARAMETERFARBPROC  pGlPointParameterfARB;
		PFNGLPOINTPARAMETERFVARBPROC pGlPointParameterfvARB;
		PFNGLSTENCILFUNCSEPARATEPROC pGlStencilFuncSeparate;
		PFNGLSTENCILOPSEPARATEPROC pGlStencilOpSeparate;
		PFNGLSTENCILFUNCSEPARATEATIPROC pGlStencilFuncSeparateATI;
		PFNGLSTENCILOPSEPARATEATIPROC pGlStencilOpSeparateATI;
		PFNGLCOMPRESSEDTEXIMAGE2DPROC pGlCompressedTexImage2D;
		#ifdef _GEO_WINDOWS_API_
		typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
		PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;
		#elif defined(_GEO_LINUX_PLATFORM_) && defined(GLX_SGI_swap_control)
		PFNGLXSWAPINTERVALSGIPROC glxSwapIntervalSGI;
		#endif
		PFNGLBINDFRAMEBUFFEREXTPROC pGlBindFramebufferEXT;
		PFNGLDELETEFRAMEBUFFERSEXTPROC pGlDeleteFramebuffersEXT;
		PFNGLGENFRAMEBUFFERSEXTPROC pGlGenFramebuffersEXT;
		PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC pGlCheckFramebufferStatusEXT;
		PFNGLFRAMEBUFFERTEXTURE2DEXTPROC pGlFramebufferTexture2DEXT;
		PFNGLBINDRENDERBUFFEREXTPROC pGlBindRenderbufferEXT;
		PFNGLDELETERENDERBUFFERSEXTPROC pGlDeleteRenderbuffersEXT;
		PFNGLGENRENDERBUFFERSEXTPROC pGlGenRenderbuffersEXT;
		PFNGLRENDERBUFFERSTORAGEEXTPROC pGlRenderbufferStorageEXT;
		PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC pGlFramebufferRenderbufferEXT;
		PFNGLACTIVESTENCILFACEEXTPROC pGlActiveStencilFaceEXT;
		PFNGLGENBUFFERSARBPROC pGlGenBuffersARB;
		PFNGLBINDBUFFERARBPROC pGlBindBufferARB;
		PFNGLBUFFERDATAARBPROC pGlBufferDataARB;
		PFNGLDELETEBUFFERSARBPROC pGlDeleteBuffersARB;
		PFNGLBUFFERSUBDATAARBPROC pGlBufferSubDataARB;
		PFNGLGETBUFFERSUBDATAARBPROC pGlGetBufferSubDataARB;
		PFNGLMAPBUFFERARBPROC pGlMapBufferARB;
		PFNGLUNMAPBUFFERARBPROC pGlUnmapBufferARB;
		PFNGLISBUFFERARBPROC pGlIsBufferARB;
		PFNGLGETBUFFERPARAMETERIVARBPROC pGlGetBufferParameterivARB;
		PFNGLGETBUFFERPOINTERVARBPROC pGlGetBufferPointervARB;
	#endif
};

inline void COpenGLExtensionHandler::extGlActiveTexture(GLenum texture)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (MultiTextureExtension && pGlActiveTextureARB)
		pGlActiveTextureARB(texture);
#else
	if (MultiTextureExtension) glActiveTextureARB(texture);
#endif
}

inline void COpenGLExtensionHandler::extGlClientActiveTexture(GLenum texture)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (MultiTextureExtension && pGlClientActiveTextureARB)
		pGlClientActiveTextureARB(texture);
#else
	if (MultiTextureExtension) glClientActiveTextureARB(texture);
#endif
}

inline void COpenGLExtensionHandler::extGlGenPrograms(GLsizei n, GLuint *programs)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGenProgramsARB)
		pGlGenProgramsARB(n, programs);
#elif defined(GL_ARB_vertex_program)
	glGenProgramsARB(n,programs);
#else
	os::Printer::log("glGenPrograms not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlBindProgram(GLenum target, GLuint program)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlBindProgramARB)
		pGlBindProgramARB(target, program);
#elif defined(GL_ARB_vertex_program)
	glBindProgramARB(target, program);
#else
	os::Printer::log("glBindProgram not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlProgramString(GLenum target, GLenum format, GLsizei len, const GLvoid *string)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlProgramStringARB)
		pGlProgramStringARB(target, format, len, string);
#elif defined(GL_ARB_vertex_program)
	glProgramStringARB(target,format,len,string);
#else
	os::Printer::log("glProgramString not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlDeletePrograms(GLsizei n, const GLuint *programs)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlDeleteProgramsARB)
		pGlDeleteProgramsARB(n, programs);
#elif defined(GL_ARB_vertex_program)
	glDeleteProgramsARB(n,programs);
#else
	os::Printer::log("glDeletePrograms not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlProgramLocalParameter4fv(GLenum n, GLuint i, const GLfloat * f)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlProgramLocalParameter4fvARB)
		pGlProgramLocalParameter4fvARB(n,i,f);
#elif defined(GL_ARB_vertex_program)
	glProgramLocalParameter4fvARB(n,i,f);
#else
	os::Printer::log("glProgramLocalParameter4fv not supported", ELL_ERROR);
#endif
}

inline GLhandleARB COpenGLExtensionHandler::extGlCreateShaderObject(GLenum shaderType)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlCreateShaderObjectARB)
		return pGlCreateShaderObjectARB(shaderType);
#elif defined(GL_ARB_shader_objects)
	return glCreateShaderObjectARB(shaderType);
#else
	os::Printer::log("glCreateShaderObject not supported", ELL_ERROR);
#endif
	return 0;
}

inline void COpenGLExtensionHandler::extGlShaderSource(GLhandleARB shader, int numOfStrings, const char **strings, int *lenOfStrings)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlShaderSourceARB)
		pGlShaderSourceARB(shader, numOfStrings, strings, lenOfStrings);
#elif defined(GL_ARB_shader_objects)
	glShaderSourceARB(shader, numOfStrings, strings, (GLint *)lenOfStrings);
#else
	os::Printer::log("glShaderSource not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlCompileShader(GLhandleARB shader)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlCompileShaderARB)
		pGlCompileShaderARB(shader);
#elif defined(GL_ARB_shader_objects)
	glCompileShaderARB(shader);
#else
	os::Printer::log("glCompileShader not supported", ELL_ERROR);
#endif
}

inline GLhandleARB COpenGLExtensionHandler::extGlCreateProgramObject(void)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlCreateProgramObjectARB)
		return pGlCreateProgramObjectARB();
#elif defined(GL_ARB_shader_objects)
	return glCreateProgramObjectARB();
#else
	os::Printer::log("glCreateProgramObject not supported", ELL_ERROR);
#endif
	return 0;
}

inline void COpenGLExtensionHandler::extGlAttachObject(GLhandleARB program, GLhandleARB shader)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlAttachObjectARB)
		pGlAttachObjectARB(program, shader);
#elif defined(GL_ARB_shader_objects)
	glAttachObjectARB(program, shader);
#else
	os::Printer::log("glAttachObject not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlLinkProgram(GLhandleARB program)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlLinkProgramARB)
		pGlLinkProgramARB(program);
#elif defined(GL_ARB_shader_objects)
	glLinkProgramARB(program);
#else
	os::Printer::log("glLinkProgram not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlUseProgramObject(GLhandleARB prog)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUseProgramObjectARB)
		pGlUseProgramObjectARB(prog);
#elif defined(GL_ARB_shader_objects)
	glUseProgramObjectARB(prog);
#else
	os::Printer::log("glUseProgramObject not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlDeleteObject(GLhandleARB object)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlDeleteObjectARB)
		pGlDeleteObjectARB(object);
#elif defined(GL_ARB_shader_objects)
	glDeleteObjectARB(object);
#else
	os::Printer::log("gldeleteObject not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlGetInfoLog(GLhandleARB object, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGetInfoLogARB)
		pGlGetInfoLogARB(object, maxLength, length, infoLog);
#elif defined(GL_ARB_shader_objects)
	glGetInfoLogARB(object, maxLength, length, infoLog);
#else
	os::Printer::log("glGetInfoLog not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlGetObjectParameteriv(GLhandleARB object, GLenum type, int *param)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGetObjectParameterivARB)
		pGlGetObjectParameterivARB(object, type, param);
#elif defined(GL_ARB_shader_objects)
	glGetObjectParameterivARB(object, type, (GLint *)param);
#else
	os::Printer::log("glGetObjectParameteriv not supported", ELL_ERROR);
#endif
}

inline GLint COpenGLExtensionHandler::extGlGetUniformLocation(GLhandleARB program, const char *name)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGetUniformLocationARB)
		return pGlGetUniformLocationARB(program, name);
#elif defined(GL_ARB_shader_objects)
	return glGetUniformLocationARB(program, name);
#else
	os::Printer::log("glGetUniformLocation not supported", ELL_ERROR);
#endif
	return 0;
}

inline void COpenGLExtensionHandler::extGlUniform4fv(GLint location, GLsizei count, const GLfloat *v)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUniform4fvARB)
		pGlUniform4fvARB(location, count, v);
#elif defined(GL_ARB_shader_objects)
	glUniform4fvARB(location, count, v);
#else
	os::Printer::log("glUniform4fv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlUniform1iv(GLint loc, GLsizei count, const GLint *v)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUniform1ivARB)
		pGlUniform1ivARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
	glUniform1ivARB(loc, count, v);
#else
	os::Printer::log("glUniform1iv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlUniform1fv(GLint loc, GLsizei count, const GLfloat *v)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUniform1fvARB)
		pGlUniform1fvARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
	glUniform1fvARB(loc, count, v);
#else
	os::Printer::log("glUniform1fv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlUniform2fv(GLint loc, GLsizei count, const GLfloat *v)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUniform2fvARB)
		pGlUniform2fvARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
	glUniform2fvARB(loc, count, v);
#else
	os::Printer::log("glUniform2fv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlUniform3fv(GLint loc, GLsizei count, const GLfloat *v)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUniform3fvARB)
		pGlUniform3fvARB(loc, count, v);
#elif defined(GL_ARB_shader_objects)
	glUniform3fvARB(loc, count, v);
#else
	os::Printer::log("glUniform3fv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlUniformMatrix2fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUniformMatrix2fvARB)
		pGlUniformMatrix2fvARB(loc, count, transpose, v);
#elif defined(GL_ARB_shader_objects)
	glUniformMatrix2fvARB(loc, count, transpose, v);
#else
	os::Printer::log("glUniformMatrix2fv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlUniformMatrix3fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUniformMatrix3fvARB)
		pGlUniformMatrix3fvARB(loc, count, transpose, v);
#elif defined(GL_ARB_shader_objects)
	glUniformMatrix3fvARB(loc, count, transpose, v);
#else
	os::Printer::log("glUniformMatrix3fv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlUniformMatrix4fv(GLint loc, GLsizei count, GLboolean transpose, const GLfloat *v)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUniformMatrix4fvARB)
		pGlUniformMatrix4fvARB(loc, count, transpose, v);
#elif defined(GL_ARB_shader_objects)
	glUniformMatrix4fvARB(loc, count, transpose, v);
#else
	os::Printer::log("glUniformMatrix4fv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlGetActiveUniform(GLhandleARB program,
		GLuint index, GLsizei maxlength, GLsizei *length,
		GLint *size, GLenum *type, GLcharARB *name)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGetActiveUniformARB)
		pGlGetActiveUniformARB(program, index, maxlength, length, size, type, name);
#elif defined(GL_ARB_shader_objects)
	glGetActiveUniformARB(program, index, maxlength, length, size, type, name);
#else
	os::Printer::log("glGetActiveUniform not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlPointParameterf(GLint loc, GLfloat f)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlPointParameterfARB)
		pGlPointParameterfARB(loc, f);
#elif defined(GL_ARB_point_parameters)
	glPointParameterfARB(loc, f);
#else
	os::Printer::log("glPointParameterf not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlPointParameterfv(GLint loc, const GLfloat *v)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlPointParameterfvARB)
		pGlPointParameterfvARB(loc, v);
#elif defined(GL_ARB_point_parameters)
	glPointParameterfvARB(loc, v);
#else
	os::Printer::log("glPointParameterfv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlStencilFuncSeparate (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlStencilFuncSeparate)
		pGlStencilFuncSeparate(frontfunc, backfunc, ref, mask);
	else if (pGlStencilFuncSeparateATI)
		pGlStencilFuncSeparateATI(frontfunc, backfunc, ref, mask);
#elif defined(GL_VERSION_2_0)
	glStencilFuncSeparate(frontfunc, backfunc, ref, mask);
#elif defined(GL_ATI_separate_stencil)
	glStencilFuncSeparateATI(frontfunc, backfunc, ref, mask);
#else
	os::Printer::log("glStencilFuncSeparate not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlStencilOpSeparate (GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlStencilOpSeparate)
		pGlStencilOpSeparate(face, fail, zfail, zpass);
	else if (pGlStencilOpSeparateATI)
		pGlStencilOpSeparateATI(face, fail, zfail, zpass);
#elif defined(GL_VERSION_2_0)
	glStencilOpSeparate(face, fail, zfail, zpass);
#elif defined(GL_ATI_separate_stencil)
	glStencilOpSeparateATI(face, fail, zfail, zpass);
#else
	os::Printer::log("glStencilOpSeparate not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width,
		GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlCompressedTexImage2D)
		pGlCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
#elif defined(GL_ARB_texture_compression)
	glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
#else
	os::Printer::log("glCompressedTexImage2D not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlBindFramebuffer(GLenum target, GLuint framebuffer)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlBindFramebufferEXT)
		pGlBindFramebufferEXT(target, framebuffer);
#elif defined(GL_EXT_framebuffer_object)
	glBindFramebufferEXT(target, framebuffer);
#else
	os::Printer::log("glBindFramebuffer not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlDeleteFramebuffers(GLsizei n, const GLuint *framebuffers)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlDeleteFramebuffersEXT)
		pGlDeleteFramebuffersEXT(n, framebuffers);
#elif defined(GL_EXT_framebuffer_object)
	glDeleteFramebuffersEXT(n, framebuffers);
#else
	os::Printer::log("glDeleteFramebuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlGenFramebuffers(GLsizei n, GLuint *framebuffers)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGenFramebuffersEXT)
		pGlGenFramebuffersEXT(n, framebuffers);
#elif defined(GL_EXT_framebuffer_object)
	glGenFramebuffersEXT(n, framebuffers);
#else
	os::Printer::log("glGenFramebuffers not supported", ELL_ERROR);
#endif
}

inline GLenum COpenGLExtensionHandler::extGlCheckFramebufferStatus(GLenum target)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlCheckFramebufferStatusEXT)
		return pGlCheckFramebufferStatusEXT(target);
	else
		return 0;
#elif defined(GL_EXT_framebuffer_object)
	return glCheckFramebufferStatusEXT(target);
#else
	os::Printer::log("glCheckFramebufferStatus not supported", ELL_ERROR);
	return 0;
#endif
}

inline void COpenGLExtensionHandler::extGlFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlFramebufferTexture2DEXT)
		pGlFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
#elif defined(GL_EXT_framebuffer_object)
	glFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
#else
	os::Printer::log("glFramebufferTexture2D not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlBindRenderbufferEXT)
		pGlBindRenderbufferEXT(target, renderbuffer);
#elif defined(GL_EXT_framebuffer_object)
	glBindRenderbufferEXT(target, renderbuffer);
#else
	os::Printer::log("glBindRenderbuffer not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlDeleteRenderbuffersEXT)
		pGlDeleteRenderbuffersEXT(n, renderbuffers);
#elif defined(GL_EXT_framebuffer_object)
	glDeleteRenderbuffersEXT(n, renderbuffers);
#else
	os::Printer::log("glDeleteRenderbuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlGenRenderbuffers(GLsizei n, GLuint *renderbuffers)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGenRenderbuffersEXT)
		pGlGenRenderbuffersEXT(n, renderbuffers);
#elif defined(GL_EXT_framebuffer_object)
	glGenRenderbuffersEXT(n, renderbuffers);
#else
	os::Printer::log("glGenRenderbuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlRenderbufferStorageEXT)
		pGlRenderbufferStorageEXT(target, internalformat, width, height);
#elif defined(GL_EXT_framebuffer_object)
	glRenderbufferStorageEXT(target, internalformat, width, height);
#else
	os::Printer::log("glRenderbufferStorage not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlFramebufferRenderbufferEXT)
		pGlFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer);
#elif defined(GL_EXT_framebuffer_object)
	glFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer);
#else
	os::Printer::log("glFramebufferRenderbuffer not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlActiveStencilFace(GLenum face)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlActiveStencilFaceEXT)
		pGlActiveStencilFaceEXT(face);
#elif defined(GL_EXT_stencil_two_side)
	glActiveStencilFaceEXT(face);
#else
	os::Printer::log("glActiveStencilFace not supported", ELL_ERROR);
#endif
}


inline void COpenGLExtensionHandler::extGlGenBuffers(GLsizei n, GLuint *buffers)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGenBuffersARB)
		pGlGenBuffersARB(n, buffers);
#elif defined(GL_ARB_vertex_buffer_object)
	glGenBuffers(n, buffers);
#else
	os::Printer::log("glGenBuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlBindBuffer(GLenum target, GLuint buffer)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlBindBufferARB)
		pGlBindBufferARB(target, buffer);
#elif defined(GL_ARB_vertex_buffer_object)
	glBindBuffer(target, buffer);
#else
	os::Printer::log("glBindBuffer not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlBufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlBufferDataARB)
		pGlBufferDataARB(target, size, data, usage);
#elif defined(GL_ARB_vertex_buffer_object)
	glBufferData(target, size, data, usage);
#else
	os::Printer::log("glBufferData not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlDeleteBuffers(GLsizei n, const GLuint *buffers)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlDeleteBuffersARB)
		pGlDeleteBuffersARB(n, buffers);
#elif defined(GL_ARB_vertex_buffer_object)
	glDeleteBuffers(n, buffers);
#else
	os::Printer::log("glDeleteBuffers not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlBufferSubData(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlBufferSubDataARB)
		pGlBufferSubDataARB(target, offset, size, data);
#elif defined(GL_ARB_vertex_buffer_object)
	glBufferSubData(target, offset, size, data);
#else
	os::Printer::log("glBufferSubData not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlGetBufferSubData(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGetBufferSubDataARB)
		pGlGetBufferSubDataARB(target, offset, size, data);
#elif defined(GL_ARB_vertex_buffer_object)
	glGetBufferSubData(target, offset, size, data);
#else
	os::Printer::log("glGetBufferSubData not supported", ELL_ERROR);
#endif
}

inline void *COpenGLExtensionHandler::extGlMapBuffer(GLenum target, GLenum access)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlMapBufferARB)
		return pGlMapBufferARB(target, access);
	return 0;
#elif defined(GL_ARB_vertex_buffer_object)
	return glMapBuffer(target, access);
#else
	os::Printer::log("glMapBuffer not supported", ELL_ERROR);
	return 0;
#endif
}

inline GLboolean COpenGLExtensionHandler::extGlUnmapBuffer (GLenum target)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlUnmapBufferARB)
		return pGlUnmapBufferARB(target);
	return false;
#elif defined(GL_ARB_vertex_buffer_object)
	return glUnmapBuffer(target);
#else
	os::Printer::log("glUnmapBuffer not supported", ELL_ERROR);
	return false;
#endif
}

inline GLboolean COpenGLExtensionHandler::extGlIsBuffer (GLuint buffer)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlIsBufferARB)
		return pGlIsBufferARB(buffer);
	return false;
#elif defined(GL_ARB_vertex_buffer_object)
	return glIsBuffer(buffer);
#else
	os::Printer::log("glDeleteBuffers not supported", ELL_ERROR);
	return false;
#endif
}

inline void COpenGLExtensionHandler::extGlGetBufferParameteriv (GLenum target, GLenum pname, GLint *params)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGetBufferParameterivARB)
		pGlGetBufferParameterivARB(target, pname, params);
#elif defined(GL_ARB_vertex_buffer_object)
	glGetBufferParameteriv(target, pname, params);
#else
	os::Printer::log("glGetBufferParameteriv not supported", ELL_ERROR);
#endif
}

inline void COpenGLExtensionHandler::extGlGetBufferPointerv (GLenum target, GLenum pname, GLvoid **params)
{
#ifdef _GEO_OPENGL_USE_EXTPOINTER_
	if (pGlGetBufferPointervARB)
		pGlGetBufferPointervARB(target, pname, params);
#elif defined(GL_ARB_vertex_buffer_object)
	glGetBufferPointerv(target, pname, params);
#else
	os::Printer::log("glGetBufferPointerv not supported", ELL_ERROR);
#endif
}


}
}

#endif

#endif

