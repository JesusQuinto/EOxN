#include "mainwindow.h"

#include <QApplication>
#include<QLoggingCategory>

///
/// \brief Main function of this application.
///
/// \param [in] argc    The number of strings pointed to by argv.
/// \param [in] argv    Name of the programm + misc.
///
/// \return             0 if the application ran successfully, otherwise an error code.
///
int main(int argc, char *argv[])
{
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
    // create application
    QApplication app(argc, argv);
    app.setOrganizationName("Visual.Soft");
    app.setApplicationName("EOxN");

    // create the main window and enter the main execution loop
    MainWindow window;
    window.show();
    int result = app.exec();

    return result;
}
