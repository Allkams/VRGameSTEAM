#pragma once

#include "StereoLayerShapes.h"
#include "OculusXRPassthroughMesh.h"
#include "OculusXRHMDPrivate.h"

UENUM()
enum EOculusXRColorMapType
{
	/** None*/
	ColorMapType_None UMETA(DisplayName = "None"),

	/** Grayscale to color */
	ColorMapType_GrayscaleToColor UMETA(DisplayName = "Grayscale To Color"),

	/** Grayscale */
	ColorMapType_Grayscale UMETA(DisplayName = "Grayscale"),

	/** Color Adjustment */
	ColorMapType_ColorAdjustment UMETA(DisplayName = "Color Adjustment"),

	ColorMapType_MAX,
};

UENUM()
enum EOculusXRPassthroughLayerOrder
{
	/** Layer is rendered on top of scene */
	PassthroughLayerOrder_Overlay UMETA(DisplayName = "Overlay"),

	/** Layer is rendered under scene */
	PassthroughLayerOrder_Underlay UMETA(DisplayName = "Underlay"),

	PassthroughLayerOrder_MAX,
};

struct FEdgeStyleParameters {
	FEdgeStyleParameters()
	:	bEnableEdgeColor(false)
	,	bEnableColorMap(false)
	,	TextureOpacityFactor(1.0f)
	,	EdgeColor{}
	,	ColorMapType{}
	,	ColorMapData{}
	{

	};

	FEdgeStyleParameters(
		bool bEnableEdgeColor, 
		bool bEnableColorMap, 
		float TextureOpacityFactor, 
		float Brightness, 
		float Contrast, 
		float Posterize, 
		FLinearColor EdgeColor, 
		FLinearColor ColorScale, 
		FLinearColor ColorOffset, 
		EOculusXRColorMapType InColorMapType,
		const TArray<FLinearColor>& InColorMapGradient)
	:	bEnableEdgeColor(bEnableEdgeColor)
	,	bEnableColorMap(bEnableColorMap)
	,	TextureOpacityFactor(TextureOpacityFactor)
	,	Brightness(Brightness)
	,	Contrast(Contrast)
	,	Posterize(Posterize)
	,	EdgeColor(EdgeColor)
	,	ColorScale(ColorScale)
	,	ColorOffset(ColorOffset)
	,	ColorMapType(GetOVRPColorMapType(InColorMapType))
	{
		ColorMapData = GenerateColorMapData(InColorMapType, InColorMapGradient);
	};

	bool bEnableEdgeColor;
	bool bEnableColorMap;
	float TextureOpacityFactor;
	float Brightness;
	float Contrast;
	float Posterize;
	FLinearColor EdgeColor;
	FLinearColor ColorScale;
	FLinearColor ColorOffset;
	ovrpInsightPassthroughColorMapType ColorMapType;
	TArray<uint8> ColorMapData;

private:

	/** Generates the corresponding color map based on given color map type */
	TArray<uint8> GenerateColorMapData(EOculusXRColorMapType InColorMapType, const TArray<FLinearColor>& InColorMapGradient);

	/** Generates a grayscale to color color map based on given gradient --> It also applies the color scale and offset */
	TArray<uint8> GenerateMonoToRGBA(const TArray<FLinearColor>& InGradient, const TArray<uint8>& InColorMapData);

	/** Generates a grayscale color map with given Brightness/Contrast/Posterize settings */
	TArray<uint8> GenerateMonoBrightnessContrastPosterizeMap();

	/** Generates a luminance based colormap from the the Brightness/Contrast */
	TArray<uint8> GenerateBrightnessContrastSaturationColorMap();

	/** Converts `EColorMapType` to `ovrpInsightPassthroughColorMapType` */
	ovrpInsightPassthroughColorMapType GetOVRPColorMapType(EOculusXRColorMapType InColorMapType);

};

class OCULUSXRHMD_API FReconstructedLayer : public IStereoLayerShape
{
	STEREO_LAYER_SHAPE_BOILERPLATE(FReconstructedLayer)

public:
	FReconstructedLayer() {};
	FReconstructedLayer(const FEdgeStyleParameters& EdgeStyleParameters, EOculusXRPassthroughLayerOrder PassthroughLayerOrder)
	:	EdgeStyleParameters(EdgeStyleParameters),
		PassthroughLayerOrder(PassthroughLayerOrder)
	{
	};
	FEdgeStyleParameters EdgeStyleParameters;
	EOculusXRPassthroughLayerOrder PassthroughLayerOrder;
};

struct FUserDefinedGeometryDesc
{
	FUserDefinedGeometryDesc(const FString& MeshName, OculusXRHMD::FOculusPassthroughMeshRef PassthroughMesh, const FTransform& Transform, bool bUpdateTransform)
	:	MeshName(MeshName)
	,	PassthroughMesh(PassthroughMesh)
	,	Transform(Transform)
	,	bUpdateTransform(bUpdateTransform)
	{
	};

	FString MeshName;
	OculusXRHMD::FOculusPassthroughMeshRef PassthroughMesh;
	FTransform  Transform;
	bool bUpdateTransform;
};

class OCULUSXRHMD_API FUserDefinedLayer : public IStereoLayerShape
{
	STEREO_LAYER_SHAPE_BOILERPLATE(FUserDefinedLayer)

public:
	FUserDefinedLayer() {};
	FUserDefinedLayer(TArray<FUserDefinedGeometryDesc> InUserGeometryList, const FEdgeStyleParameters& EdgeStyleParameters, EOculusXRPassthroughLayerOrder PassthroughLayerOrder)
	:	UserGeometryList{}
	,	EdgeStyleParameters(EdgeStyleParameters)
	,	PassthroughLayerOrder(PassthroughLayerOrder)
	{
		UserGeometryList = InUserGeometryList;
	}

	TArray<FUserDefinedGeometryDesc> UserGeometryList;
	FEdgeStyleParameters EdgeStyleParameters;
	EOculusXRPassthroughLayerOrder PassthroughLayerOrder;

private:

};
