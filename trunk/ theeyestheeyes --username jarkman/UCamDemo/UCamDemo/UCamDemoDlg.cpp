// UCamDemoDlg.cpp : implementation file
//

#include "stdafx.h"

#include <GdiPlus.h>


#include "UCamDemo.h"

#include "PictureCtrl.h"
#include "mbed.h"
#include "UCamDemoDlg.h"
#include "Frame.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

void UCamInit();
void UCamGetJpeg();

Frame* UCamGetRaw();

Frame* UCamGetDiff();
Frame* UCamResetDiff( );

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()

	
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CUCamDemoDlg dialog




CUCamDemoDlg::CUCamDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUCamDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUCamDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_picCtrl);
}

BEGIN_MESSAGE_MAP(CUCamDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CUCamDemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUCamDemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_DIFF, &CUCamDemoDlg::OnBnClickedButtonDiff)
	ON_BN_CLICKED(IDC_BUTTONNEW_BACKGROUND, &CUCamDemoDlg::OnBnClickedButtonnewBackground)
END_MESSAGE_MAP()


// CUCamDemoDlg message handlers

BOOL CUCamDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUCamDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUCamDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUCamDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUCamDemoDlg::OnBnClickedButton1()
{
	UCamGetJpeg();
	//Load an Image from File
	m_picCtrl.Load(CString(_T("c:\\mbed\\out.jpg")));
}

void CUCamDemoDlg::OnBnClickedButton2()
{


	Frame * frame = UCamGetRaw();

	showFrame( frame );

	Frame::releaseFrame( &frame );
}

void CUCamDemoDlg::showFrame( Frame *frame )
{
	Gdiplus::Bitmap *b;

	uint8_t *buff = frame->m_pixels;

	uint16_t *newBuff = (uint16_t*) malloc( frame->m_frameSize * 2 );

	for( uint32_t i = 0; i < frame->m_frameSize; i ++) // swap byte ordering to suit Windows
	{
		uint16_t fivebits = buff[i] >>3;
		newBuff [i] = ( fivebits << 11) | ( fivebits << 6 ) | (fivebits ); // make 565 RGB from 8-bit grey
		
	}

	// must match the values we have in UCamGetRaw
	int width = frame->m_width;;
	int height = frame->m_height;

	int format = PixelFormat16bppRGB565; //PixelFormat16bppGrayScale;

	int bpp = 2;

	INT stride = bpp * width;

	b = ::new Gdiplus::Bitmap( width, height, stride, format , (uint8_t*) newBuff );

	m_picCtrl.Load( b);
	

}

void CUCamDemoDlg::OnBnClickedButtonDiff()
{
	Frame * frame = UCamGetDiff();

	showFrame( frame );
}

void CUCamDemoDlg::OnBnClickedButtonnewBackground()
{
	Frame * frame = UCamResetDiff();

	showFrame( frame );
}
