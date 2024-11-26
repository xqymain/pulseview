/*
 * This file is part of the PulseView project.
 *
 * Copyright (C) 2012-2013 Joel Holdsworth <joel@airwebreathe.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>

#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QThread>
#include <QTemporaryFile>

#include "execute.hpp"


namespace pv {
namespace dialogs {

Execute::Execute(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(tr("Execute PCAPNG Unpack"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *paramLabel = new QLabel("Channels: ", this);
    paramEdit = new QLineEdit(this);
    mainLayout->addWidget(paramLabel);
    mainLayout->addWidget(paramEdit);

    QLabel *fileLabel = new QLabel("PCAPNG File: ", this);
    QHBoxLayout *fileLayout = new QHBoxLayout();
    fileEdit = new QLineEdit(this);
    QPushButton *browseButton = new QPushButton("Browse", this);
    fileLayout->addWidget(fileEdit);
    fileLayout->addWidget(browseButton);
    mainLayout->addWidget(fileLabel);
    mainLayout->addLayout(fileLayout);

    QPushButton *executeButton = new QPushButton("Execute", this);
    mainLayout->addWidget(executeButton);

    connect(browseButton, &QPushButton::clicked, this, &Execute::browseFile);
    connect(executeButton, &QPushButton::clicked, this, &Execute::executeOperation);
}


void Execute::browseFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select PCAPNG File", "", "PCAPNGfile (*.pcapng)");
    if (!filePath.isEmpty()) {
        fileEdit->setText(filePath);
    }
}

void Execute::executeOperation() {
    QString parameter = paramEdit->text();
    QString filePath = fileEdit->text();

    QFile resourceFile(":/script.py");
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QTemporaryFile tempFile;
    if (tempFile.open()) {
        tempFile.write(resourceFile.readAll());
        tempFile.flush();
    } else {
        return;
    }
    QString scriptPath = tempFile.fileName();
    QString binPath = (QFileInfo(filePath).absolutePath() + 
                        "/Gen_Channels" + parameter + ".bin");

    QMessageBox msg(this);
	msg.setStandardButtons(QMessageBox::Ok);

	QString pythonInterpreter = "python";
    QStringList arguments;
    
    arguments << scriptPath << filePath << binPath << parameter;
    
	if (!QProcess::startDetached(pythonInterpreter, arguments)) {
        msg.setText("Script Error\n\nThe script has not been started.");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
    }
    else {
        QMessageBox waitBox(this);
        waitBox.setWindowTitle("Processing");
        waitBox.setText("Script is running, please wait...");
        waitBox.setStandardButtons(QMessageBox::NoButton);
        waitBox.setIcon(QMessageBox::Information);
        waitBox.show();

        QThread::sleep(50);

        waitBox.accept();
	    msg.setText("Script Finished\n\nBinary file has started to be generated.");
        msg.setIcon(QMessageBox::Information);
        msg.exec();
    }

    accept();
}

} // namespace dialogs
} // namespace pv
