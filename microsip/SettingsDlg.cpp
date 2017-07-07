/* 
 * Copyright (C) 2011-2016 MicroSIP (http://www.microsip.org)
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

#include "StdAfx.h"
#include "SettingsDlg.h"
#include "mainDlg.h"
#include "settings.h"
#include "Preview.h"
#include "langpack.h"

static BOOL prev;

SettingsDlg::SettingsDlg(CWnd* pParent /*=NULL*/)
: CDialog(SettingsDlg::IDD, pParent)
{
	Create (IDD, pParent);
	prev = FALSE;
}

SettingsDlg::~SettingsDlg(void)
{
}

int SettingsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (langPack.rtl) {
		ModifyStyleEx(0,WS_EX_LAYOUTRTL);
	}
	return 0;
}

BOOL SettingsDlg::OnInitDialog()
{
	CComboBox *combobox;
	CComboBox *combobox2;
	unsigned count;
	int i;

	CDialog::OnInitDialog();

	TranslateDialog(this->m_hWnd);

	combobox= (CComboBox*)GetDlgItem(IDC_DTMF_METHOD);
	combobox->AddString(Translate(_T("Auto")));
	combobox->AddString(Translate(_T("In-band")));
	combobox->AddString(Translate(_T("RFC2833")));
	combobox->AddString(Translate(_T("SIP-INFO")));
	combobox->SetCurSel(accountSettings.DTMFMethod);

	combobox= (CComboBox*)GetDlgItem(IDC_AUTO_ANSWER);
	combobox->AddString(Translate(_T("No")));
	combobox->AddString(Translate(_T("All Calls")));
	combobox->SetCurSel(accountSettings.autoAnswer);

	combobox= (CComboBox*)GetDlgItem(IDC_DENY_INCOMING);
	combobox->AddString(Translate(_T("No")));
	combobox->AddString(Translate(_T("All Calls")));
	if (accountSettings.denyIncoming==_T("all"))
	{
		i=1;
	} else
	{
		i=0;
	}
	combobox->SetCurSel(i);

	GetDlgItem(IDC_STUN)->SetWindowText(accountSettings.stun);
	((CButton*)GetDlgItem(IDC_STUN_CHECKBOX))->SetCheck(accountSettings.enableSTUN);

	((CButton*)GetDlgItem(IDC_LOCAL_DTMF))->SetCheck(accountSettings.localDTMF);
	((CButton*)GetDlgItem(IDC_ANSWER_BOX_RANDOM))->SetCheck(accountSettings.randomAnswerBox);
	((CButton*)GetDlgItem(IDC_VAD))->SetCheck(accountSettings.vad);
	((CButton*)GetDlgItem(IDC_EC))->SetCheck(accountSettings.ec);
	((CButton*)GetDlgItem(IDC_FORCE_CODEC))->SetCheck(accountSettings.forceCodec);

	GetDlgItem(IDC_RINGING_SOUND)->SetWindowText(accountSettings.ringingSound);

	pjmedia_aud_dev_info aud_dev_info[128];
	count = 128;
	pjsua_enum_aud_devs(aud_dev_info, &count);

	combobox= (CComboBox*)GetDlgItem(IDC_MICROPHONE);
	combobox->AddString(Translate(_T("Default")));
	combobox->SetCurSel(0);

	for (unsigned i=0;i<count;i++)
	{
		if (aud_dev_info[i].input_count) {
			CString audDevName(aud_dev_info[i].name);
			combobox->AddString( audDevName );
			if (!accountSettings.audioInputDevice.Compare(audDevName))
			{
				combobox->SetCurSel(combobox->GetCount()-1);
			}
		}
	}
	combobox= (CComboBox*)GetDlgItem(IDC_SPEAKERS);
	combobox->AddString(Translate(_T("Default")));
	combobox->SetCurSel(0);
	combobox2= (CComboBox*)GetDlgItem(IDC_RING);
	combobox2->AddString(Translate(_T("Default")));
	combobox2->SetCurSel(0);
	for (unsigned i=0;i<count;i++)
	{
		if (aud_dev_info[i].output_count) {
			CString audDevName(aud_dev_info[i].name);
			combobox->AddString(audDevName);
			combobox2->AddString(audDevName);
			if (!accountSettings.audioOutputDevice.Compare(audDevName))
			{
				combobox->SetCurSel(combobox->GetCount()-1);
			}
			if (!accountSettings.audioRingDevice.Compare(audDevName))
			{
				combobox2->SetCurSel(combobox->GetCount()-1);
			}
		}
	}

	pjsua_codec_info codec_info[64];
	CListBox *listbox;
	CListBox *listbox2;
	listbox = (CListBox*)GetDlgItem(IDC_AUDIO_CODECS_ALL);
	listbox2 = (CListBox*)GetDlgItem(IDC_AUDIO_CODECS);

	CList<CString> disabledCodecsList;
	count = 64;
	pjsua_enum_codecs(codec_info, &count);
	for (unsigned i=0;i<count;i++)
	{
		if (StrStr(_T(_GLOBAL_CODECS_AVAILABLE),PjToStr(&codec_info[i].codec_id)))
		{
			POSITION pos = mainDlg->audioCodecList.Find(
				PjToStr(&codec_info[i].codec_id)
				);
			CString key = mainDlg->audioCodecList.GetNext(pos);
			CString value  = mainDlg->audioCodecList.GetNext(pos);
			if (codec_info[i].priority
				&& (!accountSettings.audioCodecs.IsEmpty() || StrStr(_T(_GLOBAL_CODECS_ENABLED),key))
				) {
				listbox2->AddString(value);
			} else {
				disabledCodecsList.AddTail(key);
			}	
		}
	}
	POSITION pos = mainDlg->audioCodecList.GetHeadPosition();
	while (pos) {
		CString key = mainDlg->audioCodecList.GetNext(pos);
		CString value  = mainDlg->audioCodecList.GetNext(pos);
		if (disabledCodecsList.Find(key)) {
			listbox->AddString(value);
		}
	}

#ifdef _GLOBAL_VIDEO
	((CButton*)GetDlgItem(IDC_DISABLE_H264))->SetCheck(accountSettings.disableH264);
 	((CButton*)GetDlgItem(IDC_DISABLE_H263))->SetCheck(accountSettings.disableH263);
 	((CButton*)GetDlgItem(IDC_DISABLE_VP8))->SetCheck(accountSettings.disableVP8);
	if (accountSettings.bitrateH264.IsEmpty()) {
		const pj_str_t codec_id = {"H264", 4};
		pjmedia_vid_codec_param param;
		pjsua_vid_codec_get_param(&codec_id, &param);
		accountSettings.bitrateH264.Format(_T("%d"),param.enc_fmt.det.vid.max_bps/1000);
	}
	if (accountSettings.bitrateH263.IsEmpty()) {
		const pj_str_t codec_id = {"H263", 4};
		pjmedia_vid_codec_param param;
		pjsua_vid_codec_get_param(&codec_id, &param);
		accountSettings.bitrateH263.Format(_T("%d"),param.enc_fmt.det.vid.max_bps/1000);
	}
	if (accountSettings.bitrateVP8.IsEmpty()) {
		const pj_str_t codec_id = {"VP8", 4};
		pjmedia_vid_codec_param param;
		pjsua_vid_codec_get_param(&codec_id, &param);
		accountSettings.bitrateVP8.Format(_T("%d"),param.enc_fmt.det.vid.max_bps/1000);
	}
	GetDlgItem(IDC_BITRATE_264)->SetWindowText(accountSettings.bitrateH264);
	GetDlgItem(IDC_BITRATE_263)->SetWindowText(accountSettings.bitrateH263);
	GetDlgItem(IDC_BITRATE_VP8)->SetWindowText(accountSettings.bitrateVP8);

	combobox= (CComboBox*)GetDlgItem(IDC_VID_CAP_DEV);
	combobox->AddString(Translate(_T("Default")));
	combobox->SetCurSel(0);
	pjmedia_vid_dev_info vid_dev_info[64];
	count = 64;
	pjsua_vid_enum_devs(vid_dev_info, &count);
	for (unsigned i=0;i<count;i++)
	{
		if (vid_dev_info[i].fmt_cnt && (vid_dev_info[i].dir==PJMEDIA_DIR_ENCODING || vid_dev_info[i].dir==PJMEDIA_DIR_ENCODING_DECODING))
		{
			CString vidDevName(vid_dev_info[i].name);
			combobox->AddString(vidDevName);
			if (!accountSettings.videoCaptureDevice.Compare(vidDevName))
			{
				combobox->SetCurSel(combobox->GetCount()-1);
			}
		}
	}

	combobox= (CComboBox*)GetDlgItem(IDC_VIDEO_CODEC);
	combobox->AddString(Translate(_T("Default")));
	combobox->SetCurSel(0);
	count = 64;
	pjsua_vid_enum_codecs(codec_info, &count);
	for (unsigned i=0;i<count;i++)
	{
		combobox->AddString(PjToStr(&codec_info[i].codec_id));
		if (!accountSettings.videoCodec.Compare(PjToStr(&codec_info[i].codec_id)))
		{
			combobox->SetCurSel(combobox->GetCount()-1);
		}
	}
#endif

	return TRUE;
}

