#include "Renderer.h"

#include "ext/sokol/sokol_config.h"
#include "sf/Array.h"

#if defined(SOKOL_D3D11)
	#define _WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <d3d11.h>
#elif defined(SOKOL_GLES3)
	#include <GLES3/gl3.h>
	#include <GLES2/gl2ext.h>

	#if SF_OS_EMSCRIPTEN
		#include <emscripten/html5.h>
	#endif

#elif defined(SOKOL_GLCORE33)

#if SF_OS_WINDOWS
	#define _WIN32_LEAN_AND_MEAN
	#include <Windows.h>

	typedef int GLsizei;
	typedef unsigned int GLuint;
	typedef unsigned int GLenum;
	typedef void (*PFNGLGENQUERIESPROC)(GLsizei n, GLuint *ids);
	static PFNGLGENQUERIESPROC sp_glGenQueries;
	#define glGenQueries sp_glGenQueries
	typedef void (*PFNGLBEGINQUERYPROC)(GLenum target, GLuint id);
	static PFNGLBEGINQUERYPROC sp_glBeginQuery;
	#define glBeginQuery sp_glBeginQuery
	typedef void (*PFNGLENDQUERYPROC)(GLenum target);
	static PFNGLENDQUERYPROC sp_glEndQuery;
	#define glEndQuery sp_glEndQuery
	typedef void (*PFNGLGETQUERYOBJECTUIVPROC)(GLuint id, GLenum pname, GLuint *params);
	static PFNGLGETQUERYOBJECTUIVPROC sp_glGetQueryObjectuiv;
	#define glGetQueryObjectuiv sp_glGetQueryObjectuiv

	#define GL_QUERY_RESULT 0x8866
	#define GL_TIME_ELAPSED_EXT 0x88BF

	static bool loadWindowsGlImp()
	{
		typedef PROC (WINAPI * PFN_wglGetProcAddress)(LPCSTR);
		HMODULE dll = LoadLibraryA("opengl32.dll");
		if (dll == NULL) return false;
		bool ok = true;
		PFN_wglGetProcAddress wglGetProcAddress = (PFN_wglGetProcAddress) GetProcAddress(dll, "wglGetProcAddress");
		ok = ok && (sp_glGenQueries = (PFNGLGENQUERIESPROC)(uintptr_t)wglGetProcAddress("glGenQueries")) != NULL;
		ok = ok && (sp_glBeginQuery = (PFNGLBEGINQUERYPROC)(uintptr_t)wglGetProcAddress("glBeginQuery")) != NULL;
		ok = ok && (sp_glEndQuery = (PFNGLENDQUERYPROC)(uintptr_t)wglGetProcAddress("glEndQuery")) != NULL;
		ok = ok && (sp_glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)(uintptr_t)wglGetProcAddress("glGetQueryObjectuiv")) != NULL;
		FreeLibrary(dll);
		return ok;

	}

#else
	#error "TODO"
#endif

#endif

