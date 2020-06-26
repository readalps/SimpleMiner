// simpleMinerDlg.h
//

#pragma once

#include <vector>

// CMyButton -- from Himanshu
// https://stackoverflow.com/questions/35454247/can-right-mouse-click-events-be-detected-on-a-mfc-button-control

class CMyButton : public CButton  //CMyButton  =>derive from the CButton.
{
    // Construction
public:
    CMyButton();

    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMyButton)
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CMyButton();

    // Generated message map functions
protected:
    //{{AFX_MSG(CMyButton)
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

enum ENUM_BTN_STATUS
{
    E_BtnSta_Covered,
    E_BtnSta_Open,
    E_BtnSta_Flagged
};

struct SCell
{
    CButton* pBtn;
    int nVal;
    ENUM_BTN_STATUS eStatus;

    SCell()
    {
        pBtn = NULL;
        reset();
    }
    void reset()
    {
        nVal = -1;
        eStatus = E_BtnSta_Covered;
    }
};

#define VecCell  std::vector<SCell>

#define IDC_BTN_HEAD    3000

// CsimpleMinerDlg Dialog
class CsimpleMinerDlg : public CDialogEx
{
public:
	CsimpleMinerDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_SIMPLEMINER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	HICON m_hIcon;

    UINT m_uiRowSum;
    UINT m_uiColSum;
    UINT m_uiMineSum;
    UINT m_uiFlagSum;
    UINT m_uiOpenSum;
    int m_nCustomMineSum;
    BOOL m_bShow;

    std::vector<VecCell> m_vecCells;

    void resetValues()
    {
        m_uiMineSum = 0;
        m_uiOpenSum = 0;
        m_uiFlagSum = 0;
    }
    void initButtons();
    void deleteButtons();
    UINT calcNeighborMineSum(UINT uiRow, UINT uiCol);
    void calcNeiborFlagsAndCovers(UINT uiRow, UINT uiCol, int& nNeiborFlags, int& nNeiborCovers);
    void resetButtons();
    void setLeftMineSum();
    void layoutMines();
    void layoutCustomMines();
    void fillOut();
    void uncover(UINT uiRow, UINT uiCol);
    void uncoverNeighbors(UINT uiRow, UINT uiCol);
    void smartUncoverNeighbors(UINT uiRow, UINT uiCol);
    void checkIfDone();
    void gameOver(SCell& oCell);

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnButtonClick(UINT id);
    afx_msg void OnRClicked(UINT id, NMHDR* pNotify, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCheckShow();
    afx_msg void OnBnClickedBtnCustom();
};
