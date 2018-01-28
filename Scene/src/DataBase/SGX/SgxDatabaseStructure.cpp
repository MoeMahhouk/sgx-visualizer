//
// Created by moe on 19.12.17.
//

//#include <assert.h>
#include <Utility/MathUtility.h>
#include "DataBase/SGX/SgxDatabaseStructure.h"

moe::SgxDatabaseStructure::SgxDatabaseStructure(const QString &path, const QString &type) : DataBaseManager(path) {
    m_db = QSqlDatabase::addDatabase(type);
    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        std::cerr << "Error: connection with database fail" << std::endl;
    } else {
        std::cout << "Database: Connection Ok" << std::endl;
    }
    //threads_ = QVector<MyThread>(getNumberOfRows("threads"),MyThread());
    loadExistingEnclaves();
    loadECallTypeList();
    loadOCallTypeList();
    //loadEcallsStats(); //ToDo delete this later, this is only for test

    initializeThreads();
    initializeECallsAndOCalls();

}


/**
 * @return the program's start time relatively to the PC logged with
 */
uint64_t moe::SgxDatabaseStructure::getProgramStartTime() {
    QSqlQuery query;
    query.prepare("SELECT value FROM general WHERE key LIKE 'start_time'");
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    } else {
        query.next();
        //std::cout << query.value(0).toInt() << std::endl;
        return query.value(0).toDouble();
    }
}
/**
 * @return the program's end time relatively to the PC logged with
 */
uint64_t moe::SgxDatabaseStructure::getProgramEndTime() {
    QSqlQuery query;
    query.prepare("SELECT value FROM general WHERE key LIKE 'end_time'");
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    } else {
        query.next();
        //std::cout << query.value(0).toInt() << std::endl;
        return query.value(0).toDouble();
    }
}


/**
 * @return the absolute runtime of the program
 */
uint64_t moe::SgxDatabaseStructure::getProgramTotalTime() {
    QSqlQuery query;
    query.prepare("SELECT value FROM general WHERE key LIKE 'start_time' OR key LIKE 'end_time'");
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    } else {
        query.next();
        //std::cout << query.value(0).toInt() << std::endl;
        uint64_t start_time = query.value(0).toDouble();
        query.next();
        //std::cout << query.value(0).toInt() << std::endl;
        return query.value(0).toDouble() - start_time;
    }
}


/**
 * @param tableName
 * @return the number of entries in the give table name
 */
int moe::SgxDatabaseStructure::getNumberOfRows(const QString &tableName) {
    QSqlQuery query;
    query.prepare("SELECT * FROM " + tableName);
    //query.bindValue(":tableName",tableName);
    int numRows;
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    if(m_db.driver()->hasFeature(QSqlDriver::QuerySize))
    {
        return query.size();
    } else {
        query.last();
        numRows = query.at() + 1;
        return numRows;
    }
}
/**
 * @param index
 * @return returns the start time of the thread at given index relatively to the program start time
 */

uint64_t moe::SgxDatabaseStructure::getThreadStartTime(int index) {
    QSqlQuery query;
    query.prepare("SELECT time FROM events WHERE involved_thread = (:involvedThread) AND type = 3");
    query.bindValue(":involvedThread", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    } else {
        query.next();
        int64_t result = query.value(0).toDouble() /*- getProgramStartTime()*/;
        return result;
    }
}

/**
 * initializes the threads in the vector MyThreads
 * @param index
 */
