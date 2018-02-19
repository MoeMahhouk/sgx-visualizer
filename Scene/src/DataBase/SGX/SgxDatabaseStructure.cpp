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
    initializeThreads();
    initializeECallsAndOCalls();
    loadECallDynamicAnalysis();
    loadOCallDynamicAnalysis(); //TODO its only a test

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
    if(!execAndCheckQuery(query))
        return -1; //ToDo change this later to 0 and do the following checkup if anywhere this returns 0, should be handeld immediately (this case should never happens if the database is implemented correctly tho)

    query.next();
    //std::cout << query.value(0).toInt() << std::endl;
    return query.value(0).toDouble();

}


/**
 * @return the absolute runtime of the program
 */
uint64_t moe::SgxDatabaseStructure::getProgramTotalTime() {
    QSqlQuery query;
    query.prepare("SELECT value FROM general WHERE key LIKE 'start_time' OR key LIKE 'end_time'");
    if(!execAndCheckQuery(query))
        return -1;

    query.next();
    //std::cout << query.value(0).toInt() << std::endl;
    uint64_t start_time = query.value(0).toDouble();
    query.next();
    //std::cout << query.value(0).toInt() << std::endl;
    return query.value(0).toDouble() - start_time;

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

    if(!execAndCheckQuery(query))
        return -1;

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

    if(!execAndCheckQuery(query))
        return -1;

    query.next();
    int64_t result = query.value(0).toDouble() /*- getProgramStartTime()*/;
    return result;

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
    if(!execAndCheckQuery(query))
        return;

    //ToDo ask nico to implement thread Destruction for the first thread(cant be done and should be queried separately)
    while (query.next())
    {
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
    //ToDo what about isPrivate from ecalls and symbol Address from Ocalls (still open :| )
    std::cerr << newQueryString.toStdString() << std::endl;
    query.prepare(newQueryString);

    if(!execAndCheckQuery(query))
        return;

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
                ECall *eCall = new ECall(call_id,eid,symbol_address,start_time,relative_start_time,total_time,0,symbol_name,isFail);
                calls[id] = eCall;

                //ToDo idea to save the stats without changing them after filtering and such (saving the stats at the start and never touch them)
                if (!callHoverInfoMap.count(id) && conditionQuery.isEmpty())
                {
                    callHoverInfoMap[id].callTotalTime += total_time;
                    callHoverInfoMap[id].callName += symbol_name;
                    callHoverInfoMap[id].enclaveId += eid;
                    callHoverInfoMap[id].enclaveBinaryName += enclavesList[eid];
                    callHoverInfoMap[id].status = isFail;
                    eCall->callInfo = callHoverInfoMap[id];

                } else {

                    calls[id]->callInfo = callHoverInfoMap[id];
                }

                if(call_event != 0)
                {
                    if(!directPublicEcalls.count(call_id))
                        directPublicEcalls[call_id] = false;

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
                    //If the ECall not in the list or was uptill now only found privately inside ocalls, change it to true because he should stay public
                    if(!directPublicEcalls.count(call_id) || !directPublicEcalls[call_id])
                    {
                        directPublicEcalls[call_id] = true;
                    }
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
    if(!execAndCheckQuery(query))
        return -1;

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

const QVector<moe::MyThread> &moe::SgxDatabaseStructure::getThreads_() const
{
    return threads_;
}

void moe::SgxDatabaseStructure::loadECallTypeList()
{
    int numOfECallTypes = getNumberOfRows("ecalls");
    eCallTypeList = QVector<ECallTypes>(numOfECallTypes);
    QSqlQuery query;
    query.prepare("SELECT id, eid, symbol_address, symbol_name, is_private FROM ecalls");
    if(!execAndCheckQuery(query))
        return;

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
        ECallTypes eCalltypeMember = ECallTypes(id, eid, symbol_address, symbol_name, isprivate);
        eCallTypeList[id] = eCalltypeMember;
    }

}

void moe::SgxDatabaseStructure::loadOCallTypeList()
{
    int numOfOCallTypes = getNumberOfRows("ocalls");
    oCallTypeList = QVector<OCallTypes>(numOfOCallTypes);
    QSqlQuery query;
    query.prepare("SELECT id, eid, symbol_address, symbol_name, symbol_address_normalized, symbol_file_name FROM ocalls");
    if(!execAndCheckQuery(query))
        return;

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

    if(!execAndCheckQuery(query))
        return;

    while (query.next())
    {
        int eid = query.value(0).toInt();
        QString enclaveName = query.value(1).toString();
        enclavesList[eid] = enclaveName;
    }

}

const QMap<int, QString> &moe::SgxDatabaseStructure::getEnclavesMap() const
{
    return enclavesList;
}

void moe::SgxDatabaseStructure::loadEcallsStats()
{
    QSqlQuery query;
    query.prepare("SELECT ec.id, ec.symbol_name, (total(e2.time)-total(e1.time))/count(e1.call_id) AS average, COUNT(e1.call_id) AS count "
                          "FROM ecalls AS ec JOIN events AS e1 ON ec.id = e1.call_id JOIN events AS e2 ON e1.id = e2.call_event "
                          "WHERE e1.type = 14 AND e2.type = 15 GROUP BY ec.id");

    if(!execAndCheckQuery(query))
        return;

    while(query.next())
    {
        CallStatistics ecallStats;
        ecallStats.callId_ = query.value(0).toInt();
        ecallStats.callSymbolName_ = query.value(1).toString();
        ecallStats.callAvg_ = query.value(2).toReal();
        ecallStats.count_ = query.value(3).toReal();
        ecallStatistics.push_back(ecallStats);
    }

    QMap<int, QVector<uint64_t >> medianTotalTimeListMap;
    int id;
    uint64_t totalTime;
    QSqlQuery medianQuery;
    medianQuery.prepare("SELECT e1.call_id as call_id ,(e2.time - e1.time) AS total_time "
                                "FROM events AS e1 JOIN events as e2 ON e1.id = e2.call_event "
                                "WHERE e1.type = 14 AND e2.type = 15 "
                                "order BY call_id, total_time ");

    if(!execAndCheckQuery(medianQuery))
        return;

    while (medianQuery.next())
    {
        id = medianQuery.value(0).toInt();
        totalTime = (uint64_t) medianQuery.value(1).toDouble();
        medianTotalTimeListMap[id].push_back(totalTime);
    }

    for (CallStatistics ecallStats : ecallStatistics)
    {
        ecallStatistics[ecallStats.callId_].median_ = median(medianTotalTimeListMap[ecallStats.callId_]);
        ecallStatistics[ecallStats.callId_].standardDeviation_ = standardDeviation(medianTotalTimeListMap[ecallStats.callId_],ecallStats.callAvg_);
        ecallStatistics[ecallStats.callId_]._99thPercentile_ = percentile(medianTotalTimeListMap[ecallStats.callId_],0.99);
        ecallStatistics[ecallStats.callId_]._95thPercentile_ = percentile(medianTotalTimeListMap[ecallStats.callId_],0.95);
        ecallStatistics[ecallStats.callId_]._90thPercentile_ = percentile(medianTotalTimeListMap[ecallStats.callId_],0.90);
    }

}

const QVector<moe::CallStatistics> &moe::SgxDatabaseStructure::getEcallStatistics() const
{
    return ecallStatistics;
}

void moe::SgxDatabaseStructure::loadOcallsStats()
{
    QSqlQuery query;
    query.prepare("SELECT oc.id, oc.symbol_name, (total(e2.time)-total(e1.time))/count(e1.call_id) AS average , COUNT(e1.call_id) AS count "
                          "FROM ocalls AS oc JOIN events AS e1 ON oc.id = e1.call_id JOIN events AS e2 ON e1.id = e2.call_event "
                          "WHERE e1.type = 16 AND e2.type = 17 GROUP BY oc.id");

    if(!execAndCheckQuery(query))
        return;

    while(query.next())
    {
        CallStatistics ocallStats;
        ocallStats.callId_ = query.value(0).toInt();
        ocallStats.callSymbolName_ = query.value(1).toString();
        ocallStats.callAvg_ = query.value(2).toReal();
        ocallStats.count_ = query.value(3).toReal();
        ocallStatistics.push_back(ocallStats);
    }


    QMap<int, QVector<uint64_t >> medianTotalTimeListMap;
    int id;
    uint64_t totalTime;
    QSqlQuery medianQuery;
    medianQuery.prepare("SELECT e1.call_id as call_id ,(e2.time - e1.time) AS total_time "
                                "FROM events AS e1 JOIN events as e2 ON e1.id = e2.call_event "
                                "WHERE e1.type = 16 AND e2.type = 17 "
                                "order BY call_id, total_time ");
    if(!execAndCheckQuery(medianQuery))
        return;

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
        ocallStatistics[ocallStats.callId_]._99thPercentile_ = percentile(medianTotalTimeListMap[ocallStats.callId_],0.99);
        ocallStatistics[ocallStats.callId_]._95thPercentile_ = percentile(medianTotalTimeListMap[ocallStats.callId_],0.95);
        ocallStatistics[ocallStats.callId_]._90thPercentile_ = percentile(medianTotalTimeListMap[ocallStats.callId_],0.90);
    }

}

const QVector<moe::CallStatistics> &moe::SgxDatabaseStructure::getOcallStatistics() const {
    return ocallStatistics;
}

void moe::SgxDatabaseStructure::loadEcallAnalysis() {
    if(!m_db.tables(QSql::Views).contains("ECallAnalysisView",Qt::CaseInsensitive))
    {
        QSqlQuery createViewQuery;
        QSqlQuery createViewQuery2;

        createViewQuery.prepare("create VIEW ecallAnalysisTable AS  "
                                        "SELECT e1.call_id as call_id, (e2.time - e1.time) AS total_time, ec.symbol_name AS name "
                                        "FROM events as e1 JOIN events as e2 ON e1.id = e2.call_event JOIN ecalls as ec ON e1.call_id = ec.id AND e1.eid = ec.eid "
                                        "WHERE e1.type = 14 AND e2.type = 15 "
                                        "ORDER BY call_id, total_time; ");
        createViewQuery2.prepare(
                "create VIEW ECallAnalysisView AS "
                        "SELECT et1.call_id as ecall_id , et1.name  as ecall_name , COUNT(et1.call_id) as ecall_total_count , "
                        "(SELECT COUNT(et2.call_id) FROM ecallAnalysisTable as et2 WHERE et2.total_time <= 1000 AND et2.call_id = et1.call_id) AS ecall_count_under_micro, "
                        "(SELECT COUNT(et3.call_id) FROM ecallAnalysisTable as et3 WHERE et3.total_time <= 10000 AND et3.call_id = et1.call_id) AS ecall_count_under_10micro  "
                        "FROM ecallAnalysisTable as et1 "
                        "GROUP BY et1.call_id;");

        if (!execAndCheckQuery(createViewQuery) || !execAndCheckQuery(createViewQuery2))
            return;

    }
    QSqlQuery loadECallAnalysisQuery;
    loadECallAnalysisQuery.prepare("SELECT * FROM ECallAnalysisView");

    if(!execAndCheckQuery(loadECallAnalysisQuery))
        return;

    while(loadECallAnalysisQuery.next())
    {
        ECallStaticAnalysis eCallStaticAnalysis;
        eCallStaticAnalysis.callId_ = loadECallAnalysisQuery.value(0).toInt();
        eCallStaticAnalysis.callName_ = loadECallAnalysisQuery.value(1).toString();
        eCallStaticAnalysis.totalCount_ = loadECallAnalysisQuery.value(2).toInt();
        eCallStaticAnalysis.totalOfLowerThanMicroSeconds_ = loadECallAnalysisQuery.value(3).toInt();
        eCallStaticAnalysis.totalOflowerThan10MicroSeconds_ = loadECallAnalysisQuery.value(4).toInt();
        ecallStaticAnalysis.push_back(eCallStaticAnalysis);
    }
    for (int i = 0; i < ecallStaticAnalysis.size() ; ++i)
    {
        ecallStaticAnalysis[i].shouldBePrivate_ = !directPublicEcalls[ecallStaticAnalysis[i].callId_];
        ecallStaticAnalysis[i].generateAnalysisText();
    }
}

void moe::SgxDatabaseStructure::loadOcallAnalysis() {

    if(!m_db.tables(QSql::Views).contains("OCallAnalysisView",Qt::CaseInsensitive))
    {
        QSqlQuery createViewQuery;
        QSqlQuery createViewQuery2;
        createViewQuery.prepare("create VIEW ocallAnalysisTable AS "
                                        "SELECT e1.call_id as call_id, (e2.time - e1.time) AS total_time, oc.symbol_name AS name "
                                        "FROM events as e1 JOIN events as e2 ON e1.id = e2.call_event JOIN ocalls as oc ON e1.call_id = oc.id AND e1.eid = oc.eid "
                                        "WHERE e1.type = 16 AND e2.type = 17 "
                                        "ORDER BY call_id, total_time;");

        createViewQuery2.prepare("create VIEW OCallAnalysisView AS "
                                         "SELECT ot1.call_id as ocall_id , ot1.name  as ocall_name , COUNT(ot1.call_id) as ocall_total_count , "
                                         "(SELECT COUNT(ot2.call_id) FROM ocallAnalysisTable as ot2 WHERE ot2.total_time <= 1000 AND ot2.call_id = ot1.call_id) AS ocall_count_under_micro, "
                                         "(SELECT COUNT(ot3.call_id) FROM ocallAnalysisTable as ot3 WHERE ot3.total_time <= 10000 AND ot3.call_id = ot1.call_id) AS ocall_count_under_10micro "
                                         "FROM ocallAnalysisTable as ot1 "
                                         "GROUP BY ot1.call_id;");


        if(!execAndCheckQuery(createViewQuery) || !execAndCheckQuery(createViewQuery2))
            return;

    }

    QSqlQuery loadOCallAnalysisQuery;
    loadOCallAnalysisQuery.prepare("SELECT * FROM OCallAnalysisView");

    if(!execAndCheckQuery(loadOCallAnalysisQuery)) {
        return;
    }

    while(loadOCallAnalysisQuery.next())
    {
        OCallStaticAnalysis oCallStaticAnalysis;
        oCallStaticAnalysis.callId_ = loadOCallAnalysisQuery.value(0).toInt();
        oCallStaticAnalysis.callName_ = loadOCallAnalysisQuery.value(1).toString();
        oCallStaticAnalysis.totalCount_ = loadOCallAnalysisQuery.value(2).toInt();
        oCallStaticAnalysis.totalOfLowerThanMicroSeconds_ = loadOCallAnalysisQuery.value(3).toInt();
        oCallStaticAnalysis.totalOflowerThan10MicroSeconds_ = loadOCallAnalysisQuery.value(4).toInt();
        ocallStaticAnalysis.push_back(oCallStaticAnalysis);
    }

    for (int i = 0; i < ocallStaticAnalysis.size() ; ++i)
    {
        ocallStaticAnalysis[i].generateAnalysisText();
    }
}

const QVector<moe::ECallStaticAnalysis> &moe::SgxDatabaseStructure::getEcallStaticAnalysis() const
{
    return ecallStaticAnalysis;
}

const QVector<moe::OCallStaticAnalysis> &moe::SgxDatabaseStructure::getOcallStaticAnalysis() const
{
    return ocallStaticAnalysis;
}

bool moe::SgxDatabaseStructure::execAndCheckQuery(QSqlQuery query)
{
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return false;
    }
    return true;
}

