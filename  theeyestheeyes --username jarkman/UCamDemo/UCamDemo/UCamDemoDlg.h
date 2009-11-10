// UCamDemoDlg.h : header file
//

#pragma once

#include "PictureCtrl.h"
class Frame;

// CUCamDemoDlg dialog
class CUCamDemoDlg : public CDialog
{
// Construction
public:
	CUCamDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UCAMDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CPictureCtrl m_picCtrl;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonDiff();
	
	void showFrame( Frame *frame );
	afx_msg void OnBnClickedButtonnewBackground();
};