void SettingsDlg::OnDestroy()
{
	mainDlg->settingsDlg = NULL;
	CDialog::OnDestroy();
}

void SettingsDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

BEGIN_MESSAGE_MAP(SettingsDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL, &SettingsDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &SettingsDlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MODIFY, &SettingsDlg::OnDeltaposSpinModify)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ORDER, &SettingsDlg::OnDeltaposSpinOrder)
#ifdef _GLOBAL_VIDEO
	ON_BN_CLICKED(IDC_PREVIEW, &SettingsDlg::OnBnClickedPreview)
#endif
	ON_BN_CLICKED(IDC_BROWSE, &SettingsDlg::OnBnClickedBrowse)
	ON_EN_CHANGE(IDC_RINGING_SOUND, &SettingsDlg::OnEnChangeRingingSound)
	ON_BN_CLICKED(IDC_DEFAULT, &SettingsDlg::OnBnClickedDefault)
END_MESSAGE_MAP()


void SettingsDlg::OnClose() 
{
	DestroyWindow();
}

void SettingsDlg::OnBnClickedCancel()
{
	OnClose();
}

void SettingsDlg::OnBnClickedOk()
{
	this->ShowWindow(SW_HIDE);
	mainDlg->PJDestroy();

	CComboBox *combobox;
	int i;

	combobox= (CComboBox*)GetDlgItem(IDC_DTMF_METHOD);
	accountSettings.DTMFMethod = combobox->GetCurSel();

	combobox= (CComboBox*)GetDlgItem(IDC_AUTO_ANSWER);
	accountSettings.autoAnswer = combobox->GetCurSel();

	combobox= (CComboBox*)GetDlgItem(IDC_DENY_INCOMING);
	i = combobox->GetCurSel();
	accountSettings.denyIncoming=i?_T("all"):_T("");

	GetDlgItem(IDC_STUN)->GetWindowText(accountSettings.stun);
	accountSettings.stun.Trim();
	accountSettings.enableSTUN = ((CButton*)GetDlgItem(IDC_STUN_CHECKBOX))->GetCheck();

	accountSettings.localDTMF=((CButton*)GetDlgItem(IDC_LOCAL_DTMF))->GetCheck();
	accountSettings.randomAnswerBox=((CButton*)GetDlgItem(IDC_ANSWER_BOX_RANDOM))->GetCheck();
	accountSettings.vad=((CButton*)GetDlgItem(IDC_VAD))->GetCheck();
	accountSettings.ec=((CButton*)GetDlgItem(IDC_EC))->GetCheck();
	accountSettings.forceCodec =((CButton*)GetDlgItem(IDC_FORCE_CODEC))->GetCheck();

	GetDlgItem(IDC_MICROPHONE)->GetWindowText(accountSettings.audioInputDevice);
	if (accountSettings.audioInputDevice==Translate(_T("Default")))
	{
		accountSettings.audioInputDevice = _T("");
	}

	GetDlgItem(IDC_SPEAKERS)->GetWindowText(accountSettings.audioOutputDevice);
	if (accountSettings.audioOutputDevice==Translate(_T("Default")))
	{
		accountSettings.audioOutputDevice = _T("");
	}

	GetDlgItem(IDC_RING)->GetWindowText(accountSettings.audioRingDevice);
	if (accountSettings.audioRingDevice==Translate(_T("Default")))
	{
		accountSettings.audioRingDevice = _T("");
	}

	accountSettings.audioCodecs = _T("");
	CListBox *listbox2;
	listbox2 = (CListBox*)GetDlgItem(IDC_AUDIO_CODECS);
	for (unsigned i = 0; i < listbox2->GetCount(); i++)
	{
		CString value;
		listbox2->GetText(i, value);
		POSITION pos = mainDlg->audioCodecList.Find(value);
		if (pos) {
			mainDlg->audioCodecList.GetPrev(pos);
			CString key = mainDlg->audioCodecList.GetPrev(pos);
			accountSettings.audioCodecs += key + _T(" ");
		}
	}
	accountSettings.audioCodecs.Trim();

#ifdef _GLOBAL_VIDEO
	accountSettings.disableH264=((CButton*)GetDlgItem(IDC_DISABLE_H264))->GetCheck();
	accountSettings.disableH263=((CButton*)GetDlgItem(IDC_DISABLE_H263))->GetCheck();
	accountSettings.disableVP8=((CButton*)GetDlgItem(IDC_DISABLE_VP8))->GetCheck();
	GetDlgItem(IDC_BITRATE_VP8)->GetWindowText(accountSettings.bitrateVP8);
	if (!atoi(CStringA(accountSettings.bitrateH264))) {
		accountSettings.bitrateH264=_T("");
	}
	GetDlgItem(IDC_BITRATE_263)->GetWindowText(accountSettings.bitrateH263);
	if (!atoi(CStringA(accountSettings.bitrateH263))) {
		accountSettings.bitrateH263=_T("");
	}
	GetDlgItem(IDC_BITRATE_VP8)->GetWindowText(accountSettings.bitrateVP8);
	if (!atoi(CStringA(accountSettings.bitrateVP8))) {
		accountSettings.bitrateVP8=_T("");
	}
	GetDlgItem(IDC_VID_CAP_DEV)->GetWindowText(accountSettings.videoCaptureDevice);
	if (accountSettings.videoCaptureDevice==Translate(_T("Default")))
	{
		accountSettings.videoCaptureDevice = _T("");
	}

	GetDlgItem(IDC_VIDEO_CODEC)->GetWindowText(accountSettings.videoCodec);
	if (accountSettings.videoCodec==Translate(_T("Default")))
	{
		accountSettings.videoCodec = _T("");
	}
#endif

	GetDlgItem(IDC_RINGING_SOUND)->GetWindowText(accountSettings.ringingSound);

	accountSettings.SettingsSave();
	mainDlg->PJCreate();
	mainDlg->PJAccountAdd();

	OnClose();
}

