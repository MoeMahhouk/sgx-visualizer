//
// Created by moe on 11.02.18.
//

#ifndef SCENE_CALLDYNAMICANALYSIS_H
#define SCENE_CALLDYNAMICANALYSIS_H


#include <QtCore/QString>
#include <QtCore/QMap>

namespace moe {
    struct CallDynamicAnalysis {
        //ToDo refine this with the eid for later traces
        int callId_,eid_,counter_ = 0; //ToDo added the counter to check later the successorsId counter percent from this calls total count and evently give better analysis
        QString callName_, analysisText_;
        QMap<int,int> sucessorId_Counter_Map_;
        QMap<int,int> predecessoreId_Counter_Map_; //ToDo remove this later if it appears to be useless


        void incrementSuccessorIdCounter(int id)
        {
            if(sucessorId_Counter_Map_.count(id))
            {
                sucessorId_Counter_Map_[id] += 1;
            } else {
                sucessorId_Counter_Map_[id] = 1;
            }
        }

        void incrementPredecessoreIdCounter(int id)
        {
            if(predecessoreId_Counter_Map_.count(id))
            {
                predecessoreId_Counter_Map_[id] += 1;
            } else {
                predecessoreId_Counter_Map_[id] = 1;
            }
        }

        void generateAnalysisText()
        {
            QString tempAnlytxt = "";
            QString successorPatternIds = checkForSuccessorPattern();
            QString predecessorsPatternIds = checkForPredessorPattern();
            if(!successorPatternIds.isEmpty())
            {
                tempAnlytxt.append("It appears that this Call builds up some kind of pattern as a predecessor with the call id ( ");
                //successorPatternIds.remove(successorPatternIds.size()-1,1);
                tempAnlytxt.append(successorPatternIds);
               // tempAnlytxt.append(" )\n");
            }
            if(!predecessorsPatternIds.isEmpty())
            {
                tempAnlytxt.append("It appears that this Call builds up some kind of pattern as a successor with the call id ( ");
                //predecessorsPatternIds.remove(predecessorsPatternIds.size()-1,1);
                tempAnlytxt.append(predecessorsPatternIds);
                //tempAnlytxt.append(" )\n");
            }
            if(successorPatternIds.isEmpty() && predecessorsPatternIds.isEmpty()) {
                analysisText_ = "No Suspicious Pattern was found for this Call";
            } else {
                analysisText_ = tempAnlytxt;
            }
        }

    private:

        QString checkForSuccessorPattern()
        {
            QString successorsIDs = "";
            QMap<int, int>::iterator i;
            for (i = sucessorId_Counter_Map_.begin(); i != sucessorId_Counter_Map_.end(); ++i)
            {
                //ToDo add here later a check for the percentage of how often this pattern appears according to the total calls counter
                if (i.value() >= 2 && (patternPercentage(i.value()) > 50.0))
                {
                    std::cerr << " patternpercentage " << patternPercentage(i.value())  << " is it bigger than 50 ? " << (patternPercentage(i.value()) > 50) << std::endl;
                    //successorsIDs.append(QString::number(i.key()) + " ,");
                    successorsIDs.append(QString::number(i.key()) + " ) , the exact occurrence of this Pattern is ( " +  QString::number(patternPercentage(i.value())) + "% ) \n");
                }
            }
            return successorsIDs;
        }

        QString checkForPredessorPattern()
        {
            QString predecessorsIDs = "";
            QMap<int,int>::iterator i;
            for (i = predecessoreId_Counter_Map_.begin(); i != predecessoreId_Counter_Map_.end(); ++i)
            {
                if(i.value() >= 2 && (patternPercentage(i.value()) > 50.0))
                    predecessorsIDs.append(QString::number(i.key()) + " ) , the exact occurrence of this Pattern is ( " +  QString::number(patternPercentage(i.value())) + "% ) \n");
            }
            return predecessorsIDs;
        }

        float patternPercentage(int neighbourCount)
        {
            return ((float)neighbourCount/(float)counter_) * 100;
        }
    };
}
#endif //SCENE_CALLDYNAMICANALYSIS_H
