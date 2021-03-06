; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "MJ Music Player"
!define PRODUCT_VERSION "2.0"
!define PRODUCT_PUBLISHER "Fant"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\MJ.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "D:\Qt_Projects\MJ1\music.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "D:\Qt_Projects\MJ1\ReadMe.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\MJ.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "SimpChinese"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "$PROGRAMFILES\MJ Music Player"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "MainSection" SEC01
  SetOutPath "$INSTDIR\audio"
  SetOverwrite try
  File "D:\Qt_Projects\MJ1\audio\qtaudio_windows.dll"
  SetOutPath "$INSTDIR\bearer"
  File "D:\Qt_Projects\MJ1\bearer\qgenericbearer.dll"
  File "D:\Qt_Projects\MJ1\bearer\qnativewifibearer.dll"
  SetOutPath "$INSTDIR"
  File "D:\Qt_Projects\MJ1\D3Dcompiler_47.dll"
  SetOutPath "$INSTDIR\iconengines"
  File "D:\Qt_Projects\MJ1\iconengines\qsvgicon.dll"
  SetOutPath "$INSTDIR\imageformats"
  File "D:\Qt_Projects\MJ1\imageformats\qdds.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qgif.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qicns.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qico.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qjp2.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qjpeg.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qmng.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qsvg.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qtga.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qtiff.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qwbmp.dll"
  File "D:\Qt_Projects\MJ1\imageformats\qwebp.dll"
  SetOutPath "$INSTDIR"
  File "D:\Qt_Projects\MJ1\libeay32.dll"
  File "D:\Qt_Projects\MJ1\libEGL.dll"
  File "D:\Qt_Projects\MJ1\libGLESV2.dll"
  SetOutPath "$INSTDIR\mediaservice"
  File "D:\Qt_Projects\MJ1\mediaservice\dsengine.dll"
  File "D:\Qt_Projects\MJ1\mediaservice\qtmedia_audioengine.dll"
  File "D:\Qt_Projects\MJ1\mediaservice\wmfengine.dll"
  SetOutPath "$INSTDIR"
  File "D:\Qt_Projects\MJ1\MJ.exe"
  CreateDirectory "$SMPROGRAMS\MJ Music Player"
  CreateShortCut "$SMPROGRAMS\MJ Music Player\MJ Music Player.lnk" "$INSTDIR\MJ.exe"
  CreateShortCut "$DESKTOP\MJ Music Player.lnk" "$INSTDIR\MJ.exe"
  File "D:\Qt_Projects\MJ1\msvcp120.dll"
  File "D:\Qt_Projects\MJ1\msvcr120.dll"
  File "D:\Qt_Projects\MJ1\music.ico"
  File "D:\Qt_Projects\MJ1\opengl32sw.dll"
  SetOutPath "$INSTDIR\platforms"
  File "D:\Qt_Projects\MJ1\platforms\qwindows.dll"
  SetOutPath "$INSTDIR\playlistformats"
  File "D:\Qt_Projects\MJ1\playlistformats\qtmultimedia_m3u.dll"
  SetOutPath "$INSTDIR"
  File "D:\Qt_Projects\MJ1\Qt5Core.dll"
  File "D:\Qt_Projects\MJ1\Qt5Gui.dll"
  File "D:\Qt_Projects\MJ1\Qt5Multimedia.dll"
  File "D:\Qt_Projects\MJ1\Qt5Network.dll"
  File "D:\Qt_Projects\MJ1\Qt5Svg.dll"
  File "D:\Qt_Projects\MJ1\Qt5Widgets.dll"
  File "D:\Qt_Projects\MJ1\ReadMe.txt"
  File "D:\Qt_Projects\MJ1\ssleay32.dll"
  SetOutPath "$INSTDIR\translations"
  File "D:\Qt_Projects\MJ1\translations\qt_ca.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_cs.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_de.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_en.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_fi.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_fr.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_he.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_hu.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_it.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_ja.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_ko.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_lv.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_ru.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_sk.qm"
  File "D:\Qt_Projects\MJ1\translations\qt_uk.qm"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  CreateShortCut "$SMPROGRAMS\MJ Music Player\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\MJ.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\MJ.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) 已成功地从你的计算机移除。"
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "你确实要完全移除 $(^Name) ，其及所有的组件？" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\translations\qt_uk.qm"
  Delete "$INSTDIR\translations\qt_sk.qm"
  Delete "$INSTDIR\translations\qt_ru.qm"
  Delete "$INSTDIR\translations\qt_lv.qm"
  Delete "$INSTDIR\translations\qt_ko.qm"
  Delete "$INSTDIR\translations\qt_ja.qm"
  Delete "$INSTDIR\translations\qt_it.qm"
  Delete "$INSTDIR\translations\qt_hu.qm"
  Delete "$INSTDIR\translations\qt_he.qm"
  Delete "$INSTDIR\translations\qt_fr.qm"
  Delete "$INSTDIR\translations\qt_fi.qm"
  Delete "$INSTDIR\translations\qt_en.qm"
  Delete "$INSTDIR\translations\qt_de.qm"
  Delete "$INSTDIR\translations\qt_cs.qm"
  Delete "$INSTDIR\translations\qt_ca.qm"
  Delete "$INSTDIR\ssleay32.dll"
  Delete "$INSTDIR\ReadMe.txt"
  Delete "$INSTDIR\Qt5Widgets.dll"
  Delete "$INSTDIR\Qt5Svg.dll"
  Delete "$INSTDIR\Qt5Network.dll"
  Delete "$INSTDIR\Qt5Multimedia.dll"
  Delete "$INSTDIR\Qt5Gui.dll"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\playlistformats\qtmultimedia_m3u.dll"
  Delete "$INSTDIR\platforms\qwindows.dll"
  Delete "$INSTDIR\opengl32sw.dll"
  Delete "$INSTDIR\music.ico"
  Delete "$INSTDIR\msvcr120.dll"
  Delete "$INSTDIR\msvcp120.dll"
  Delete "$INSTDIR\MJ.exe"
  Delete "$INSTDIR\mediaservice\wmfengine.dll"
  Delete "$INSTDIR\mediaservice\qtmedia_audioengine.dll"
  Delete "$INSTDIR\mediaservice\dsengine.dll"
  Delete "$INSTDIR\libGLESV2.dll"
  Delete "$INSTDIR\libEGL.dll"
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\imageformats\qwebp.dll"
  Delete "$INSTDIR\imageformats\qwbmp.dll"
  Delete "$INSTDIR\imageformats\qtiff.dll"
  Delete "$INSTDIR\imageformats\qtga.dll"
  Delete "$INSTDIR\imageformats\qsvg.dll"
  Delete "$INSTDIR\imageformats\qmng.dll"
  Delete "$INSTDIR\imageformats\qjpeg.dll"
  Delete "$INSTDIR\imageformats\qjp2.dll"
  Delete "$INSTDIR\imageformats\qico.dll"
  Delete "$INSTDIR\imageformats\qicns.dll"
  Delete "$INSTDIR\imageformats\qgif.dll"
  Delete "$INSTDIR\imageformats\qdds.dll"
  Delete "$INSTDIR\iconengines\qsvgicon.dll"
  Delete "$INSTDIR\D3Dcompiler_47.dll"
  Delete "$INSTDIR\bearer\qnativewifibearer.dll"
  Delete "$INSTDIR\bearer\qgenericbearer.dll"
  Delete "$INSTDIR\audio\qtaudio_windows.dll"

  Delete "$SMPROGRAMS\MJ Music Player\Uninstall.lnk"
  Delete "$DESKTOP\MJ Music Player.lnk"
  Delete "$SMPROGRAMS\MJ Music Player\MJ Music Player.lnk"

  RMDir "$SMPROGRAMS\MJ Music Player"
  RMDir "$INSTDIR\translations"
  RMDir "$INSTDIR\playlistformats"
  RMDir "$INSTDIR\platforms"
  RMDir "$INSTDIR\mediaservice"
  RMDir "$INSTDIR\imageformats"
  RMDir "$INSTDIR\iconengines"
  RMDir "$INSTDIR\bearer"
  RMDir "$INSTDIR\audio"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd