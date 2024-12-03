
// EdgeSearchDlg.h: 헤더 파일
//

#pragma once


// CEdgeSearchDlg 대화 상자
class CEdgeSearchDlg : public CDialogEx
{
    // 생성
public:
    CEdgeSearchDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
    virtual ~CEdgeSearchDlg();

    // 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_EDGESEARCH_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

    // 구현입니다.
protected:
    HICON m_hIcon;            // 아이콘
    HBITMAP m_hBitmap;        // Picture Control에 표시할 Bitmap 핸들
    cv::Mat m_image;          // OpenCV 이미지 데이터
    CStatic m_pictureControl; // Picture Control 멤버 변수

    DECLARE_MESSAGE_MAP()

    // 사용자 정의 함수
public:
    void DisplayImageInPictureControl(cv::Mat& img); // Picture Control에 이미지 표시 함수

    // 메시지 처리기
protected:
    virtual BOOL OnInitDialog(); // 초기화 함수
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam); // 시스템 명령 처리
    afx_msg void OnPaint(); // 창 그리기
    afx_msg HCURSOR OnQueryDragIcon(); // 아이콘 처리
    afx_msg void OnBnClickedBtnExit(); // 종료 버튼 클릭 이벤트
    afx_msg void OnBnClickedBtnImgLoad(); // 이미지 로드 버튼 클릭 이벤트
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI); // 창 크기 제한 처리
};