void moe::SgxDatabaseStructure::initializeThreads(QString conditionQuery) {
    uint64_t pthread_id,start_address, start_address_normalized, start_symbol, start_time, total_time;

    std::string name, start_symbol_file_name; // ToDo add start symbol name and start address normalized and start symbol
    int id;
    //int ecallNumbers = getEcallsNumberOfThreadAtIndex(index); //ToDo causes problems when added (should be fixed later)

    //start_time = getThreadStartTime(index) - getProgramStartTime(); // Absolute start_time

    /* total time relative to the program start time and not the thread start time
     * ToDo later this should be replaced with thread_destruction_time - ProgramStartTime to get the length of the threads timeline
     * ToDo because at the moment all threads sequence diagrams will be drawn at the top despite their creation time which will be later displayed by hovering (maybe)
     */
    total_time = getProgramTotalTime(); // ToDo another query according the thread destruction event
    QSqlQuery query;
    QString queryString = "SELECT t.id, t.pthread_id, t.start_address, t.name, IFNULL(t.start_symbol,0),"
            " IFNULL(t.start_symbol_file_name, \"\"),"
            " IFNULL(t.start_address_normalized,0), e1.time AS start_time"
            " FROM threads AS t JOIN events AS e1  ON t.id = e1.involved_thread "
            "WHERE e1.type = 3";

    if (!conditionQuery.isEmpty())
    {
        queryString.append(conditionQuery);
    }

    queryString.append(" ORDER BY t.id");
    query.prepare(queryString);
    //query.bindValue(":id", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else { //ToDo ask nico to implement thread Destruction for the first thread(cant be done and should be queried separately)
        while (query.next()) {
            id = query.value(0).toInt();
            pthread_id = (uint64_t) query.value(1).toDouble();
            start_address = (uint64_t) query.value(2).toDouble();
            name = query.value(3).toString().toStdString();
            start_symbol = (uint64_t) query.value(4).toDouble();
            start_symbol_file_name = query.value(5).toString().toStdString();
            start_address_normalized = (uint64_t) query.value(6).toDouble();
            threads_.push_back(MyThread(id, pthread_id, start_address, start_address_normalized, start_symbol,
                                        start_time, total_time, name, start_symbol_file_name));
        }
    }
}

/**
 *
 * @param index
 * @return the number of Ecalls which are called from the thread at the given index
 */
