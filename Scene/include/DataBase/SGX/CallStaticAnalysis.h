//
// Created by moe on 06.02.18.
//

#ifndef SCENE_CALLSTATICANALYSIS_H
#define SCENE_CALLSTATICANALYSIS_H


#include <QtCore/QString>

namespace moe {

    struct CallStaticAnalysis {
        int callId_, totalCount_, totalOfLowerThanMicroSeconds_, totalOflowerThan10MicroSeconds_;
        QString callName_;
        QString analysisText_;

        void generateAnalysisText()
        {

            qreal underOneMicroPercent = 0;
            qreal underTenMicroPercent = 0;
            QString text = "";
            if(totalOfLowerThanMicroSeconds_ > 0)
            {
                underOneMicroPercent = ((qreal)totalOfLowerThanMicroSeconds_/(qreal)totalCount_) * 100;
                text.append(QString::number(totalOfLowerThanMicroSeconds_) + " ( " + QString::number(underOneMicroPercent)
                + " %) of this call's Total count ( " + QString::number(totalCount_) + " ) are under 1µs (micro second).");
                //ToDo ask nico what are the proper percent to show a warning
                underOneMicroPercent > 15 ? text.append(" ( Notice me Plz :'(  ) \n")  : text.append("\n");
                //ToDo ad here later a check for the synchro ocalls to add a proper warning to change them to spin locks for later :)
            }
            if(totalOflowerThan10MicroSeconds_ > totalOfLowerThanMicroSeconds_)
            {
                underTenMicroPercent = ((qreal)totalOflowerThan10MicroSeconds_/(qreal)totalCount_) * 100;
                text.append(QString::number(totalOflowerThan10MicroSeconds_) + " ( " + QString::number(underTenMicroPercent)
                + " %) of this call's Total count ( " + QString::number(totalCount_) + " ) are under 10µs (micro seconds).");
                underTenMicroPercent > 35 ? text.append(" ( Notice me plz :'(  ) \n") : text.append("\n");
            }

            if(underOneMicroPercent || underTenMicroPercent)
            {
                analysisText_ = text;
            } else {
                analysisText_ = " This Call has from Total of ( " + QString::number(totalCount_) + " ) neither under 1µs nor 10µs instance-s \n";
            }
        }
    };
}
#endif //SCENE_CALLSTATICANALYSIS_H
