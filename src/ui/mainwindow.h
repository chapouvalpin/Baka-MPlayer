#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QTimer>
#include <QTranslator>
#include <QHash>
#include <QAction>

#if defined(Q_OS_WIN)
#include <QWinThumbnailToolBar>
#include <QWinThumbnailToolButton>
#endif

namespace Ui {
class MainWindow;
}

class KuroEngine;
class MpvHandler;

class MainWindow : public QMainWindow
{
friend class KuroEngine;
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getLang()          { return lang; }
    QString getOnTop()         { return onTop; }
    int getAutoFit()           { return autoFit; }
    bool getHidePopup()        { return hidePopup; }
    bool getRemaining()        { return remaining; }
    bool getScreenshotDialog() { return screenshotDialog; }
    bool getDebug()            { return debug; }
    bool getGestures()         { return gestures; }

    Ui::MainWindow  *ui;
    QImage albumArt;
public slots:
    void Load(QString f = QString());
    void MapShortcuts();

protected:
    void dragEnterEvent(QDragEnterEvent *event);    // drag file into
    void dropEvent(QDropEvent *event);              // drop file into
    void mousePressEvent(QMouseEvent *event);       // pressed mouse down
    void mouseReleaseEvent(QMouseEvent *event);     // released mouse up
    void mouseMoveEvent(QMouseEvent *event);        // moved mouse on the form
    void mouseDoubleClickEvent(QMouseEvent *event); // double clicked the form
    bool eventFilter(QObject *obj, QEvent *event);  // event filter (get mouse move events from mpvFrame)
    void wheelEvent(QWheelEvent *event);            // the mouse wheel is used
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void SetIndexLabels(bool enable);
    void SetPlaybackControls(bool enable);          // macro to enable/disable playback controls
    void TogglePlaylist();                          // toggles playlist visibility
    bool isPlaylistVisible();                       // is the playlist visible?

private slots:
    void FullScreen(bool fs);                       // makes window fullscreen
    void ShowPlaylist(bool visible);                // sets the playlist visibility
    void HideAlbumArt(bool hide);                   // hides the album art
    void UpdateRecentFiles();                       // populate recentFiles menu
    void SetPlayButtonIcon(bool play);
    void SetNextButtonEnabled(bool enable);
    void SetPreviousButtonEnabled(bool enable);

private:
    KuroEngine      *kuro;
    MpvHandler      *mpv;

#if defined(Q_OS_WIN)
    QWinThumbnailToolBar    *thumbnail_toolbar;
    QWinThumbnailToolButton *prev_toolbutton,
                            *playpause_toolbutton,
                            *next_toolbutton;
#endif
    bool            pathChanged,
                    menuVisible,
                    firstItem       = false,
                    init            = false;
    QTimer          *autohide       = nullptr;

    // variables
    QStringList recent;
    QString lang,
            onTop;
    int autoFit,
        maxRecent;
    bool hidePopup,
         remaining,
         screenshotDialog,
         debug,
         gestures;

    QHash<QString, QAction*> commandActionMap;

public slots:
    void setLang(QString s)          { emit langChanged(lang = s); }
    void setOnTop(QString s)         { emit onTopChanged(onTop = s); }
    void setAutoFit(int b)           { emit autoFitChanged(autoFit = b); }
    void setHidePopup(bool b)        { emit hidePopupChanged(hidePopup = b); }
    void setRemaining(bool b)        { emit remainingChanged(remaining = b); }
    void setScreenshotDialog(bool b) { emit screenshotDialogChanged(screenshotDialog = b); }
    void setDebug(bool b)            { emit debugChanged(debug = b); }
    void setGestures(bool b)         { emit gesturesChanged(gestures = b); }

signals:
    void langChanged(QString);
    void onTopChanged(QString);
    void autoFitChanged(int);
    void hidePopupChanged(bool);
    void remainingChanged(bool);
    void screenshotDialogChanged(bool);
    void debugChanged(bool);
    void gesturesChanged(bool);
};

#endif // MAINWINDOW_H
