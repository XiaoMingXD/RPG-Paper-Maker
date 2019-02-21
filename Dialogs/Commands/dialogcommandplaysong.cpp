/*
    RPG Paper Maker Copyright (C) 2017-2019 Marie Laporte

    This file is part of RPG Paper Maker.

    RPG Paper Maker is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RPG Paper Maker is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dialogcommandplaysong.h"
#include "ui_dialogcommandplaysong.h"
#include "rpm.h"

// -------------------------------------------------------
//
//  CONSTRUCTOR / DESTRUCTOR / GET / SET
//
// -------------------------------------------------------

DialogCommandPlaySong::DialogCommandPlaySong(QString title, SongKind kind,
    EventCommand *command, SystemCommonObject *object, QStandardItemModel
    *parameters, QWidget *parent) :
    DialogCommand(parent),
    m_kind(kind),
    m_song(nullptr),
    ui(new Ui::DialogCommandPlaySong)
{
    ui->setupUi(this);
    
    this->setWindowTitle(title);
    ui->widget->setSongKind(kind);
    ui->widget->initializePrimitives(parameters, nullptr);
    ui->panelPrimitiveValueSongID->initializeNumber(parameters, nullptr);

    if (command != nullptr) {
        initialize(command);
    }
}

DialogCommandPlaySong::DialogCommandPlaySong(QString title, SystemPlaySong
    *song) :
    DialogCommandPlaySong(title, song->kind())
{
    EventCommand command;
    EventCommandKind commandKind;

    m_song = song;
    switch (m_kind) {
    case SongKind::Music:
        commandKind = EventCommandKind::PlayMusic;
        break;
    case SongKind::BackgroundSound:
        commandKind = EventCommandKind::PlayBackgroundSound;
        break;
    case SongKind::Sound:
        commandKind = EventCommandKind::PlayASound;
        break;
    case SongKind::MusicEffect:
        commandKind = EventCommandKind::PlayMusicEffect;
        break;
    default:
        commandKind = EventCommandKind::None;
    }

    song->toEventCommand(command, commandKind);
    initialize(&command);
}

DialogCommandPlaySong::~DialogCommandPlaySong() {
    delete ui;
}

EventCommandKind DialogCommandPlaySong::getCommandKind() const {
    switch(m_kind) {
    case SongKind::Music:
        return EventCommandKind::PlayMusic;
    case SongKind::BackgroundSound:
        return EventCommandKind::PlayBackgroundSound;
    case SongKind::Sound:
        return EventCommandKind::PlayASound;
    case SongKind::MusicEffect:
        return EventCommandKind::PlayMusicEffect;
    case SongKind::None:
        return EventCommandKind::None;
    }

    return EventCommandKind::None;
}

// -------------------------------------------------------
//
//  INTERMEDIARY FUNCTIONS
//
// -------------------------------------------------------

void DialogCommandPlaySong::initialize(EventCommand* command) {
    int i = 0;

    ui->checkBoxSongID->setChecked(command->valueCommandAt(i++) == "1");
    ui->panelPrimitiveValueSongID->initializeCommand(command, i);
    ui->widget->initialize(command, i);
}

// -------------------------------------------------------

EventCommand* DialogCommandPlaySong::getCommand() const {
    QVector<QString> command;
    command.append(ui->checkBoxSongID->isChecked() ? "1" : "0");
    ui->panelPrimitiveValueSongID->getCommand(command);
    ui->widget->getCommand(command);

    return new EventCommand(getCommandKind(), command);
}

// -------------------------------------------------------
//
//  SLOTS
//
// -------------------------------------------------------

void DialogCommandPlaySong::accept() {
    RPM::get()->project()->writeSongsDatas();

    if (m_song != nullptr) {
        m_song->setIsSelectedByID(ui->checkBoxSongID->isChecked());
        m_song->valueID()->setCopy(*ui->panelPrimitiveValueSongID->model());
        ui->widget->updatePlaySong(m_song);
        m_song->updateName();
    }

    QDialog::accept();
}

// -------------------------------------------------------

void DialogCommandPlaySong::reject() {
    RPM::get()->project()->readSongsDatas();
    QDialog::reject();
}

// -------------------------------------------------------

void DialogCommandPlaySong::on_checkBoxSongID_toggled(bool checked) {
    ui->panelPrimitiveValueSongID->setEnabled(checked);
}
