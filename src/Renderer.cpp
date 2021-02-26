#include "Renderer.h"

Renderer::Renderer()
	:m_pD3DDevice(nullptr), m_pImmediateContext(nullptr), m_pSwapChain(nullptr), m_pRenderTargetView(nullptr), m_pDepthView(nullptr),
	m_driverType(D3D_DRIVER_TYPE_HARDWARE), m_featureLevel(D3D_FEATURE_LEVEL_11_0), m_clear_colour{ 0.2f, 0.2f, 0.2f, 1.0f }
{
	EngineStatics::SetRenderer(this);


	//Enables devices debugging capabilties
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED;
	//_DEBUG Enables debug layer in debug builds. D3D will send debug messages to VC++ output window
	//_SINGLETHREADED Improves performance if D3D is not called from multiple threads
#else
	createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#endif

	//HRESULT hr;

	/*
		1 - Create device and context
	*/

	HR(D3D11CreateDevice(
			NULL,						//Specifies display adapter the device will represent - NULL uses default
			D3D_DRIVER_TYPE_HARDWARE,	//_HARDWARE uses 3D hardware acceleration
			NULL,						//Supplying software driver - NULL as using hardware for renderering
			createDeviceFlags,			//Optional device creation flags
			NULL,						//Array of feature level elements - NULL uses the greatest feature level supported
			0,							//Size of feature level array - 0 as NULL is used in previous parameter
			D3D11_SDK_VERSION,			//SDK version to use
			&m_pD3DDevice,				//Returns created device
			&m_featureLevel,			//Returns the greatest feature level supported
			&m_pImmediateContext		//Returns created device context
	));

	if (m_featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
	}

	/*
		2 - Check for 4X MSAA support
	*/

	HR(m_pD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	assert(m4xMsaaQuality > 0); 

	/*
		3 - Describe the swap chain
	*/

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width						= EngineStatics::GetScreenWidth();		//Desired back buffer width
	sd.BufferDesc.Height					= EngineStatics::GetScreenHeight();		//Desired back buffer height
	sd.BufferDesc.RefreshRate.Numerator		= 60;									//Display mode refresh rate
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;			//Back buffer pixel format
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//Display scanline mode
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;		//Display scaling mode
	sd.SampleDesc.Count						= 4;									//Number of multisamples
	sd.SampleDesc.Quality					= m4xMsaaQuality - 1;					//Quality level
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;		//_RENDER_TARGET_OUTPUT - renderering to backbuffer
	sd.BufferCount							= 1;									//Number of buffers to use in swap chain
	sd.OutputWindow							= *EngineStatics::GetHWND();			//Handle to window being rendered to
	sd.Windowed								= true;									//Window in windowed mode, not fullscreen
	sd.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;				//_DISCARD - lets display driver select most effcient presentation method
	sd.Flags								= 0;									//Optional flags

	/*
		4 - Create the swap chain instance
	*/

	IDXGIDevice* dxgiDevice = 0;
	HR(m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	//Get IDXGIFactory interface
	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	//Finally create swap chain
	HR(dxgiFactory->CreateSwapChain(m_pD3DDevice, &sd, &m_pSwapChain));

	//Release COM interfaces
	SafeRelease(dxgiDevice);
	SafeRelease(dxgiAdapter);
	SafeRelease(dxgiFactory);

	/*
		5 - Create the render target view
	*/

	CreateRenderTargetView();

	/*
		6 - Create depth/stencil buffer and view
	*/

	CreateDepthStencilView();

	/*
		7 - Bind render target view and depth/stencil view to output merger stage of pipeline
	*/

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthView); //Bind the backbuffer view and depthbuffer view to output merger stage of pipeline

	/*
		8 - Set the viewport
	*/

	CreateViewport();

	/*
		Initilize values
	*/

	//Create the projection matrix
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0), static_cast<float>(EngineStatics::GetScreenWidth()) / EngineStatics::GetScreenHeight(), 1.0, 100.0);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //Use triangles

	//Set engine statics values
	EngineStatics::SetD3DDevice(m_pD3DDevice);
	EngineStatics::SetD3DContext(m_pImmediateContext);
	EngineStatics::SetProjectionMatrix(projection);

	/*
		Create rasterizer states
	*/

	D3D11_RASTERIZER_DESC rDesc;
	ZeroMemory(&rDesc, sizeof(rDesc));
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.CullMode = D3D11_CULL_BACK;
	rDesc.FrontCounterClockwise = false;

	HR(m_pD3DDevice->CreateRasterizerState(&rDesc, &m_pDefaultRasterizer));
	rasterizerStateList.push_back(m_pDefaultRasterizer);

	rDesc.FillMode = D3D11_FILL_WIREFRAME;
	rDesc.CullMode = D3D11_CULL_NONE;

	HR(m_pD3DDevice->CreateRasterizerState(&rDesc, &m_pWireFrameRasterizer));
	rasterizerStateList.push_back(m_pWireFrameRasterizer);

	/*
		Create shaders
	*/

	//Load and compile the pixel and vertex shaders. Use vs_5_0 to target DX11 hardware only
	ID3DBlob* VS;
	ID3DBlob* PS;
	ID3DBlob* error;

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined (_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	HR(D3DX11CompileFromFile(
		"res/shaders/DefaultShader.hlsl", //Filename where the shaders are
		0,
		0,
		"VShader",		//Name of the shader in that file we want to compile
		"vs_4_0",		//Target pixel shader version
		shaderFlags,
		0,
		0,
		&VS,			//Returns an object that stores a buffer of data (ID3DBlob)
		&error,			//Returns an error message if the shader fails to compile
		0
	));

	//Check for shader compile error
	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		SafeRelease(error);
	}

	HR(D3DX11CompileFromFile("res/shaders/DefaultShader.hlsl", 0, 0, "PShader", "ps_4_0", shaderFlags, 0, 0, &PS, &error, 0));

	//Check for shader compile error
	if (error != 0)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		SafeRelease(error);
	}

	//Create shader objects
	m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pDefaultVShader);
	VertexShaderList.push_back(m_pDefaultVShader);

	m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pDefaultPShader);
	PixelShaderList.push_back(m_pDefaultPShader);

	

	/*
		Create input layouts
	*/

	//Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] = //Tells D3D the format of vertices so it can correctly interpret them
	{
		//Input layout is a list of the elements of a vertex descibed by a D3D11_INPUT_ELEMENT_DESC
		{
			"POSITION",						//Tells GPU what value is used for
			0,								//Semantic index
			DXGI_FORMAT_R32G32B32_FLOAT,	//Format of the data (Needs to match what was used to create the vertices)
			0,								//Input slot
			0,								//Offset (How many bytes into the struct the element is)
			D3D11_INPUT_PER_VERTEX_DATA,	//What element is used as
			0
		},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pDefaultInputLayout); //Uses the struct to return an input layout
	InputLayoutList.push_back(m_pDefaultInputLayout);
	//Vertex shader takes a list of vertex elements as input parameters. 

	SafeRelease(VS);
	SafeRelease(PS);
	SafeRelease(error);

	/*
		Create texture manager
	*/

	textureManager = new TextureManager();


}

