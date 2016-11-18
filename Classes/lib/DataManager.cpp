//
//  DataManager.cpp
//  Pointcast-Mobile-App
//
//  Created by 岡田 光央 on 2016/11/18.
//
//

#include "DataManager.hpp"
#include "entity/chartItemEntity.hpp"


namespace lib {
    
    DataManager* DataManager::mManager = NULL;

    DataManager::DataManager()
    {
        
    }
    
    DataManager* DataManager::getInstance()
    {
        if(mManager == NULL)
        {
            mManager = new DataManager();
        }
        
        return mManager;
    }
    
    // DBをオープン
    sqlite3* DataManager::openDB()
    {
        // DBファイルを開いた時間を記憶しておく（処理時間計測のため）
        _dbOpenTime = std::chrono::system_clock::now();
        
        // SQLiteから読込
        std::string dbPath = cocos2d::FileUtils::getInstance()->getWritablePath() + DB_FILE_NAME;
        
        sqlite3 *db = nullptr;
        // DBファイルオープン
        auto status = sqlite3_open(dbPath.c_str(), &db);
        if(status != SQLITE_OK)
        {
            CCLOG("▼sqlite3_open failed.");
            return nullptr;
        }
        // 暗号化する
        /*
        status = sqlite3_key(db, ENCRYPT_DB_PASSWORD, (int)strlen(ENCRYPT_DB_PASSWORD));
        if(status != SQLITE_OK)
        {
            CCLOG("▼sqlite3_key failed.");
            return nullptr;
        }
        */
        
        CCLOG("○DB opened successfully. File : %s",dbPath.c_str());
        
        return db;
    }
    // DBをオープンしてトランザクションをスタートする
    sqlite3* DataManager::openDBAndStartTransaction()
    {
        sqlite3 *db = this->openDB();
        
        if(db)
        {
            // トランザクション開始
            auto status = sqlite3_exec(db, "BEGIN;", nullptr, nullptr, nullptr);
            if(status != SQLITE_OK)
            {
                CCLOG("▼Starting transaction failed.");
                return nullptr;
            }
        }
        
        return db;
    }
    // DBをクローズ
    bool DataManager::closeDB(sqlite3 *db)
    {
        auto status = sqlite3_close(db);
        if(status != SQLITE_OK)
        {
            CCLOG("▼Closing DB failed.");
            return false;
        }
        
        auto duration = std::chrono::system_clock::now() - _dbOpenTime;
        CCLOG("○DB Closed. time : %dms.",(int)std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
        
        return true;
    }
    // トランザクションをコミットしてDBをクローズする
    bool DataManager::commitTransactionAndCloseDB(sqlite3 *db)
    {
        // トランザクションコミット
        auto status = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        if(status != SQLITE_OK)
        {
            CCLOG("▼Commiting transaction failed.");
            return false;
        }
        
        return this->closeDB(db);
    }
    
    // テーブルを作成する
    void DataManager::createTable()
    {
        // エラーメッセージ格納用
        char* errorMessage = NULL;
        
        sqlite3 *db = this->openDB();	// トランザクションなし接続にする
        int status;
        
        // SQLファイル読み込み
        std::string createDBSQL = cocos2d::FileUtils::getInstance()->getStringFromFile("res/create_table.sql");
        // テーブル作成
        status = sqlite3_exec(db,createDBSQL.c_str(),nullptr,nullptr,&errorMessage);
        if(status != SQLITE_OK)
        {
            cocos2d::log("▼Creating table failed. Message : %s",errorMessage);
            CCASSERT(false, errorMessage);
        }
        
        // DBファイルクローズ
        this->closeDB(db);
    }
    
    // データを登録もしくは更新する
    void DataManager::insertOrUpdateUnitData(int id, std::string name, float weight)
    {
        sqlite3 *db = this->openDBAndStartTransaction();
        int status = 0;
        // エラーメッセージ格納用
        char* errorMessage = NULL;
        
        // InsertSQL
        auto insertSQL = sqlite3_mprintf("insert or replace into unit_data(id,name,weight) values(%d,%Q,%f)",
                                         id,name.c_str(),weight
                                         );
        status = sqlite3_exec(db, insertSQL, nullptr, nullptr, &errorMessage);
        if(status != SQLITE_OK)
        {
            CCLOG("▼Inserting UnitData data failed. Message : %s",errorMessage);
            CCASSERT(false, errorMessage);
        }
        else
        {
            CCLOG("○UnitData data successfully updated. no : %d",id);
        }
        
        // 解放（忘れてはいけない）
        sqlite3_free(insertSQL);
        
        this->commitTransactionAndCloseDB(db);
    }
    
    void DataManager::insertChartItem(int device_id, long timestamp, int value)
    {
        sqlite3 *db = this->openDBAndStartTransaction();
        int status = 0;
        // エラーメッセージ格納用
        char* errorMessage = NULL;
        
        // InsertSQL
        auto insertSQL = sqlite3_mprintf("INSERT INTO chart_item(device_id, timestamp,value) values(%d, %ld, %d)",
                                         device_id, timestamp, value
                                         );
        status = sqlite3_exec(db, insertSQL, nullptr, nullptr, &errorMessage);
        if(status != SQLITE_OK)
        {
            CCLOG("▼Inserting UnitData data failed. Message : %s",errorMessage);
            CCASSERT(false, errorMessage);
        }
        else
        {
            CCLOG("○ChartItem data successfully updated. timestamp : %ld", timestamp);
        }
        
        // 解放（忘れてはいけない）
        sqlite3_free(insertSQL);
        
        this->commitTransactionAndCloseDB(db);

    }
    
    
    // データを検索する
    std::vector<cocos2d::Node*> DataManager::selectUnitDataList()
    {
        /*
        vector<cocos2d::Node*> unitDataList;
        
        sqlite3 *db = this->openDB();
        
        // Select
        sqlite3_stmt *stmt = nullptr;
        auto selectSQL = "select id,name,weight from unit_data order by id desc";
        if(sqlite3_prepare_v2(db,selectSQL,-1,&stmt,nullptr) == SQLITE_OK)
        {
            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                UnitData *unitData = UnitData::create();
                unitData->_id = (int)sqlite3_column_int(stmt, 0);
                unitData->_name = StringUtils::format("%s",sqlite3_column_text(stmt, 1));
                unitData->_weight = (float)sqlite3_column_double(stmt, 2);
                
                unitDataList.pushBack(unitData);
            }
        }
        else
        {
            CCASSERT(false,"Select UnitDataList error.");
        }
        
        // Statementをクローズ
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
        
        this->closeDB(db);
        
        return unitDataList;
         */
    }
}