//ToDo this methode should be reworked later for the failed Ecall Creation etc... (not used, later maybe for optimization phase)
int moe::SgxDatabaseStructure::getEcallsNumberOfThreadAtIndex(int index) {
    QSqlQuery query;
    //this returns only the successfull Ecalls that operate directly on the thread and not children from Ocalls
    query.prepare("SELECT COUNT (*) FROM events AS e1 JOIN events AS e2 ON e1.id = e2.call_event "
                          "WHERE e1.involved_thread = (:involved_thread) AND e1.type = 14 AND e2.return_value = 0 AND e1.call_event is NULL");
    query.bindValue(":involved_thread", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}

/**
 * initializes the Ecalls and their children according to the given Thread
 * @param index
 */
//ToDo initialize all ECalls and OCalls according to the existing threads (should be done for now i hope :S)
void moe::SgxDatabaseStructure::initializeECallsAndOCalls(QString conditionQuery)
{
    QMap<int, Call*> calls;
    int id,call_id,call_event,isFail, involvedThreadId, eid;
    uint64_t start_time, relative_start_time, total_time, symbol_address;
    QString symbol_name;
    uint64_t programStratTime = getProgramStartTime();
    QSqlQuery query;


    QString newQueryString = "with recursive events_children as ("
            " select e1.id, e1.type, e1.time, e1.call_id, e1.eid, e1.call_event, e1.involved_thread, e1.aex_count, 0 as _level from events as e1";

    newQueryString.append(getInvolvedThreads());
    if (!conditionQuery.isEmpty())
    {
        newQueryString.append(conditionQuery);
        newQueryString.append( " union all "
                "select e2.id, e2.type, e2.time, e2.call_id, e2.eid, e2.call_event, e2.involved_thread, e2.aex_count, _level-1 "
                "from events as e2 inner join events_children ON e2.id = events_children.call_event" );
    }
    newQueryString.append(
            " ), "
            "filtered_events as ( select e_.id, e_.type, e_.time as start_time, e.time as end_time, e_.call_id,"
                    " e_.call_event, e.return_value, e_.involved_thread, e_.eid, e.aex_count "
                    "from events_children as e_ inner join events as e on e.call_event = e_.id "
            "where e.type IN (15, 17) ) "
            "select distinct e.id, e.type, e.start_time, e.end_time, e.call_id, e.call_event, e.return_value, e.involved_thread, e.eid, e.aex_count, "
            "IFNULL(ec.symbol_name, oc.symbol_name) as symbol_name, IFNULL(ec.symbol_address, oc.symbol_address_normalized),"
            " oc.symbol_file_name as symbol_file_name "
                    "from filtered_events as e left join ecalls as ec on ec.id = e.call_id and e.type = 14 "
                    "left join ocalls as oc on oc.id = e.call_id and e.type = 16 "
                    "order by start_time ASC");

    //ToDo aex_count should be IFNULL(aex_count , 0) ?? and what should be done with it exactly ? (still open  :| )
    //ToDo ask nico why we took symbol_address_normalized instead of symbol address of the ocalls ? (done :) )
    //ToDo ask nico is it on purpose that symbol_file_name is addressed as symbol_address or is it not right (it was fast typing issue :) )
    //ToDo what about isPrivate from ecalls and symbol Address from Ocalls (still open :| )
    std::cerr << newQueryString.toStdString() << std::endl;
    query.prepare(newQueryString);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    }
    while(query.next())
    {
        id = query.value(0).toInt(); //line unique id in the table events
        relative_start_time = start_time = (uint64_t) query.value(2).toDouble() - programStratTime;
        total_time = (uint64_t) query.value(3).toDouble() - query.value(2).toDouble();
        call_id = query.value(4).toInt(); //ecall's or ocall's unique id from the table ecalls / ocalls
        call_event = query.value(5).toInt();
        isFail = query.value(6).toInt();
        involvedThreadId = query.value(7).toInt();
        eid = query.value(8).toInt();
        symbol_name = query.value(10).toString();
        symbol_address = (uint64_t)query.value(11).toDouble(); //ToDo maybe should be fixed later because of Ocall normalized address

        switch(query.value(1).toInt()) { //EventType

            case (int)EventMap::EnclaveECallEvent: {
                //bool is_private = query.value(11).toInt();
                ECall *eCall = new ECall(call_id,eid,symbol_address,start_time,relative_start_time,total_time,0,symbol_name,isFail);
                calls[id] = eCall;
                //ToDo idea to save the stats without changing them after filtering and such (saving the stats at the start and never touch them)
                if (!callHoverInfoMap.count(id) && conditionQuery.isEmpty())
                {
                    callHoverInfoMap[id].callTotalTime += total_time;
                    callHoverInfoMap[id].callName += symbol_name;
                    callHoverInfoMap[id].enclaveId += eid;
                    callHoverInfoMap[id].enclaveBinaryName += enclavesList[eid];
                    eCall->callInfo = callHoverInfoMap[id];

                } else {

                    calls[id]->callInfo = callHoverInfoMap[id];
                }

                if(call_event != 0)
                {
                    //here i calculate the relative start time of the child according to his parent start time
                    eCall->relative_start_time_ -= calls[call_event]->start_time_;
                    calls[call_event]->children_.push_back(eCall);
                    /*
                     * ToDo add these for the statistic implementation later
                     * ToDo ask nico, after filtering those infos will be deleted and zeros will be
                     */
                    //because we only need to gather these information at the initilizing the database phase and then they should stay untouched
                    if (conditionQuery.isEmpty())
                    {
                        calls[call_event]->callInfo.childrenCounter += 1;
                        calls[call_event]->callInfo.childrenTotalRuntime += total_time;

                        callHoverInfoMap[call_event].childrenCounter += 1;
                        callHoverInfoMap[call_event].childrenTotalRuntime += total_time;
                    }
                } else {
                    threads_[searchThreadIndex(involvedThreadId)].threadEcalls_.push_back(eCall);
                }
                break;
            }

            case (int)EventMap::EnclaveOCallEvent : {
                std::string symbol_file_name = query.value(12).toString().toStdString();
                uint64_t symbol_address_normalized = symbol_address; //ToDo fix this later (only normalized is important for ocalls and it represents the real symbol address)

                OCall *oCall = new OCall(call_id,eid,symbol_address,start_time,relative_start_time,total_time,symbol_name,
                                         symbol_address_normalized,symbol_file_name, isFail);
                calls[id] = oCall;
                //ToDo idea to save the stats without changing them after filtering and such (saving the stats at the start and never touch them)
                if (!callHoverInfoMap.count(id) && conditionQuery.isEmpty())
                {
                    callHoverInfoMap[id].callTotalTime = total_time;
                    callHoverInfoMap[id].callName = symbol_name;
                    oCall->callInfo = callHoverInfoMap[id];
                } else {
                    calls[id]->callInfo = callHoverInfoMap[id];
                }

                if(call_event != 0)
                {
                    //here i calculate the relative start time of the child according to his parent start time
                    oCall->relative_start_time_ -= calls[call_event]->start_time_;
                    calls[call_event]->children_.push_back(oCall);
                    /*
                     * ToDo add these for the statistic implementation later
                     */
                    if (conditionQuery.isEmpty())
                    {
                        calls[call_event]->callInfo.childrenCounter += 1;
                        calls[call_event]->callInfo.childrenTotalRuntime += total_time;

                        callHoverInfoMap[call_event].childrenCounter += 1;
                        callHoverInfoMap[call_event].childrenTotalRuntime += total_time;
                    }

                } else { //this should never be reachable unless the database is corrupted
                    std::cerr << "OCall has no ecall id from which its triggered " << std::endl;
                }
                break;
            }
            default:
                break;
        }
    }
}

