
// EdgeSearchDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "EdgeSearch.h"
#include "EdgeSearchDlg.h"
#include "afxdialogex.h"
#include "pch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEdgeSearchDlg 대화 상자



CEdgeSearchDlg::CEdgeSearchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDGESEARCH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEdgeSearchDlg::~CEdgeSearchDlg()
{
}

void CEdgeSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEdgeSearchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CEdgeSearchDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_IMG_LOAD, &CEdgeSearchDlg::OnBnClickedBtnImgLoad)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


BOOL CEdgeSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.


	// 대화 상자의 크기 설정 (좌측 상단 X, Y 좌표, 가로, 세로 크기)
	SetWindowPos(NULL, 0, 0, 1920, 1080, SWP_NOZORDER);


	// Picture Control 초기화
	m_pictureControl.SubclassDlgItem(IDC_PICTURE, this);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CEdgeSearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
void CEdgeSearchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기 위한 DC

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.m_ps.hdc), 0);
		dc.DrawIcon(0, 0, m_hIcon);
	}

	CDialogEx::OnPaint();
}


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CEdgeSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEdgeSearchDlg::OnBnClickedBtnExit()
{
	m_image.release(); // OpenCV 메모리 해제

	DestroyWindow(); // 프로그램 종료 시 메모리 해제
}


void CEdgeSearchDlg::OnBnClickedBtnImgLoad()
{
	CFileDialog dlg(TRUE, _T("*.jpg;*.png;*.bmp"), NULL, OFN_FILEMUSTEXIST, _T("Image Files (*.jpg;*.png;*.bmp)|*.jpg;*.png|All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();
		CT2CA pszConvertedAnsiString(filePath);
		std::string imagePath(pszConvertedAnsiString);

		// OpenCV로 이미지 로드
		m_image = imread(imagePath);
		if (m_image.empty())
		{
			AfxMessageBox(_T("이미지를 로드할 수 없습니다."));
			return;
		}

		// 이미지를 Picture Control에 표시
		DisplayImageInPictureControl(m_image);
		//CString filePath = dlg.GetPathName();
		//CT2CA pszConvertedAnsiString(filePath);
		//std::string imagePath(pszConvertedAnsiString);

		//m_cvImage.release(); // OpenCV 메모리 해제

		//m_cvImage = cv::imread(imagePath, cv::IMREAD_UNCHANGED);


		//// 1. 클라이언트 영역에 대한 DC 생성
		//CClientDC dc(GetDlgItem(IDC_CAMERA_IMAGE));

		//// 2. 카메라 이미지를 표시할 컨트롤의 크기 가져오기
		//CRect rect;
		//GetDlgItem(IDC_CAMERA_IMAGE)->GetClientRect(&rect);

		//// 3. 이미지가 그레이스케일(1채널)인지 확인하고, 그레이스케일인 경우 3채널(BGR)로 변환
		//if (m_cvImage.channels() == 1) 
		//{
		//	cv::cvtColor(m_cvImage, m_cvImage, cv::COLOR_GRAY2BGR);
		//}

		//// 5. BITMAPINFO 설정
		//BITMAPINFO bitmapInfo;
		//memset(&bitmapInfo, 0, sizeof(BITMAPINFO));
		//bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		//bitmapInfo.bmiHeader.biWidth = m_cvImage.cols;// scaledImg.cols;  // 축소된 이미지의 너비 설정
		//bitmapInfo.bmiHeader.biHeight = -m_cvImage.rows;  // 높이를 음수로 설정해 상하 반전 방지
		//bitmapInfo.bmiHeader.biPlanes = 1;
		//bitmapInfo.bmiHeader.biBitCount = 24;  // BGR 24비트
		//bitmapInfo.bmiHeader.biCompression = BI_RGB;  // 압축 사용 안 함

		//// 6. 이미지를 화면에 출력
		//SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
		//StretchDIBits(dc.GetSafeHdc(),
		//	0, 0, rect.Width(), rect.Height(),        // 출력 영역의 크기
		//	0, 0, m_cvImage.cols, m_cvImage.rows,     // 축소된 이미지의 크기
		//	m_cvImage.data, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		//
		//if (m_cvImage.empty()) 
		//{
		//	AfxMessageBox(_T("Failed Load Image"));
		//	return;
		//}
	}
}


void CEdgeSearchDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	// 최소 크기 설정
	lpMMI->ptMinTrackSize.x = 800; // 최소 너비
	lpMMI->ptMinTrackSize.y = 600; // 최소 높이

	// 최대 크기 설정 (필요 시)
	lpMMI->ptMaxTrackSize.x = 1920; // 최대 너비
	lpMMI->ptMaxTrackSize.y = 1080; // 최대 높이

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CEdgeSearchDlg::DisplayImageInPictureControl(cv::Mat& img)
{
	// OpenCV Mat 이미지를 BMP로 변환
	Mat rgbImg;
	cvtColor(img, rgbImg, COLOR_BGR2RGB); // OpenCV는 BGR, MFC는 RGB 사용

	// Bitmap 정보 생성
	BITMAPINFO bitmapInfo = { 0 };
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = rgbImg.cols;
	bitmapInfo.bmiHeader.biHeight = -rgbImg.rows; // 상하 반전을 방지하기 위해 음수 사용
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = 24;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;

	// Bitmap 생성
	HDC hdc = ::GetDC(m_pictureControl.m_hWnd);
	void* pBits = nullptr;
	HBITMAP hBitmap = CreateDIBSection(hdc, &bitmapInfo, DIB_RGB_COLORS, &pBits, nullptr, 0);
	::ReleaseDC(m_pictureControl.m_hWnd, hdc);

	if (pBits)
	{
		memcpy(pBits, rgbImg.data, rgbImg.total() * rgbImg.elemSize());
	}

	// Picture Control에 Bitmap 설정
	m_pictureControl.SetBitmap(hBitmap);
}
