#ifndef DETAILWINDOW_H
#define DETAILWINDOW_H

#include <QDialog>
#include <QSplitter>
#include "mainctrl.h"

class detailwindow : public QDialog
{
    Q_OBJECT
public:
    explicit detailwindow(QWidget *parent = nullptr);

private:
    ///
    /// \brief Main controller used for controlling the nodes (both visual and logical) of the graph.
    ///
    MainCtrl* m_mainCtrl;

    ///
    /// \brief Main splitter between the Zodiac Graph and the Property editor.
    ///
    QSplitter* m_mainSplitter;

};

#endif // DETAILWINDOW_H
