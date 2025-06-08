# ATM系統完整UML文檔

## 文檔概述

本文檔提供了 ATM (自動櫃員機) 系統的完整 UML 設計文檔，包含詳盡的類圖、架構圖和序列圖。該系統採用 Qt C++ 框架開發，實現了完整的銀行自動化服務功能。

## 📁 文檔結構

### 🎯 **完整系統文檔 (Complete System Documentation)**
```
📋 ATM_Complete_System_UML_ClassDiagram.puml      - 完整系統類圖
🏗️ ATM_Complete_System_Architecture.puml          - 完整系統架構圖  
⚡ ATM_Complete_System_SignalSlot_Sequence.puml   - 完整信號槽序列圖
📖 ATM_Complete_System_Documentation.md           - 完整系統說明文檔
```

### 👨‍💼 **管理員子系統文檔 (Administrator Subsystem)**
```
📋 ATM_Administrator_UML_ClassDiagram.puml        - 管理員類圖
🏗️ ATM_Administrator_Core_Architecture.puml       - 管理員核心架構
⚡ ATM_Administrator_SignalSlot_Sequence.puml     - 管理員信號槽序列圖
```

### 👤 **用戶子系統文檔 (User Subsystem)**
```
📋 ATM_User_UML_ClassDiagram.puml                 - 用戶類圖
🏗️ ATM_User_Core_Architecture.puml                - 用戶核心架構
⚡ ATM_User_SignalSlot_Sequence.puml              - 用戶信號槽序列圖
```

## 🏛️ 系統架構概述

### 📊 **分層架構設計**

#### 1️⃣ **表現層 (Presentation Layer)**
- **認證介面**: `loginWindow` - 統一的登入認證入口
- **管理員介面群組**: 
  - `adminWindow` - 管理員主控台
  - `acclistWindow` - 帳戶列表管理
  - `addaccWindow` - 新增帳戶功能
  - `setpasswordWindow` - 密碼重設功能
  - `setstatusWindow` - 帳戶狀態管理
- **用戶介面群組**:
  - `userwindow` - 用戶主控台
  - `depositWindow` - 存款作業
  - `withdrowWindow` - 提款作業
  - `TransWindow` - 轉帳作業
  - `changepasswordWindow` - 密碼變更
  - `docuWindow` - 交易歷史查詢
  - `summarywindow` - 交易摘要確認

#### 2️⃣ **業務邏輯層 (Business Logic Layer)**
- **核心控制器**: `Controller` - 中央調度與協調
- **管理員業務邏輯**: 帳戶CRUD操作、權限管理
- **用戶業務邏輯**: 交易處理、餘額管理、歷史查詢
- **安全業務邏輯**: 認證、加密、會話管理

#### 3️⃣ **服務層 (Service Layer)**
- **認證服務**: `AuthenticationService`
- **帳戶管理服務**: `AccountService`
- **交易處理服務**: `TransactionService`
- **歷史查詢服務**: `HistoryService`
- **安全管理服務**: `SecurityService`

#### 4️⃣ **資料存取層 (Data Access Layer)**
- **資料庫管理器**: `DatabaseManager`
- **資料存取模式**: Repository Pattern
  - `AccountRepository` - 帳戶資料存取
  - `TransactionRepository` - 交易資料存取
  - `UserRepository` - 用戶資料存取

#### 5️⃣ **資料模型層 (Data Model Layer)**
- **核心資料模型**:
  - `AccountInfo` - 帳戶資訊模型
  - `TransactionInfo` - 交易資訊模型
  - `UserSession` - 用戶會話模型
  - `SystemConfig` - 系統配置模型

#### 6️⃣ **持久化層 (Persistence Layer)**
- **SQLite 資料庫**:
  - `accounts` 表 - 帳戶基本資訊
  - `transactions` 表 - 交易記錄
  - `user_sessions` 表 - 會話管理
  - `system_logs` 表 - 系統日誌

## 🔧 核心功能模組

### 👨‍💼 **管理員功能模組**

#### 🔐 **帳戶管理**
- **新增帳戶**: 建立新的用戶帳戶，設定初始餘額
- **帳戶列表**: 查看所有帳戶資訊，支援排序與篩選
- **帳戶刪除**: 安全刪除不活躍帳戶
- **狀態管理**: 啟用/暫停/凍結帳戶狀態

#### 🔑 **安全管理**
- **密碼重設**: 為用戶重設新密碼
- **權限控制**: 管理員權限驗證 (status 500/501)
- **操作日誌**: 記錄所有管理員操作
- **系統監控**: 監控系統運行狀態

### 👤 **用戶功能模組**

#### 💰 **金融交易**
- **存款服務**: 
  - 快速金額選擇 (100, 500, 1000, 5000)
  - 自訂金額輸入
  - 即時餘額更新
- **提款服務**:
  - 預設金額選擇
  - 餘額充足性檢查
  - 提款限額控制
- **轉帳服務**:
  - 目標帳戶驗證
  - 雙方帳戶原子性更新
  - 轉帳限額管理

#### 📊 **帳戶服務**
- **交易歷史**: 顯示最近5筆交易記錄
- **餘額查詢**: 即時餘額顯示
- **密碼變更**: 安全的密碼更新機制
- **交易摘要**: 詳細的交易確認與結果

## 🔒 安全機制

### 🛡️ **身份認證**
- **多重驗證**: 帳號密碼雙重驗證
- **角色區分**: 管理員 (500/501) vs 用戶 (200/201)
- **加密保護**: SHA-256 密碼加密存儲
- **登入限制**: 失敗次數限制與帳戶鎖定

