
// LanguageLearnDlg.cpp : implementation file
//
#include "stdafx.h"
#include "LanguageLearn.h"
using namespace std;


#include "LanguageLearnDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLanguageLearnDlg dialog

CLanguageLearnDlg::CLanguageLearnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LANGUAGELEARN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_automated = false;
}

void CLanguageLearnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

CLanguageLearnDlg::~CLanguageLearnDlg()
{
	delete m_pFont;
	delete m_pButtonFont;
	delete m_pStatsFont;
}

BEGIN_MESSAGE_MAP(CLanguageLearnDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_WRONG, &CLanguageLearnDlg::OnBnClickedWrong)
	ON_BN_CLICKED(IDC_RIGHT, &CLanguageLearnDlg::OnBnClickedRight)
	ON_BN_CLICKED(IDC_SHOWANSWER, &CLanguageLearnDlg::OnBnClickedShowanswer)
	ON_BN_CLICKED(IDC_LOADLANGUAGE, &CLanguageLearnDlg::OnBnClickedLoadlanguage)
	ON_BN_CLICKED(IDOK, &CLanguageLearnDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DELETEPHRASE, &CLanguageLearnDlg::OnBnClickedDeletephrase)
	ON_BN_CLICKED(IDC_MILESTONES, &CLanguageLearnDlg::OnBnClickedMilestones)
	ON_BN_CLICKED(IDC_REVERSE, &CLanguageLearnDlg::OnBnClickedReverse)
	ON_BN_CLICKED(IDC_WIPE_HISTORY, &CLanguageLearnDlg::OnBnClickedWipeHistory)
	ON_BN_CLICKED(IDC_OUTPUT_DUE, &CLanguageLearnDlg::OnBnClickedOutputDue)
	ON_BN_CLICKED(IDC_COPY, &CLanguageLearnDlg::OnBnClickedCopy)
END_MESSAGE_MAP()

// CLanguageLearnDlg message handlers

BOOL CLanguageLearnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_pFont = new CFont;
	m_pFont->CreatePointFont(95, _T("Microsoft Sans Serif"));

	m_pButtonFont = new CFont;
	m_pButtonFont->CreatePointFont(110, _T("Microsoft Sans Serif"));

	m_pStatsFont = new CFont;
	m_pStatsFont->CreatePointFont(80, _T("Courier"));

	GetDlgItem(IDC_CHALLENGE)->SetFont(m_pFont);
	GetDlgItem(IDC_CHALLENGE2)->SetFont(m_pFont);
	GetDlgItem(IDC_ANSWER)->SetFont(m_pFont);
	GetDlgItem(IDC_ANSWER2)->SetFont(m_pFont);

	GetDlgItem(IDC_RIGHT)->SetFont(m_pButtonFont);
	GetDlgItem(IDC_WRONG)->SetFont(m_pButtonFont);
	GetDlgItem(IDC_SHOWANSWER)->SetFont(m_pButtonFont);

	showNoButtons();
	for (int i = 0; i < 12; ++i)
	{
		GetDlgItem(IDs[i])->SetFont(m_pFont);
		GetDlgItem(IDs[i])->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLanguageLearnDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLanguageLearnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLanguageLearnDlg::showAnswerButton()
{
	GetDlgItem(IDC_SHOWANSWER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RIGHT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_WRONG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ANSWER2)->ShowWindow(SW_HIDE);
}

void CLanguageLearnDlg::showRightAndWrongButtons()
{
	GetDlgItem(IDC_SHOWANSWER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RIGHT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_WRONG)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ANSWER2)->ShowWindow(SW_SHOW);
}

void CLanguageLearnDlg::showNoButtons()
{
	GetDlgItem(IDC_SHOWANSWER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RIGHT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_WRONG)->ShowWindow(SW_HIDE);
}

