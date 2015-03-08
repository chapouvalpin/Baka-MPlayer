#include "playlistwidget.h"

#include "kuroengine.h"
#include "mpvhandler.h"

#include <QListWidgetItem>
#include <QMenu>
#include <QFont>

#include <algorithm> // for std::random_shuffle and std::sort

PlaylistWidget::PlaylistWidget(QWidget *parent) :
    QListWidget(parent),
    newPlaylist(false),
    refresh(false),
    showAll(true)
{
    setAttribute(Qt::WA_NoMousePropagation);
}

void PlaylistWidget::AttachEngine(KuroEngine *kuro)
{
    this->kuro = kuro;
    connect(kuro->mpv, &MpvHandler::playlistChanged,
            [=](const QStringList &list)
            {
                playlist = list;
                newPlaylist = true;
                if(refresh)
                {
                    Populate();
                    BoldText(file, true);
                    refresh = false;
                }
            });

    connect(kuro->mpv, &MpvHandler::fileChanged,
            [=](QString f)
            {
                if(newPlaylist)
                {
                    file = f;
                    if(f != QString())
                        suffix = file.split('.').last();
                    Populate();
                    BoldText(file, true);
                    newPlaylist = false;
                }
                else
                {
                    BoldText(file, false);
                    BoldText(f, true);
                    file = f;
                }
                SelectIndex(CurrentIndex());
            });

    connect(this, &PlaylistWidget::doubleClicked,
            [=](const QModelIndex &i)
            {
                PlayIndex(i.row());
            });
}

void PlaylistWidget::Populate()
{
    if(playlist.empty())
        return;

    QListWidgetItem *current = currentItem();
    QString item;
    if(current != nullptr)
        item = current->text();
    else
        item = file;

    if(showAll == true)
    {
        clear();
        addItems(playlist);
    }
    else
    {
        // filter by suffix
        QStringList newPlaylist;
        for(auto i = playlist.begin(); i != playlist.end(); ++i)
            if(i->endsWith(suffix))
                newPlaylist.append(*i);
        // load
        clear();
        addItems(newPlaylist);
    }
    SelectItem(item);
}

void PlaylistWidget::RefreshPlaylist()
{
    refresh = true;
    kuro->mpv->LoadPlaylist(kuro->mpv->getPath()+file);
}

QString PlaylistWidget::CurrentItem()
{
    QListWidgetItem *current = currentItem();
    if(current != nullptr)
        return current->text();
    return QString();
}

int PlaylistWidget::CurrentIndex()
{
    auto items = findItems(file, Qt::MatchExactly);
    if(!items.empty())
        return indexFromItem(items.first()).row();
    return 0;
}

void PlaylistWidget::SelectIndex(int index, bool relative)
{
    int newIndex = relative ? currentRow() : 0;
    if(newIndex < 0)
        newIndex = 0;

    newIndex += index;

    if(newIndex < 0)
        newIndex = 0;
    else if(newIndex > count())
        newIndex = count();

    setCurrentRow(newIndex);
    scrollToItem(currentItem());
}

void PlaylistWidget::PlayIndex(int index, bool relative)
{
    int newIndex = 0;
    if(relative)
    {
        auto items = findItems(file, Qt::MatchExactly);
        if(!items.empty())
            newIndex = indexFromItem(items.first()).row();
    }
    newIndex += index;

    if(newIndex < 0)
        newIndex = 0;
    else if(newIndex > count())
        newIndex = count();

    QListWidgetItem *current = item(newIndex);
    if(current != nullptr)
    {
        kuro->mpv->PlayFile(current->text());
        scrollToItem(current);
    }
}

void PlaylistWidget::RemoveIndex(int index)
{
    if(index < 0)
        index = 0;
    else if(index > count())
        index = count();

    QListWidgetItem *current = item(index);
    if(current != nullptr)
        RemoveFromPlaylist(current);
}

void PlaylistWidget::BoldText(const QString &f, bool state)
{
    auto items = findItems(f, Qt::MatchExactly);
    if(items.empty())
        return;
    auto *item = items.first();
    if(item)
    {
        QFont font = item->font();
        font.setBold(state);
        item->setFont(font);
    }
}

void PlaylistWidget::Search(const QString &s)
{
    QListWidgetItem *current = currentItem();
    QString item;
    if(current != nullptr)
        item = current->text();
    else
        item = file;

    QStringList newPlaylist;
    for(QStringList::iterator item = playlist.begin(); item != playlist.end(); item++)
    {
        if(item->contains(s, Qt::CaseInsensitive) && (showAll || item->endsWith(suffix)))
            newPlaylist.append(*item);
    }

    clear();
    addItems(newPlaylist);

    BoldText(file, true);
    SelectItem(item);
}

void PlaylistWidget::ShowAll(bool b)
{
    QListWidgetItem *current = currentItem();
    QString item;
    if(current != nullptr)
        item = current->text();
    else
        item = file;

    showAll = b;
    Populate();

    BoldText(file, true);
    SelectItem(item);
}

void PlaylistWidget::Shuffle()
{
    if(this->count() == 0)
        return;

    QListWidgetItem *current = currentItem();
    QString item;
    if(current != nullptr)
        item = current->text();
    else
        item = file;

    QStringList newPlaylist;
    for(int i = 0; i < count(); ++i)
        newPlaylist.append(this->item(i)->text());

    std::random_shuffle(newPlaylist.begin(), newPlaylist.end());
    // make current playing item the first
    auto iter = std::find(newPlaylist.begin(), newPlaylist.end(), file);
    std::swap(*iter, *newPlaylist.begin());
    // load
    clear();
    addItems(newPlaylist);

    BoldText(file, true);
    SelectItem(item);
}

void PlaylistWidget::SelectItem(const QString &item)
{
    if(item != QString())
    {
        auto items = this->findItems(item, Qt::MatchExactly);
        if(!items.empty())
        {
            setCurrentItem(items.first());
            scrollToItem(items.first());
            return;
        }
    }
    setCurrentRow(0);
    scrollToItem(currentItem());
}

void PlaylistWidget::RemoveFromPlaylist(QListWidgetItem *item)
{
    playlist.removeOne(item->text());
    delete item;
    emit currentRowChanged(currentRow());
}

void PlaylistWidget::DeleteFromDisk(QListWidgetItem *item)
{
    playlist.removeOne(item->text());
    QFile f(kuro->mpv->getPath()+item->text());
    f.remove();
    delete item;
    emit currentRowChanged(currentRow());
}

void PlaylistWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QListWidgetItem *item = itemAt(event->pos());
    if(item != nullptr)
    {
        QMenu *menu = new QMenu();
        connect(menu->addAction(tr("R&emove from Playlist")), &QAction::triggered, // Playlist: Remove from playlist (right-click)
                [=]
                {
                    RemoveFromPlaylist(item);
                });
        connect(menu->addAction(tr("&Delete from Disk")), &QAction::triggered,     // Playlist: Delete from Disk (right-click)
                [=]
                {
                    DeleteFromDisk(item);
                });
        connect(menu->addAction(tr("&Refresh")), &QAction::triggered,              // Playlist: Refresh (right-click)
                [=]
                {
                    RefreshPlaylist();
                });
        menu->exec(viewport()->mapToGlobal(event->pos()));
        delete menu;
    }
}

void PlaylistWidget::dropEvent(QDropEvent *event)
{
    QListWidget::dropEvent(event);
    emit currentRowChanged(currentRow());
}