void moe::SgxDatabaseStructure::close()
{
    m_db.close();
}


/* ToDo
 * this method is still buggy and needs lot of work and the whole program should be then reworked because the ecalls and ocalls
 * will be printed way further than the relative thread they are called from
 * and this problem is originated because the threads are beside each other and not to their own real start time
 */
uint64_t moe::SgxDatabaseStructure::getThreadTotalTime(int index) {
    QSqlQuery query;
    query.prepare("SELECT time FROM events WHERE involved_thread = (:involvedThread) AND type = 5");
    query.bindValue(":involvedThread", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    } else {
        uint64_t threadStartTime = getThreadStartTime(index);
        uint64_t result = 0;
        if (query.size() != 0) {
            query.next();
            result = query.value(0).toDouble() - threadStartTime;
            return (uint64_t)result;
        } else {
            result = getProgramEndTime() - threadStartTime;
            return (uint64_t) result;
        }
    }
}

const QVector<moe::MyThread> &moe::SgxDatabaseStructure::getThreads_() const
{
    return threads_;
}

void moe::SgxDatabaseStructure::loadECallTypeList() {
    int numOfECallTypes = getNumberOfRows("ecalls");
    eCallTypeList = QVector<ECallTypes>(numOfECallTypes);
    QSqlQuery query;
    query.prepare("SELECT id, eid, symbol_address, symbol_name, is_private FROM ecalls");
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else {
        int id, eid;
        uint64_t symbol_address;
        QString symbol_name;
        bool isprivate;
        while (query.next())
        {
            id = query.value(0).toInt();
            eid = query.value(1).toInt();
            symbol_address = (uint64_t) query.value(2).toDouble();
            symbol_name = query.value(3).toString();
            isprivate = query.value(4).toBool();
           // std::cerr << id << "   "  << symbol_name.toStdString() << std::endl;
            ECallTypes eCalltypeMember = ECallTypes(id, eid, symbol_address, symbol_name, isprivate);
            eCallTypeList[id] = eCalltypeMember;
        }
    }
}

