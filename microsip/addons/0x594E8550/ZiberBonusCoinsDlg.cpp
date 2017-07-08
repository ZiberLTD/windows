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
#include "utf.h"

enum EUserWndMessagesBindWallet
{
	UM_BIND_WALLET_FIRST = (WM_USER + 0x100 + 1),
	UM_SEND_WALLET_LOADED,
	UM_CONTRACT_LOADED
};

ZiberBonusCoinsDlg::ZiberBonusCoinsDlg(CWnd* pParent /*=NULL*/)
: CDialog(ZiberBonusCoinsDlg::IDD, pParent)
{
	mainDlg = (CmainDlg* ) AfxGetMainWnd();	
	Create (IDD, pParent);
}

ZiberBonusCoinsDlg::~ZiberBonusCoinsDlg(void)
{
}


BOOL ZiberBonusCoinsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//TranslateDialog(this->m_hWnd);

	CComboBox *combobox;
	
	combobox= (CComboBox*)GetDlgItem(IDC_ZIBER_COMBO_TYPE);
	combobox->AddString(_T("BTC"));
	combobox->AddString(_T("ETH"));
	combobox->SetCurSel(0);
	
	return TRUE;
}

void ZiberBonusCoinsDlg::OnDestroy()
{
	mainDlg->ziberBonusCoinsDlg = NULL;
	CDialog::OnDestroy();
}

void ZiberBonusCoinsDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(ZiberBonusCoinsDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &ZiberBonusCoinsDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &ZiberBonusCoinsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ZIBER_BUTON_SEND, &ZiberBonusCoinsDlg::OnBnClickedSendWallet)
	ON_MESSAGE(UM_SEND_WALLET_LOADED, &ZiberBonusCoinsDlg::onSendWalletLoaded)
	ON_MESSAGE(UM_CONTRACT_LOADED, &ZiberBonusCoinsDlg::onContractLoaded)
	ON_CBN_SELCHANGE(IDC_ZIBER_COMBO_TYPE, OnBnChangeType)
	ON_EN_SETFOCUS(IDC_ZIBER_EDIT_BONUS, OnBonusSetFocus)
	ON_EN_CHANGE(IDC_ZIBER_EDIT_ADDR, OnChangeAddr)
END_MESSAGE_MAP()

void ZiberBonusCoinsDlg::OnClose() 
{
	DestroyWindow();
}

void ZiberBonusCoinsDlg::OnBnClickedCancel()
{
	OnClose();
}

void ZiberBonusCoinsDlg::OnBnClickedOk()
{
}

void ZiberBonusCoinsDlg::OnChangeAddr()
{
	CComboBox *combobox;
	combobox= (CComboBox*)GetDlgItem(IDC_ZIBER_COMBO_TYPE);
	CString str;
	GetDlgItem(IDC_ZIBER_EDIT_ADDR)->GetWindowText(str);
	if (combobox->GetCurSel()==1) {
		GetDlgItem(IDC_ZIBER_EDIT_BONUS)->SetWindowText(str);
	}
}

void ZiberBonusCoinsDlg::OnBnChangeType()
{
	CComboBox *combobox;
	combobox= (CComboBox*)GetDlgItem(IDC_ZIBER_COMBO_TYPE);
	CString str;
	GetDlgItem(IDC_ZIBER_EDIT_ADDR)->GetWindowText(str);
	if (combobox->GetCurSel()==1) {
		GetDlgItem(IDC_ZIBER_EDIT_BONUS)->EnableWindow(FALSE);
		//if (str.IsEmpty() && !accountSettings.ziberEthereumPublicKey.IsEmpty()) {
		//	str = accountSettings.ziberEthereumPublicKey;
		//	GetDlgItem(IDC_ZIBER_EDIT_ADDR)->SetWindowText(str);
		//}
		GetDlgItem(IDC_ZIBER_EDIT_BONUS)->SetWindowText(str);
	} else {
		GetDlgItem(IDC_ZIBER_EDIT_BONUS)->EnableWindow(TRUE);
	}
}

