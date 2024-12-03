
// EdgeSearchDlg.h: 헤더 파일
//

#pragma once


// CEdgeSearchDlg 대화 상자
class CEdgeSearchDlg : public CDialogEx
{
// 생성입니다.
public:
	CEdgeSearchDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual ~CEdgeSearchDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDGESEARCH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnImgLoad();

private:

public:
	CStatic m_pictureControl;  // Picture Control 멤버 변수
	cv::Mat m_image;           // OpenCV 이미지 데이터를 저장할 변수
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	void DisplayImageInPictureControl(cv::Mat& img);
};
