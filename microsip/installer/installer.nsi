;NSIS Modern User Interface
;Welcome/Finish Page Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include "Library.nsh"
  !include "FileFunc.nsh"  
;--------------------------------
;General

!define APPNAME "Ziber blockchain telephony"
!define APPCOMPANY "Ziber.io"
!define CUSTOM
!define FAST
 
!ifndef APPLINKNAME
  !define APPLINKNAME "${APPNAME}"
!endif

!ifdef CUSTOM
  !define PREFIX "langpack\"
!else
  !define PREFIX ""
!endif

!ifdef THEME_ORANGE
!ifndef APPICON
  !insertmacro MUI_DEFAULT MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!else
  !insertmacro MUI_DEFAULT MUI_ICON "${APPICON}"
!endif
  !insertmacro MUI_DEFAULT MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"
  !define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange.bmp"
  !define MUI_HEADERIMAGE_UNBITMAP  "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall.bmp"
  !insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
  !insertmacro MUI_DEFAULT MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!endif

  XPStyle on

  ;Default installation folder
!ifdef APPFOLDER
  InstallDir "$PROGRAMFILES\${APPFOLDER}"
!else
  InstallDir "$PROGRAMFILES\${APPNAME}"
!endif

  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\${APPNAME}" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
  
  !define ARP "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"

;--------------------------------
;Variables

  Var STARTMENU_FOLDER
  Var MUI_TEMP

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING
  !define MUI_LANGDLL_ALLLANGUAGES  
  !define MUI_HEADERIMAGE
!ifndef MUI_HEADERIMAGE_BITMAP
  !define MUI_HEADERIMAGE_BITMAP "header.bmp"
  !define MUI_HEADERIMAGE_UNBITMAP  "header-uninstall.bmp"
  !insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "wizard.bmp"
  !insertmacro MUI_DEFAULT MUI_UNWELCOMEFINISHPAGE_BITMAP "wizard.bmp"
!ifndef APPICON
  !insertmacro MUI_DEFAULT MUI_ICON "install.ico"
!else
  !insertmacro MUI_DEFAULT MUI_ICON "${APPICON}"
!endif
  !insertmacro MUI_DEFAULT MUI_UNICON "uninstall.ico"
!endif

;--------------------------------
;Pages

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${APPNAME}"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!ifdef APPFOLDER
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "${APPFOLDER}"
!endif

  !insertmacro MUI_PAGE_WELCOME
!ifndef FAST
  !insertmacro MUI_PAGE_LICENSE "full\License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
!endif
  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
  !insertmacro MUI_PAGE_INSTFILES
!ifndef CUSTOM
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE FinishedInstall
Function FinishedInstall
ExecShell "open" "http://www.microsip.org/donate"
FunctionEnd 
!endif

  !define MUI_FINISHPAGE_RUN
  !define MUI_FINISHPAGE_RUN_FUNCTION "LaunchApp"
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_COMPONENTS
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH


;--------------------------------
;Languages

;--------------------------------
;Languages

!ifndef APPLANG

!ifdef APPLANGDEFAULT
!insertmacro MUI_LANGUAGE "${APPLANGDEFAULT}"
!endif

!insertmacro MUI_LANGUAGE "English" ;first language is the default language

