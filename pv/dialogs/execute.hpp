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

#ifndef PULSEVIEW_PV_DIALOGS_EXECUTE_HPP
#define PULSEVIEW_PV_DIALOGS_EXECUTE_HPP

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>


namespace pv {


namespace dialogs {

class Execute : public QDialog
{
	Q_OBJECT

public:
	Execute(QWidget *parent);

private Q_SLOTS:
	void browseFile();
    void executeOperation();

private:
	QLineEdit *paramEdit;
    QLineEdit *fileEdit;
};

} // namespace dialogs
} // namespace pv

#endif // PULSEVIEW_PV_DIALOGS_EXECUTE_HPP
