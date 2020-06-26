// CustomDlg.cpp
//

#include "stdafx.h"
#include "simpleMiner.h"
#include "CustomDlg.h"
#include "afxdialogex.h"


// CCustomDlg Dialog

IMPLEMENT_DYNAMIC(CCustomDlg, CDialogEx)

CCustomDlg::CCustomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCustomDlg::IDD, pParent)
    , m_nRows(18)
    , m_nCols(18)
    , m_nMines(72)
{

}

CCustomDlg::~CCustomDlg()
{
}

void CCustomDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ROWS, m_nRows);
    DDV_MinMaxInt(pDX, m_nRows, 1, 18);
    DDX_Text(pDX, IDC_EDIT_COLS, m_nCols);
    DDV_MinMaxInt(pDX, m_nCols, 1, 32);
    DDX_Text(pDX, IDC_EDIT_MINES, m_nMines);
	DDV_MinMaxInt(pDX, m_nMines, 0, 32 * 18);
}


BEGIN_MESSAGE_MAP(CCustomDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CCustomDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CCustomDlg::OnBnClickedOk()
{
    UpdateData(TRUE);
    if (m_nMines > m_nCols * m_nRows)
    {
        GetDlgItem(IDC_EDIT_MINES)->SetCapture();
        CString str;
        str.Format(_T("Mines cannot be greater than %d."), m_nRows * m_nCols);
        AfxMessageBox(str);
        return;
    }
    CDialogEx::OnOK();
}
