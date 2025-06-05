# ATM 應用程式發行腳本
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host "     ATM 應用程式自動發行腳本" -ForegroundColor Cyan  
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host ""

# 設定路徑變數
$ProjectDir = $PSScriptRoot
$BuildDir = Join-Path $ProjectDir "build\Desktop_Qt_6_9_0_MinGW_64_bit-Debug\debug"
$ReleaseDir = Join-Path $ProjectDir "Release_Package"
$ExeName = "ATM.exe"

# 創建發行目錄
if (!(Test-Path $ReleaseDir)) {
    New-Item -ItemType Directory -Path $ReleaseDir -Force | Out-Null
}

Write-Host "1. 複製主程式..." -ForegroundColor Yellow
$ExePath = Join-Path $BuildDir $ExeName
if (Test-Path $ExePath) {
    Copy-Item $ExePath $ReleaseDir -Force
    Write-Host "   ✓ 已複製 $ExeName" -ForegroundColor Green
} else {
    Write-Host "   ✗ 找不到 $ExeName，請先建置專案" -ForegroundColor Red
    Read-Host "按 Enter 鍵結束"
    exit 1
}

Write-Host "2. 複製資料庫檔案..." -ForegroundColor Yellow
$DbPath = Join-Path $ProjectDir "atm.db"
if (Test-Path $DbPath) {
    Copy-Item $DbPath $ReleaseDir -Force
    Write-Host "   ✓ 已複製 atm.db" -ForegroundColor Green
}

Write-Host "3. 尋找並複製 Qt 執行庫..." -ForegroundColor Yellow

# 常見的 Qt 安裝路徑
$QtPaths = @(
    "C:\Qt\6.9.0\mingw_64\bin",
    "C:\Qt\6.8.0\mingw_64\bin", 
    "C:\Qt\6.7.0\mingw_64\bin",
    "C:\Qt\6.6.0\mingw_64\bin",
    "C:\Qt\6.5.0\mingw_64\bin",
    "C:\Qt\Tools\mingw1120_64\bin",
    "C:\Qt\Tools\mingw_64\bin"
)

# 如果設定了 QTDIR 環境變數
if ($env:QTDIR) {
    $QtPaths += "$env:QTDIR\bin"
}

# 尋找 Qt 安裝路徑
$QtBinDir = $null
foreach ($path in $QtPaths) {
    if (Test-Path "$path\Qt6Core.dll") {
        $QtBinDir = $path
        break
    }
}

if ($QtBinDir) {
    Write-Host "   找到 Qt 執行庫路徑: $QtBinDir" -ForegroundColor Green
    
    # 複製基本的 Qt DLL
    $QtDlls = @("Qt6Core.dll", "Qt6Gui.dll", "Qt6Widgets.dll", "Qt6Sql.dll")
    foreach ($dll in $QtDlls) {
        $dllPath = Join-Path $QtBinDir $dll
        if (Test-Path $dllPath) {
            Copy-Item $dllPath $ReleaseDir -Force
            Write-Host "   ✓ 已複製 $dll" -ForegroundColor Green
        } else {
            Write-Host "   ✗ 找不到 $dll" -ForegroundColor Red
        }
    }
    
    # 複製 MinGW 執行庫
    $MingwDlls = @("libgcc_s_seh-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll")
    foreach ($dll in $MingwDlls) {
        $dllPath = Join-Path $QtBinDir $dll
        if (Test-Path $dllPath) {
            Copy-Item $dllPath $ReleaseDir -Force
            Write-Host "   ✓ 已複製 $dll" -ForegroundColor Green
        }
    }
    
    # 創建並複製 platforms 外掛程式
    $PlatformsDir = Join-Path $ReleaseDir "platforms"
    if (!(Test-Path $PlatformsDir)) {
        New-Item -ItemType Directory -Path $PlatformsDir -Force | Out-Null
    }
    
    $QtDir = Split-Path $QtBinDir -Parent
    $PlatformPlugin = Join-Path $QtDir "plugins\platforms\qwindows.dll"
    if (Test-Path $PlatformPlugin) {
        Copy-Item $PlatformPlugin $PlatformsDir -Force
        Write-Host "   ✓ 已複製 qwindows.dll 到 platforms 資料夾" -ForegroundColor Green
    }
    
    # 創建並複製 SQL 驅動程式
    $SqlDriversDir = Join-Path $ReleaseDir "sqldrivers"
    if (!(Test-Path $SqlDriversDir)) {
        New-Item -ItemType Directory -Path $SqlDriversDir -Force | Out-Null
    }
    
    $SqliteDriver = Join-Path $QtDir "plugins\sqldrivers\qsqlite.dll"
    if (Test-Path $SqliteDriver) {
        Copy-Item $SqliteDriver $SqlDriversDir -Force
        Write-Host "   ✓ 已複製 qsqlite.dll 到 sqldrivers 資料夾" -ForegroundColor Green
    }
    
} else {
    Write-Host "   ✗ 找不到 Qt 安裝路徑" -ForegroundColor Red
    Write-Host "   請手動複製以下檔案到 Release_Package 資料夾：" -ForegroundColor Yellow
    Write-Host "   - Qt6Core.dll" -ForegroundColor Yellow
    Write-Host "   - Qt6Gui.dll" -ForegroundColor Yellow  
    Write-Host "   - Qt6Widgets.dll" -ForegroundColor Yellow
    Write-Host "   - Qt6Sql.dll" -ForegroundColor Yellow
    Write-Host "   - libgcc_s_seh-1.dll" -ForegroundColor Yellow
    Write-Host "   - libstdc++-6.dll" -ForegroundColor Yellow
    Write-Host "   - libwinpthread-1.dll" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "4. 創建使用說明檔案..." -ForegroundColor Yellow

$ReadmeContent = @"
ATM 銀行管理系統
==================

執行方式：
1. 雙擊 ATM.exe 啟動程式
2. 確保 atm.db 檔案與 ATM.exe 在同一目錄

系統需求：
- Windows 10 或更新版本
- 支援中文顯示

如果程式無法啟動，請確認：
1. 已安裝 Microsoft Visual C++ Redistributable
2. 所有 DLL 檔案都在同一目錄

功能說明：
- 使用者登入/登出
- 存款、提款、轉帳功能  
- 交易紀錄查詢
- 密碼變更
- 客服聯繫

開發者：吳維盛
聯絡信箱：B3230665@ulive.pccu.edu.tw
GitHub：github.com/sheng5526cs

版本：1.0
日期：$(Get-Date -Format "yyyy-MM-dd")
"@

$ReadmeContent | Out-File -FilePath (Join-Path $ReleaseDir "README.txt") -Encoding UTF8
Write-Host "   ✓ 已創建 README.txt" -ForegroundColor Green

Write-Host ""
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host "發行完成！" -ForegroundColor Green
Write-Host ""
Write-Host "發行檔案位置: $ReleaseDir" -ForegroundColor Cyan
Write-Host ""
Write-Host "您可以將整個 Release_Package 資料夾複製到其他電腦上執行" -ForegroundColor Yellow
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host ""

# 開啟發行資料夾
Start-Process explorer.exe -ArgumentList $ReleaseDir

Read-Host "按 Enter 鍵結束"