void moe::SgxDatabaseStructure::loadOCallTypeList() {
    int numOfOCallTypes = getNumberOfRows("ocalls");
    oCallTypeList = QVector<OCallTypes>(numOfOCallTypes);
    QSqlQuery query;
    query.prepare("SELECT id, eid, symbol_address, symbol_name, symbol_address_normalized, symbol_file_name FROM ocalls");
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else {
        int id, eid;
        uint64_t symbol_address, symbol_address_normalized;
        QString symbol_name, symbol_file_name;
        while (query.next())
        {
            id = query.value(0).toInt();
            eid = query.value(1).toInt();
            symbol_address = (uint64_t) query.value(2).toDouble();
            symbol_name = query.value(3).toString();
            symbol_address_normalized = (uint64_t) query.value(4).toDouble();
            symbol_file_name = query.value(5).toString();
           // std::cerr << id << "   "  << symbol_name.toStdString() << std::endl;
            OCallTypes oCalltypeMember = OCallTypes(id, eid, symbol_address, symbol_name, symbol_address_normalized, symbol_file_name);
            oCallTypeList[id] = (oCalltypeMember);
        }
    }
}

const QVector<moe::OCallTypes> &moe::SgxDatabaseStructure::getOCallTypeList() const {
    return oCallTypeList;
}

const QVector<moe::ECallTypes> &moe::SgxDatabaseStructure::getECallTypeList() const {
    return eCallTypeList;
}

void moe::SgxDatabaseStructure::SetAction(TYPES::ACTION_LIST action)
{
    currentAction = action;
}

void moe::SgxDatabaseStructure::getResult(QString conditionQuery)
{
    if (currentAction == TYPES::ACTION_LIST::THREADFILTER)
    {
        threads_.clear();
        initializeThreads(conditionQuery);
        //initializeECallsAndOCalls();

    } else if (currentAction == TYPES::ACTION_LIST::ECALLOCALLFILTER)
    {
        for (int i = 0; i < threads_.length() ; ++i)
        {
            auto it = threads_[i].threadEcalls_.begin();
            while (it != threads_[i].threadEcalls_.end())
            {
                delete *it;
                it++;
            }
            threads_[i].threadEcalls_.clear();
        }
        initializeECallsAndOCalls(conditionQuery);
    }
}

QString moe::SgxDatabaseStructure::getInvolvedThreads()
{
    QString availableThreads = " WHERE e1.involved_thread IN ( ";
    if (!threads_.isEmpty())
    {
        for (int i = 0; i < threads_.size() ; ++i)
        {
            availableThreads.append(QString::number(threads_[i].id_));
            availableThreads.append(" ,");
        }
        availableThreads.remove(availableThreads.length()-1,1);
        availableThreads.append(")");
       // std::cerr << " involved Threads are " << availableThreads.toStdString() << std::endl;
        return availableThreads;
    } else {
        availableThreads.append(")");
        return availableThreads;
    }
}

int moe::SgxDatabaseStructure::searchThreadIndex(int threadId)
{
    for (int i = 0; i < threads_.length() ; i++)
    {
        if(threads_[i].id_ == threadId)
        {
            return i;
        }
    }
    return -1;
}

void moe::SgxDatabaseStructure::loadExistingEnclaves()
{
    QSqlQuery query;
    query.prepare("SELECT eid, file_name as enclave_name FROM events WHERE type = 8 ORDER BY eid");
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else {
        while (query.next())
        {
            int eid = query.value(0).toInt();
            QString enclaveName = query.value(1).toString();
            enclavesList[eid] = enclaveName;
        }
    }
}

const QMap<int, QString> &moe::SgxDatabaseStructure::getEnclavesMap() const
{
    return enclavesList;
}