bool moe::SgxDatabaseStructure::statsGenerated()
{
    return !ecallStatistics.isEmpty() && !ocallStatistics.isEmpty();
}

bool moe::SgxDatabaseStructure::staticAnalysisGenerated()
{
    return !ecallStaticAnalysis.isEmpty() && !ocallStaticAnalysis.isEmpty();
}

void moe::SgxDatabaseStructure::loadECallDynamicAnalysis()
{
    for (int i = 0; i < threads_.size() ; ++i)
    {
        for (int j = 0; j < threads_[i].threadEcalls_.size() ; ++j)
        {
            int ecallId = threads_[i].threadEcalls_[j]->id_;
            if (!ecallDynamicAnalysis.count(ecallId))
            {
                CallDynamicAnalysis ecall;
                ecall.callId_ = ecallId;
                ecall.eid_ = threads_[i].threadEcalls_[j]->eid_;
                ecall.callName_ = threads_[i].threadEcalls_[j]->symbol_name_;
                ecallDynamicAnalysis[ecallId] = ecall;
            }
            ecallDynamicAnalysis[ecallId].counter_ += 1;
            //ToDo ask nico if this is also important to address.
            if(j != 0)
            {
                int predecessorEcallId = threads_[i].threadEcalls_[j-1]->id_;
                ecallDynamicAnalysis[ecallId].incrementPredecessoreIdCounter(predecessorEcallId);
            }
            if(j != threads_[i].threadEcalls_.size() - 1)
            {
                int successorEcallId = threads_[i].threadEcalls_[j+1]->id_;
                ecallDynamicAnalysis[ecallId].incrementSuccessorIdCounter(successorEcallId);
            }
        }
    }
    QMap<int,CallDynamicAnalysis>::iterator k;
    for (k = ecallDynamicAnalysis.begin(); k != ecallDynamicAnalysis.end() ; ++k)
    {
        k.value().generateAnalysisText();
    }
}

