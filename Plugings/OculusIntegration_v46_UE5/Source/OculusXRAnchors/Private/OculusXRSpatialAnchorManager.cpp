/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#include "OculusXRSpatialAnchorManager.h"


namespace OculusXRAnchors
{
	bool FOculusXRSpatialAnchorManager::CreateSpatialAnchor(const FTransform& InTransform, uint64& OutRequestId)
	{
		return CreateAnchor(InTransform, OutRequestId, FTransform::Identity);
	}
}