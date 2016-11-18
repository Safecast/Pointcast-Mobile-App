//
//  DataManager.hpp
//  Pointcast-Mobile-App
//
//  Created by 岡田 光央 on 2016/11/18.
//
//

#ifndef DataManager_hpp
#define DataManager_hpp

#include <stdio.h>
#include <vector>

#include "cocos2d.h"
#include "sqlite3.h"

#define DB_FILE_NAME "master.db"	// データベースを暗号化する場合のファイル名
#define ENCRYPT_DB_PASSWORD "1234bgaigie"	// データベースを暗号化する場合のパスワード

namespace lib {
    class DataManager : cocos2d::Node{
        // member
        public:
            static DataManager* getInstance();
        private:
            static DataManager* mManager;
            // データベースファイルを開いた時に記憶しておく時間（処理時間計測用）
            std::chrono::system_clock::time_point _dbOpenTime;
        // function
        public:
            DataManager();
            sqlite3* openDB();
            sqlite3* openDBAndStartTransaction();
            bool closeDB(sqlite3 *db);
            bool commitTransactionAndCloseDB(sqlite3 *db);
            void createTable();
            void insertOrUpdateUnitData(int id, std::string name, float weight);
            void insertChartItem(int device_id, long timestamp, int value);
            std::vector<cocos2d::Node*> selectUnitDataList();
        private:
    };
}

#endif /* DataManager_hpp */
