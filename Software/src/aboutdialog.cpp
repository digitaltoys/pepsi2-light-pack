/* 
 * aboutdialog.cpp
 *
 *  Created on: 26.07.2010
 *      Author: Mike Shatohin (brunql)
 *     Project: Lightpack
 *
 *  Lightpack is very simple implementation of the backlight for a laptop
 *
 *  Copyright (c) 2010, 2011 Mike Shatohin, mikeshatohin [at] gmail.com
 *
 *  Lightpack is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Lightpack is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "version.h"

#include <QtDebug>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint );

    versionTemplatesUpdate();

    softwareVersionUpdate();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        // Retranslate UI and update templates for new locale
        versionTemplatesUpdate();
        // Fill templates with software version and revision and show it in dialog
        softwareVersionUpdate();
        break;
    default:
        break;
    }
}

void AboutDialog::closeEvent(QCloseEvent *event)
{    
    this->hide();
    event->ignore();
}


void AboutDialog::versionTemplatesUpdate()
{
    ui->retranslateUi(this);

    // Save templetes for construct version strings
    softwareVersionTemplate     = ui->labelVersionSoftware->text();
    softwareRevisionTemplate    = ui->labelRevisionSoftware->text();
    fimwareVersionTemplate      = ui->labelVersionFirmware->text();
}


void AboutDialog::setFirmwareVersion(const QString &firmwareVersion)
{
    ui->labelVersionFirmware->setText( fimwareVersionTemplate.arg( firmwareVersion ) );
}


void AboutDialog::softwareVersionUpdate()
{
    ui->labelVersionSoftware->setText( softwareVersionTemplate.arg( QApplication::applicationVersion() ) );

#ifdef HG_REVISION
    ui->labelRevisionSoftware->setText( softwareRevisionTemplate.arg( HG_REVISION ) );
#else
    ui->labelRevisionSoftware->hide();
#endif

    adjustSize();

    setFixedSize( sizeHint() );
}
