#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication application(argc, argv);
   application.setOrganizationName("BachelorArbeit IBR");
   application.setApplicationName("SGX AnalysisTool");
   MainWindow window;
   window.resize(QApplication::desktop()->screen()->rect().width(), QApplication::desktop()->screen()->rect().height());
   window.move(QApplication::desktop()->screen()->rect().center()- window.rect().center());
   window.show();

   return application.exec();



}