!insertmacro MUI_LANGUAGE "Afrikaans"
!insertmacro MUI_LANGUAGE "Albanian"
!insertmacro MUI_LANGUAGE "Arabic"
!insertmacro MUI_LANGUAGE "Armenian"
!insertmacro MUI_LANGUAGE "Basque"
!insertmacro MUI_LANGUAGE "Belarusian"
!insertmacro MUI_LANGUAGE "Bosnian"
!insertmacro MUI_LANGUAGE "Breton"
!insertmacro MUI_LANGUAGE "Bulgarian"
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "Cibemba"
!insertmacro MUI_LANGUAGE "Croatian"
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "Danish"
!insertmacro MUI_LANGUAGE "Dutch"
!insertmacro MUI_LANGUAGE "Efik"
!insertmacro MUI_LANGUAGE "Esperanto"
!insertmacro MUI_LANGUAGE "Estonian"
!insertmacro MUI_LANGUAGE "Farsi"
!insertmacro MUI_LANGUAGE "Finnish"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "Galician"
!insertmacro MUI_LANGUAGE "Georgian"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "GermanSwitzerland"
!insertmacro MUI_LANGUAGE "Greek"
!insertmacro MUI_LANGUAGE "Hebrew"
!insertmacro MUI_LANGUAGE "Hungarian"
!insertmacro MUI_LANGUAGE "Icelandic"
!insertmacro MUI_LANGUAGE "Igbo"
!insertmacro MUI_LANGUAGE "Indonesian"
!insertmacro MUI_LANGUAGE "Irish"
!insertmacro MUI_LANGUAGE "Italian"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Khmer"
!insertmacro MUI_LANGUAGE "Korean"
!insertmacro MUI_LANGUAGE "Kurdish"
!insertmacro MUI_LANGUAGE "Latvian"
!insertmacro MUI_LANGUAGE "Lithuanian"
!insertmacro MUI_LANGUAGE "Luxembourgish"
!insertmacro MUI_LANGUAGE "Macedonian"
!insertmacro MUI_LANGUAGE "Malagasy"
!insertmacro MUI_LANGUAGE "Malay"
!insertmacro MUI_LANGUAGE "Mongolian"
!insertmacro MUI_LANGUAGE "Norwegian"
!insertmacro MUI_LANGUAGE "NorwegianNynorsk"
!insertmacro MUI_LANGUAGE "Polish"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "PortugueseBR"
!insertmacro MUI_LANGUAGE "Romanian"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Serbian"
!insertmacro MUI_LANGUAGE "SerbianLatin"
!insertmacro MUI_LANGUAGE "Sesotho"
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "Slovak"
!insertmacro MUI_LANGUAGE "Slovenian"
!insertmacro MUI_LANGUAGE "Spanish"
!insertmacro MUI_LANGUAGE "SpanishInternational"
!insertmacro MUI_LANGUAGE "Swahili"
!insertmacro MUI_LANGUAGE "Swedish"
!insertmacro MUI_LANGUAGE "Tamil"
!insertmacro MUI_LANGUAGE "Thai"
!insertmacro MUI_LANGUAGE "TradChinese"
!insertmacro MUI_LANGUAGE "Turkish"
!insertmacro MUI_LANGUAGE "Twi"
!insertmacro MUI_LANGUAGE "Ukrainian"
!insertmacro MUI_LANGUAGE "Uyghur"
!insertmacro MUI_LANGUAGE "Uzbek"
!insertmacro MUI_LANGUAGE "Vietnamese"
!insertmacro MUI_LANGUAGE "Welsh"
!insertmacro MUI_LANGUAGE "Yoruba"
!insertmacro MUI_LANGUAGE "Zulu"

!else

!insertmacro MUI_LANGUAGE "${APPLANG}"

!ifdef APPLANG2
!insertmacro MUI_LANGUAGE "${APPLANG2}"
!endif
!ifdef APPLANG3
!insertmacro MUI_LANGUAGE "${APPLANG3}"
!endif
!ifdef APPLANG4
!insertmacro MUI_LANGUAGE "${APPLANG4}"
!endif
!ifdef APPLANG5
!insertmacro MUI_LANGUAGE "${APPLANG5}"
!endif


!endif

;--------------------------------
;VersionInfo

  VIProductVersion "${APPVERSON}.0"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "${APPNAME}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductDescription" "${APPNAME} Setup"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductVersion" "${APPVERSON}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "${APPCOMPANY}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "${APPCOMPANY}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "${APPNAME} Setup"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${APPVERSON}"

;--------------------------------
; Init

!ifdef APPLITE
	!ifdef CUSTOM
		!define EXEFILE "lite\custom\${APPNAME}.exe"
	!else
		!define EXEFILE "lite\${APPNAME}.exe"	
	!endif
!else
	!ifdef CUSTOM
		!define EXEFILE "full\custom\${APPNAME}.exe"
	!else
		!define EXEFILE "full\${APPNAME}.exe"	
	!endif
!endif

!define FileCopy `!insertmacro FileCopy`
!macro FileCopy FilePath TargetDir
  CreateDirectory `${TargetDir}`
  CopyFiles `${FilePath}` `${TargetDir}`
!macroend


!macro RemoveLinksAssociation

; for WinXP

  ReadRegStr $0 HKLM "SOFTWARE\Classes\sip" "Owner Name"
  StrCmp $0 "${APPNAME}" 0 +2
  DeleteRegKey HKLM "SOFTWARE\Classes\sip"

  ReadRegStr $0 HKLM "SOFTWARE\Classes\tel" "Owner Name"
  StrCmp $0 "${APPNAME}" 0 +2
  DeleteRegKey HKLM "SOFTWARE\Classes\tel"

  ReadRegStr $0 HKLM "SOFTWARE\Classes\callto" "Owner Name"
  StrCmp $0 "${APPNAME}" 0 +2
  DeleteRegKey HKLM "SOFTWARE\Classes\callto"


; for Vista

  ReadRegStr $0 HKCU "SOFTWARE\Microsoft\Windows\Shell\Associations\UrlAssociations\sip\UserChoice" "Progid"
  StrCmp $0 "${APPNAME}" 0 +2
  DeleteRegKey HKCU "SOFTWARE\Microsoft\Windows\Shell\Associations\UrlAssociations\sip"

  ReadRegStr $0 HKCU "SOFTWARE\Microsoft\Windows\Shell\Associations\UrlAssociations\tel\UserChoice" "Progid"
  StrCmp $0 "${APPNAME}" 0 +2
  DeleteRegKey HKCU "SOFTWARE\Microsoft\Windows\Shell\Associations\UrlAssociations\tel"

  ReadRegStr $0 HKCU "SOFTWARE\Microsoft\Windows\Shell\Associations\UrlAssociations\callto\UserChoice" "Progid"
  StrCmp $0 "${APPNAME}" 0 +2
  DeleteRegKey HKCU "SOFTWARE\Microsoft\Windows\Shell\Associations\UrlAssociations\callto"

