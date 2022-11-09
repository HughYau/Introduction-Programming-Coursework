#ifndef ICON_H
#define ICON_H
#include<QObject>
#include<QProxyStyle>


class QTMyIconStyle : public QProxyStyle
{
    Q_OBJECT
public:
    QTMyIconStyle (){}
    virtual ~QTMyIconStyle (){}
    virtual int pixelMetric(PixelMetric metric, const QStyleOption * option, const QWidget * widget) const;
};


#endif // ICON_H
