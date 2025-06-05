@echo off
echo ===============================================
echo     ATM 應用程式自動發行腳本
echo ===============================================
echo.

REM 設定變數
set "PROJECT_DIR=%~dp0"
set "BUILD_DIR=%PROJECT_DIR%build\Desktop_Qt_6_9_0_MinGW_64_bit-Debug\debug"
set "RELEASE_DIR=%PROJECT_DIR%Release_Package"
set "EXE_NAME=ATM.exe"

REM 創建發行目錄
if not exist "%RELEASE_DIR%" mkdir "%RELEASE_DIR%"

echo 1. 複製主程式...
if exist "%BUILD_DIR%\%EXE_NAME%" (
    copy "%BUILD_DIR%\%EXE_NAME%" "%RELEASE_DIR%\" >nul
    echo    ✓ 已複製 %EXE_NAME%
) else (
    echo    ✗ 找不到 %EXE_NAME%，請先建置專案
    pause
    exit /b 1
)

echo 2. 複製資料庫檔案...
if exist "%PROJECT_DIR%atm.db" (
    copy "%PROJECT_DIR%atm.db" "%RELEASE_DIR%\" >nul
    echo    ✓ 已複製 atm.db
)

echo 3. 尋找並複製 Qt 執行庫...

REM 常見的 Qt 安裝路徑
set "QT_PATHS=C:\Qt\6.9.0\mingw_64\bin;C:\Qt\Tools\mingw1120_64\bin;%QTDIR%\bin"

REM 尋找 windeployqt.exe
set "WINDEPLOYQT="
for %%p in (%QT_PATHS%) do (
    if exist "%%p\windeployqt.exe" (
        set "WINDEPLOYQT=%%p\windeployqt.exe"
        goto :found_windeployqt
    )
)

REM 如果找不到 windeployqt，手動複製必要的 DLL
echo    找不到 windeployqt.exe，嘗試手動複製 Qt DLL...

REM 尋找 Qt DLL
set "QT_BIN_DIR="
for %%p in (%QT_PATHS%) do (
    if exist "%%p\Qt6Core.dll" (
        set "QT_BIN_DIR=%%p"
        goto :found_qt_bin
    )
)

if "%QT_BIN_DIR%"=="" (
    echo    ✗ 找不到 Qt 安裝路徑，請手動複製以下檔案到 Release_Package 資料夾：
    echo      - Qt6Core.dll
    echo      - Qt6Gui.dll  
    echo      - Qt6Widgets.dll
    echo      - Qt6Sql.dll
    echo      - libgcc_s_seh-1.dll
    echo      - libstdc++-6.dll
    echo      - libwinpthread-1.dll
    goto :manual_copy
)

:found_qt_bin
echo    找到 Qt 執行庫路徑: %QT_BIN_DIR%

REM 複製基本的 Qt DLL
set "QT_DLLS=Qt6Core.dll Qt6Gui.dll Qt6Widgets.dll Qt6Sql.dll"
for %%d in (%QT_DLLS%) do (
    if exist "%QT_BIN_DIR%\%%d" (
        copy "%QT_BIN_DIR%\%%d" "%RELEASE_DIR%\" >nul 2>&1
        echo    ✓ 已複製 %%d
    ) else (
        echo    ✗ 找不到 %%d
    )
)

REM 複製 MinGW 執行庫
set "MINGW_DLLS=libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll"
for %%d in (%MINGW_DLLS%) do (
    if exist "%QT_BIN_DIR%\%%d" (
        copy "%QT_BIN_DIR%\%%d" "%RELEASE_DIR%\" >nul 2>&1
        echo    ✓ 已複製 %%d
    )
)

REM 創建 platforms 目錄並複製平台外掛程式
if not exist "%RELEASE_DIR%\platforms" mkdir "%RELEASE_DIR%\platforms"
if exist "%QT_BIN_DIR%\..\plugins\platforms\qwindows.dll" (
    copy "%QT_BIN_DIR%\..\plugins\platforms\qwindows.dll" "%RELEASE_DIR%\platforms\" >nul 2>&1
    echo    ✓ 已複製 qwindows.dll 到 platforms 資料夾
)

REM 創建 sqldrivers 目錄並複製 SQL 驅動程式
if not exist "%RELEASE_DIR%\sqldrivers" mkdir "%RELEASE_DIR%\sqldrivers"
if exist "%QT_BIN_DIR%\..\plugins\sqldrivers\qsqlite.dll" (
    copy "%QT_BIN_DIR%\..\plugins\sqldrivers\qsqlite.dll" "%RELEASE_DIR%\sqldrivers\" >nul 2>&1
    echo    ✓ 已複製 qsqlite.dll 到 sqldrivers 資料夾
)

goto :end

:found_windeployqt
echo    找到 windeployqt.exe: %WINDEPLOYQT%
echo    使用 windeployqt 自動部署...
"%WINDEPLOYQT%" --release --force "%RELEASE_DIR%\%EXE_NAME%"
echo    ✓ 自動部署完成

goto :end

:manual_copy
echo.
echo 4. 創建使用說明...

:end
echo.
echo 4. 創建使用說明檔案...
(
echo ATM 銀行管理系統
echo ==================
echo.
echo 執行方式：
echo 1. 雙擊 ATM.exe 啟動程式
echo 2. 確保 atm.db 檔案與 ATM.exe 在同一目錄
echo.
echo 系統需求：
echo - Windows 10 或更新版本
echo - 支援中文顯示
echo.
echo 如果程式無法啟動，請確認：
echo 1. 已安裝 Microsoft Visual C++ Redistributable
echo 2. 所有 DLL 檔案都在同一目錄
echo.
echo 開發者：吳維盛
echo 聯絡信箱：B3230665@ulive.pccu.edu.tw
echo.
echo 版本：1.0
echo 日期：%DATE%
) > "%RELEASE_DIR%\README.txt"

echo    ✓ 已創建 README.txt

echo.
echo ===============================================
echo 發行完成！
echo.
echo 發行檔案位置: %RELEASE_DIR%
echo.
echo 您可以將整個 Release_Package 資料夾複製到其他電腦上執行
echo ===============================================
echo.

REM 開啟發行資料夾
explorer "%RELEASE_DIR%"

pause