void ZiberBonusCoinsDlg::OnBonusSetFocus()
{
	//CComboBox *combobox;
	//combobox= (CComboBox*)GetDlgItem(IDC_ZIBER_COMBO_TYPE);
	//if (combobox->GetCurSel()==0) {
	//	CString str;
	//	GetDlgItem(IDC_ZIBER_EDIT_BONUS)->GetWindowText(str);
	//	str.Trim();
	//	if (str.IsEmpty() && !accountSettings.ziberEthereumPublicKey.IsEmpty()) {
	//		GetDlgItem(IDC_ZIBER_EDIT_BONUS)->SetWindowText(accountSettings.ziberEthereumPublicKey);
	//	}
	//}
}

void ZiberBonusCoinsDlg::OnBnClickedSendWallet()
{
	CString types[] = {\
_T("BTC"),\
_T("ETH")\
	};
	CString type;
	CString address;
	CString bonus;

	CEdit* edit;
	CComboBox *combobox;

	combobox= (CComboBox*)GetDlgItem(IDC_ZIBER_COMBO_TYPE);
	type = types[combobox->GetCurSel()];

	edit = (CEdit*)GetDlgItem(IDC_ZIBER_EDIT_ADDR);
	edit->GetWindowText(address);
	address.Trim();

	edit = (CEdit*)GetDlgItem(IDC_ZIBER_EDIT_BONUS);
	edit->GetWindowText(bonus);
	bonus.Trim();

	if (accountSettings.ziberNodeIP.IsEmpty()) {
		AfxMessageBox(Translate(_T("Ziber node is not selected.")));
		return;
	}
	if (address.IsEmpty() || bonus.IsEmpty()) {
		AfxMessageBox(Translate(_T("Please fill out all required fields.")));
		return;
	}
	if (
		(type==_T("BTC") && (address.GetLength()<27 || address.GetLength()>34))
		||
		(type==_T("ETH") && address.GetLength()!=42)
		) {
		AfxMessageBox(Translate(_T("Please enter a valid Wallet Address.")));
		return;
	}
	if (bonus.GetLength()!=42) {
		AfxMessageBox(Translate(_T("Please enter a valid Bonus ETH Account.")));
		return;
	}
	CString url;
	url.Format(_T("http://%s/node/auth_client/?chain_type=%s&chain_addr=%s&bonus_eth_wallet=%s"),accountSettings.ziberNodeIP,type,
		CString(urlencode(Utf8EncodeUcs2(address))),
		CString(urlencode(Utf8EncodeUcs2(bonus)))
		);
	URLGetAsync(url,m_hWnd,UM_SEND_WALLET_LOADED);
	GetDlgItem(IDC_ZIBER_BUTON_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_ZIBER_EDIT_CONFIRM)->SetWindowText(_T(""));
	GetDlgItem(IDC_ZIBER_EDIT_CONFIRM)->EnableWindow(FALSE);
	GetDlgItem(IDC_ZIBER_EDIT_AMOUNT)->SetWindowText(_T(""));
	GetDlgItem(IDC_ZIBER_EDIT_AMOUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_ZIBER_COMBO_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ZIBER_EDIT_ADDR)->EnableWindow(FALSE);
	GetDlgItem(IDC_ZIBER_EDIT_BONUS)->EnableWindow(FALSE);
}

