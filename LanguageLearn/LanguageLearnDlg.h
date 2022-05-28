
// LanguageLearnDlg.h : header file
//

//MFC APP defines this header file twice, which means we have to include every fucking header it needs here

#include <string>
#include <vector>

#pragma once

#include "backend.h"

class Phrase; // for Gods sake
class Milestone;


// CLanguageLearnDlg dialog
class CLanguageLearnDlg : public CDialogEx
{
// Construction
public:
	CLanguageLearnDlg(CWnd* pParent = NULL);	// standard constructor
	~CLanguageLearnDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LANGUAGELEARN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void display(const std::string& text);
	void nextQuestion();
	void populateLanguageList();

	void displayStats();
	void showSelection();
	void showRecentHistory();
	void showAnswerButton();
	void showRightAndWrongButtons();
	void showNoButtons();
	bool doAreYouSure(CString text);

	BackEnd b;

	CFont* m_pFont;
	CFont* m_pButtonFont;
	CFont* m_pStatsFont;

	string test;
	// For the test markers:
	int IDs[12] = {
		IDC_TBM1,
		IDC_TB0,
		IDC_TB1,
		IDC_TB2,
		IDC_TB3,
		IDC_TB4,
		IDC_TB5,
		IDC_TB6,
		IDC_TB7,
		IDC_TB8,
		IDC_TB9,
		IDC_TB10
	};

	afx_msg void OnBnClickedWrong();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedShowanswer();
	afx_msg void OnBnClickedLoadlanguage();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDeletephrase();
	afx_msg void OnBnClickedMilestones();
public:
	afx_msg void OnBnClickedReverse();
	afx_msg void OnBnClickedWipeHistory();
	afx_msg void OnBnClickedSmoothStartDates();
	afx_msg void OnBnClickedCopy();
	afx_msg void OnBnClickedTb7();
};