!macroend


;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
!ifdef INNER
  !echo "Inner invocation"                  ; just to see what's going on
  OutFile "$%TEMP%\tempinstaller.exe"       ; not really important where this is
  SetCompress off                           ; for speed
!else

;Name and file
Name "${APPNAME}"
!ifdef APPLITE
  !define OUTFILE "out\${APPNAME}-Lite-${APPVERSON}.exe"
!else
  !define OUTFILE "out\${APPNAME}-${APPVERSON}.exe"
!endif
OutFile '${OUTFILE}'

!ifdef APPSING
  !echo "Outer invocation"

  ; Call makensis again, defining INNER.  This writes an installer for us which, when
  ; it is invoked, will just write the uninstaller to some location, and then exit.
  ; Be sure to substitute the name of this script here.
 
  !system "$\"${NSISDIR}\makensis$\" /DINNER installer.nsi" = 0
 
  ; So now run that installer we just created as %TEMP%\tempinstaller.exe.  Since it
  ; calls quit the return value isn't zero.
 
  !system "$%TEMP%\tempinstaller.exe" = 2
 
  ; That will have written an uninstaller binary for us.  Now we sign it with your
  ; favourite code signing tool.

  !system 'sign.bat "certs/${APPNAME}.pfx" "${APPSING}" "$%TEMP%\Uninstall.exe"' = 0
  !system 'sign.bat "certs/${APPNAME}.pfx" "${APPSING}" "${EXEFILE}"' = 0
 ; !finalize 'sign.bat "certs/${APPNAME}.pfx" "${APPSING}" "${OUTFILE}"' = 0
  !appendfile "sign_${APPNAME}.bat" 'sign.bat "certs/${APPNAME}.pfx" "${APPSING}" "${OUTFILE}"'

!endif
 
!endif

;--------------------------------
;Installer Sections

Section "${APPNAME} (required)" SecInstall

  SectionIn RO

  Push "${APPNAME}"
  Call .closeProgram

  SetOutPath "$INSTDIR"

; for versions <= 3.9.7
  SetShellVarContext current
  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
  RMDir /r /REBOOTOK "$SMPROGRAMS\$MUI_TEMP"

  IfFileExists "${APPNAME}.ini" 0 SkipINI
  ${FileCopy} "${APPNAME}.ini" "$APPDATA\${APPNAME}\"
  Delete "${APPNAME}.ini"
SkipINI:
  IfFileExists "Contacts.xml" 0 SkipContacts
  ${FileCopy} "Contacts.xml" "$APPDATA\${APPNAME}\"
  Delete "Contacts.xml"
SkipContacts:

; Setup predefined ini file
  IfFileExists "$APPDATA\${APPNAME}\${APPNAME}.ini" SkipPredefinedINI 0
  IfFileExists "$EXEDIR\${APPNAME}.ini" 0 SkipPredefinedINI
  ${FileCopy} "$EXEDIR\${APPNAME}.ini" "$APPDATA\${APPNAME}\"
SkipPredefinedINI:

!ifdef CUSTOM
  CreateDirectory "$APPDATA\${APPNAME}"
  File /nonfatal "/oname=$APPDATA\${APPNAME}\${APPNAME}.ini" "custom\${APPNAME}.ini"
!endif


!ifdef WRITEINIFILE

StrCpy $2 ""

${GetParameters} $1
${GetOptions} $1 "/profile" $1
IfErrors +2 0
StrCpy $2 "$2profile=$1$\r$\n"

${GetParameters} $1
${GetOptions} $1 "/username" $1
IfErrors +2 0
StrCpy $2 "$2username=$1$\r$\n"

${GetParameters} $1
${GetOptions} $1 "/password" $1
IfErrors +2 0
StrCpy $2 "$2password=$1$\r$\n"

${GetParameters} $1
${GetOptions} $1 "/domain" $1
IfErrors +2 0
StrCpy $2 "$2domain=$1$\r$\n"

${GetParameters} $1
${GetOptions} $1 "/port" $1
IfErrors +2 0
StrCpy $2 "$2port=$1$\r$\n"

${GetParameters} $1
${GetOptions} $1 "/transport" $1
IfErrors +2 0
StrCpy $2 "$2transport=$1$\r$\n"

StrCmp $2 "" SkipIniWrite
CreateDirectory "$APPDATA\${APPNAME}"
FileOpen $3 "$APPDATA\${APPNAME}\${APPNAME}.ini" w
FileWrite $3 "[Settings]$\r$\naccountId=1$\r$\n[Account1]$\r$\n$2"
FileClose $3
SkipIniWrite:

