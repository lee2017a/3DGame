#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
//D3D���ȫ�ֱ���
ID3D11Device        *dev = NULL;
ID3D11DeviceContext    *devContext = NULL;
IDXGISwapChain        *SwapChain = NULL;

ID3D11DepthStencilView    *depthStencilView = NULL;
ID3D11RenderTargetView    *backBuffer = NULL;
float color[4] = { 0.0, 1.0, 0.0, 1.6 };
void Release()
{
	SwapChain->SetFullscreenState(FALSE, NULL);

	dev->Release();
	devContext->Release();
	backBuffer->Release();
	SwapChain->Release();
}
void Render()
{
	devContext->ClearRenderTargetView(backBuffer, reinterpret_cast<float*>(&color));
	devContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0);
	SwapChain->Present(0, 0);
}
BOOL CreateD3DObject(HWND hwnd)
{
	//��ʼ��������
	/*
	typedef struct DXGI_SWAP_CHAIN_DESC {
	DXGI_MODE_DESC   BufferDesc;        //��ָ������������
	DXGI_SAMPLE_DESC SampleDesc;        //���ز���������ݣ�
	DXGI_USAGE       BufferUsage;        //���ڽ�������ΪDXGI_USAGE_RENDER_TARGET_OUTPUT
	UINT             BufferCount;        //����ֻ����һ���󻺳�����˫���壩�����Ϊ1
	HWND             OutputWindow;    //ָ�����ھ����ʹ��Windows��������ʼ���괴���������ھ��
	BOOL             Windowed;        //FASE ȫ����ʾ��TRUE������ʾ
	DXGI_SWAP_EFFECT SwapEffect;        //ͨ��ΪDXGI_SWAP_EFFECT_DISCARD
	UINT             Flags;            //��ѡ��������Ϊ0
	} DXGI_SWAP_CHAIN_DESC;

	typedef struct DXGI_MODE_DESC {
	UINT                     Width;            //����������ֱ����йأ�
	UINT                     Height;        //�������ߣ���ֱ����йأ�
	DXGI_RATIONAL            RefreshRate;    //   ��������һ��Ϊ�̶��ģ��μ����롣
	DXGI_FORMAT              Format;
	DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
	DXGI_MODE_SCALING        Scaling;
	} DXGI_MODE_DESC, *LPDXGI_MODE_DESC;

	*/
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = 1200;
	scd.BufferDesc.Height = 600;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	/*HRESULT  D3D11CreateDeviceAndSwapChain(
	__in   IDXGIAdapter *pAdapter,            //ѡ��ͼ��������
	__in   D3D_DRIVER_TYPE DriverType,        //�������ͣ�һ��ΪD3D_DRIVER_TYPE_HARDWARE
	__in   HMODULE Software,                //NULL����
	__in   UINT Flags,                        // FlagsΪ��ѡ������һ��ΪNULL
	__in   const D3D_FEATURE_LEVEL *pFeatureLevels,  // pFeatureLevelsΪ�����ṩ������������ȼ���һ�����飬��һ������Ϊ������Ԫ�ظ���
	__in   UINT FeatureLevels,
	__in   UINT SDKVersion,                // SDKVersion�㶨ΪD3D11_SDK_VERSION��
	__in   const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,    //DXGI_SWAP_CHAIN_DESC��ʼ���Ľ�����
	__out  IDXGISwapChain       **ppSwapChain,    //����һ��IDXGISwapChain�����ָ���ַ�������������������Ⱦ����άָ�룩
	__out  ID3D11Device **ppDevice,        //  ppDeviceΪ�豸ָ��ĵ�ַ��ע���豸��ָ�����ͣ����ﴫ�ݵ���ָ��ĵ�ַ����άָ�룬d3d���������еĽӿڶ�����Ϊָ�����ͣ���
	__out  D3D_FEATURE_LEVEL *pFeatureLevel, //    pFeatureLevelΪ������ѡ�е������ȼ������Ƕ�����Ӧ�ı������������ĵ�ַ������
	__out  ID3D11DeviceContext **ppImmediateContext  //    pFeatureLevelΪ������ѡ�е������ȼ������Ƕ�����Ӧ�ı������������ĵ�ַ����
	;
	����ֵ
	HRESULT
	�����������Direct3D 11����ֵ��
	*/
	//�����豸�뻺����
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &SwapChain, &dev, NULL, &devContext);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"����d3d11�豸ʧ�ܣ�", L"����", MB_OK);
		return FALSE;
	}

	ID3D11Texture2D *t2D;
	//��ȡ�󻺳�����ַ
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&t2D);
	/*
	HRESULT CreateRenderTargetView
	(
	ID3D11Resource *pResource,             //��ͼ��Ӧ��Դ
	const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,        //��ͼ����
	ID3D11RenderTargetView **ppRTView          //Ҫ��������ͼ��ָ��ĵ�ַ��
	);  */
	//������ͼ  
	dev->CreateRenderTargetView(t2D, NULL, &backBuffer);
	//�ͷź󻺳�������  
	t2D->Release();
	//������Ȼ�������ͼ
	//Ҫ�ȴ�����Ӧ������
	//����������Texture2D��Ҫ�ȸ�������
	UINT msaa4xQuality(0);
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = 1200;
	desc.Height = 600;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 4;
	desc.SampleDesc.Quality = msaa4xQuality - 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	ID3D11Texture2D *depthStencilBuffer(NULL);
	hr = dev->CreateTexture2D(&desc, NULL, &depthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create d3d11 depthStencil Fail��", L"Fail", MB_OK);
		return FALSE;
	}
	hr = dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Create DepthStencilView Fai��", L"Fail", MB_OK);
		return FALSE;
	}
	//������ͼҪ�󶨵�������Ӧ�׶�
	devContext->OMSetRenderTargets(1, &backBuffer, depthStencilView);
	depthStencilBuffer->Release();

	//�������portview
	D3D11_VIEWPORT viewPort = { 0 };
	viewPort.Width = static_cast<FLOAT>(1200);
	viewPort.Height = static_cast<FLOAT>(600);
	viewPort.MinDepth = 0.f;
	viewPort.MaxDepth = 1.f;
	viewPort.TopLeftX = 0.f;
	viewPort.TopLeftY = 0.f;
	devContext->RSSetViewports(1, &viewPort);

	return TRUE;
}
///////////////////////////////////////////////////