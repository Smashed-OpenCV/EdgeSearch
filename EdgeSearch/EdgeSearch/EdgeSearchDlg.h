
// EdgeSearchDlg.h: 헤더 파일
//

#pragma once


// CEdgeSearchDlg 대화 상자
class CEdgeSearchDlg : public CDialogEx
{
// 생성입니다.
public:
	CEdgeSearchDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDGESEARCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	cv::Mat m_image;           // OpenCV 이미지 저장용
	CStatic* m_pPictureCtrl;   // Picture Control 멤버 변수
	HBITMAP m_hBitmap;         // Picture Control에 사용할 비트맵

	void DisplayImageOnControl(const cv::Mat& img); // 이미지를 Picture Control에 표시하는 함수

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnImgLoad();

};
