#pragma once
#include "RawModel.h"
#include "..\Textures\Texture.h"
#include <iostream>
class TexturedModel
{
public:
	TexturedModel();
	TexturedModel(RawModel rawModel, Texture modelTexture);
	RawModel GetRawModel();
	Texture GetModelTexture();
	
private:
	RawModel rawModel;
	Texture modelTexture;
};