Renderer::~Renderer()
{
	SafeRelease(m_pRenderTargetView);
	SafeRelease(m_pDepthView);
	SafeRelease(m_pSwapChain);
	SafeRelease(m_pImmediateContext);
	SafeRelease(m_pD3DDevice);
}

void Renderer::Resize()
{

	SafeRelease(m_pRenderTargetView); //Release old RTV

	m_pImmediateContext->OMSetRenderTargets(0, 0, 0); //Unbind anything to pipeline

	//Resize swap chain
	HR(m_pSwapChain->ResizeBuffers(1, EngineStatics::GetScreenWidth(), EngineStatics::GetScreenHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	//Create render target view
	CreateRenderTargetView();

	//Create the depth/stencil buffer and view
	CreateDepthStencilView();

	//Bind render target view and depth stencil view to pipeline
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthView);

	//Create viewport
	CreateViewport();

	XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0), static_cast<float>(EngineStatics::GetScreenWidth()) / EngineStatics::GetScreenHeight(), 1.0, 100.0);
	EngineStatics::SetProjectionMatrix(projection);

}

void Renderer::Update()
{

	HR(m_pSwapChain->Present(0, 0)); //Present is before clearing buffers as draw calls are done in Mesh class

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, m_clear_colour); //Clear back buffer
	m_pImmediateContext->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); //Clear Z-buffer
}

