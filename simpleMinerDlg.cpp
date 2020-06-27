// simpleMinerDlg.cpp
//

#include "stdafx.h"
#include "simpleMiner.h"
#include "simpleMinerDlg.h"
#include "CustomDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton -- from Himanshu
// https://stackoverflow.com/questions/35454247/can-right-mouse-click-events-be-detected-on-a-mfc-button-control

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
    //{{AFX_MSG_MAP(CMyButton)
    ON_WM_RBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

void CMyButton::OnRButtonUp(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    NMHDR hdr;
    hdr.code = NM_RCLICK;
    hdr.hwndFrom = this->GetSafeHwnd();
    hdr.idFrom = GetDlgCtrlID();
    //TRACE("OnRButtonUp");
    this->GetParent()->SendMessage(WM_NOTIFY, (WPARAM)hdr.idFrom, (LPARAM)&hdr);
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsimpleMinerDlg Dialog

CsimpleMinerDlg::CsimpleMinerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsimpleMinerDlg::IDD, pParent)
    , m_bShow(FALSE), m_nCustomMineSum(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    resetValues();
    m_uiRowSum = 18;
    m_uiColSum = 18;
}

void CsimpleMinerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK_SHOW, m_bShow);
}

BEGIN_MESSAGE_MAP(CsimpleMinerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_HEAD, IDC_BTN_HEAD + 1000, OnButtonClick)
    ON_NOTIFY_RANGE(NM_RCLICK, IDC_BTN_HEAD, IDC_BTN_HEAD + 1000, OnRClicked)
    ON_BN_CLICKED(IDC_CHECK_SHOW, OnBnClickedCheckShow)
    ON_BN_CLICKED(IDC_BTN_CUSTOM, OnBnClickedBtnCustom)
END_MESSAGE_MAP()

// 

BOOL CsimpleMinerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	SetIcon(m_hIcon, TRUE);			// set big icon
	SetIcon(m_hIcon, FALSE);		// set small icon

	// TODO
    initButtons();

	return TRUE;
}

void CsimpleMinerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CsimpleMinerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

HCURSOR CsimpleMinerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CsimpleMinerDlg::calcNeighborMineSum(UINT uiRow, UINT uiCol)
{
    UINT uiRet = 0;

    /// concerning last row
    if (uiRow > 0)
    {
        if (uiCol > 0 && m_vecCells[uiRow - 1][uiCol - 1].nVal == 9) // left cell
        {
            ++uiRet;
        }
        if (m_vecCells[uiRow - 1][uiCol].nVal == 9) // middle cell
        {
            ++uiRet;
        }
        if (uiCol + 1 < m_uiColSum && m_vecCells[uiRow - 1][uiCol + 1].nVal == 9) // right cell
        {
            ++uiRet;
        }
    }

    /// concerning current row
    if (uiCol > 0 && m_vecCells[uiRow][uiCol - 1].nVal == 9) // left cell
    {
        ++uiRet;
    }
    if (uiCol + 1 < m_uiColSum && m_vecCells[uiRow][uiCol + 1].nVal == 9) // right cell
    {
        ++uiRet;
    }

    /// concerning next row
    if (uiRow + 1 < m_uiRowSum)
    {
        if (uiCol > 0 && m_vecCells[uiRow + 1][uiCol - 1].nVal == 9) // left one
        {
            ++uiRet;
        }
        if (m_vecCells[uiRow + 1][uiCol].nVal == 9) // middle one
        {
            ++uiRet;
        }
        if (uiCol + 1 < m_uiColSum && m_vecCells[uiRow + 1][uiCol + 1].nVal == 9) // right one
        {
            ++uiRet;
        }
    }

    return uiRet;
}

void cumuFlagsAndCovers(ENUM_BTN_STATUS eState, int& nNeiborFlags, int& nNeiborCovers)
{
    if (eState == E_BtnSta_Flagged)
    {
        nNeiborFlags++;
    }
    else if (eState == E_BtnSta_Covered)
    {
        nNeiborCovers++;
    }
}

