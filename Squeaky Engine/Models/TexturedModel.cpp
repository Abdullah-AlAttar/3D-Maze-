#include "TexturedModel.h"


TexturedModel::TexturedModel()
{
}

TexturedModel::TexturedModel(RawModel rawModel, Texture modelTexture)
{
	this->rawModel = rawModel;
	this->modelTexture = modelTexture;
}

RawModel TexturedModel::GetRawModel()
{
	return rawModel;
}

Texture TexturedModel::GetModelTexture()
{
	return modelTexture;
}
