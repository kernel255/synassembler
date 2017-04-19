; Turn off old selected section
; 22 01 2014: Stefano Mario Chiappa
 
; -------------------------------
; Start
 
;!define MUI_PRODUCT "SynAssembler"
  
!define PRODUCT_NAME "SynAssembler"
!define PRODUCT_VERSION "1.0.0"
!define SETUP_NAME "SynAssemblerSetup.exe"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
  
!define MUI_FILE "SynAssembler"
;!define MUI_VERSION "2.0.7"
!define MUI_BRANDINGTEXT "Pluggable Virtual Synthesizer"
CRCCheck On
 
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
 
;---------------------------------
;General
 
OutFile ${SETUP_NAME}
ShowInstDetails "show"
ShowUninstDetails "show"
;SetCompressor "bzip2"

!define MUI_ICON "SynAssembler.ico"
!define MUI_UNICON "SynAssembler.ico"
!define MUI_SPECIALBITMAP "Bitmap.bmp"
 
 
;--------------------------------
;Folder selection page
 
  InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
 
 
;--------------------------------
;Modern UI Configuration
 
Page directory
Page instfiles

!define MUI_WELCOMEPAGE  
!define MUI_LICENSEPAGE
!define MUI_DIRECTORYPAGE
!define MUI_ABORTWARNING
!define MUI_UNINSTALLER
!define MUI_UNCONFIRMPAGE
!define MUI_FINISHPAGE  
  
;--------------------------------
;Language
 
!insertmacro MUI_LANGUAGE "English"
 
 
;-------------------------------- 
;Modern UI System
 
  ;!insertmacro MUI_SYSTEM 
 
 
;--------------------------------
;Data
 
;LicenseData "ReadMe.txt"
 
 
;-------------------------------- 
;Installer Sections     
Section "install"
; Installation info
 
;Add files
  SetOutPath "$INSTDIR"
 
  File "${PRODUCT_NAME}.exe"
  File "${PRODUCT_NAME}.vshost.exe"
  File "AudioEngine.dll"
  File "GeneralUtils.dll"
  File "BasicAudioControls.dll"
  File "BasicElaborationUnits.dll"
  File "BasicElementsSynthFactory.dll"
  File "ClayAudioEngine.dll"
  File "InputOutput.dll"
  File "InputOutputSynthFactory.dll"
  File "SynthPanel.dll"
  File "Xceed.Wpf.AvalonDock.dll"
  File "Xceed.Wpf.AvalonDock.Themes.Aero.dll"
  File "Xceed.Wpf.AvalonDock.Themes.Metro.dll"
  File "Xceed.Wpf.AvalonDock.Themes.VS2010.dll"
  File "Xceed.Wpf.DataGrid.dll"
  File "Xceed.Wpf.Toolkit.dll"
  File "SynAssembler.application"
  File "SynAssembler.vshost.application"
  File "SynAssembler.exe.config"
  File "SynAssembler.vshost.exe.config"
   
  
  SetOutPath "$INSTDIR\Resources"
  ;file "playlists\${PRODUCT_NAME}.epp"
  ;SetOutPath "$INSTDIR\data"
  ;file "data\*.cst"
  ;file "data\errorlog.txt"
  ; hier komen dan nog de bestanden die in de playlist staan
  SetOutPath "$INSTDIR"  
  ;file /r mpg
  SetOutPath "$INSTDIR"  
  ;file /r xtras  
 
;create desktop shortcut
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_NAME}.exe" ""
 
;create start-menu items
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_NAME}.exe" "" "$INSTDIR\${PRODUCT_NAME}.exe" 0
 
;write uninstall information to the registry
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayName" "${PRODUCT_NAME} (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString" "$INSTDIR\Uninstall.exe"
 
  WriteUninstaller "$INSTDIR\Uninstall.exe"
 
SectionEnd
 
 
;--------------------------------    
;Uninstaller Section  
Section "Uninstall"
 
;Delete Files 
  RMDir /r "$INSTDIR\*.*"    
 
;Remove the installation directory
  RMDir "$INSTDIR"
 
;Delete Start Menu Shortcuts
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\*.*"
  RmDir  "$SMPROGRAMS\${PRODUCT_NAME}"
 
;Delete Uninstaller And Unistall Registry Entries
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${PRODUCT_NAME}"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"  
 
SectionEnd
 
 
;--------------------------------    
;MessageBox Section
 
 
;Function that calls a messagebox when installation finished correctly
Function .onInstSuccess
  MessageBox MB_OK "You have successfully installed ${PRODUCT_NAME}. Use the desktop icon to start the program."
FunctionEnd
 
 
Function un.onUninstSuccess
  MessageBox MB_OK "You have successfully uninstalled ${PRODUCT_NAME}."
FunctionEnd
 
 
;eof