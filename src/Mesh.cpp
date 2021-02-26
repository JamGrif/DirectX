#include "Mesh.h"

Mesh::Mesh()
	:m_pLocalDevice(EngineStatics::GetD3DDevice()), m_pLocalContext(EngineStatics::GetD3DContext()), m_Position({ 0.0f, 0.0f, 0.0f }), m_Rotation({ 0.0f, 0.0f, 0.0f }), m_Scale({ 1.0, 1.0f, 1.0f })
{
	LoadMesh((char*)"res/mesh/cube.obj");						//Loads cube mesh by default - Fills vertex buffer
	SetupMesh();												//Sets up properties to render the mesh
	AddTexture((char*)"res/textures/NOASSIGNEDTEXTURE.png");	//Loads NOASSIGNEDTEXTURE.png by default

	
}

Mesh::~Mesh()
{
	m_pLocalDevice = nullptr;
	m_pLocalContext = nullptr;

	SafeRelease(m_pcbPerObject);
	SafeRelease(m_pcbPerFrame);

	//SafeRelease(m_pTexture);
	SafeRelease(m_pSampler);

	m_currentState = nullptr;
	m_currentVShader = nullptr;
	m_currentPShader = nullptr;
	m_currentInputLayout = nullptr;

	SafeRelease(m_pVertexBuffer);


}

void Mesh::Update()
{
	Draw();
}

void Mesh::Draw()
{
	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	XMMATRIX translation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMMATRIX rotation = XMMatrixRotationX(XMConvertToRadians(m_Rotation.x));
	rotation *= XMMatrixRotationY(XMConvertToRadians(m_Rotation.y));
	rotation *= XMMatrixRotationZ(XMConvertToRadians(m_Rotation.z));
	world = scale * rotation * translation;

	//Required after world_transform for lighting
	//XMMATRIX transpose;
	//transpose = XMMatrixTranspose(world);
	//
	//XMVECTOR determinant; //Inverse function returns determinant, bit it isnt used
	//XMMATRIX inverse;
	//inverse = XMMatrixInverse(&determinant, world);

	XMMATRIX A = world;
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR det = XMMatrixDeterminant(A);
	XMMATRIX inverse = XMMatrixTranspose(XMMatrixInverse(&det, A));


	m_pLocalContext->IASetInputLayout(m_currentInputLayout); // Bind input layout to device

	//Set constant buffer values

	m_object_cb_values.WorldViewProjection = world * (EngineStatics::GetCamera()->GetViewMatrix()) * (EngineStatics::GetProjectionMatrix());
	m_object_cb_values.WorldMatrix = world;
	m_object_cb_values.WorldInvTransposeMatrix = inverse;
	m_object_cb_values.material = meshMaterial;

	m_frame_cb_values.DirLight = *EngineStatics::GetLightManager()->GetDirectionalLight();
	m_frame_cb_values.position = EngineStatics::GetCamera()->GetPosition();

	D3D11_MAPPED_SUBRESOURCE ms;
	//Update and set OBJECT constant buffer

	m_pLocalContext->Map(m_pcbPerObject, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); //Lock buffer for writing
	memcpy(ms.pData, &m_object_cb_values, sizeof(PerObject_CONSTANT_BUFFER));		//Copy data in	
	m_pLocalContext->Unmap(m_pcbPerObject, NULL);									//Unlock buffer
	m_pLocalContext->VSSetConstantBuffers(0, 1, &m_pcbPerObject);					//Set the constant buffer
	//m_pLocalContext->PSSetConstantBuffers(0, 1, &m_pcbPerObject);

	D3D11_MAPPED_SUBRESOURCE ms2;
	//Update and set FRAME constant buffer 
	m_pLocalContext->Map(m_pcbPerFrame, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms2);
	memcpy(ms2.pData, &m_frame_cb_values, sizeof(PerFrame_CONSTANT_BUFFER));
	m_pLocalContext->Unmap(m_pcbPerFrame, NULL);
	m_pLocalContext->VSSetConstantBuffers(1, 1, &m_pcbPerFrame);
	//m_pLocalContext->PSSetConstantBuffers(1, 1, &m_pcbPerObject);
	//Map / Unmap is required for D3D11_USAGE_DYNAMIC


	m_pLocalContext->PSSetSamplers(0, 1, &m_pSampler);
	m_pLocalContext->PSSetShaderResources(0, 1, &m_pLocalTexture);

	m_pLocalContext->VSSetShader(m_currentVShader, 0, 0);
	m_pLocalContext->PSSetShader(m_currentPShader, 0, 0);

	m_pLocalContext->RSSetState(m_currentState);


	UINT stride = sizeof(MODEL_POS_TEX_NORM_VERTEX);
	UINT offset = 0;
	m_pLocalContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pLocalContext->Draw(numverts, 0);
}


HRESULT Mesh::AddTexture(char* filename)
{
	HRESULT hr{ S_OK };

	//SafeRelease(m_pLocalTexture);

	//if (m_pLocalTexture != nullptr)
	//{
	//	m_pLocalTexture->Release();
	//	m_pLocalTexture = nullptr;
	//}
	

	std::string test = "hello";
	
	//hr = D3DX11CreateShaderResourceViewFromFile(m_pLocalDevice, test.c_str(), NULL, NULL, &m_pTexture, NULL);
	hr = D3DX11CreateShaderResourceViewFromFile(m_pLocalDevice, filename, NULL, NULL, &m_pLocalTexture, NULL);
	
	if (hr == D3D11_ERROR_FILE_NOT_FOUND)
	{
		hr = D3DX11CreateShaderResourceViewFromFile(m_pLocalDevice, "res/textures/MISSINGTEXTURE.png", NULL, NULL, &m_pLocalTexture, NULL);
	}
	
	if (FAILED(hr))
	{
		return hr;
	}

	m_pLocalTexture = EngineStatics::GetRenderer()->GetTextureManager()->GetTexture(filename);

	return S_OK;
}

