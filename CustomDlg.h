#pragma once


// CCustomDlg Dialog

class CCustomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomDlg)

public:
	CCustomDlg(CWnd* pParent = NULL);
	virtual ~CCustomDlg();
    void setValues(int rows, int cols, int mines)
    {
        m_nRows = rows;
        m_nCols = cols;
        m_nMines = mines;
    }

	enum { IDD = IDD_CUSTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    int m_nRows;
    int m_nCols;
    int m_nMines;
    afx_msg void OnBnClickedOk();
};
