#include "login.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/images/images/camin_real_image.jpg");

    //Get screen Size
   const auto screens = qApp->screens();

   double width = screens[0]->geometry().width()*0.3;
   double height = screens[0]->geometry().height()*0.3;

    QSplashScreen splash(pixmap.scaled( static_cast<int>(width),static_cast<int>(height), Qt::KeepAspectRatio, Qt::SmoothTransformation), Qt::WindowStaysOnTopHint);
    splash.show();

    QTimer::singleShot(1000, &splash, &QWidget::close);

    Login w;
    w.show();
    return a.exec();
}