void CsimpleMinerDlg::calcNeiborFlagsAndCovers(UINT uiRow, UINT uiCol, int& nNeiborFlags, int& nNeiborCovers)
{
    nNeiborFlags = 0;
    nNeiborCovers = 0;

    /// having last row
    if (uiRow > 0)
    {
        if (uiCol > 0) // left
        {
            cumuFlagsAndCovers(m_vecCells[uiRow - 1][uiCol - 1].eStatus, nNeiborFlags, nNeiborCovers);
        }
        cumuFlagsAndCovers(m_vecCells[uiRow - 1][uiCol].eStatus, nNeiborFlags, nNeiborCovers); // mid
        if (uiCol + 1 < m_uiColSum) // right
        {
            cumuFlagsAndCovers(m_vecCells[uiRow - 1][uiCol + 1].eStatus, nNeiborFlags, nNeiborCovers);
        }
    }

    /// current row
    if (uiCol > 0) // left
    {
        cumuFlagsAndCovers(m_vecCells[uiRow][uiCol - 1].eStatus, nNeiborFlags, nNeiborCovers);
    }
    if (uiCol + 1 < m_uiColSum) // right
    {
        cumuFlagsAndCovers(m_vecCells[uiRow][uiCol + 1].eStatus, nNeiborFlags, nNeiborCovers);
    }

    /// next row
    if (uiRow + 1 < m_uiRowSum)
    {
        if (uiCol > 0) // left
        {
            cumuFlagsAndCovers(m_vecCells[uiRow + 1][uiCol - 1].eStatus, nNeiborFlags, nNeiborCovers);
        }
        cumuFlagsAndCovers(m_vecCells[uiRow + 1][uiCol].eStatus, nNeiborFlags, nNeiborCovers); // mid
        if (uiCol + 1 < m_uiColSum) // right
        {
            cumuFlagsAndCovers(m_vecCells[uiRow + 1][uiCol + 1].eStatus, nNeiborFlags, nNeiborCovers);
        }
    }
}

