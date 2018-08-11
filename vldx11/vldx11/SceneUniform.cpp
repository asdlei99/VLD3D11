#include "SceneUniform.h"



SceneUniform::SceneUniform() : sceneUniformBuffer(nullptr)
{
}


SceneUniform::~SceneUniform()
{
	if (sceneUniformBuffer != nullptr) sceneUniformBuffer->Release();
}

bool SceneUniform::CreateBuffer(ID3D11Device* d3d11Device)
{
	HRESULT hr;

	//PerScene Constant Buffer Start
	D3D11_BUFFER_DESC sceneBufferDesc;
	ZeroMemory(&sceneBufferDesc, sizeof(D3D11_BUFFER_DESC));
	sceneBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	sceneBufferDesc.ByteWidth = sizeof(SceneUniformData);
	sceneBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sceneBufferDesc.CPUAccessFlags = 0;
	sceneBufferDesc.MiscFlags = 0;
	hr = d3d11Device->CreateBuffer(&sceneBufferDesc, NULL, &sceneUniformBuffer);
	if (!CheckError(hr, nullptr)) return false;
	//PerScene Constant Buffer Finish

	return true;
}

void SceneUniform::SceneUniformBufferData(ID3D11DeviceContext* d3d11DevCon)
{
	d3d11DevCon->UpdateSubresource(sceneUniformBuffer, 0, NULL, &sceneUniformData, 0, 0);
}

void SceneUniform::SetSceneUniformBufferVS(ID3D11DeviceContext* d3d11DevCon)
{
	d3d11DevCon->VSSetConstantBuffers(SCENE_UNIFORM_SOLT, 1, &sceneUniformBuffer);
}

void SceneUniform::SetSceneUniformBufferPS(ID3D11DeviceContext* d3d11DevCon)
{
	d3d11DevCon->PSSetConstantBuffers(SCENE_UNIFORM_SOLT, 1, &sceneUniformBuffer);
}

void SceneUniform::SetSceneUniformBufferVSPS(ID3D11DeviceContext* d3d11DevCon)
{
	SetSceneUniformBufferVS(d3d11DevCon);
	SetSceneUniformBufferPS(d3d11DevCon);
}

void SceneUniform::SetLight(Light* pLight)
{
	sceneUniformData.lightCol = pLight->col;
	sceneUniformData.lightPos = pLight->pos;
}

void SceneUniform::SetCamera(Camera* pCamera)
{
	sceneUniformData.farClip = pCamera->farClipPlane;
}

bool SceneUniform::InitSceneUniform(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon)
{
	if (!CreateBuffer(d3d11Device)) return false;
	SceneUniformBufferData(d3d11DevCon);
	SetSceneUniformBufferVSPS(d3d11DevCon);
	printf("frameUniform create buffer done!\n");
	return true;
}