ID3D11RasterizerState* Renderer::GetRasterizerState(rasterizerStates state)
{

	return rasterizerStateList.at(state);

}

ID3D11VertexShader* Renderer::GetVertexShader(Shaders shader)
{
	return VertexShaderList.at(shader);
}

ID3D11PixelShader* Renderer::GetPixelShader(Shaders shader)
{
	return PixelShaderList.at(shader);
}

ID3D11InputLayout* Renderer::GetInputLayout(InputLayouts input)
{
	return InputLayoutList.at(input);
}

TextureManager* Renderer::GetTextureManager()
{
	return textureManager;
}



void Renderer::CreateViewport()
{

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, (sizeof(vp)));
	vp.TopLeftX = 0.0f;															//Viewport start width - relative to client area rectangle
	vp.TopLeftY = 0.0f;															//Viewport start height
	vp.Width	= static_cast<float>(EngineStatics::GetScreenWidth());			//Width of viewport
	vp.Height	= static_cast<float>(EngineStatics::GetScreenHeight());			//Height of viewport
	vp.MinDepth = 0.0f;															//Minimum depth buffer value
	vp.MaxDepth = 1.0f;															//Maximum depth buffer value

	m_pImmediateContext->RSSetViewports(1, &vp);

}

void Renderer::CreateRenderTargetView()
{

	SafeRelease(m_pRenderTargetView); //Ensure no currently made rtv

	ID3D11Texture2D* backbuffer;
	ZeroMemory(&backbuffer, sizeof(backbuffer));
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer)));	//Get pointer to swapchains backbuffer
	HR(m_pD3DDevice->CreateRenderTargetView(backbuffer, 0, &m_pRenderTargetView));
	SafeRelease(backbuffer);

}

void Renderer::CreateDepthStencilView()
{
	SafeRelease(m_pDepthView);
	SafeRelease(m_pDepthBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, (sizeof(depthStencilDesc)));
	depthStencilDesc.Width = EngineStatics::GetScreenWidth();		//Width of texture (in texels)
	depthStencilDesc.Height = EngineStatics::GetScreenHeight();		//Height of texture (in texels)
	depthStencilDesc.MipLevels = 1;									//Number of mipmap levels
	depthStencilDesc.ArraySize = 1;									//Number of textures in array
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		//Format of the texels
	depthStencilDesc.SampleDesc.Count = 4;							//Number of multisamples(must match render target MSAA settings)
	depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;					//How texture will be used - _DEFAULT means only GPU can read/write not CPU
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;			//Where resource will be bound to the pipeline
	depthStencilDesc.CPUAccessFlags = 0;							//How CPU will write to resource 
	depthStencilDesc.MiscFlags = 0;									//Optional flags

	HR(m_pD3DDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthBuffer));
	HR(m_pD3DDevice->CreateDepthStencilView(m_pDepthBuffer, 0, &m_pDepthView));

}

TextureManager::TextureManager()
{
	//Ensure vector list of textures is empty
	textureAtlas.clear();

	//Load MISSINGTEXTURE texture by default
	GetTexture((char*)"res/textures/MISSINGTEXTURE.png");
}

TextureManager::~TextureManager()
{
}

ID3D11ShaderResourceView* TextureManager::GetTexture(char* filename)
{
	
	//Check if texture already exists by checking its filename
	for (TextureStruct* t : textureAtlas)
	{
		if (t->Address == filename)
		{
			//If does, return shaderresourceview
			return t->Texture;
		}
	}

	
	//else save new texture into texture atlas and then return it
	//t = new TextureStruct();
	return SaveTexture(filename);

}

ID3D11ShaderResourceView* TextureManager::SaveTexture(char* filename)
{
	HRESULT hr;
	t = new TextureStruct();

	hr = D3DX11CreateShaderResourceViewFromFile(EngineStatics::GetD3DDevice(), filename, NULL, NULL, &t->Texture, NULL);

	//If texture exists, add texture to vector and return it
	if (hr == S_OK)
	{
		t->Address = filename;
		textureAtlas.push_back(t);
		return t->Texture;
	}

	
}
