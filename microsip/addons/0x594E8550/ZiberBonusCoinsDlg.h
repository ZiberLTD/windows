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

#include "resource.h"

class ZiberBonusCoinsDlg :
	public CDialog
{
public:
	ZiberBonusCoinsDlg(CWnd* pParent = NULL);	// standard constructor
	~ZiberBonusCoinsDlg();
	enum { IDD = IDD_ZIBER_BONUS_COINS };

	CString uid;

	CString GetContractFilename();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSendWallet();
	afx_msg void OnBnChangeType();
	afx_msg void OnBonusSetFocus();
	afx_msg void OnChangeAddr();
	afx_msg LRESULT onSendWalletLoaded(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT onContractLoaded(WPARAM wParam,LPARAM lParam);
};
