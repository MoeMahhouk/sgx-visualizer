#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication application(argc, argv);
   application.setOrganizationName("BachelorArbeit");
   application.setApplicationName("SceneTest");
   MainWindow window;
   window.move(QApplication::desktop()->screen()->rect().center()- window.rect().center());
   window.show();

   return application.exec();



}