void SettingsDlg::OnBnClickedBrowse()
{
	CFileDialog dlgFile( TRUE, _T("wav"), 0, OFN_NOCHANGEDIR, _T("WAV Files (*.wav)|*.wav|") );
	if (dlgFile.DoModal()==IDOK) {
		CString cwd;
		LPTSTR ptr = cwd.GetBuffer(MAX_PATH);
		::GetCurrentDirectory(MAX_PATH, ptr);
		cwd.ReleaseBuffer();
		if ( cwd.MakeLower() + _T("\\") + dlgFile.GetFileName().MakeLower() == dlgFile.GetPathName().MakeLower() ) {
			GetDlgItem(IDC_RINGING_SOUND)->SetWindowText(dlgFile.GetFileName());
		} else {
			GetDlgItem(IDC_RINGING_SOUND)->SetWindowText(dlgFile.GetPathName());
		}
	}
}

void SettingsDlg::OnEnChangeRingingSound()
{
	CString str;
	GetDlgItem(IDC_RINGING_SOUND)->GetWindowText(str);
	GetDlgItem(IDC_DEFAULT)->EnableWindow(str.GetLength()>0);
}

void SettingsDlg::OnBnClickedDefault()
{
	GetDlgItem(IDC_RINGING_SOUND)->SetWindowText(_T(""));
}