void CLanguageLearnDlg::populateLanguageList()
{
	char szFilters[] = "Text Files (*.txt)|*.txt";
	CFileDialog fileDlg(TRUE, _T("txt"), _T("G:\\*.txt"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		CString strPathname = fileDlg.GetPathName();

		CT2CA pszConvertedAnsiString(strPathname);
		b.m_filePath = pszConvertedAnsiString;

		b.readFile();
		b.readReferenceFile();
		b.getSelection();
		showSelection();
	}
}

void CLanguageLearnDlg::showSelection()
{
	if (b.m_batch.size() == 0) return;
	
	SetDlgItemText(IDC_CHALLENGE2, CString((*b.m_batchItr)->getPhrase().c_str()));
	SetDlgItemText(IDC_ANSWER2, CString((*b.m_batchItr)->getTranslation().c_str()));
	showAnswerButton();
	showRecentHistory();

	Phrase altPhrase("","");
	if (b.lookupTranslation(*(b.m_batchItr), &altPhrase) != 0)
	{
		if (altPhrase.getTranslation() != "" || altPhrase.getPhrase() != "")
		{
			string alternatives("Alternative text available. Do you want to replace with this:\r\n");
			// something matched
			if (altPhrase.getTranslation() != "")
			{
				alternatives += (*b.m_batchItr)->getTranslation();
				alternatives += " --->\r\n";
				alternatives += altPhrase.getTranslation();
				alternatives += "\r\n";
			}
			alternatives += altPhrase.getPhrase();
			
			CString altText (alternatives.c_str());
			if (AfxMessageBox(altText, MB_YESNO) == IDYES)
			{
				b.updatePhrase(altPhrase);
				SetDlgItemText(IDC_CHALLENGE2, CString((*b.m_batchItr)->getPhrase().c_str()));
				SetDlgItemText(IDC_ANSWER2, CString((*b.m_batchItr)->getTranslation().c_str()));
			}
		}
		else
		{
			GetDlgItem(IDC_COPY)->ShowWindow(SW_HIDE);
		}
	}
}

void CLanguageLearnDlg::display(const string& text)
{
	CString txt(text.c_str());
	SetDlgItemText(IDC_STATS_TEXT, txt);

	CString dueDaysText;
	
	// TO DO: coordinate this with the const. At the moment the const is pointless if the length is hardcoded here
	dueDaysText.Format(_T("%d\n%d\n%d\n%d\n%d\n%d\n%d"),
		b.m_stats.dueByDay[0],
		b.m_stats.dueByDay[1],
		b.m_stats.dueByDay[2],
		b.m_stats.dueByDay[3],
		b.m_stats.dueByDay[4],
		b.m_stats.dueByDay[5],
		b.m_stats.dueByDay[6]);

	SetDlgItemText(IDC_DUEBYDAY, dueDaysText);
}

void CLanguageLearnDlg::showRecentHistory()
{
	for (int i = 0; i < 12; ++i)
	{
		GetDlgItem(IDs[i])->ShowWindow(SW_HIDE);
	}

	size_t count = (*b.m_batchItr)->history.size();

	if (count == 0) return; // no history to display

	int i = 0;
	
	for (auto x : (*b.m_batchItr)->history)
	{
		// only show if this is within our last eleven tests
		if (count - i <= 12)
		{
			int offset = (count - 12);
			SetDlgItemText(IDs[i - offset], x->getSuccess() ? _T("o") : _T(" "));
			GetDlgItem(IDs[i - offset])->ShowWindow(SW_SHOW);
		}
		i++;
	}
	if (i >= 14) { // corresponds to a year
		GetDlgItem(IDC_COPY)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_COPY)->ShowWindow(SW_HIDE);
	}
}

void CLanguageLearnDlg::displayStats()
{
	std::string statsString;
	statsString += "Vocabulary: ";
	statsString += std::to_string(b.m_stats.vocabularySize) + " phrases\n";
	statsString += "Active vocabulary: ";
	statsString += std::to_string(b.m_stats.activeVocabularySize) + " phrases\n\n";
	statsString += "Due vocabulary: ";
	statsString += std::to_string(b.m_stats.dueVocabularySize) + " phrases\n\n";
	statsString += "Success rate (this session): ";
	statsString += std::to_string((b.m_stats.successesThisSession * 100) / (b.m_stats.successesThisSession + b.m_stats.failuresThisSession));
	statsString += "% of ";
	statsString += std::to_string(b.m_stats.successesThisSession + b.m_stats.failuresThisSession);
	statsString += "\nSuccess rate (overall): ";
	statsString += std::to_string((b.m_stats.successes * 100) / (b.m_stats.successes + b.m_stats.failures));
	statsString += "%\n";

	display(statsString);
}

void CLanguageLearnDlg::OnBnClickedWrong()
{
	moveToNext(false);
}


void CLanguageLearnDlg::OnBnClickedRight()
{
	moveToNext(true);
}

void CLanguageLearnDlg::moveToNext(bool result)
{
	showAnswerButton();
	b.logTest(result);
	displayStats();
	nextQuestion();
}

void CLanguageLearnDlg::nextQuestion()
{
	b.getSelection();
	showSelection();
}

void CLanguageLearnDlg::OnBnClickedShowanswer()
{
	showRightAndWrongButtons();
}

void CLanguageLearnDlg::OnBnClickedLoadlanguage()
{
	populateLanguageList();
}

void CLanguageLearnDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
	CString szError = b.saveFile();
	if (szError != _T(""))
	{
		AfxMessageBox(szError, MB_OK);
	}
}

void CLanguageLearnDlg::OnBnClickedDeletephrase()
{
	b.deletePhrase();
	showSelection();
}

void CLanguageLearnDlg::OnBnClickedMilestones()
{
}


void CLanguageLearnDlg::OnBnClickedReverse()
{
	b.reverse();
}


void CLanguageLearnDlg::OnBnClickedWipeHistory()
{
	// TODO: Add your control notification handler code here
}


void CLanguageLearnDlg::OnBnClickedOutputDue()
{
	if (AfxMessageBox(_T("Output all due vocabulary to a text file and mark as done?"), MB_YESNO) == IDYES)
	{
		m_automated = true;
		bool allOutputted = false;
		b.openDueFile();
		
		while (!allOutputted) {
			allOutputted = !b.outputDueItem();

			// call same activities next event:
			moveToNext(true);
		}

		m_automated = false;
		b.saveDueFile();

		AfxMessageBox(_T("All due items outputted."), MB_OK);
	}

}

bool CLanguageLearnDlg::doAreYouSure(CString text)
{
	return (AfxMessageBox(text, MB_YESNO) == IDYES);
}

void CLanguageLearnDlg::OnBnClickedCopy()
{
	CString szCopyText = CString((*b.m_batchItr)->getTranslation().c_str());
	szCopyText += "|";
	szCopyText += CString((*b.m_batchItr)->getPhrase().c_str());
	szCopyText += "\r\n";

	OpenClipboard();
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, szCopyText.GetLength() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}

	// Convert a TCHAR string to a LPCSTR
	CT2CA pszConvertedAnsiString(szCopyText);
	// construct a std::string using the LPCSTR input
	std::string strStd(pszConvertedAnsiString);

	memcpy(GlobalLock(hg), strStd.c_str(), szCopyText.GetLength() + 1);
	
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}