const QMap<int, moe::CallDynamicAnalysis> &moe::SgxDatabaseStructure::getEcallDynamicAnalysis() const
{
    return ecallDynamicAnalysis;
}

const QMap<int, moe::CallDynamicAnalysis> &moe::SgxDatabaseStructure::getOcallDynamicAnalysis() const
{
    return ocallDynamicAnalysis;
}

void moe::SgxDatabaseStructure::loadOCallDynamicAnalysis()
{
    for (int i = 0; i < threads_.size() ; ++i)
    {
        for (int j = 0; j < threads_[i].threadEcalls_.size() ; ++j)
        {
            for (int k = 0; k < threads_[i].threadEcalls_[j]->children_.size() ; ++k)
            {
                int ocallId = threads_[i].threadEcalls_[j]->children_[k]->id_;
                if (!ocallDynamicAnalysis.count(ocallId))
                {
                    CallDynamicAnalysis ocall;
                    ocall.callId_ = ocallId;
                    ocall.eid_ = threads_[i].threadEcalls_[j]->children_[k]->eid_;
                    ocall.callName_ = threads_[i].threadEcalls_[j]->children_[k]->symbol_name_;
                    ocallDynamicAnalysis[ocallId] = ocall;
                }
                ocallDynamicAnalysis[ocallId].counter_ += 1;
                //ToDo ask nico if this is also important to address.
                if(k != 0)
                {
                    int predecessorOcallId = threads_[i].threadEcalls_[j]->children_[k-1]->id_;
                    ocallDynamicAnalysis[ocallId].incrementPredecessoreIdCounter(predecessorOcallId);
                }
                if(k != threads_[i].threadEcalls_[j]->children_.size() - 1)
                {
                    int successorOcallId = threads_[i].threadEcalls_[j]->children_[k+1]->id_;
                    ocallDynamicAnalysis[ocallId].incrementSuccessorIdCounter(successorOcallId);
                }
            }
        }
    }
    QMap<int,CallDynamicAnalysis>::iterator l;
    for (l = ocallDynamicAnalysis.begin(); l != ocallDynamicAnalysis.end() ; ++l)
    {
        l.value().generateAnalysisText();
    }
}