!endif

  !insertmacro RemoveLinksAssociation

  ;Delete desktop link
  Delete "$DESKTOP\${APPLINKNAME}.lnk"

  ;Delete startup link
  Delete "$SMSTARTUP\${APPLINKNAME}.lnk"

  SetShellVarContext all
  
  ;ADD YOUR OWN FILES HERE...
  
  File "${EXEFILE}"
!ifdef APPLITE
  Delete /REBOOTOK "$INSTDIR\*.dll"	
!else
  File "full\*.dll"
!endif
!ifdef CUSTOM_DLL
  File "dlls\${CUSTOM_DLL}"
!endif
  File full\ringin.wav
  File full\ringin2.wav
  File full\ringout.wav
  File full\hangup.wav
  File full\messageout.wav
  File full\messagein.wav
  File full\License.txt
!ifndef CUSTOM
  File "full\${APPNAME} Website.url"
!endif

!ifdef APPCOPYFILE
  File "${APPCOPYFILE}"
!endif

Delete "$INSTDIR\langpack_*"

!ifndef CUSTOM
File "${PREFIX}langpack_english.txt.sample"
!endif

!ifndef APPLANG_EMBED
  
IntCmpU $LANGUAGE 1078 0 +2 +2
File "${PREFIX}langpack_afrikaans.txt"
IntCmpU $LANGUAGE 1052 0 +2 +2
File "${PREFIX}langpack_albanian.txt"
IntCmpU $LANGUAGE 1025 0 +2 +2
File "${PREFIX}langpack_arabic.txt"
IntCmpU $LANGUAGE 1067 0 +2 +2
File "${PREFIX}langpack_armenian.txt"
IntCmpU $LANGUAGE 1069 0 +2 +2
File "${PREFIX}langpack_basque.txt"
IntCmpU $LANGUAGE 1059 0 +2 +2
File "${PREFIX}langpack_belarusian.txt"
IntCmpU $LANGUAGE 5146 0 +2 +2
File "${PREFIX}langpack_bosnian.txt"
IntCmpU $LANGUAGE 1150 0 +2 +2
File "${PREFIX}langpack_breton.txt"
IntCmpU $LANGUAGE 1026 0 +2 +2
File "${PREFIX}langpack_bulgarian.txt"
IntCmpU $LANGUAGE 1027 0 +2 +2
File "${PREFIX}langpack_catalan.txt"
IntCmpU $LANGUAGE 1537 0 +2 +2
File "${PREFIX}langpack_cibemba.txt"
IntCmpU $LANGUAGE 1050 0 +2 +2
File "${PREFIX}langpack_croatian.txt"
IntCmpU $LANGUAGE 1029 0 +2 +2
File "${PREFIX}langpack_czech.txt"
IntCmpU $LANGUAGE 1030 0 +2 +2
File "${PREFIX}langpack_danish.txt"
IntCmpU $LANGUAGE 1043 0 +2 +2
File "${PREFIX}langpack_dutchnetherlands.txt"
IntCmpU $LANGUAGE 1538 0 +2 +2
File "${PREFIX}langpack_efik.txt"
IntCmpU $LANGUAGE 9998 0 +2 +2
File "${PREFIX}langpack_esperanto.txt"
IntCmpU $LANGUAGE 1061 0 +2 +2
File "${PREFIX}langpack_estonian.txt"
IntCmpU $LANGUAGE 1065 0 +2 +2
File "${PREFIX}langpack_farsi.txt"
IntCmpU $LANGUAGE 1035 0 +2 +2
File "${PREFIX}langpack_finnish.txt"
IntCmpU $LANGUAGE 1036 0 +2 +2
File "${PREFIX}langpack_french.txt"
IntCmpU $LANGUAGE 1110 0 +2 +2
File "${PREFIX}langpack_galician.txt"
IntCmpU $LANGUAGE 1079 0 +2 +2
File "${PREFIX}langpack_georgian.txt"
IntCmpU $LANGUAGE 1031 0 +2 +2
File "${PREFIX}langpack_german.txt"
IntCmpU $LANGUAGE 2055 0 +2 +2
File "${PREFIX}langpack_germanswitzerland.txt"
IntCmpU $LANGUAGE 1032 0 +2 +2
File "${PREFIX}langpack_greek.txt"
IntCmpU $LANGUAGE 1037 0 +2 +2
File "${PREFIX}langpack_hebrew.txt"
IntCmpU $LANGUAGE 1038 0 +2 +2
File "${PREFIX}langpack_hungarian.txt"
IntCmpU $LANGUAGE 15 0 +2 +2
File "${PREFIX}langpack_icelandic.txt"
IntCmpU $LANGUAGE 1136 0 +2 +2
File "${PREFIX}langpack_igbo.txt"
IntCmpU $LANGUAGE 1057 0 +2 +2
File "${PREFIX}langpack_indonesian.txt"
IntCmpU $LANGUAGE 2108 0 +2 +2
File "${PREFIX}langpack_irish.txt"
IntCmpU $LANGUAGE 1040 0 +2 +2
File "${PREFIX}langpack_italian.txt"
IntCmpU $LANGUAGE 1041 0 +2 +2
File "${PREFIX}langpack_japanese.txt"
IntCmpU $LANGUAGE 10311 0 +2 +2
File "${PREFIX}langpack_khmer.txt"
IntCmpU $LANGUAGE 1042 0 +2 +2
File "${PREFIX}langpack_korean.txt"
IntCmpU $LANGUAGE 9999 0 +2 +2
File "${PREFIX}langpack_kurdish.txt"
IntCmpU $LANGUAGE 1062 0 +2 +2
File "${PREFIX}langpack_latvian.txt"
IntCmpU $LANGUAGE 1063 0 +2 +2
File "${PREFIX}langpack_lithuanian.txt"
IntCmpU $LANGUAGE 4103 0 +2 +2
File "${PREFIX}langpack_luxembourgish.txt"
IntCmpU $LANGUAGE 1071 0 +2 +2
File "${PREFIX}langpack_macedonian.txt"
IntCmpU $LANGUAGE 1536 0 +2 +2
File "${PREFIX}langpack_malagasy.txt"
IntCmpU $LANGUAGE 1086 0 +2 +2
File "${PREFIX}langpack_malay.txt"
IntCmpU $LANGUAGE 1104 0 +2 +2
File "${PREFIX}langpack_mongolian.txt"
IntCmpU $LANGUAGE 1044 0 +2 +2
File "${PREFIX}langpack_norwegian.txt"
IntCmpU $LANGUAGE 2068 0 +2 +2
File "${PREFIX}langpack_norwegiannynorsk.txt"
IntCmpU $LANGUAGE 1045 0 +2 +2
File "${PREFIX}langpack_polish.txt"
IntCmpU $LANGUAGE 2070 0 +2 +2
File "${PREFIX}langpack_portuguese.txt"
IntCmpU $LANGUAGE 1046 0 +2 +2
File "${PREFIX}langpack_portuguesebr.txt"
IntCmpU $LANGUAGE 1048 0 +2 +2
File "${PREFIX}langpack_romanian.txt"
IntCmpU $LANGUAGE 1049 0 +2 +2
File "${PREFIX}langpack_russian.txt"
IntCmpU $LANGUAGE 3098 0 +2 +2
File "${PREFIX}langpack_serbian.txt"
IntCmpU $LANGUAGE 2074 0 +2 +2
File "${PREFIX}langpack_serbianlatin.txt"
IntCmpU $LANGUAGE 1132 0 +2 +2
File "${PREFIX}langpack_sesotho.txt"
IntCmpU $LANGUAGE 2052 0 +2 +2
File "${PREFIX}langpack_simpchinese.txt"
IntCmpU $LANGUAGE 1051 0 +2 +2
File "${PREFIX}langpack_slovak.txt"
IntCmpU $LANGUAGE 1060 0 +2 +2
File "${PREFIX}langpack_slovenian.txt"
IntCmpU $LANGUAGE 1034 0 +2 +2
File "${PREFIX}langpack_spanish.txt"
IntCmpU $LANGUAGE 3082 0 +2 +2
File "${PREFIX}langpack_spanishinternational.txt"
IntCmpU $LANGUAGE 1089 0 +2 +2
File "${PREFIX}langpack_swahili.txt"
IntCmpU $LANGUAGE 1053 0 +2 +2
File "${PREFIX}langpack_swedish.txt"
IntCmpU $LANGUAGE 1097 0 +2 +2
File "${PREFIX}langpack_tamil.txt"
IntCmpU $LANGUAGE 1054 0 +2 +2
File "${PREFIX}langpack_thai.txt"
IntCmpU $LANGUAGE 1028 0 +2 +2
File "${PREFIX}langpack_tradchinese.txt"
IntCmpU $LANGUAGE 1055 0 +2 +2
File "${PREFIX}langpack_turkish.txt"
IntCmpU $LANGUAGE 1539 0 +2 +2
File "${PREFIX}langpack_twi.txt"
IntCmpU $LANGUAGE 1058 0 +2 +2
File "${PREFIX}langpack_ukrainian.txt"
IntCmpU $LANGUAGE 1152 0 +2 +2
File "${PREFIX}langpack_uyghur.txt"
IntCmpU $LANGUAGE 1091 0 +2 +2
File "${PREFIX}langpack_uzbek.txt"
IntCmpU $LANGUAGE 1066 0 +2 +2
File "${PREFIX}langpack_vietnamese.txt"
IntCmpU $LANGUAGE 1160 0 +2 +2
File "${PREFIX}langpack_welsh.txt"
IntCmpU $LANGUAGE 1130 0 +2 +2
File "${PREFIX}langpack_yoruba.txt"
IntCmpU $LANGUAGE 1077 0 +2 +2
File "${PREFIX}langpack_zulu.txt"

