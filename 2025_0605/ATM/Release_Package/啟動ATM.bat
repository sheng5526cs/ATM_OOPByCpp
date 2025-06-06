@echo off
chcp 65001 >nul
echo.
echo ===================================
echo     ATM 銀行管理系統啟動器
echo ===================================
echo.

REM 檢查必要檔案
if not exist "ATM.exe" (
    echo ❌ 錯誤：找不到 ATM.exe
    echo 請確保所有檔案都在同一資料夾中
    pause
    exit /b 1
)

if not exist "atm.db" (
    echo ❌ 錯誤：找不到 atm.db 資料庫檔案
    echo 請確保資料庫檔案存在
    pause
    exit /b 1
)

if not exist "platforms\qwindows.dll" (
    echo ❌ 錯誤：找不到 platforms\qwindows.dll
    echo 請確保 platforms 資料夾及其內容完整
    pause
    exit /b 1
)

echo ✅ 檔案檢查完成，正在啟動 ATM 系統...
echo.

REM 啟動程式
start "" "ATM.exe"

echo ✅ ATM 系統已啟動！
echo.
echo 如果程式無法正常運行，請檢查：
echo 1. 是否已安裝 Microsoft Visual C++ Redistributable
echo 2. 系統是否支援 Qt 6.9.0
echo 3. 查看 README.txt 獲取更多幫助
echo.

timeout /t 3 >nul
