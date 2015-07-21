#include "extension_loader.h"
#include <iostream>
FPglMatrixLoadIdentityEXT glMatrixLoadIdentityEXT = 0;
FPglMatrixPushEXT glMatrixPushEXT = 0;
FPglMatrixPopEXT glMatrixPopEXT = 0;
FPglMatrixOrthoEXT glMatrixOrthoEXT = 0;
FPglMatrixLoadfEXT glMatrixLoadfEXT = 0;
FPglGenPathsNV glGenPathsNV = 0;
FPglDeletePathsNV glDeletePathsNV = 0;
FPglIsPathNV glIsPathNV = 0;
FPglPathCommandsNV glPathCommandsNV = 0;
FPglPathCoordsNV glPathCoordsNV = 0;
FPglPathSubCommandsNV glPathSubCommandsNV = 0;
FPglPathSubCoordsNV glPathSubCoordsNV = 0;
FPglPathStringNV glPathStringNV = 0;
FPglPathGlyphsNV glPathGlyphsNV = 0;
FPglPathGlyphRangeNV glPathGlyphRangeNV = 0;
FPglWeightPathsNV glWeightPathsNV = 0;
FPglCopyPathNV glCopyPathNV = 0;
FPglInterpolatePathsNV glInterpolatePathsNV = 0;
FPglTransformPathNV glTransformPathNV = 0;
FPglPathParameterivNV glPathParameterivNV = 0;
FPglPathParameteriNV glPathParameteriNV = 0;
FPglPathParameterfvNV glPathParameterfvNV = 0;
FPglPathParameterfNV glPathParameterfNV = 0;
FPglPathDashArrayNV glPathDashArrayNV = 0;
FPglPathStencilFuncNV glPathStencilFuncNV = 0;
FPglPathStencilDepthOffsetNV glPathStencilDepthOffsetNV = 0;
FPglStencilFillPathNV glStencilFillPathNV = 0;
FPglStencilStrokePathNV glStencilStrokePathNV = 0;
FPglStencilFillPathInstancedNV glStencilFillPathInstancedNV = 0;
FPglStencilStrokePathInstancedNV glStencilStrokePathInstancedNV = 0;
FPglPathCoverDepthFuncNV glPathCoverDepthFuncNV = 0;
FPglPathColorGenNV glPathColorGenNV = 0;
FPglPathTexGenNV glPathTexGenNV = 0;
FPglPathFogGenNV glPathFogGenNV = 0;
FPglCoverFillPathNV glCoverFillPathNV = 0;
FPglCoverStrokePathNV glCoverStrokePathNV = 0;
FPglCoverFillPathInstancedNV glCoverFillPathInstancedNV = 0;
FPglCoverStrokePathInstancedNV glCoverStrokePathInstancedNV = 0;
FPglGetPathParameterivNV glGetPathParameterivNV = 0;
FPglGetPathParameterfvNV glGetPathParameterfvNV = 0;
FPglGetPathCommandsNV glGetPathCommandsNV = 0;
FPglGetPathCoordsNV glGetPathCoordsNV = 0;
FPglGetPathDashArrayNV glGetPathDashArrayNV = 0;
FPglGetPathMetricsNV glGetPathMetricsNV = 0;
FPglGetPathMetricRangeNV glGetPathMetricRangeNV = 0;
FPglGetPathSpacingNV glGetPathSpacingNV = 0;
FPglGetPathColorGenivNV glGetPathColorGenivNV = 0;
FPglGetPathColorGenfvNV glGetPathColorGenfvNV = 0;
FPglGetPathTexGenivNV glGetPathTexGenivNV = 0;
FPglGetPathTexGenfvNV glGetPathTexGenfvNV = 0;
FPglIsPointInFillPathNV glIsPointInFillPathNV = 0;
FPglIsPointInStrokePathNV glIsPointInStrokePathNV = 0;
FPglGetPathLengthNV glGetPathLengthNV = 0;
FPglPointAlongPathNV glPointAlongPathNV = 0;

extension_loader::extension_loader()
{
}


extension_loader::~extension_loader()
{
}