void moe::SgxDatabaseStructure::loadEcallsStats()
{
    QSqlQuery query;
    query.prepare("SELECT ec.id, ec.symbol_name, (total(e2.time)-total(e1.time))/count(e1.call_id) AS average "
                          "FROM ecalls AS ec JOIN events AS e1 ON ec.id = e1.call_id JOIN events AS e2 ON e1.id = e2.call_event "
                          "WHERE e1.type = 14 AND e2.type = 15 GROUP BY ec.id");

    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else {
        while(query.next())
        {
            CallStatistics ecallStats;
            ecallStats.callId_ = query.value(0).toInt();
            ecallStats.callSymbolName_ = query.value(1).toString();
            ecallStats.callAvg_ = query.value(2).toReal();
            ecallStatistics.push_back(ecallStats);
        }
    }

    QMap<int, QVector<uint64_t >> medianTotalTimeListMap;
    int id;
    uint64_t totalTime;
    QSqlQuery medianQuery;
    medianQuery.prepare("SELECT e1.call_id as call_id ,(e2.time - e1.time) AS total_time "
                                "FROM events AS e1 JOIN events as e2 ON e1.id = e2.call_event "
                                "WHERE e1.type = 14 AND e2.type = 15 "
                                "order BY call_id, total_time ");
    if(!medianQuery.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else {
        while (medianQuery.next())
        {
            id = medianQuery.value(0).toInt();
            totalTime = (uint64_t) medianQuery.value(1).toDouble();
            medianTotalTimeListMap[id].push_back(totalTime);
            /*if(!medianTotalTimeListMap.count(id)){
                QVector<uint64_t > totalTimeList;
                totalTimeList.push_back(totalTime);
                medianTotalTimeListMap.insert(id,totalTimeList);
            } else {
                medianTotalTimeListMap.value(id).push_back(totalTime);
            }*/
        }

        for (CallStatistics ecallStats : ecallStatistics)
        {
            ecallStatistics[ecallStats.callId_].median_ = median(medianTotalTimeListMap[ecallStats.callId_]);
            ecallStatistics[ecallStats.callId_].standardDeviation_ = standardDeviation(medianTotalTimeListMap[ecallStats.callId_],ecallStats.callAvg_);
        }
    }
}

const QVector<moe::CallStatistics> &moe::SgxDatabaseStructure::getEcallStatistics() const
{
    return ecallStatistics;
}

void moe::SgxDatabaseStructure::loadOcallsStats()
{
    QSqlQuery query;
    query.prepare("SELECT oc.id, oc.symbol_name, (total(e2.time)-total(e1.time))/count(e1.call_id) AS average "
                          "FROM ocalls AS oc JOIN events AS e1 ON oc.id = e1.call_id JOIN events AS e2 ON e1.id = e2.call_event "
                          "WHERE e1.type = 16 AND e2.type = 17 GROUP BY oc.id");

    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else {
        while(query.next())
        {
            CallStatistics ocallStats;
            ocallStats.callId_ = query.value(0).toInt();
            ocallStats.callSymbolName_ = query.value(1).toString();
            ocallStats.callAvg_ = query.value(2).toReal();
            ocallStatistics.push_back(ocallStats);
        }
    }

    QMap<int, QVector<uint64_t >> medianTotalTimeListMap;
    int id;
    uint64_t totalTime;
    QSqlQuery medianQuery;
    medianQuery.prepare("SELECT e1.call_id as call_id ,(e2.time - e1.time) AS total_time "
                                "FROM events AS e1 JOIN events as e2 ON e1.id = e2.call_event "
                                "WHERE e1.type = 16 AND e2.type = 17 "
                                "order BY call_id, total_time ");
    if(!medianQuery.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else {
        while (medianQuery.next())
        {
            id = medianQuery.value(0).toInt();
            totalTime = (uint64_t) medianQuery.value(1).toDouble();
            medianTotalTimeListMap[id].push_back(totalTime);
        }

        for (CallStatistics ocallStats : ocallStatistics)
        {
            ocallStatistics[ocallStats.callId_].median_ = median(medianTotalTimeListMap[ocallStats.callId_]);
            ocallStatistics[ocallStats.callId_].standardDeviation_ = standardDeviation(medianTotalTimeListMap[ocallStats.callId_],ocallStats.callAvg_);
        }
    }
}
