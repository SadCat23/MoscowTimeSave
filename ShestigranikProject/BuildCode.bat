@ECHO OFF
set UnrealVersion=5.1
set ProjectName=ShestigranikProject
set KEY_NAME=HKLM\Software\EpicGames\Unreal Engine\%UnrealVersion%
set VALUE_NAME=InstalledDirectory
REM GET THE FOLDER WHERE UE WAS INSTALLED
FOR /F "tokens=2*" %%A IN ('REG.exe query "%KEY_NAME%" /v "%VALUE_NAME%" 2^>nul') DO (set pInstallDir=%%B)
IF NOT DEFINED pInstallDir (
	echo %mERROR%%cRED%Unreal Engine Version %UnrealVersion% Not Installed or registry not set... %cReset%
	echo %cSoft%Script will now quit.%cReset%
	echo.
	SET EPICERROR=1
	goto exit
)
echo %mINFO%[1mUnreal Engine Version %UnrealVersion% is installed in: %pInstallDir%
set PathToEngine=%pInstallDir%
%PathToEngine%/Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.exe  -projectfiles -project="%~dp0\%ProjectName%.uproject" -game -rocket -progress 
%PathToEngine%/Engine/Build/BatchFiles/RunUAT.bat BuildEditor -project="%~dp0\%ProjectName%.uproject" -notools