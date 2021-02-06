[Setup]
AppName=OpenSiv3D(0.4.3) for Web
AppVersion=0.4.3.4
WizardStyle=modern
SetupIconFile=..\..\WindowsDesktop\App\icon.ico
OutputBaseFilename=OpenSiv3D(0.4.3)Web-asmjs
DefaultDirName={autopf}\OpenSiv3D(0.4.3)Web
VersionInfoVersion=0.4.3.4
VersionInfoDescription=OpenSiv3D (0.4.3) for Web Installer
Compression=lzma2
SolidCompression=yes
ChangesEnvironment=yes
OutputDir=userdocs:Inno Setup Examples Output

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "ja"; MessagesFile: "compiler:Languages\Japanese.isl"

[Files]
Source: "..\..\Siv3D\include\*"; DestDir: "{app}\include"; Flags: recursesubdirs
Source: "..\Build\release-asmjs\libSiv3D.a"; DestDir: "{app}\lib"
Source: "..\App\Siv3D.js"; DestDir: "{app}\lib"
Source: "..\..\Siv3D\lib\Web\asmjs\*"; DestDir: "{app}\lib"; Flags: recursesubdirs
Source: "..\App\resources\*"; DestDir: "{app}\resources"; Flags: recursesubdirs
Source: "..\App\example\*"; DestDir: "{app}\example"; Flags: recursesubdirs
Source: "..\VisualStudioTemplate\OpenSiv3D(0.4.3)Web.zip"; DestDir: "{%USERPROFILE|{app}}\Documents\Visual Studio 2019\Templates\ProjectTemplates"

[Registry]
Root: HKCU; Subkey: "Environment"; ValueType:string; ValueName: "SIV3D_0_4_3_WEB"; \
    ValueData: "{app}"; Flags: preservestringtype