void extension_loader::load_extension(QOpenGLContext* context)
{
	if (context == 0)
	{
		std::cout << "error initializing OpenGL" << std::endl;
		return;
	}
	if (!context->hasExtension("GL_NV_path_rendering"))
	{
		std::cout << "NVPR extension does not exist" << std::endl;
		return;
	}
	if (!context->hasExtension("GL_NV_path_rendering"))
	{
		std::cout << "DSA extension does not exist" << std::endl;
		return;
	}
	std::cout << "OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;

	glMatrixLoadIdentityEXT = (FPglMatrixLoadIdentityEXT)load_func(context, "glMatrixLoadIdentityEXT");
	glMatrixPushEXT = (FPglMatrixPushEXT)load_func(context, "glMatrixPushEXT");
	glMatrixPopEXT = (FPglMatrixPopEXT)load_func(context, "glMatrixPopEXT");
	glMatrixOrthoEXT = (FPglMatrixOrthoEXT)load_func(context, "glMatrixOrthoEXT");
	glMatrixLoadfEXT = (FPglMatrixLoadfEXT)load_func(context, "glMatrixLoadfEXT");
	glGenPathsNV = (FPglGenPathsNV)load_func(context, "glGenPathsNV");
	glDeletePathsNV = (FPglDeletePathsNV)load_func(context, "glDeletePathsNV");
	glIsPathNV = (FPglIsPathNV)load_func(context, "glIsPathNV");
	glPathCommandsNV = (FPglPathCommandsNV)load_func(context, "glPathCommandsNV");
	glPathCoordsNV = (FPglPathCoordsNV)load_func(context, "glPathCoordsNV");
	glPathSubCommandsNV = (FPglPathSubCommandsNV)load_func(context, "glPathSubCommandsNV");
	glPathSubCoordsNV = (FPglPathSubCoordsNV)load_func(context, "glPathSubCoordsNV");
	glPathStringNV = (FPglPathStringNV)load_func(context, "glPathStringNV");
	glPathGlyphsNV = (FPglPathGlyphsNV)load_func(context, "glPathGlyphsNV");
	glPathGlyphRangeNV = (FPglPathGlyphRangeNV)load_func(context, "glPathGlyphRangeNV");
	glWeightPathsNV = (FPglWeightPathsNV)load_func(context, "glWeightPathsNV");
	glCopyPathNV = (FPglCopyPathNV)load_func(context, "glCopyPathNV");
	glInterpolatePathsNV = (FPglInterpolatePathsNV)load_func(context, "glInterpolatePathsNV");
	glTransformPathNV = (FPglTransformPathNV)load_func(context, "glTransformPathNV");
	glPathParameterivNV = (FPglPathParameterivNV)load_func(context, "glPathParameterivNV");
	glPathParameteriNV = (FPglPathParameteriNV)load_func(context, "glPathParameteriNV");
	glPathParameterfvNV = (FPglPathParameterfvNV)load_func(context, "glPathParameterfvNV");
	glPathParameterfNV = (FPglPathParameterfNV)load_func(context, "glPathParameterfNV");
	glPathDashArrayNV = (FPglPathDashArrayNV)load_func(context, "glPathDashArrayNV");
	glPathStencilFuncNV = (FPglPathStencilFuncNV)load_func(context, "glPathStencilFuncNV");
	glPathStencilDepthOffsetNV = (FPglPathStencilDepthOffsetNV)load_func(context, "glPathStencilDepthOffsetNV");
	glStencilFillPathNV = (FPglStencilFillPathNV)load_func(context, "glStencilFillPathNV");
	glStencilStrokePathNV = (FPglStencilStrokePathNV)load_func(context, "glStencilStrokePathNV");
	glStencilFillPathInstancedNV = (FPglStencilFillPathInstancedNV)load_func(context, "glStencilFillPathInstancedNV");
	glStencilStrokePathInstancedNV = (FPglStencilStrokePathInstancedNV)load_func(context, "glStencilStrokePathInstancedNV");
	glPathCoverDepthFuncNV = (FPglPathCoverDepthFuncNV)load_func(context, "glPathCoverDepthFuncNV");
	glPathColorGenNV = (FPglPathColorGenNV)load_func(context, "glPathColorGenNV");
	glPathTexGenNV = (FPglPathTexGenNV)load_func(context, "glPathTexGenNV");
	glPathFogGenNV = (FPglPathFogGenNV)load_func(context, "glPathFogGenNV");
	glCoverFillPathNV = (FPglCoverFillPathNV)load_func(context, "glCoverFillPathNV");
	glCoverStrokePathNV = (FPglCoverStrokePathNV)load_func(context, "glCoverStrokePathNV");
	glCoverFillPathInstancedNV = (FPglCoverFillPathInstancedNV)load_func(context, "glCoverFillPathInstancedNV");
	glCoverStrokePathInstancedNV = (FPglCoverStrokePathInstancedNV)load_func(context, "glCoverStrokePathInstancedNV");
	glGetPathParameterivNV = (FPglGetPathParameterivNV)load_func(context, "glGetPathParameterivNV");
	glGetPathParameterfvNV = (FPglGetPathParameterfvNV)load_func(context, "glGetPathParameterfvNV");
	glGetPathCommandsNV = (FPglGetPathCommandsNV)load_func(context, "glGetPathCommandsNV");
	glGetPathCoordsNV = (FPglGetPathCoordsNV)load_func(context, "glGetPathCoordsNV");
	glGetPathDashArrayNV = (FPglGetPathDashArrayNV)load_func(context, "glGetPathDashArrayNV");
	glGetPathMetricsNV = (FPglGetPathMetricsNV)load_func(context, "glGetPathMetricsNV");
	glGetPathMetricRangeNV = (FPglGetPathMetricRangeNV)load_func(context, "glGetPathMetricRangeNV");
	glGetPathSpacingNV = (FPglGetPathSpacingNV)load_func(context, "glGetPathSpacingNV");
	glGetPathColorGenivNV = (FPglGetPathColorGenivNV)load_func(context, "glGetPathColorGenivNV");
	glGetPathColorGenfvNV = (FPglGetPathColorGenfvNV)load_func(context, "glGetPathColorGenfvNV");
	glGetPathTexGenivNV = (FPglGetPathTexGenivNV)load_func(context, "glGetPathTexGenivNV");
	glGetPathTexGenfvNV = (FPglGetPathTexGenfvNV)load_func(context, "glGetPathTexGenfvNV");
	glIsPointInFillPathNV = (FPglIsPointInFillPathNV)load_func(context, "glIsPointInFillPathNV");
	glIsPointInStrokePathNV = (FPglIsPointInStrokePathNV)load_func(context, "glIsPointInStrokePathNV");
	glGetPathLengthNV = (FPglGetPathLengthNV)load_func(context, "glGetPathLengthNV");
	glPointAlongPathNV = (FPglPointAlongPathNV)load_func(context, "glPointAlongPathNV");
}

QFunctionPointer extension_loader::load_func(QOpenGLContext* context, const QByteArray &fun_name)
{
	auto fun = context->getProcAddress(fun_name);
	if (fun == 0)
	{
		std::cout << "Function not found: " << fun_name.toStdString() << std::endl;
		exit(1);
	}
	return fun;
}