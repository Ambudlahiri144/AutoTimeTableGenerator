#ifndef MYCHECKBOX_H
#define MYCHECKBOX_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>


class SelectorDialog;

/**
 * @brief The Mycheckbox class used for changing state on checkboxes and store some variables
 */
class MyCheckBox : public QCheckBox
{
    Q_OBJECT

public:
    /**
     * @brief MyCheckBox constructor
     * @param x is number of days
     * @param y is number of pairs
     */
    MyCheckBox(int x, int y);
    int x;
    int y;
    SelectorDialog* parentClass;

public slots:
    /**
     * @brief onStateChanged2 function change state on checkboxes
     * @param state is bool param for chechbox
     */
    void onStateChanged2(int state);

};

#endif // MYCHECKBOX_H

