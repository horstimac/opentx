#ifndef AUTOCOMBOBOX_H_
#define AUTOCOMBOBOX_H_

#include <QComboBox>
#include "genericpanel.h"

class AutoComboBox: public QComboBox
{
  Q_OBJECT

  public:
    explicit AutoComboBox(QWidget *parent = 0):
      QComboBox(parent),
      field(NULL),
      panel(NULL),
      next(0),
      lock(false)
    {
      connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
    }

    void clear()
    {
      lock = true;
      QComboBox::clear();
      next = 0;
      lock = false;
    }

    virtual void insertItems(int index, const QStringList & items)
    {
      foreach(QString item, items) {
        addItem(item);
      }
    }

    virtual void addItem(const QString & item)
    {
      addItem(item, next++);
    }

    virtual void addItem(const QString & item, int value)
    {
      lock = true;
      QComboBox::addItem(item, value);
      if (field && *field==value) {
        setCurrentIndex(count()-1);
      }
      lock = false;
    }

    void setField(unsigned int & field, GenericPanel * panel=NULL)
    {
      this->field = (int *)&field;
      this->panel = panel;
      for (int i=0; i<count(); ++i) {
        if ((int)field == itemData(i))
          setCurrentIndex(i);
      }
    }

    void setField(int & field, GenericPanel * panel=NULL)
    {
      this->field = &field;
      this->panel = panel;
      for (int i=0; i<count(); ++i) {
        if ((int)field == itemData(i))
          setCurrentIndex(i);
      }
    }

    void setAutoIndexes()
    {
      for (int i=0; i<count(); ++i) {
        setItemData(i, i);
        if (*this->field == i)
          setCurrentIndex(i);
      }
    }

  protected slots:
    void onCurrentIndexChanged(int index)
    {
      if (field && !lock) {
        *field = itemData(index).toInt();
        if (panel) {
          emit panel->modified();
        }
      }
    }

  protected:
    int * field;
    GenericPanel * panel;
    int next;
    bool lock;
};

#endif /* AUTOCOMBOBOX_H_ */
