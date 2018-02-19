//
// Created by moe on 06.02.18.
//

#ifndef SCENE_CALLSTATICANALYSIS_H
#define SCENE_CALLSTATICANALYSIS_H


#include <QtCore/QString>

namespace moe {

    struct CallStaticAnalysis {
        int callId_, eid_ , totalCount_, totalOfLowerThanMicroSeconds_, totalOflowerThan10MicroSeconds_;
        QString callName_;
        QString analysisText_;
        bool warning = false;

        virtual void generateAnalysisText() = 0;
    };

    struct ECallStaticAnalysis : public CallStaticAnalysis {
        bool shouldBePrivate_;

        virtual void generateAnalysisText()
        {
            qreal underOneMicroPercent = 0;
            qreal underTenMicroPercent = 0;
            QString text = "";
            if(totalOfLowerThanMicroSeconds_ > 0)
            {
                underOneMicroPercent = ((qreal)totalOfLowerThanMicroSeconds_/(qreal)totalCount_) * 100;
                text.append(QString::number(totalOfLowerThanMicroSeconds_) + " ( " + QString::number(underOneMicroPercent)
                            + " %) of this ecall's Total count ( " + QString::number(totalCount_) + " ) are under 1µs (micro second).");
                //ToDo ask nico what are the proper percent to show a warning
                underOneMicroPercent > 15 ? text.append(" ( Warning ) \n")  : text.append("\n");
                underOneMicroPercent > 15 ? warning = true : warning = false;
                //ToDo ad here later a check for the synchro ocalls to add a proper warning to change them to spin locks for later :)
            }
            if(totalOflowerThan10MicroSeconds_ > totalOfLowerThanMicroSeconds_)
            {
                underTenMicroPercent = ((qreal)totalOflowerThan10MicroSeconds_/(qreal)totalCount_) * 100;
                text.append(QString::number(totalOflowerThan10MicroSeconds_) + " ( " + QString::number(underTenMicroPercent)
                            + " %) of this ecall's Total count ( " + QString::number(totalCount_) + " ) are under 10µs (micro seconds).");
                underTenMicroPercent > 35 ? text.append(" ( Warning ) \n") : text.append("\n");
                underTenMicroPercent > 35 ? warning = true : warning = false;
            }

            if(underOneMicroPercent || underTenMicroPercent)
            {
                analysisText_ = text;
            } else {
                analysisText_ = "Nothing found. Status Ok";
            }

            if(shouldBePrivate_)
            {
                analysisText_.append("\n (Hint: This ECall is only called inside OCalls, Why not make it private :) )");
            }
        }
    };

    //ToDo later for warning massages in red (but first we should change the CallStaticAnalysis to abstract class and reimplement the generate method)
    //ToDo ask nico if ecalls needs to be tested for under 1µs and10 µs
    struct OCallStaticAnalysis : public CallStaticAnalysis {

        virtual void generateAnalysisText()
        {
            qreal underOneMicroPercent = 0;
            qreal underTenMicroPercent = 0;
            QString text = "";
            if(totalOfLowerThanMicroSeconds_ > 0)
            {
                underOneMicroPercent = ((qreal)totalOfLowerThanMicroSeconds_/(qreal)totalCount_) * 100;
                text.append(QString::number(totalOfLowerThanMicroSeconds_) + " ( " + QString::number(underOneMicroPercent)
                            + " %) of this ocall's Total count ( " + QString::number(totalCount_) + " ) are under 1µs (micro second).");
                //ToDo ask nico what are the proper percent to show a warning
                underOneMicroPercent > 15 ? text.append(" ( Warning ) \n")  : text.append("\n");
                underOneMicroPercent > 15 ? warning = true : warning = false;
                //ToDo ad here later a check for the synchro ocalls to add a proper warning to change them to spin locks for later :)
            }
            if(totalOflowerThan10MicroSeconds_ > totalOfLowerThanMicroSeconds_)
            {
                underTenMicroPercent = ((qreal)totalOflowerThan10MicroSeconds_/(qreal)totalCount_) * 100;
                text.append(QString::number(totalOflowerThan10MicroSeconds_) + " ( " + QString::number(underTenMicroPercent)
                            + " %) of this ocall's Total count ( " + QString::number(totalCount_) + " ) are under 10µs (micro seconds).");
                underTenMicroPercent > 35 ? text.append(" ( Warning ) \n") : text.append("\n");
                underTenMicroPercent > 35 ? warning = true : warning = false;
            }

            if(underOneMicroPercent || underTenMicroPercent)
            {
                analysisText_ = text;
            } else {
                analysisText_ = "Nothing found. Status Ok";
            }

        }
    };
}
#endif //SCENE_CALLSTATICANALYSIS_H