namespace sp {

static sf::Array<PassTime> g_passTimes;

#if defined(SOKOL_D3D11)

struct Query
{
	sf::Symbol name;
	ID3D11Query *begin, *end;
};

struct QueryFrame
{
	ID3D11Query *disjointQuery;
	sf::Array<Query> queries;
	uint32_t numQueries = 0;
};

static ID3D11Device *queryDevice;
static ID3D11DeviceContext *queryContext;
static QueryFrame queryFrames[8];
static uint32_t queryFrameIndex = 0;

static void beginQueryFrame()
{
	if (!queryDevice) {
		sg_desc desc = sg_query_desc();
		queryDevice = (ID3D11Device*)desc.d3d11_device;
		queryContext = (ID3D11DeviceContext*)desc.d3d11_device_context;
	}

	QueryFrame &frame = queryFrames[queryFrameIndex];
	if (!frame.disjointQuery) {
		D3D11_QUERY_DESC desc = { };
		desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
		HRESULT hr = queryDevice->CreateQuery(&desc, &frame.disjointQuery);
		sf_assert(SUCCEEDED(hr));
	}

	queryContext->Begin(frame.disjointQuery);
}

static void endQueryFrame()
{
	{
		QueryFrame &frame = queryFrames[queryFrameIndex];
		queryContext->End(frame.disjointQuery);
	}

	queryFrameIndex = (queryFrameIndex + 1) % sf_arraysize(queryFrames);

	QueryFrame &frame = queryFrames[queryFrameIndex];
	if (frame.disjointQuery) {
		while (queryContext->GetData(frame.disjointQuery, NULL, 0, 0) == S_FALSE) {
			Sleep(1);
		}
		D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint;
		queryContext->GetData(frame.disjointQuery, &disjoint, sizeof(disjoint), 0);
		if (!disjoint.Disjoint) {
			g_passTimes.reserve(frame.queries.size);
			g_passTimes.clear();
			for (Query &query : frame.queries) {
				PassTime &time = g_passTimes.push();
				time.name = query.name;
				UINT64 begin, end;
				queryContext->GetData(query.begin, &begin, sizeof(UINT64), 0);
				queryContext->GetData(query.end, &end, sizeof(UINT64), 0);
				time.time = (double)(end - begin) / (double)disjoint.Frequency;
			}
		}
		frame.queries.clear();
		frame.numQueries = 0;
	}
}

static void beginQueryImp(sf::Symbol name)
{
	QueryFrame &frame = queryFrames[queryFrameIndex];
	if (frame.numQueries == frame.queries.size) {
		Query &query = frame.queries.push();
		D3D11_QUERY_DESC desc = { };
		desc.Query = D3D11_QUERY_TIMESTAMP;
		HRESULT hr = queryDevice->CreateQuery(&desc, &query.begin);
		sf_assert(SUCCEEDED(hr));
		hr = queryDevice->CreateQuery(&desc, &query.end);
		sf_assert(SUCCEEDED(hr));
	}
	Query &query = frame.queries[frame.numQueries];
	query.name = name;
	queryContext->End(query.begin);
}

static void endQueryImp()
{
	QueryFrame &frame = queryFrames[queryFrameIndex];
	Query &query = frame.queries[frame.numQueries++];
	queryContext->End(query.end);
}

#elif defined(SOKOL_GLES3) || defined(SOKOL_GLCORE33)

struct Query
{
	sf::Symbol name;
	GLuint query = 0;
};

struct QueryFrame
{
	sf::Array<Query> queries;
	uint32_t numQueries = 0;
};

static bool isQueryInitialized = false;
static bool hasQuerySupport = false;
static QueryFrame queryFrames[8];
static uint32_t queryFrameIndex = 0;

static void beginQueryFrame()
{
	if (!isQueryInitialized) {
		isQueryInitialized = true;
		#if SF_OS_EMSCRIPTEN
		{
			EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_get_current_context();
			if (ctx) {
				if (emscripten_webgl_enable_extension(ctx, "EXT_disjoint_timer_query")) hasQuerySupport = true;
				if (emscripten_webgl_enable_extension(ctx, "EXT_disjoint_timer_query_webgl2")) hasQuerySupport = true;
			}
		}
		#elif SF_OS_WINDOWS
		{
			hasQuerySupport = loadWindowsGlImp();
		}
		#else
			#error "TODO"
		#endif
	}

	if (!hasQuerySupport) return;
}

static void endQueryFrame()
{
	if (!hasQuerySupport) return;

	queryFrameIndex = (queryFrameIndex + 1) % sf_arraysize(queryFrames);

	QueryFrame &frame = queryFrames[queryFrameIndex];
	g_passTimes.reserve(frame.queries.size);
	g_passTimes.clear();
	for (Query &query : frame.queries) {
		PassTime &time = g_passTimes.push();
		GLuint duration = 0;
		glGetQueryObjectuiv(query.query, GL_QUERY_RESULT, &duration);
		time.name = query.name;
		time.time = (double)duration * 1e-9;
	}
	frame.queries.clear();
	frame.numQueries = 0;
}

static void beginQueryImp(sf::Symbol name)
{
	if (!hasQuerySupport) return;

	QueryFrame &frame = queryFrames[queryFrameIndex];
	if (frame.numQueries == frame.queries.size) {
		Query &query = frame.queries.push();
		glGenQueries(1, &query.query);
	}
	Query &query = frame.queries[frame.numQueries];
	query.name = name;
	glBeginQuery(GL_TIME_ELAPSED_EXT, query.query);
}

static void endQueryImp()
{
	if (!hasQuerySupport) return;

	QueryFrame &frame = queryFrames[queryFrameIndex];
	Query &query = frame.queries[frame.numQueries++];
	glEndQuery(GL_TIME_ELAPSED_EXT);
}

#else

// TODO

#endif

void RenderTarget::init(const char *label, const sf::Vec2i &resolution, sg_pixel_format format, uint32_t samples, const sg_image_desc &desc)
{
	if (image.id) {
		sg_destroy_image(image);
		image.id = 0;
	}

	this->resolution = resolution;
	this->format = format;
	this->msaaSamples = samples;

	sg_image_desc d = desc;
	d.render_target = true;
	d.width = resolution.x;
	d.height = resolution.y;
	d.pixel_format = format;
	d.sample_count = samples;
	d.label = label;
	d.mag_filter = desc.mag_filter ? desc.mag_filter : SG_FILTER_LINEAR;
	d.min_filter = desc.min_filter ? desc.min_filter : SG_FILTER_LINEAR;
	d.wrap_u = desc.wrap_u ? desc.wrap_u : SG_WRAP_CLAMP_TO_EDGE;
	d.wrap_v = desc.wrap_v ? desc.wrap_v : SG_WRAP_CLAMP_TO_EDGE;
	d.wrap_w = desc.wrap_w ? desc.wrap_w : SG_WRAP_CLAMP_TO_EDGE;
	image = sg_make_image(&d);
}

void RenderPass::init(const char *label, sf::Slice<const RenderTarget*> targets)
{
	if (pass.id) {
		sg_destroy_pass(pass);
		pass.id = 0;
	}

	name = sf::Symbol(label);

	sg_pass_desc d = { };
	d.label = label;
	desc.msaaSamples = 0;

	uint32_t numColors = 0;
	for (const RenderTarget *target : targets) {
		sf_assert(desc.msaaSamples == 0 || desc.msaaSamples == target->msaaSamples);
		desc.msaaSamples = target->msaaSamples;
		if (target->format == SG_PIXELFORMAT_DEPTH || target->format == SG_PIXELFORMAT_DEPTH_STENCIL) {
			d.depth_stencil_attachment.image = target->image;
			desc.depthFormat = target->format;
		} else {
			sf_assert(numColors < SG_MAX_COLOR_ATTACHMENTS);
			if (numColors == 0) {
				desc.colorFormat = target->format;
				resolution = target->resolution;
			}
			d.color_attachments[numColors++].image = target->image;
		}
	}

	pass = sg_make_pass(&d);
}

void RenderPass::init(const char *label, const RenderTarget &a) { const RenderTarget *t[] = { &a, }; init(label, t); }
void RenderPass::init(const char *label, const RenderTarget &a, const RenderTarget &b) { const RenderTarget *t[] = { &a, &b, }; init(label, t); }
void RenderPass::init(const char *label, const RenderTarget &a, const RenderTarget &b, const RenderTarget &c) { const RenderTarget *t[] = { &a, &b, &c, }; init(label, t); }
void RenderPass::init(const char *label, const RenderTarget &a, const RenderTarget &b, const RenderTarget &c, const RenderTarget &d) { const RenderTarget *t[] = { &a, &b, &c, &d, }; init(label, t); }

sg_pipeline_desc &Pipeline::init(sg_shader shader, uint32_t flags)
{
	for (PipelineImp &imp : imps) {
		if (imp.pipeline.id) {
			sg_destroy_pipeline(imp.pipeline);
			imp.pipeline.id = 0;
			imp.framebufferType = 0;
		}
	}

	sg_pipeline_desc &d = desc;

	d.shader = shader;

	if (flags & sp::PipeDepthTest) {
		d.depth_stencil.depth_compare_func = SG_COMPAREFUNC_LESS_EQUAL;
	}

	if (flags & sp::PipeDepthWrite) {
		d.depth_stencil.depth_write_enabled = true;
		d.depth_stencil.depth_compare_func = SG_COMPAREFUNC_LESS_EQUAL;
	}

	if (flags & sp::PipeCullCCW) {
		d.rasterizer.cull_mode = SG_CULLMODE_BACK;
		d.rasterizer.face_winding = SG_FACEWINDING_CCW;
	}

	if (flags & sp::PipeCullAuto) {
		d.rasterizer.cull_mode = SG_CULLMODE_BACK;
		d.rasterizer.face_winding = sg_query_features().origin_top_left ? SG_FACEWINDING_CCW : SG_FACEWINDING_CW;
	}

	if (flags & sp::PipeIndex16) {
		d.index_type = SG_INDEXTYPE_UINT16;
	}

	if (flags & sp::PipeIndex32) {
		d.index_type = SG_INDEXTYPE_UINT32;
	}

	if (flags & sp::PipeBlendOver) {
		d.blend.enabled = true;
		d.blend.src_factor_alpha = d.blend.src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA;
		d.blend.dst_factor_alpha = d.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	}

	if (flags & sp::PipeBlendPremultiply) {
		d.blend.enabled = true;
		d.blend.src_factor_alpha = d.blend.src_factor_rgb = SG_BLENDFACTOR_ONE;
		d.blend.dst_factor_alpha = d.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
	}

	if (flags & sp::PipeBlendAdd) {
		d.blend.enabled = true;
		d.blend.src_factor_alpha = d.blend.src_factor_rgb = SG_BLENDFACTOR_ONE;
		d.blend.dst_factor_alpha = d.blend.dst_factor_rgb = SG_BLENDFACTOR_ONE;
	}

	if (flags & sp::PipeVertexFloat2) {
		d.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT2;
	}

	return d;
}

sf::Array<FramebufferDesc> g_framebufferTypes;
uint32_t g_activeFramebufferType = 0;
sg_pipeline g_prevPipeline = { };

const FramebufferDesc defaultFramebufferDesc = {
	_SG_PIXELFORMAT_DEFAULT, _SG_PIXELFORMAT_DEFAULT, 0u,
};

static uint32_t findFramebufferType(const FramebufferDesc &desc)
{
	FramebufferDesc *ptr = sf::find(g_framebufferTypes, desc);
	if (ptr) return (uint32_t)(ptr - g_framebufferTypes.data) + 1;
	g_framebufferTypes.push(desc);
	return g_framebufferTypes.size;
}


bool Pipeline::bind()
{
	sf_assert(g_activeFramebufferType != 0);

	uint32_t framebufferType = g_activeFramebufferType;
	uint32_t use = ++lastUse;
	for (PipelineImp &imp : imps) {
		if (imp.framebufferType == framebufferType) {
			if (imp.pipeline.id != g_prevPipeline.id) {
				sg_apply_pipeline(imp.pipeline);
				imp.lastUse = use;
				g_prevPipeline = imp.pipeline;
				return true;
			} else {
				return false;
			}
		}
	}

	PipelineImp *bestImp = NULL;
	uint32_t bestUse = UINT32_MAX;
	for (PipelineImp &imp : imps) {
		if (imp.lastUse < bestUse) {
			bestUse = imp.lastUse;
			bestImp = &imp;
		}
	}

	if (bestImp->pipeline.id) {
		sg_destroy_pipeline(bestImp->pipeline);
	}

	FramebufferDesc fbDesc = getFramebufferTypeDesc(framebufferType);
	desc.blend.color_format = fbDesc.colorFormat;
	desc.blend.depth_format = fbDesc.depthFormat;
	desc.rasterizer.sample_count = fbDesc.msaaSamples;
	bestImp->framebufferType = g_activeFramebufferType;
	bestImp->pipeline = sg_make_pipeline(&desc);
	bestImp->lastUse = use;

	sg_apply_pipeline(bestImp->pipeline);
	g_prevPipeline = bestImp->pipeline;
	return true;
}

FramebufferDesc getFramebufferTypeDesc(uint32_t typeIndex)
{
	return g_framebufferTypes[typeIndex - 1];
}

static const sg_pass_action defaultAction = {
	0,
	{ 
		{ SG_ACTION_CLEAR, { 0.0f, 0.0f, 0.0f, 0.0f } },
		{ SG_ACTION_CLEAR, { 0.0f, 0.0f, 0.0f, 0.0f } },
		{ SG_ACTION_CLEAR, { 0.0f, 0.0f, 0.0f, 0.0f } },
		{ SG_ACTION_CLEAR, { 0.0f, 0.0f, 0.0f, 0.0f } },
	},
	{ SG_ACTION_CLEAR, 1.0f },
	{ SG_ACTION_CLEAR, 0 },
	0,
};

void beginPass(const RenderPass &pass, const sg_pass_action *action)
{
	sf_assert(g_activeFramebufferType == 0);
	if (!action) action = &defaultAction;

	beginQueryImp(pass.name);
	sg_begin_pass(pass.pass, action);

	g_activeFramebufferType = findFramebufferType(pass.desc);
	g_prevPipeline.id = 0;
}

static const sf::Symbol defaultPassName { "Default pass" };

void beginDefaultPass(uint32_t width, uint32_t height, const sg_pass_action *action)
{
	sf_assert(g_activeFramebufferType == 0);
	if (!action) action = &defaultAction;

	beginQueryImp(defaultPassName);
	sg_begin_default_pass(action, (int)width, (int)height);

	g_activeFramebufferType = findFramebufferType(defaultFramebufferDesc);
	g_prevPipeline.id = 0;
}

void endPass()
{
	sg_end_pass();
	endQueryImp();
	g_activeFramebufferType = 0;
}

void beginFrame()
{
	beginQueryFrame();
}

void endFrame()
{
	endQueryFrame();
}

sf::Slice<const PassTime> getPassTimes()
{
	return g_passTimes;
}


}