!endif
  
  ;Store installation folder
WriteRegStr HKLM "Software\${APPNAME}" "" $INSTDIR

  ;Register in default applications
; for Vista and later
WriteRegStr HKLM "SOFTWARE\${APPNAME}\Capabilities" "ApplicationDescription" "${APPLINKNAME} - Lightweight Softphone."
WriteRegStr HKLM "SOFTWARE\${APPNAME}\Capabilities" "ApplicationName" "${APPLINKNAME}"
WriteRegStr HKLM "SOFTWARE\${APPNAME}\Capabilities\UrlAssociations" "tel" "${APPNAME}"
WriteRegStr HKLM "SOFTWARE\${APPNAME}\Capabilities\UrlAssociations" "callto" "${APPNAME}"
WriteRegStr HKLM "SOFTWARE\${APPNAME}\Capabilities\UrlAssociations" "sip" "${APPNAME}"

WriteRegStr HKLM "SOFTWARE\Classes\${APPNAME}" "" "Internet Call Protocol"
WriteRegStr HKLM "SOFTWARE\Classes\${APPNAME}\DefaultIcon" "" "$INSTDIR\${APPNAME}.exe,0"
WriteRegStr HKLM "SOFTWARE\Classes\${APPNAME}\shell\open\command" "" "$\"$INSTDIR\${APPNAME}.exe$\" $\"%1$\""