void Mesh::SetupMesh()
{

	/*
		Constant Buffer
	*/

	D3D11_BUFFER_DESC constantbufferDesc;
	ZeroMemory(&constantbufferDesc, sizeof(constantbufferDesc));
	constantbufferDesc.ByteWidth = sizeof(PerObject_CONSTANT_BUFFER);
	constantbufferDesc.Usage = D3D11_USAGE_DYNAMIC;						//DYNAMIC - Must use Map/Unmap to update constant buffer 
	constantbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//Use as constant buffer
	constantbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantbufferDesc.MiscFlags = 0;
	constantbufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &m_object_cb_values;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HR(m_pLocalDevice->CreateBuffer(&constantbufferDesc, &InitData, &m_pcbPerObject)); //PerObject constant buffer

	constantbufferDesc.ByteWidth = sizeof(PerFrame_CONSTANT_BUFFER);
	InitData.pSysMem = &m_frame_cb_values;
	
	HR(m_pLocalDevice->CreateBuffer(&constantbufferDesc, &InitData, &m_pcbPerFrame)); //PerFrame constant buffer


	/*
		Sampler
	*/

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //Texture filtering is linear
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = 0;

	HR(m_pLocalDevice->CreateSamplerState(&sampler_desc, &m_pSampler));

	//Loads default rendering state by default
	m_currentState = EngineStatics::GetRenderer()->GetRasterizerState(RDefault);

	//Loads default shaders and input layout

	m_currentVShader = EngineStatics::GetRenderer()->GetVertexShader(SDefault);
	m_currentPShader = EngineStatics::GetRenderer()->GetPixelShader(SDefault);

	m_currentInputLayout = EngineStatics::GetRenderer()->GetInputLayout(IDefault);

}

void Mesh::SetRasterizerState(rasterizerStates state = RDefault)
{
	m_currentState = EngineStatics::GetRenderer()->GetRasterizerState(state);
}

void Mesh::SetShaders(Shaders shader)
{
	m_currentVShader = EngineStatics::GetRenderer()->GetVertexShader(shader);
	m_currentPShader = EngineStatics::GetRenderer()->GetPixelShader(shader);
}

void Mesh::SetInputLayout(InputLayouts input)
{
	m_currentInputLayout = EngineStatics::GetRenderer()->GetInputLayout(input);
}


void Mesh::LoadMesh(char* filename)
{
	MeshLoader* ml = new MeshLoader();
	ml->LoadObj(filename, m_pVertexBuffer, numverts);
	delete ml;
	ml = nullptr;
}

void Mesh::SetXPos(float num) { m_Position.x = num; }

void Mesh::SetYPos(float num) { m_Position.y = num; }

void Mesh::SetZPos(float num) { m_Position.z = num; }

float Mesh::GetXPos() { return m_Position.x; }

float Mesh::GetYPos() { return m_Position.y; }

float Mesh::GetZPos() { return m_Position.z; }

void Mesh::SetXRot(float num) { m_Rotation.x = num; }

void Mesh::SetYRot(float num) { m_Rotation.y = num; }

void Mesh::SetZRot(float num) { m_Rotation.z = num; }

float Mesh::GetXRot() { return m_Rotation.x; }

float Mesh::GetYRot() { return m_Rotation.y; }

float Mesh::GetZRot() { return m_Rotation.z; }

void Mesh::SetXScale(float num) { m_Scale.x = num; }

void Mesh::SetYScale(float num) { m_Scale.y = num; }

void Mesh::SetZScale(float num) { m_Scale.z = num; }

float Mesh::GetXScale() { return m_Scale.x; }

float Mesh::GetYScale() { return m_Scale.y; }

float Mesh::GetZScale() { return m_Scale.z; }

void Mesh::IncXPos(float num) { m_Position.x += num; }

void Mesh::IncYPos(float num) { m_Position.y += num; }

void Mesh::IncZPos(float num) { m_Position.z += num; }

void Mesh::DecXPos(float num) { m_Position.x -= num; }

void Mesh::DecYPos(float num) { m_Position.y -= num; }

void Mesh::DecZPos(float num) { m_Position.z -= num; }

void Mesh::IncXRot(float num) { m_Rotation.x += num; }

void Mesh::IncYRot(float num) { m_Rotation.y += num; }

void Mesh::IncZRot(float num) { m_Rotation.z += num; }

void Mesh::DecXRot(float num) { m_Rotation.x -= num; }

void Mesh::DecYRot(float num) { m_Rotation.y -= num; }

void Mesh::DecZRot(float num) { m_Rotation.z -= num; }

void Mesh::IncXScale(float num) { m_Scale.x += num; }

void Mesh::IncYScale(float num) { m_Scale.y += num; }

void Mesh::IncZScale(float num) { m_Scale.z += num; }

void Mesh::DecXScale(float num) { m_Scale.x -= num; }

void Mesh::DecYScale(float num) { m_Scale.y -= num; }

void Mesh::DecZScale(float num) { m_Scale.z -= num; }

