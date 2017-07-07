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

#include "stdafx.h"
#include "settings.h"
#include "Crypto.h"

#include <algorithm>
#include <vector>

using namespace MFC;

AccountSettings accountSettings;
bool firstRun;
bool pj_ready;

void AccountSettings::Init()
{
	CString str;
	LPTSTR ptr;

	accountId = 0;
	//--
	wchar_t *p = exeFile.GetBuffer(MAX_PATH);
	GetModuleFileName(NULL, p, MAX_PATH);
	exeFile.ReleaseBuffer();
	//--
	CString fileName = PathFindFileName(exeFile);
	fileName = fileName.Mid(0,fileName.ReverseFind('.'));
	logFile.Format(_T("%s.log"), fileName);
	iniFile.Format(_T("%s.ini"), fileName);
	pathRoaming=_T("");
	pathLocal=_T("");
	if (pathRoaming.IsEmpty()) {
		CString contactsFile = _T("Contacts.xml");
		CFileStatus rStatus;
		CRegKey regKey;
		CString pathInstaller;
		CString rab;
		ULONG pnChars;
		rab.Format(_T("Software\\%s"), _T(_GLOBAL_NAME));
		if (regKey.Open(HKEY_LOCAL_MACHINE,rab,KEY_READ)==ERROR_SUCCESS) {
			ptr = pathInstaller.GetBuffer(255);
			pnChars = 256;
			regKey.QueryStringValue(NULL,ptr,&pnChars);
			pathInstaller.ReleaseBuffer();
			regKey.Close();
		}

		CString pathExe;
		ptr = pathExe.GetBuffer(MAX_PATH+1);
		GetModuleFileName(NULL, ptr, MAX_PATH);
		pathExe.ReleaseBuffer();
		pathExe = pathExe.Mid(0,pathExe.ReverseFind('\\'));

		CString appDataRoaming;
		ptr = appDataRoaming.GetBuffer(MAX_PATH);
		SHGetSpecialFolderPath(
			0,
			ptr, 
			CSIDL_APPDATA,
			FALSE ); 
		appDataRoaming.ReleaseBuffer();
		appDataRoaming.AppendFormat(_T("\\%s\\"),_T(_GLOBAL_NAME));

		if (!pathInstaller.IsEmpty() && pathInstaller.CompareNoCase(pathExe) == 0) {
			// installer
			CreateDirectory(appDataRoaming, NULL);
			pathRoaming = appDataRoaming;
			CString appDataLocal;
			ptr = appDataLocal.GetBuffer(MAX_PATH);
			SHGetSpecialFolderPath(
				0,
				ptr, 
				CSIDL_LOCAL_APPDATA, 
				FALSE ); 
			appDataLocal.ReleaseBuffer();
			appDataLocal.AppendFormat(_T("\\%s\\"),_T(_GLOBAL_NAME));
			CreateDirectory(appDataLocal, NULL);
			pathLocal = appDataLocal;
			logFile = pathLocal + logFile;
		} else {
			// portable
			pathRoaming = pathExe + _T("\\");
			pathLocal = pathRoaming;
			// for version <= 3.14.5 move ini file from currdir to exedir
			CString pathCurrent;
			ptr = pathCurrent.GetBuffer(MAX_PATH);
			::GetCurrentDirectory(MAX_PATH, ptr);
			pathCurrent.ReleaseBuffer();
			if (pathCurrent.CompareNoCase(pathExe) != 0) {
				pathCurrent.Append(_T("\\"));
				if (CopyFile(pathCurrent + iniFile, pathRoaming + iniFile,  TRUE)) {
					DeleteFile(pathCurrent + iniFile);
				}
				if (CopyFile(pathCurrent + contactsFile, pathRoaming + contactsFile,  TRUE)) {
					DeleteFile(pathCurrent + contactsFile);
				}
				DeleteFile(pathCurrent + logFile);
			}
			// copy ini from installer path
			CopyFile(appDataRoaming + iniFile, pathRoaming + iniFile,  TRUE);
			CopyFile(appDataRoaming + contactsFile, pathRoaming + contactsFile,  TRUE);
			logFile = pathLocal + logFile;
		}

		iniFile = pathRoaming + iniFile;
		
		firstRun = true;
		if (CFile::GetStatus(iniFile, rStatus)) {
			firstRun = false;
		}
	}
	//--

	CString section;
	section = _T("Settings");

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("ziberNodeAuto"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	ziberNodeAuto=str==_T("0")?0:1;

	ptr = ziberNodeIP.GetBuffer(255);
	GetPrivateProfileString(section,_T("ziberNodeIP"), NULL, ptr, 256, iniFile);
	ziberNodeIP.ReleaseBuffer();

	ptr = ziberEthereumPublicKey.GetBuffer(255);
	GetPrivateProfileString(section,_T("ziberEthereumPublicKey"), NULL, ptr, 256, iniFile);
	ziberEthereumPublicKey.ReleaseBuffer();

	ptr = portKnockerHost.GetBuffer(255);
	GetPrivateProfileString(section,_T("portKnockerHost"), NULL, ptr, 256, iniFile);
	portKnockerHost.ReleaseBuffer();

	ptr = portKnockerPorts.GetBuffer(255);
	GetPrivateProfileString(section,_T("portKnockerPorts"), NULL, ptr, 256, iniFile);
	portKnockerPorts.ReleaseBuffer();

	enableLocalAccount=0;

	crashReport=0;

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("DTMFMethod"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	DTMFMethod=str==_T("1")?1:(str==_T("2")?2:(str==_T("3")?3:0));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("autoAnswer"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	autoAnswer=str==_T("1")?1:(str==_T("2")?2:0);

	ptr = userAgent.GetBuffer(255);
	GetPrivateProfileString(section,_T("userAgent"), NULL, ptr, 256, iniFile);
	userAgent.ReleaseBuffer();

	ptr = denyIncoming.GetBuffer(255);
	GetPrivateProfileString(section,_T("denyIncoming"), NULL, ptr, 256, iniFile);
	denyIncoming.ReleaseBuffer();

	ptr = stun.GetBuffer(255);
	GetPrivateProfileString(section,_T("STUN"), NULL, ptr, 256, iniFile);
	stun.ReleaseBuffer();

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("enableSTUN"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	enableSTUN = str == "1" ? 1 : 0;

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("localDTMF"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	localDTMF=str==_T("0")?0:1;

	ptr = ringingSound.GetBuffer(255);
	GetPrivateProfileString(section,_T("ringingSound"), NULL, ptr, 256, iniFile);
	ringingSound.ReleaseBuffer();
	ptr = audioInputDevice.GetBuffer(255);
	GetPrivateProfileString(section,_T("audioInputDevice"), NULL, ptr, 256, iniFile);
	audioInputDevice.ReleaseBuffer();
	ptr = audioOutputDevice.GetBuffer(255);
	GetPrivateProfileString(section,_T("audioOutputDevice"), NULL, ptr, 256, iniFile);
	audioOutputDevice.ReleaseBuffer();
	ptr = audioRingDevice.GetBuffer(255);
	GetPrivateProfileString(section,_T("audioRingDevice"), NULL, ptr, 256, iniFile);
	audioRingDevice.ReleaseBuffer();
	
	ptr = audioCodecs.GetBuffer(255);
	GetPrivateProfileString(section,_T("audioCodecs"), NULL, ptr, 256, iniFile);
	audioCodecs.ReleaseBuffer();

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("sourcePort"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	sourcePort = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("VAD"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	vad = str == "1" ? 1 : 0;

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("EC"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	ec = str==_T("1")?1:0;

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("forceCodec"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	forceCodec = str == "1" ? 1 : 0;

#ifdef _GLOBAL_VIDEO
	ptr = videoCaptureDevice.GetBuffer(255);
	GetPrivateProfileString(section,_T("videoCaptureDevice"), NULL, ptr, 256, iniFile);
	videoCaptureDevice.ReleaseBuffer();
	ptr = videoCodec.GetBuffer(255);
	GetPrivateProfileString(section,_T("videoCodec"), NULL, ptr, 256, iniFile);
	videoCodec.ReleaseBuffer();

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("disableH264"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	disableH264 = str == "1" ? 1 : 0;
	ptr = bitrateH264.GetBuffer(255);
	GetPrivateProfileString(section,_T("bitrateH264"), NULL, ptr, 256, iniFile);
	bitrateH264.ReleaseBuffer();

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("disableH263"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	disableH263 = str == "1" ? 1 : 0;
	ptr = bitrateH263.GetBuffer(255);
	GetPrivateProfileString(section,_T("bitrateH263"), NULL, ptr, 256, iniFile);
	bitrateH263.ReleaseBuffer();

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("disableVP8"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	disableVP8 = str == "1" ? 1 : 0;
	ptr = bitrateVP8.GetBuffer(255);
	GetPrivateProfileString(section,_T("bitrateVP8"), NULL, ptr, 256, iniFile);
	bitrateVP8.ReleaseBuffer();
#endif

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("mainX"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	mainX = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("mainY"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	mainY = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("mainW"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	mainW = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("mainH"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	mainH = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("messagesX"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	messagesX = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("messagesY"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	messagesY = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("messagesW"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	messagesW = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("messagesH"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	messagesH = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("ringinX"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	ringinX = atoi(CStringA(str));
	
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("ringinY"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	ringinY = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("callsWidth0"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth0 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("callsWidth1"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth1 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("callsWidth2"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth2 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("callsWidth3"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth3 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("callsWidth4"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	callsWidth4 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("contactsWidth0"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	contactsWidth0 = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("volumeOutput"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	volumeOutput = str.IsEmpty()?100:atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("volumeInput"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	volumeInput = str.IsEmpty()?100:atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("activeTab"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	activeTab = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("alwaysOnTop"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	alwaysOnTop = atoi(CStringA(str));

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("autoHangUpTime"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	autoHangUpTime = atoi(CStringA(str));

	ptr = cmdCallStart.GetBuffer(255);
	GetPrivateProfileString(section,_T("cmdCallStart"), NULL, ptr, 256, iniFile);
	cmdCallStart.ReleaseBuffer();

	ptr = cmdCallEnd.GetBuffer(255);
	GetPrivateProfileString(section,_T("cmdCallEnd"), NULL, ptr, 256, iniFile);
	cmdCallEnd.ReleaseBuffer();

	ptr = cmdIncomingCall.GetBuffer(255);
	GetPrivateProfileString(section,_T("cmdIncomingCall"), NULL, ptr, 256, iniFile);
	cmdIncomingCall.ReleaseBuffer();

	ptr = cmdCallAnswer.GetBuffer(255);
	GetPrivateProfileString(section,_T("cmdCallAnswer"), NULL, ptr, 256, iniFile);
	cmdCallAnswer.ReleaseBuffer();

	enableLog=0;

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("randomAnswerBox"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	randomAnswerBox=str==_T("1")?1:0;

	singleMode = 1;

	hidden = 0;

	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("silent"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	silent = str == "1" ? 1 : 0;

	//--
	ptr = str.GetBuffer(255);
	GetPrivateProfileString(section,_T("accountId"), NULL, ptr, 256, iniFile);
	str.ReleaseBuffer();
	if (str.IsEmpty()) {
		if (AccountLoad(-1,&account)) {
			accountId = 1;
			WritePrivateProfileString(section,_T("accountId"), _T("1"), iniFile);
		}
	} else {
		accountId=atoi(CStringA(str));
		if (!accountId && !enableLocalAccount) {
			accountId = 1;
		}
		if (accountId>0) {
			if (!AccountLoad(accountId,&account)) {
				accountId = 0;
			}
		}
	}

}

AccountSettings::AccountSettings()
{
	Init();
}

void AccountSettings::AccountDelete(int id)
{
	CString section;
	section.Format(_T("Account%d"),id);
	WritePrivateProfileStruct(section, NULL, NULL, 0, iniFile);
}

bool AccountSettings::AccountLoad(int id, Account *account)
{
	CString str;
	CString rab;
	LPTSTR ptr;

	CString section;
	if (id==-1) {
		section = _T("Settings");
	} else {
		section.Format(_T("Account%d"),id);
	}

	account->label = _T(_GLOBAL_ACCOUNT_LABEL);

	account->server = _T(_GLOBAL_ACCOUNT_SIP_SERVER);

	account->proxy = _T(_GLOBAL_ACCOUNT_SIP_PROXY);

	account->domain = _T(_GLOBAL_ACCOUNT_DOMAIN);


	CString usernameLocal;
	CString passwordLocal;

	ptr = usernameLocal.GetBuffer(255);
	GetPrivateProfileString(section,_T("username"), NULL, ptr, 256, iniFile);
	usernameLocal.ReleaseBuffer();

	ptr = passwordLocal.GetBuffer(255);
	GetPrivateProfileString(section,_T("password"), NULL, ptr, 256, iniFile);
	passwordLocal.ReleaseBuffer();
	if (!passwordLocal.IsEmpty()) {
		CByteArray arPassword;
		String2Bin(passwordLocal, &arPassword);
		CCrypto crypto;
		if (crypto.DeriveKey((LPCTSTR)_GLOBAL_KEY)) {
			try {			
				if (!crypto.Decrypt(arPassword,passwordLocal)) {
					//--decode from old format
					ptr = str.GetBuffer(255);
					GetPrivateProfileString(section,_T("passwordSize"), NULL, ptr, 256, iniFile);
					str.ReleaseBuffer();
					if (!str.IsEmpty()) {
						int size = atoi(CStringA(str));
						arPassword.SetSize(size>0?size:16);
						GetPrivateProfileStruct(section,_T("password"), arPassword.GetData(),arPassword.GetSize(), iniFile);
						crypto.Decrypt(arPassword,passwordLocal);
					}
					//--end decode from old format
					if (crypto.Encrypt(passwordLocal,arPassword)) {
						WritePrivateProfileString(section,_T("password"), Bin2String(&arPassword), iniFile);
						//--delete old format addl.data
						WritePrivateProfileString(section,_T("passwordSize"),NULL,iniFile);
					}
				}
			} catch (CArchiveException *e) {
			}
		}
	}

	account->rememberPassword = !usernameLocal.GetLength() || passwordLocal.GetLength()?1:0;


	account->username = usernameLocal;
	account->password = passwordLocal;


	account->authID = _T(_GLOBAL_ACCOUNT_LOGIN);

	account->displayName = _T(_GLOBAL_ACCOUNT_NAME);

	account->srtp = _T(_GLOBAL_ACCOUNT_SRTP);

	account->transport = _T(_GLOBAL_ACCOUNT_TRANSPORT);

	account->publish=_GLOBAL_ACCOUNT_PUBLISH;

	account->ice=_GLOBAL_ACCOUNT_ICE;

	account->allowRewrite=_GLOBAL_ACCOUNT_ALLOW_REWRITE;


	account->disableSessionTimer=_GLOBAL_ACCOUNT_DISABLE_SESSION_TIMER;

	bool sectionExists = IniSectionExists(section,iniFile);

	if (id==-1) {
		// delete old
		WritePrivateProfileString(section,_T("server"), NULL, iniFile);
		WritePrivateProfileString(section,_T("proxy"), NULL, iniFile);
		WritePrivateProfileString(section,_T("SRTP"), NULL, iniFile);
		WritePrivateProfileString(section,_T("transport"), NULL, iniFile);
		WritePrivateProfileString(section,_T("publicAddr"), NULL, iniFile);
		WritePrivateProfileString(section,_T("publish"), NULL, iniFile);
		WritePrivateProfileString(section,_T("STUN"), NULL, iniFile);
		WritePrivateProfileString(section,_T("ICE"), NULL, iniFile);
		WritePrivateProfileString(section,_T("allowRewrite"), NULL, iniFile);
		WritePrivateProfileString(section,_T("domain"), NULL, iniFile);
		WritePrivateProfileString(section,_T("authID"), NULL, iniFile);
		WritePrivateProfileString(section,_T("username"), NULL, iniFile);
		WritePrivateProfileString(section,_T("passwordSize"), NULL, iniFile);
		WritePrivateProfileString(section,_T("password"), NULL, iniFile);
		WritePrivateProfileString(section,_T("id"), NULL, iniFile);
		WritePrivateProfileString(section,_T("displayName"), NULL, iniFile);
		// save new
		//if (!account->domain.IsEmpty() && !account->username.IsEmpty()) {
		if (sectionExists && !account->domain.IsEmpty()) {
			AccountSave(1, account);
		}
	}
	//return !account->domain.IsEmpty() && !account->username.IsEmpty();
	return  sectionExists && !account->domain.IsEmpty();
}

void AccountSettings::AccountSave(int id, Account *account)
{
	CString section;
	section.Format(_T("Account%d"),id);

	CString usernameLocal;
	CString passwordLocal;

	usernameLocal = account->username;
	passwordLocal = account->password;

	if (!account->rememberPassword) {
		WritePrivateProfileString(section,_T("username"),_T(""),iniFile);
		WritePrivateProfileString(section,_T("password"),_T(""),iniFile);
	}

	if (account->rememberPassword) {
		WritePrivateProfileString(section,_T("username"),usernameLocal,iniFile);
	CCrypto crypto;
	CByteArray arPassword;
	if (crypto.DeriveKey((LPCTSTR)_GLOBAL_KEY)
		&& crypto.Encrypt(passwordLocal,arPassword)
		) {
			WritePrivateProfileString(section,_T("password"), Bin2String(&arPassword), iniFile);
	} else {
		WritePrivateProfileString(section,_T("password"), passwordLocal, iniFile);
	}
	}

}

void AccountSettings::SettingsSave()
{
	CString str;

CString section;
section = _T("Settings");

	WritePrivateProfileString(section,_T("ziberNodeAuto"),ziberNodeAuto?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(section,_T("ziberNodeIP"),ziberNodeIP,iniFile);
	WritePrivateProfileString(section,_T("ziberEthereumPublicKey"),ziberEthereumPublicKey,iniFile);

	str.Format(_T("%d"),accountId);
	WritePrivateProfileString(section,_T("accountId"),str,iniFile);

	WritePrivateProfileString(section,_T("randomAnswerBox"),randomAnswerBox?_T("1"):_T("0"),iniFile);

	WritePrivateProfileString(section,_T("portKnockerHost"),portKnockerHost,iniFile);
	WritePrivateProfileString(section,_T("portKnockerPorts"),portKnockerPorts,iniFile);

	WritePrivateProfileString(section,_T("DTMFMethod"),DTMFMethod==1?_T("1"):(DTMFMethod==2?_T("2"):(DTMFMethod==3?_T("3"):_T("0"))),iniFile);
	WritePrivateProfileString(section,_T("autoAnswer"),autoAnswer==1?_T("1"):(autoAnswer==2?_T("2"):_T("0")),iniFile);
	WritePrivateProfileString(section,_T("denyIncoming"),denyIncoming,iniFile);

	WritePrivateProfileString(section,_T("STUN"),stun,iniFile);
	WritePrivateProfileString(section,_T("enableSTUN"),enableSTUN?_T("1"):_T("0"),iniFile);

	WritePrivateProfileString(section,_T("silent"),silent?_T("1"):_T("0"),iniFile);

	WritePrivateProfileString(section,_T("localDTMF"),localDTMF?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(section,_T("ringingSound"),ringingSound,iniFile);
	WritePrivateProfileString(section,_T("audioInputDevice"),_T("\"")+audioInputDevice+_T("\""),iniFile);
	WritePrivateProfileString(section,_T("audioOutputDevice"),_T("\"")+audioOutputDevice+_T("\""),iniFile);
	WritePrivateProfileString(section,_T("audioRingDevice"),_T("\"")+audioRingDevice+_T("\""),iniFile);
	WritePrivateProfileString(section,_T("audioCodecs"),audioCodecs,iniFile);
	WritePrivateProfileString(section,_T("VAD"),vad?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(section,_T("EC"),ec?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(section,_T("forceCodec"),forceCodec?_T("1"):_T("0"),iniFile);
#ifdef _GLOBAL_VIDEO
	WritePrivateProfileString(section,_T("videoCaptureDevice"),_T("\"")+videoCaptureDevice+_T("\""),iniFile);
	WritePrivateProfileString(section,_T("videoCodec"),videoCodec,iniFile);
	WritePrivateProfileString(section,_T("disableH264"),disableH264?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(section,_T("bitrateH264"),bitrateH264,iniFile);
	WritePrivateProfileString(section,_T("disableH263"),disableH263?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(section,_T("bitrateH263"),bitrateH263,iniFile);
	WritePrivateProfileString(section,_T("disableVP8"),disableVP8?_T("1"):_T("0"),iniFile);
	WritePrivateProfileString(section,_T("bitrateVP8"),bitrateVP8,iniFile);
#endif

	str.Format(_T("%d"),mainX);
	WritePrivateProfileString(section,_T("mainX"),str,iniFile);

	str.Format(_T("%d"),mainY);
	WritePrivateProfileString(section,_T("mainY"),str,iniFile);

	str.Format(_T("%d"),mainW);
	WritePrivateProfileString(section,_T("mainW"),str,iniFile);

	str.Format(_T("%d"),mainH);
	WritePrivateProfileString(section,_T("mainH"),str,iniFile);

	str.Format(_T("%d"),messagesX);
	WritePrivateProfileString(section,_T("messagesX"),str,iniFile);

	str.Format(_T("%d"),messagesY);
	WritePrivateProfileString(section,_T("messagesY"),str,iniFile);

	str.Format(_T("%d"),messagesW);
	WritePrivateProfileString(section,_T("messagesW"),str,iniFile);

	str.Format(_T("%d"),messagesH);
	WritePrivateProfileString(section,_T("messagesH"),str,iniFile);

	str.Format(_T("%d"),ringinX);
	WritePrivateProfileString(section,_T("ringinX"),str,iniFile);

	str.Format(_T("%d"),ringinY);
	WritePrivateProfileString(section,_T("ringinY"),str,iniFile);

	str.Format(_T("%d"),callsWidth0);
	WritePrivateProfileString(section,_T("callsWidth0"),str,iniFile);
		
	str.Format(_T("%d"),callsWidth1);
	WritePrivateProfileString(section,_T("callsWidth1"),str,iniFile);

	str.Format(_T("%d"),callsWidth2);
	WritePrivateProfileString(section,_T("callsWidth2"),str,iniFile);

	str.Format(_T("%d"),callsWidth3);
	WritePrivateProfileString(section,_T("callsWidth3"),str,iniFile);

	str.Format(_T("%d"),callsWidth4);
	WritePrivateProfileString(section,_T("callsWidth4"),str,iniFile);

	str.Format(_T("%d"),contactsWidth0);
	WritePrivateProfileString(section,_T("contactsWidth0"),str,iniFile);

	str.Format(_T("%d"),volumeOutput);
	WritePrivateProfileString(section,_T("volumeOutput"),str,iniFile);

	str.Format(_T("%d"),volumeInput);
	WritePrivateProfileString(section,_T("volumeInput"),str,iniFile);

	str.Format(_T("%d"),activeTab);
	WritePrivateProfileString(section,_T("activeTab"),str,iniFile);

	str.Format(_T("%d"),alwaysOnTop);
	WritePrivateProfileString(section,_T("alwaysOnTop"),str,iniFile);
	
	str.Format(_T("%d"),autoHangUpTime);
	WritePrivateProfileString(section,_T("autoHangUpTime"),str,iniFile);

	WritePrivateProfileString(section,_T("cmdCallStart"),cmdCallStart,iniFile);
	WritePrivateProfileString(section,_T("cmdCallEnd"),cmdCallEnd,iniFile);
	WritePrivateProfileString(section,_T("cmdIncomingCall"),cmdIncomingCall,iniFile);
	WritePrivateProfileString(section,_T("cmdCallAnswer"),cmdCallAnswer,iniFile);
}
