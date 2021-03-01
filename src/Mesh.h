#pragma once
#include "EngineStatics.h"
#include "MeshLoader.h"

#include <d3d11.h>
#include <d3dx11.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <DirectXMath.h>
using namespace DirectX;

#include "Renderer.h"

struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	//Default
	XMFLOAT4 Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	XMFLOAT4 Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 8.0f); // w = SpecPower
	XMFLOAT4 Reflect;
};

//CB must be multiple of 16
struct PerObject_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProjection; //64 bytes
	XMMATRIX WorldMatrix;
	XMMATRIX WorldInvTransposeMatrix;
	Material material;
};

struct PerFrame_CONSTANT_BUFFER
{
	DirectionalLight DirLight;
	XMFLOAT3 position;
	float packing;
};



class Mesh 
{
public:
    Mesh();
    ~Mesh();

	void Update();

    void Draw();

    void LoadMesh(char* filename);
    HRESULT AddTexture(char* filename);

    void SetupMesh();

	void SetRasterizerState(rasterizerStates state);
	void SetShaders(Shaders shader);
	void SetInputLayout(InputLayouts input);
	

	void		SetXPos(float num), SetYPos(float num), SetZPos(float num);
	float		GetXPos(), GetYPos(), GetZPos();

	void		SetXRot(float num), SetYRot(float num), SetZRot(float num);
	float		GetXRot(), GetYRot(), GetZRot();

	void		SetXScale(float num), SetYScale(float num), SetZScale(float num);
	float		GetXScale(), GetYScale(), GetZScale();

	void		IncXPos(float num), IncYPos(float num), IncZPos(float num);
	void		DecXPos(float num), DecYPos(float num), DecZPos(float num);

	void		IncXRot(float num), IncYRot(float num), IncZRot(float num);
	void		DecXRot(float num), DecYRot(float num), DecZRot(float num);

	void		IncXScale(float num), IncYScale(float num), IncZScale(float num);
	void		DecXScale(float num), DecYScale(float num), DecZScale(float num);

	

private:

	ID3D11Device* m_pLocalDevice;
	ID3D11DeviceContext* m_pLocalContext;

	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMFLOAT3 m_Scale;

	ID3D11Buffer* m_pcbPerObject;
	ID3D11Buffer* m_pcbPerFrame;

	ID3D11ShaderResourceView* m_pLocalTexture;
	ID3D11SamplerState* m_pSampler;
	PerObject_CONSTANT_BUFFER	m_object_cb_values;
	PerFrame_CONSTANT_BUFFER	m_frame_cb_values;

	//From renderer class
	ID3D11RasterizerState* m_currentState;

	ID3D11VertexShader* m_currentVShader;
	ID3D11PixelShader* m_currentPShader;

	ID3D11InputLayout* m_currentInputLayout;

	//Mesh
	ID3D11Buffer* m_pVertexBuffer;
	unsigned int numverts;

	//Material
	Material meshMaterial;

	
};