LRESULT ZiberBonusCoinsDlg::onSendWalletLoaded(WPARAM wParam,LPARAM lParam)
{
	bool success = false;
	URLGetAsyncData *response = (URLGetAsyncData *)wParam;
	if (response->statusCode == 200) {
		if (!response->body.IsEmpty()) {
			Json::Value root;
			Json::Reader reader;

			bool parsedSuccess = reader.parse((LPCSTR)response->body, 
				root, 
				false);
			if(parsedSuccess) {
				try {
					if (root["error"].isString() && root["error"].asCString() != "") {
						AfxMessageBox(Utf8DecodeUni(root["error"].asCString()));
					} else if (root.isMember("reg_open_date")) {
						CString message;
						if (root["reg_open_date"].isString()) {
							message.Format(_T("Wallet registration will be available %s"),Utf8DecodeUni(root["reg_open_date"].asCString()));
						} else {
							message.Format(_T("Wallet registration will be available soon"));
						}
						AfxMessageBox(message);
					} else {
						if (root["success"].isBool() && root["success"].asBool()) {
							CString confirmAddress;
							CString confirmAmount;
							if (root["confirm_address"].isString()) {
								confirmAddress = Utf8DecodeUni(root["confirm_address"].asCString());
							}
							if (root["amount"].isString()) {
								confirmAmount = Utf8DecodeUni(root["amount"].asCString());
							} else if (root["confirm_amount"].isString()) {
								confirmAmount = Utf8DecodeUni(root["confirm_amount"].asCString());
							} else {
								confirmAmount = _T("0.0001");
							}
							if (root.isMember("etherium_contract_bytecode")) {
								CFile file;
								CFileStatus fileStatus;
								CString filename = GetContractFilename();
								if (file.GetStatus(filename, fileStatus)) {
									file.Remove(filename);
								}
								if (root["etherium_contract_bytecode"].isString()) {
									CString contractURL = Utf8DecodeUni(root["etherium_contract_bytecode"].asCString());
									if (!contractURL.IsEmpty()) {
										URLGetAsync(contractURL,m_hWnd,UM_CONTRACT_LOADED);
									}
								}
							}
							GetDlgItem(IDC_ZIBER_EDIT_CONFIRM)->SetWindowText(confirmAddress);
							GetDlgItem(IDC_ZIBER_EDIT_AMOUNT)->SetWindowText(confirmAmount);
							GetDlgItem(IDC_ZIBER_EDIT_CONFIRM)->EnableWindow(TRUE);
							GetDlgItem(IDC_ZIBER_EDIT_AMOUNT)->EnableWindow(TRUE);
							success = true;
						} else {
							AfxMessageBox(_T("Success is not true in JSON response"), MB_ICONEXCLAMATION);
						}
					}
				} catch (std::exception const& e) {
					AfxMessageBox(_T("Wrong data type in JSON response"), MB_ICONEXCLAMATION);
				}
			} else {
				AfxMessageBox(_T("JSON response parse error"), MB_ICONEXCLAMATION);
			}
		} else {
			AfxMessageBox(_T("Response is empty"), MB_ICONEXCLAMATION);
		}
	} else {
		CString str;
		str.Format(_T("Request failed. Response code: %d"),response->statusCode);
		AfxMessageBox(str, MB_ICONEXCLAMATION);
	}
	if (!success) {
		GetDlgItem(IDC_ZIBER_BUTON_SEND)->EnableWindow(TRUE);

		GetDlgItem(IDC_ZIBER_COMBO_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_ZIBER_EDIT_ADDR)->EnableWindow(TRUE);
		GetDlgItem(IDC_ZIBER_EDIT_BONUS)->EnableWindow(TRUE);
	}
	return 0;
}

CString ZiberBonusCoinsDlg::GetContractFilename()
{
	CString filename = accountSettings.pathLocal;
	filename.Append(_T("custom_contract.zbr"));
	return filename;
}

typedef void (*FUNCPTR)(); 

LRESULT ZiberBonusCoinsDlg::onContractLoaded(WPARAM wParam,LPARAM lParam)
{
	URLGetAsyncData *response = (URLGetAsyncData *)wParam;
	if (response->statusCode == 200) {
		/*
		CFile file;
		if (file.Open(GetContractFilename(),CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)) {
			file.Write(response->body.GetBuffer(),response->body.GetLength());
			file.Close();
		}
		*/
	}
	return 0;
}