void CsimpleMinerDlg::initButtons()
{
    resetValues();

    for (UINT idxR = 0; idxR < m_uiRowSum; ++idxR)
    {
        VecCell vec;
        for (UINT idxC = 0; idxC < m_uiColSum; ++idxC)
        {
            SCell oCell;
            oCell.pBtn = new CMyButton;
            vec.push_back(oCell);
        }
        m_vecCells.push_back(vec);
    }

    srand((UINT)time(0));
    /// create button images and arrange the mines
    for (UINT idxR = 0; idxR < m_uiRowSum; ++idxR)
    {
        for (UINT idxC = 0; idxC < m_uiColSum; ++idxC)
        {
            if (m_nCustomMineSum == -1 && (UINT)rand() % 5 == 0)
            {
                ++m_uiMineSum;
                m_vecCells[idxR][idxC].nVal = 9;
            }
            m_vecCells[idxR][idxC].pBtn->Create(_T(""), WS_VISIBLE | WS_CHILD | WS_BORDER,
                CRect(10 + idxC * 40, 50 + idxR * 40, 10 + idxC * 40 + 38, 50 + idxR * 40 + 38), this, IDC_BTN_HEAD + idxR * m_uiColSum + idxC);
        }
    }
    if (m_nCustomMineSum != -1)
    {
        layoutCustomMines();
    }

    GetDlgItem(IDC_STATIC_LEFT_SUM)->ShowWindow(m_bShow ? SW_SHOW : SW_HIDE);
    fillOut();

    int nEndX = 40 * m_uiColSum + 40;
    if (nEndX < 305)
    {
        nEndX = 305;
    }
    SetWindowPos(NULL, -1, -1, nEndX, 40 * m_uiRowSum + 100, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CsimpleMinerDlg::deleteButtons()
{
    for (UINT idxR = 0; idxR < m_uiRowSum; ++idxR)
    {
        for (UINT idxC = 0; idxC < m_uiColSum; ++idxC)
        {
            m_vecCells[idxR][idxC].pBtn->DestroyWindow();
            delete m_vecCells[idxR][idxC].pBtn;
        }
    }
    m_vecCells.clear();
}

void CsimpleMinerDlg::setLeftMineSum()
{
    CString str;
    str.Format(_T("%d"), (int)m_uiMineSum - (int)m_uiFlagSum);
    GetDlgItem(IDC_STATIC_LEFT_SUM)->SetWindowText(str);
}

void CsimpleMinerDlg::resetButtons()
{
    resetValues();
    for (UINT idxR = 0; idxR < m_uiRowSum; ++idxR)
    {
        for (UINT idxC = 0; idxC < m_uiColSum; ++idxC)
        {
            SCell& oCell = m_vecCells[idxR][idxC];
            oCell.reset();
            if (oCell.pBtn)
            {
                oCell.pBtn->SetWindowTextW(_T(""));
            }
        }
    }    
}

/// arrange mines
void CsimpleMinerDlg::layoutMines()
{
    if (m_nCustomMineSum != -1)
    {
        layoutCustomMines();
        return;
    }
    for (UINT idxR = 0; idxR < m_uiRowSum; ++idxR)
    {
        for (UINT idxC = 0; idxC < m_uiColSum; ++idxC)
        {
            UINT uiRand = (UINT)rand() % 5;
            if (uiRand == 0)
            {
                ++m_uiMineSum;
                m_vecCells[idxR][idxC].nVal = 9;
            }
        }
    }
}

void CsimpleMinerDlg::layoutCustomMines()
{
    /// layout by custom
    while ((int)m_uiMineSum < m_nCustomMineSum)
    {
        int nRand = rand();
        UINT val = (UINT)nRand % (m_uiRowSum * m_uiColSum);
        UINT row = val / m_uiColSum;
        UINT col = val % m_uiColSum;
        if (m_vecCells[row][col].nVal != 9)
        {
            ++m_uiMineSum;
            m_vecCells[row][col].nVal = 9;
        }
    }
}

/// set value for each non-mine cell
void CsimpleMinerDlg::fillOut()
{
    setLeftMineSum();
    for (UINT idxR = 0; idxR < m_uiRowSum; ++idxR)
    {
        for (UINT idxC = 0; idxC < m_uiColSum; ++idxC)
        {
            SCell& oCell = m_vecCells[idxR][idxC];
            if (oCell.nVal == -1)
            {
                oCell.nVal = calcNeighborMineSum(idxR, idxC);
            }
        }
    }
}

void removeFlag(SCell& oCell)
{
    oCell.pBtn->SetWindowTextW(_T(""));
    oCell.eStatus = E_BtnSta_Covered;
}

void addFlag(SCell& oCell)
{
    oCell.pBtn->SetWindowTextW(_T("¡Ì"));
    oCell.eStatus = E_BtnSta_Flagged;
}

void CsimpleMinerDlg::uncover(UINT uiRow, UINT uiCol)
{
    SCell& oCell = m_vecCells[uiRow][uiCol];
    if (oCell.eStatus != E_BtnSta_Covered)
    {
        return;
    }
    if (oCell.nVal == 9)
    {
        gameOver(oCell);
        return;
    }
    CString str;
    str.Format(_T("%d"), oCell.nVal);
    oCell.pBtn->SetWindowTextW(str);
    oCell.eStatus = E_BtnSta_Open;
    ++m_uiOpenSum;
    if (oCell.nVal != 0)
    {
        return;
    }
    uncoverNeighbors(uiRow, uiCol);
}

void CsimpleMinerDlg::uncoverNeighbors(UINT uiRow, UINT uiCol)
{
    /// uncover neighboring cells
    if (uiRow > 0) // having last row
    {
        if (uiCol > 0)
        {
            uncover(uiRow - 1, uiCol - 1);
        }
        uncover(uiRow - 1, uiCol);
        if (uiCol + 1 < m_uiColSum)
        {
            uncover(uiRow - 1, uiCol + 1);
        }
    }
    if (uiCol > 0)
    {
        uncover(uiRow, uiCol - 1);
    }
    if (uiCol + 1 < m_uiColSum)
    {
        uncover(uiRow, uiCol + 1);
    }
    if (uiRow + 1 < m_uiRowSum) // having next row
    {
        if (uiCol > 0)
        {
            uncover(uiRow + 1, uiCol - 1);
        }
        uncover(uiRow + 1, uiCol);
        if (uiCol + 1 < m_uiColSum)
        {
            uncover(uiRow + 1, uiCol + 1);
        }
    }
}

void CsimpleMinerDlg::smartUncoverNeighbors(UINT uiRow, UINT uiCol)
{
    SCell& oCell = m_vecCells[uiRow][uiCol];
    if (oCell.eStatus != E_BtnSta_Open || oCell.nVal == 0)
    {
        return;
    }
    int nNeiborFlags = 0;
    int nNeiborCovers = 0;
    calcNeiborFlagsAndCovers(uiRow, uiCol, nNeiborFlags, nNeiborCovers);
    if (nNeiborFlags != oCell.nVal || nNeiborCovers == 0)
    {
        return;
    }
    uncoverNeighbors(uiRow, uiCol);
}

void CsimpleMinerDlg::checkIfDone()
{
    if (m_uiOpenSum + m_uiMineSum != m_uiRowSum * m_uiColSum)
    {
        return;
    }
    AfxMessageBox(_T("Well Done!"), MB_ICONINFORMATION);
    resetButtons();
    layoutMines();
    fillOut();
}

void CsimpleMinerDlg::gameOver(SCell& oCell)
{
    oCell.pBtn->SetWindowTextW(_T("M"));
    AfxMessageBox(_T("Game Over!"), MB_ICONEXCLAMATION);
    resetButtons();
    layoutMines();
    fillOut();
}

void CsimpleMinerDlg::OnButtonClick(UINT id)
{
    UINT uiRow = (id - IDC_BTN_HEAD) / m_uiColSum;
    UINT uiCol = (id - IDC_BTN_HEAD) % m_uiColSum;
    SCell& oCell = m_vecCells[uiRow][uiCol];
    if (oCell.eStatus == E_BtnSta_Open)
    {
        smartUncoverNeighbors(uiRow, uiCol);
        checkIfDone();
        return;
    }
    if (oCell.eStatus == E_BtnSta_Flagged)
    {
        removeFlag(oCell);
        return;
    }
    if (oCell.nVal == 9)
    {
        gameOver(oCell);
        return;
    }
    uncover(uiRow, uiCol);
    checkIfDone();
}

void CsimpleMinerDlg::OnRClicked(UINT id, NMHDR* pNotify, LRESULT* pResult)
{
    UINT uiRow = (id - IDC_BTN_HEAD) / m_uiColSum;
    UINT uiCol = (id - IDC_BTN_HEAD) % m_uiColSum;
    SCell& oCell = m_vecCells[uiRow][uiCol];
    if (oCell.eStatus == E_BtnSta_Covered)
    {
        addFlag(oCell);
        m_uiFlagSum++;
        setLeftMineSum();
    }
    else if (oCell.eStatus == E_BtnSta_Flagged)
    {
        removeFlag(oCell);
        m_uiFlagSum--;
        setLeftMineSum();
    }
}

void CsimpleMinerDlg::OnBnClickedCheckShow()
{
    m_bShow = !m_bShow;
    GetDlgItem(IDC_STATIC_LEFT_SUM)->ShowWindow(m_bShow ? SW_SHOW : SW_HIDE);
}


void CsimpleMinerDlg::OnBnClickedBtnCustom()
{
    CCustomDlg dlg;
    dlg.setValues(m_uiRowSum, m_uiColSum, m_uiMineSum);
    if (IDOK != dlg.DoModal())
    {
        return;
    }
    m_nCustomMineSum = dlg.m_nMines;
    if ((int)m_uiRowSum == dlg.m_nRows && (int)m_uiColSum == dlg.m_nCols)
    {
        resetButtons();
        layoutMines();
        fillOut();
        return;
    }
    deleteButtons();
    m_uiRowSum = (UINT)dlg.m_nRows;
    m_uiColSum = (UINT)dlg.m_nCols;
    initButtons();
}
