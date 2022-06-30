[Setup]
AppName=OpenSiv3D(0.6.4) for Web
AppVersion=0.6.4
WizardStyle=modern
SetupIconFile=..\..\WindowsDesktop\App\icon.ico
OutputBaseFilename=OpenSiv3D(0.6.4)Web
DefaultDirName={autopf}\OpenSiv3D(0.6.4)Web
VersionInfoVersion=0.6.4.1
VersionInfoDescription=OpenSiv3D (0.6.4) for Web Installer
Compression=lzma2
SolidCompression=yes
ChangesEnvironment=yes
PrivilegesRequired=lowest
OutputDir=userdocs:Inno Setup Examples Output

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "ja"; MessagesFile: "compiler:Languages\Japanese.isl"

[Files]
Source: "..\..\Siv3D\include\*"; DestDir: "{app}\include"; Flags: recursesubdirs
Source: "..\Build\libSiv3D.a"; DestDir: "{app}\lib"
Source: "..\Build\libSiv3DScript.a"; DestDir: "{app}\lib"
Source: "..\Siv3D.js"; DestDir: "{app}\lib"
Source: "..\Siv3D.pre.js"; DestDir: "{app}\lib"
Source: "..\Siv3D.post.js"; DestDir: "{app}\lib"
Source: "..\..\Siv3D\lib\Web\wasm\*"; DestDir: "{app}\lib"; Flags: recursesubdirs
Source: "..\App\resources\*"; DestDir: "{app}\resources"; Flags: recursesubdirs
Source: "..\App\example\*"; DestDir: "{app}\example"; Flags: recursesubdirs
Source: "..\VisualStudioTemplate\OpenSiv3D(0.6.4)Web.zip"; DestDir: "{userdocs}\Visual Studio 2017\Templates\ProjectTemplates"
Source: "..\VisualStudioTemplate\OpenSiv3D(0.6.4)Web.zip"; DestDir: "{userdocs}\Visual Studio 2019\Templates\ProjectTemplates"
Source: "..\VisualStudioTemplate\OpenSiv3D(0.6.4)Web.zip"; DestDir: "{userdocs}\Visual Studio 2022\Templates\ProjectTemplates"

[Registry]
Root: HKCU; Subkey: "Environment"; ValueType:string; ValueName: "SIV3D_0_6_4_WEB"; \
    ValueData: "{app}"; Flags: preservestringtype
