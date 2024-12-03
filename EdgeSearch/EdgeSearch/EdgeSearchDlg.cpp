#include "pch.h"
#include "framework.h"
#include "EdgeSearch.h"
#include "EdgeSearchDlg.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}
void CAboutDlg::DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CEdgeSearchDlg::CEdgeSearchDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_EDGESEARCH_DIALOG, pParent), m_hBitmap(nullptr)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CEdgeSearchDlg::~CEdgeSearchDlg()
{
    if (m_hBitmap)
    {
        DeleteObject(m_hBitmap);
    }
}

void CEdgeSearchDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PICTURE, m_pictureControl);
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

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    SetWindowPos(NULL, 0, 0, 1920, 1080, SWP_NOZORDER);
    return TRUE;
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

void CEdgeSearchDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.m_ps.hdc), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CEdgeSearchDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CEdgeSearchDlg::OnBnClickedBtnExit()
{
    if (m_hBitmap)
    {
        DeleteObject(m_hBitmap);
        m_hBitmap = nullptr;
    }

    m_image.release();
    DestroyWindow();
}

void CEdgeSearchDlg::OnBnClickedBtnImgLoad()
{
    CFileDialog dlg(TRUE, _T("*.jpg;*.png;*.bmp"), NULL, OFN_FILEMUSTEXIST, _T("Image Files (*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp||"));
    if (dlg.DoModal() == IDOK)
    {
        CString filePath = dlg.GetPathName();
        CT2CA pszConvertedAnsiString(filePath);
        std::string imagePath(pszConvertedAnsiString);

        m_image = imread(imagePath);
        if (m_image.empty())
        {
            AfxMessageBox(_T("이미지를 로드할 수 없습니다."));
            return;
        }

        DisplayImageInPictureControl(m_image);
    }
}

void CEdgeSearchDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    lpMMI->ptMinTrackSize.x = 800;
    lpMMI->ptMinTrackSize.y = 600;
    lpMMI->ptMaxTrackSize.x = 1920;
    lpMMI->ptMaxTrackSize.y = 1080;

    CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CEdgeSearchDlg::DisplayImageInPictureControl(cv::Mat& img)
{
    Mat rgbImg;
    cvtColor(img, rgbImg, COLOR_BGR2RGB);

    CRect rect;
    m_pictureControl.GetClientRect(&rect);

    Mat resizedImg;
    resize(rgbImg, resizedImg, Size(rect.Width(), rect.Height()));

    BITMAPINFO bitmapInfo = { 0 };
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = resizedImg.cols;
    bitmapInfo.bmiHeader.biHeight = -resizedImg.rows;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 24;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    HDC hdc = ::GetDC(m_pictureControl.m_hWnd);
    void* pBits = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdc, &bitmapInfo, DIB_RGB_COLORS, &pBits, nullptr, 0);
    ::ReleaseDC(m_pictureControl.m_hWnd, hdc);

    if (pBits)
    {
        memcpy(pBits, resizedImg.data, resizedImg.total() * resizedImg.elemSize());
    }

    if (m_hBitmap)
    {
        DeleteObject(m_hBitmap);
    }

    m_hBitmap = hBitmap;
    m_pictureControl.SetBitmap(hBitmap);
}
