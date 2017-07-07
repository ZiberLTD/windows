/* 
* Copyright (C) 2017 MicroSIP (http://www.microsip.org)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
*/

#include "langpack.h"
#include "resource.h"
#include "utf.h"

ZiberDlg::ZiberDlg(CWnd* pParent /*=NULL*/)
: CDialog(ZiberDlg::IDD, pParent)
{
	mainDlg = (CmainDlg* ) AfxGetMainWnd();	
	Create (IDD, pParent);
}

ZiberDlg::~ZiberDlg(void)
{
}


BOOL ZiberDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//TranslateDialog(this->m_hWnd);
	InitBinding();

	return TRUE;
}

void ZiberDlg::OnDestroy()
{
	mainDlg->ziberDlg = NULL;
	CDialog::OnDestroy();
}

void ZiberDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(ZiberDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &ZiberDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &ZiberDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ZIBER_BUTTON_SAVE, &ZiberDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_ZIBER_BUTTON_EDIT, &ZiberDlg::OnBnClickedEdit)
	ON_BN_CLICKED(IDC_ZIBER_BUTTON_BONUS_COINS, &ZiberDlg::OnBnClickedBonusCoins)
END_MESSAGE_MAP()

void ZiberDlg::OnClose() 
{
	DestroyWindow();
}

void ZiberDlg::OnBnClickedCancel()
{
	OnClose();
}

void ZiberDlg::OnBnClickedOk()
{
}

void ZiberDlg::OnBnClickedSave()
{
	CEdit* edit;
	edit = (CEdit*)GetDlgItem(IDC_ZIBER_EDIT_ETHEREUM);
	CString str;
	edit->GetWindowText(str);
	str.Trim();
	if (str.GetLength()!=42) {
		AfxMessageBox(_T("Please enter a valid Ethereum public key"));
		return;
	}
	accountSettings.ziberEthereumPublicKey = str;
	accountSettings.SettingsSave();
	InitBinding();
	if (accountSettings.accountId) {
		mainDlg->PJAccountDelete();
		accountSettings.accountId = 0;
	}
	mainDlg->PJAccountAdd();
	OnClose();
}

void ZiberDlg::OnBnClickedEdit()
{
	CEdit* edit;
	edit = (CEdit*)GetDlgItem(IDC_ZIBER_EDIT_ETHEREUM);
	edit->EnableWindow(TRUE);
	CButton* button;
	button = (CButton*)GetDlgItem(IDC_ZIBER_BUTTON_EDIT);
	button->ShowWindow(SW_HIDE);
	button = (CButton*)GetDlgItem(IDC_ZIBER_BUTTON_SAVE);
	button->ShowWindow(SW_SHOW);
}

void ZiberDlg::InitBinding()
{
	if (!accountSettings.ziberEthereumPublicKey.IsEmpty()) {
		CEdit* edit;
		edit = (CEdit*)GetDlgItem(IDC_ZIBER_EDIT_ETHEREUM);
		edit->SetWindowText(accountSettings.ziberEthereumPublicKey);
		edit->EnableWindow(FALSE);
		CButton* button;
		button = (CButton*)GetDlgItem(IDC_ZIBER_BUTTON_SAVE);
		button->ShowWindow(SW_HIDE);
		button = (CButton*)GetDlgItem(IDC_ZIBER_BUTTON_EDIT);
		button->ShowWindow(SW_SHOW);
	}
}

void ZiberDlg::OnBnClickedBonusCoins()
{
	if (!mainDlg->ziberBonusCoinsDlg) {
		mainDlg->ziberBonusCoinsDlg = new ZiberBonusCoinsDlg(this);
	} else {
		mainDlg->ziberBonusCoinsDlg->SetForegroundWindow();
	}
}