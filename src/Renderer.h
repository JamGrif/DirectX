#pragma once
#include <d3d11.h>
#include <dxgi.h> 
#include <d3dx11.h>
#include <dxerr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALGINMENT
#include <DirectXMath.h>
using namespace DirectX;

#include "CoreObject.h"
#include "EngineStatics.h"

enum rasterizerStates
{
    RDefault = 0,
    RWireframe = 1,
};

enum Shaders
{
    SDefault = 0
};

enum InputLayouts
{
    IDefault = 0
};


class Renderer :
    public CoreObject
{
public:
    Renderer();
    ~Renderer();


    void Resize();

    void Update() override;

    ID3D11RasterizerState* GetRasterizerState(rasterizerStates state = RDefault);

    ID3D11VertexShader* GetVertexShader(Shaders shader = SDefault);
    ID3D11PixelShader* GetPixelShader(Shaders shader = SDefault);

    ID3D11InputLayout* GetInputLayout(InputLayouts input = IDefault);


private:

    void CreateViewport();
    void CreateRenderTargetView();
    void CreateDepthStencilView();

private:
                        
    ID3D11Device*           m_pD3DDevice; //Virtual representation of video adapter in program
    ID3D11DeviceContext*    m_pImmediateContext; //Responsible for rendering
    IDXGISwapChain*         m_pSwapChain; //Contains screen buffers where scene is rendered to
    ID3D11RenderTargetView* m_pRenderTargetView; //Render target (Area of memory where pixels can be drawn to) Viewport is the rectangle a scene is projected onto. Also determines depth of the view frustum
    ID3D11DepthStencilView* m_pDepthView;
    ID3D11Texture2D*        m_pDepthBuffer;

    //Direct3D renderer essentials
    D3D_DRIVER_TYPE	    m_driverType;
    D3D_FEATURE_LEVEL   m_featureLevel;

    const float m_clear_colour[4];

    UINT m4xMsaaQuality;

    //RasterizerStates
    std::vector<ID3D11RasterizerState*> rasterizerStateList;
    ID3D11RasterizerState* m_pDefaultRasterizer;
    ID3D11RasterizerState* m_pWireFrameRasterizer;

    //Shaders
    std::vector<ID3D11VertexShader*> VertexShaderList;
    std::vector<ID3D11PixelShader*> PixelShaderList;
    ID3D11VertexShader* m_pDefaultVShader;
    ID3D11PixelShader* m_pDefaultPShader;

    //InputLayouts
    std::vector<ID3D11InputLayout*> InputLayoutList;
    ID3D11InputLayout* m_pDefaultInputLayout;

};