void SettingsDlg::OnDeltaposSpinModify(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CListBox *listbox;
	CListBox *listbox2;
	listbox = (CListBox*)GetDlgItem(IDC_AUDIO_CODECS_ALL);
	listbox2 = (CListBox*)GetDlgItem(IDC_AUDIO_CODECS);
	if (pNMUpDown->iDelta == -1) {
		//add
		int selected = listbox->GetCurSel();
		if (selected != LB_ERR) 
		{
			CString str;
			listbox->GetText(selected, str);
			listbox2->AddString(str);
			listbox->DeleteString(selected);
			listbox->SetCurSel( selected < listbox->GetCount() ? selected : selected-1 );
		}
	} else {
		//remove
		int selected = listbox2->GetCurSel();
		if (selected != LB_ERR) 
		{
			CString str;
			listbox2->GetText(selected, str);
			listbox->AddString(str);
			listbox2->DeleteString(selected);
			listbox2->SetCurSel( selected < listbox2->GetCount() ? selected : selected-1 );
		}
	}
	*pResult = 0;
}

void SettingsDlg::OnDeltaposSpinOrder(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CListBox *listbox2;
	listbox2 = (CListBox*)GetDlgItem(IDC_AUDIO_CODECS);
	int selected = listbox2->GetCurSel();
	if (selected != LB_ERR) 
	{
		CString str;
		listbox2->GetText(selected, str);
		if (pNMUpDown->iDelta == -1) {
			//up
			if (selected > 0)
			{
				listbox2->DeleteString(selected);
				listbox2->InsertString(selected-1,str);
				listbox2->SetCurSel(selected-1);
			}
		} else {
			//down
			if (selected < listbox2->GetCount()-1)
			{
				listbox2->DeleteString(selected);
				listbox2->InsertString(selected+1,str);
				listbox2->SetCurSel(selected+1);
			}
		}
	}
	*pResult = 0;
}

#ifdef _GLOBAL_VIDEO
void SettingsDlg::OnBnClickedPreview()
{
	CComboBox *combobox;
	combobox = (CComboBox*)GetDlgItem(IDC_VID_CAP_DEV);
	CString name;
	combobox->GetWindowText(name);
	if (!mainDlg->previewWin) {
		mainDlg->previewWin = new Preview(mainDlg);
	}
	mainDlg->previewWin->Start(mainDlg->VideoCaptureDeviceId(name));
}
#endif


