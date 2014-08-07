#include "customslider.h"

#include <QTime>
#include <QToolTip>

CustomSlider::CustomSlider(QWidget *parent):
    QSlider(parent),
    totalTime(0)
{
}

void CustomSlider::setTracking(int _totalTime)
{
    if(_totalTime != 0)
    {
        totalTime = _totalTime;
        setMouseTracking(true);
    }
    else
        setMouseTracking(false);
}

void CustomSlider::setValueNoSignal(int value)
{
    this->blockSignals(true);
    this->setValue(value);
    this->blockSignals(false);
}

QString CustomSlider::formatTrackingTime(int _time)
{
    QTime time = QTime::fromMSecsSinceStartOfDay((int)(((double)_time/maximum())*totalTime) * 1000);
    if(time.hour() > 0)
        return time.toString("h:mm:ss");
    if(time.minute() > 0)
        return time.toString("mm:ss");
    return time.toString("0:ss");
}

void CustomSlider::mouseMoveEvent(QMouseEvent* event)
{
    if(totalTime != 0)
    {
        // Thanks to cmannett85: http://stackoverflow.com/questions/12417636/qt-show-mouse-position-like-tooltip
        //  and spyke (see below) for initial code
        if (orientation() == Qt::Vertical)
        {
            QToolTip::showText(event->globalPos(),
                               formatTrackingTime(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height()),
                               this, rect());
        }
        else
        {
            QToolTip::showText(event->globalPos(),
                               formatTrackingTime(minimum() + ((maximum()-minimum()) * event->x()) / width()),
                               this, rect());
        }
    }
    QSlider::mouseMoveEvent(event);
}

void CustomSlider::mousePressEvent(QMouseEvent *event)
{
  // Thanks to spyke: http://stackoverflow.com/questions/11132597/qslider-mouse-direct-jump
  if (event->button() == Qt::LeftButton)
  {
      if (orientation() == Qt::Vertical)
          setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height()) ;
      else
          setValue(minimum() + ((maximum()-minimum()) * event->x()) / width()) ;

      event->accept();
  }
  QSlider::mousePressEvent(event);
}