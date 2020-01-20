#include "login.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QScreen>
#include <QLockFile>
#include  <QDir>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLockFile lockFile(QDir::tempPath()+"/locker_lock.lock");

    if(!lockFile.tryLock(100)){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("El programa ya se encuentra en ejecución");
        msgBox.exec();
        return 1;
    }

    QPixmap pixmap(":/images/images/camin_real_image.jpg");

    //Get screen Size
   const auto screens = qApp->screens();

   double width = screens[0]->geometry().width()*0.45;
   double height = screens[0]->geometry().height()*0.4;

    QSplashScreen splash(pixmap.scaled( static_cast<int>(width),static_cast<int>(height), Qt::KeepAspectRatio, Qt::SmoothTransformation), Qt::WindowStaysOnTopHint);
    splash.show();

    QTimer::singleShot(1000, &splash, &QWidget::close);

    Login w;

    QIcon icon(":/images/images/icono.ico");
    w.setWindowIcon(icon);

    w.show();
    return a.exec();
}
