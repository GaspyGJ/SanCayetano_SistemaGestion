#ifndef STYLEITEM_H
#define STYLEITEM_H

#include "qapplication.h"
#include "qpainter.h"
#include <QStyledItemDelegate>

class StyleItem : public QStyledItemDelegate{
    //Clase para poder mejorar la tabla de Ventas. (q no aparezcan los "..." y demas )
public:
    StyleItem();    
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
     {
         if (!index.isValid())
             return;

         QStyleOptionViewItem opt = option;
         initStyleOption(&opt, index);
         int padding = 3;

         painter->save();
         painter->setClipRect(opt.rect);
         opt.rect = opt.rect.adjusted(padding, padding, -padding, -padding);
         painter->drawText(opt.rect, Qt::AlignCenter,
                           opt.fontMetrics.elidedText(opt.text, Qt::ElideRight,
                                                      opt.rect.width()));
         painter->restore();

         opt.rect = option.rect;
         opt.textElideMode = Qt::ElideNone;
         opt.text = "";

         QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);
     }
};

#endif // STYLEITEM_H