WriteRegStr HKLM "SOFTWARE\RegisteredApplications" "${APPNAME}" "SOFTWARE\${APPNAME}\Capabilities"

; reset flags
  WriteRegDWORD HKLM "Software\${APPNAME}" "DesktopShortcut" "0"
  WriteRegDWORD HKLM "Software\${APPNAME}" "RunAtSystemStartup" "0"
  WriteRegDWORD HKLM "Software\${APPNAME}" "LinksAssociation" "0"


  ;Create uninstaller
!ifndef APPSING
  WriteUninstaller "$INSTDIR\Uninstall.exe"
!else
!ifndef INNER
  SetOutPath $INSTDIR
  ; this packages the signed uninstaller
  File $%TEMP%\Uninstall.exe
!endif
!endif

  ; write uninstall strings
  WriteRegStr HKLM "${ARP}" "DisplayName" "${APPNAME}"
  WriteRegStr HKLM "${ARP}" "DisplayIcon" "$INSTDIR\${APPNAME}.exe,0"
  WriteRegStr HKLM "${ARP}" "Publisher" "${APPCOMPANY}"
  WriteRegStr HKLM "${ARP}" "DisplayVersion" "${APPVERSON}"
  WriteRegDWORD HKLM "${ARP}" "NoModify" "1"
  WriteRegDWORD HKLM "${ARP}" "NoRepair" "1"
  WriteRegStr HKLM "${ARP}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
  
 ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
 IntFmt $0 "0x%08X" $0
 WriteRegDWORD HKLM "${ARP}" "EstimatedSize" "$0"
 
 
 !ifdef DSCP
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS" "Application DSCP Marking Request" "Allowed"

  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Version" "1.0"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Application Name" "${APPNAME}.exe"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Protocol" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Local Port" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Local IP" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Local IP Prefix Length" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Remote Port" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Remote IP" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Remote IP Prefix Length" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "DSCP Value" "51"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\VoIP" "Throttle Rate" "-1"
  
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Version" "1.0"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Application Name" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Protocol" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Local Port" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Local IP" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Local IP Prefix Length" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Remote Port" "*"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Remote IP" "85.119.188.0"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Remote IP Prefix Length" "24"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "DSCP Value" "51"
  WriteRegStr HKLM "Software\Policies\Microsoft\Windows\QoS\WebPhone" "Throttle Rate" "-1"

  !endif

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
!ifdef APPMINIMIZED
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\${APPLINKNAME}.lnk" "$INSTDIR\${APPNAME}.exe" /minimized
!else
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\${APPLINKNAME}.lnk" "$INSTDIR\${APPNAME}.exe"
!endif    
!ifndef APPMENULITE
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\License.lnk" "$INSTDIR\License.txt"
!endif    
!ifndef CUSTOM    
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\${APPNAME} Website.lnk" "$INSTDIR\${APPNAME} Website.url"
!endif    
!ifndef APPMENULITE
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
!endif    
  !insertmacro MUI_STARTMENU_WRITE_END
  
SectionEnd

!ifdef NODESKTOP
Section /o "Desktop Shortcut" SecDesktopShortCut
!else
Section "Desktop Shortcut" SecDesktopShortCut
!endif

  WriteRegDWORD HKLM "Software\${APPNAME}" "DesktopShortcut" "1"

  SetShellVarContext current