### ⏰ **會話管理**
- **90秒超時**: 自動會話過期機制
- **活動追蹤**: 用戶操作活動監控
- **超時警告**: 提前警告機制
- **強制登出**: 超時自動清理會話

### 🔐 **資料保護**
- **交易完整性**: ACID 事務保證
- **資料加密**: 敏感資料加密存儲
- **存取控制**: 基於角色的存取控制
- **審計追蹤**: 完整的操作日誌記錄

## ⚡ 信號槽架構

### 🎯 **事件驅動設計**
- **統一信號槽**: 所有用戶交互通過 Qt 信號槽機制
- **鬆散耦合**: 視窗間通過 Controller 間接通信
- **非同步處理**: 非阻塞的事件處理機制
- **錯誤處理**: 統一的異常處理與錯誤回報

### 🔄 **信號槽流程**
1. **用戶操作** → **視窗信號**
2. **視窗信號** → **Controller 槽**
3. **Controller** → **業務邏輯處理**
4. **業務邏輯** → **資料庫操作**
5. **結果返回** → **視窗更新**
6. **用戶反馈** → **操作完成**

## 🗄️ 資料庫設計

### 📋 **資料表結構**

#### 👥 **accounts 表**
```sql
CREATE TABLE accounts (
    account_id VARCHAR(20) PRIMARY KEY,
    password_hash VARCHAR(64) NOT NULL,
    balance DECIMAL(15,2) DEFAULT 0.00,
    status INTEGER DEFAULT 200,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);
```

#### 💳 **transactions 表**
```sql
CREATE TABLE transactions (
    transaction_id VARCHAR(36) PRIMARY KEY,
    account_id VARCHAR(20),
    transaction_type VARCHAR(10),
    amount DECIMAL(15,2),
    balance_after DECIMAL(15,2),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    description TEXT,
    FOREIGN KEY (account_id) REFERENCES accounts(account_id)
);
```

#### 🕒 **user_sessions 表**
```sql
CREATE TABLE user_sessions (
    session_id VARCHAR(36) PRIMARY KEY,
    account_id VARCHAR(20),
    login_time DATETIME,
    last_activity DATETIME,
    is_admin BOOLEAN DEFAULT FALSE,
    is_active BOOLEAN DEFAULT TRUE,
    FOREIGN KEY (account_id) REFERENCES accounts(account_id)
);
```

#### 📝 **system_logs 表**
```sql
CREATE TABLE system_logs (
    log_id INTEGER PRIMARY KEY AUTOINCREMENT,
    account_id VARCHAR(20),
    action VARCHAR(50),
    ip_address VARCHAR(15),
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    details TEXT
);
```

## 🎨 設計模式

### 🏗️ **架構模式**
- **MVC (Model-View-Controller)**: 清晰的職責分離
- **Repository Pattern**: 統一的資料存取介面
- **Singleton Pattern**: DatabaseManager 單例管理
- **Observer Pattern**: Qt 信號槽機制
- **Factory Pattern**: 視窗物件動態建立

### 🔧 **設計原則**
- **單一職責原則**: 每個類別職責明確
- **開放封閉原則**: 對擴展開放，對修改封閉
- **依賴反轉原則**: 高層模組不依賴低層模組
- **介面隔離原則**: 客戶端不依賴不需要的介面
- **里氏替換原則**: 子類別可以替換父類別

## 🚀 技術特性

### 💻 **開發技術**
- **程式語言**: C++17
- **GUI 框架**: Qt 6.9.0
- **資料庫**: SQLite 3
- **建構工具**: Qt Creator + MinGW
- **版本控制**: Git

### 📦 **核心組件**
- **QtWidgets**: 圖形用戶介面
- **QtSql**: 資料庫連接與操作
- **QtCore**: 核心功能與信號槽
- **QtCrypto**: 密碼加密功能

### 🎯 **效能特性**
- **連接池管理**: 最佳化資料庫連接
- **記憶體管理**: 智慧指標與RAII
- **非同步處理**: 非阻塞用戶介面
- **快取機制**: 減少資料庫查詢次數

## 📋 使用說明

### 🔐 **管理員操作**
1. 使用管理員帳號登入 (status 500/501)
2. 選擇所需的管理功能
3. 執行帳戶管理操作
4. 查看操作結果與日誌
5. 安全登出系統

### 👤 **用戶操作**
1. 使用用戶帳號登入 (status 200/201)
2. 查看帳戶餘額資訊
3. 選擇所需的金融服務
4. 確認交易資訊
5. 完成交易並查看摘要

### ⚠️ **注意事項**
- 會話將在90秒無活動後自動過期
- 所有交易操作均需要確認
- 密碼修改立即生效
- 系統會記錄所有操作日誌

## 🔮 未來擴展

### 📈 **功能擴展**
- **多語言支援**: 國際化介面
- **生物識別**: 指紋/人臉識別
- **行動支付**: QR Code 支付
- **投資理財**: 基金/保險產品

### 🛠️ **技術升級**
- **雲端部署**: 分散式架構
- **微服務**: 服務模組化
- **區塊鏈**: 交易安全增強
- **AI 智能**: 智慧客服助手

---

## 📞 技術支援

如有任何技術問題或改進建議，請聯繫開發團隊。

**文檔版本**: v1.0  
**更新日期**: 2025年6月8日  
**作者**: ATM系統開發團隊