!ifdef APPMINIMIZED
    CreateShortCut "$DESKTOP\${APPLINKNAME}.lnk" "$INSTDIR\${APPNAME}.exe" /minimized
!else
    CreateShortCut "$DESKTOP\${APPLINKNAME}.lnk" "$INSTDIR\${APPNAME}.exe"
!endif   

  SetShellVarContext all
  Delete "$DESKTOP\${APPLINKNAME}.lnk"

SectionEnd

!ifndef AUTORUN
!ifndef CUSTOM
!define AUTORUN
!endif
!endif  

!ifdef AUTORUN
Section "Run at System Startup" SecStartupShortCut
!else
Section /o "Run at System Startup" SecStartupShortCut
!endif  

  WriteRegDWORD HKLM "Software\${APPNAME}" "RunAtSystemStartup" "1"

  SetShellVarContext current
  CreateShortCut "$SMSTARTUP\${APPLINKNAME}.lnk" "$INSTDIR\${APPNAME}.exe" /minimized

  SetShellVarContext all
  Delete "$SMSTARTUP\${APPLINKNAME}.lnk"

SectionEnd

Section "Links association" SecAssociate

  WriteRegDWORD HKLM "Software\${APPNAME}" "LinksAssociation" "1"

; for WinXP and Chrome?
DeleteRegKey HKCU "SOFTWARE\Classes\sip"
DeleteRegKey HKCU "SOFTWARE\Classes\tel"
DeleteRegKey HKCU "SOFTWARE\Classes\callto"
DeleteRegKey HKLM "SOFTWARE\Classes\sip"
DeleteRegKey HKLM "SOFTWARE\Classes\tel"
DeleteRegKey HKLM "SOFTWARE\Classes\callto"

WriteRegStr HKLM "SOFTWARE\Classes\tel" "" "Internet Call Protocol"
WriteRegStr HKLM "SOFTWARE\Classes\tel" "URL Protocol" ""
WriteRegStr HKLM "SOFTWARE\Classes\tel" "Owner Name" "${APPNAME}"
WriteRegStr HKLM "SOFTWARE\Classes\tel\DefaultIcon" "" "$INSTDIR\${APPNAME}.exe,0"
WriteRegStr HKLM "SOFTWARE\Classes\tel\shell\open\command" "" "$\"$INSTDIR\${APPNAME}.exe$\" $\"%1$\""

WriteRegStr HKLM "SOFTWARE\Classes\callto" "" "Internet Call Protocol"
WriteRegStr HKLM "SOFTWARE\Classes\callto" "URL Protocol" ""
WriteRegStr HKLM "SOFTWARE\Classes\callto" "Owner Name" "${APPNAME}"
WriteRegStr HKLM "SOFTWARE\Classes\callto\DefaultIcon" "" "$INSTDIR\${APPNAME}.exe,0"
WriteRegStr HKLM "SOFTWARE\Classes\callto\shell\open\command" "" "$\"$INSTDIR\${APPNAME}.exe$\" $\"%1$\""

WriteRegStr HKLM "SOFTWARE\Classes\sip" "" "Internet Call Protocol"
WriteRegStr HKLM "SOFTWARE\Classes\sip" "URL Protocol" ""
WriteRegStr HKLM "SOFTWARE\Classes\sip" "Owner Name" "${APPNAME}"
WriteRegStr HKLM "SOFTWARE\Classes\sip\DefaultIcon" "" "$INSTDIR\${APPNAME}.exe,0"
WriteRegStr HKLM "SOFTWARE\Classes\sip\shell\open\command" "" "$\"$INSTDIR\${APPNAME}.exe$\" $\"%1$\""

; for Vista and later
WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\Shell\Associations\UrlAssociations\tel\UserChoice" "Progid" "${APPNAME}"
WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\Shell\Associations\UrlAssociations\callto\UserChoice" "Progid" "${APPNAME}"
WriteRegStr HKCU "SOFTWARE\Microsoft\Windows\Shell\Associations\UrlAssociations\sip\UserChoice" "Progid" "${APPNAME}"

SectionEnd

;--------------------------------
;Installer Functions

Function .onInit

!ifdef INNER
  ; If INNER is defined, then we aren't supposed to do anything except write out
  ; the installer.  This is better than processing a command line option as it means
  ; this entire code path is not present in the final (real) installer.
 
  WriteUninstaller "$%TEMP%\Uninstall.exe"
  Quit  ; just bail out quickly when running the "inner" installer
!endif

  !insertmacro MUI_LANGDLL_DISPLAY

  ReadRegDWORD $0 HKLM "Software\${APPNAME}" "DesktopShortcut"
  StrCmp $0 "1" 0 +2
  SectionSetFlags ${SecDesktopShortCut} ${SF_SELECTED}
  StrCmp $0 "0" 0 +2
  SectionSetFlags ${SecDesktopShortCut} 0

  ReadRegDWORD $0 HKLM "Software\${APPNAME}" "RunAtSystemStartup"
  StrCmp $0 "1" 0 +2
  SectionSetFlags ${SecStartupShortCut} ${SF_SELECTED}
  StrCmp $0 "0" 0 +2
  SectionSetFlags ${SecStartupShortCut} 0

  ReadRegDWORD $0 HKLM "Software\${APPNAME}" "LinksAssociation"
  StrCmp $0 "1" 0 +2
  SectionSetFlags ${SecAssociate} ${SF_SELECTED}
  StrCmp $0 "0" 0 +2
  SectionSetFlags ${SecAssociate} 0

FunctionEnd

Function .onInstSuccess
${If} ${Silent}
  Call LaunchApp
${EndIf}
FunctionEnd

Function .closeProgram
  Exch $1
  Push $0
  loop:
    FindWindow $0 $1
    IntCmp $0 0 done
      #SendMessage $0 ${WM_DESTROY} 0 0
      SendMessage $0 ${WM_CLOSE} 0 0
    Sleep 100 
    Goto loop 
  done: 
  Pop $0 
  Pop $1
FunctionEnd

Function LaunchApp
;  ExecShell "" "$INSTDIR\${APPNAME}.exe"
;  Exec '"$WINDIR\explorer.exe" "$INSTDIR\${APPNAME}.exe"'
  Exec '"$WINDIR\explorer.exe" "$SMPROGRAMS\$STARTMENU_FOLDER\${APPLINKNAME}.lnk"'
FunctionEnd

;--------------------------------
;Uninstaller Section

Section "un.${APPNAME}" UninstallSec

  Push "${APPNAME}"
  Call un.closeProgram

  SetShellVarContext current
  Delete "$DESKTOP\${APPLINKNAME}.lnk"
  Delete "$SMSTARTUP\${APPLINKNAME}.lnk"

  SetShellVarContext all

  DeleteRegKey HKLM "${ARP}"

  ;ADD YOUR OWN FILES HERE...

  Delete /REBOOTOK "$INSTDIR\ringin.wav"
  Delete /REBOOTOK "$INSTDIR\ringin2.wav"
  Delete /REBOOTOK "$INSTDIR\ringout.wav"
  Delete /REBOOTOK "$INSTDIR\hangup.wav"
  Delete /REBOOTOK "$INSTDIR\messageout.wav"
  Delete /REBOOTOK "$INSTDIR\messagein.wav"
  Delete /REBOOTOK "$INSTDIR\License.txt"
  Delete /REBOOTOK "$INSTDIR\${APPNAME}.exe"
  Delete /REBOOTOK "$INSTDIR\*.dll"
  Delete /REBOOTOK "$INSTDIR\${APPNAME}.log"
  Delete /REBOOTOK "$INSTDIR\${APPNAME} Website.url"
  Delete /REBOOTOK "$INSTDIR\langpack_*"
  Delete /REBOOTOK "$INSTDIR\Uninstall.exe"

  Delete "$DESKTOP\${APPLINKNAME}.lnk"
  Delete "$SMSTARTUP\${APPLINKNAME}.lnk"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
  RMDir /r /REBOOTOK "$SMPROGRAMS\$MUI_TEMP"

  RMDir /r /REBOOTOK "$LOCALAPPDATA\${APPNAME}"

  DeleteRegValue HKLM "SOFTWARE\RegisteredApplications" "${APPNAME}"
  DeleteRegKey HKLM "SOFTWARE\Classes\${APPNAME}"
  DeleteRegKey HKLM "SOFTWARE\${APPNAME}"
  DeleteRegKey HKCU "SOFTWARE\${APPNAME}"

  !insertmacro RemoveLinksAssociation

SectionEnd

Section /o "un.Configuration" UnSecConfig

  SetShellVarContext current
  RMDir /r /REBOOTOK "$APPDATA\${APPNAME}"
  SetShellVarContext all
  RMDir /r /REBOOTOK "$APPDATA\${APPNAME}"
  RMDir /r /REBOOTOK "$INSTDIR"
   
SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd

Function un.closeProgram
  Exch $1
  Push $0
  loop:
    FindWindow $0 $1
    IntCmp $0 0 done
      #SendMessage $0 ${WM_DESTROY} 0 0
      SendMessage $0 ${WM_CLOSE} 0 0
    Sleep 100 
    Goto loop 
  done: 
  Pop $0 
  Pop $1
FunctionEnd

;--------------------------------
;Descriptions

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecInstall} "${APPNAME}"
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktopShortCut} "Create Desktop Shortcut"
    !insertmacro MUI_DESCRIPTION_TEXT ${SecStartupShortCut} "Create Startup Shortcut"
    !insertmacro MUI_DESCRIPTION_TEXT ${SecAssociate} "Associate with the links tel: callto: and sip:"
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
