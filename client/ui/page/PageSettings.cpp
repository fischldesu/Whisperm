#include "ui/PageSettings.h"
#include "ui_PageSettings.h"

#include <QDesktopServices>
#include "tools/multiplatform.hpp"

PageSettings::PageSettings(QWidget *parent) :
    QWidget(parent), ui(new Ui::PageSettings) {
    ui->setupUi(this);

    //template: Whisperm v0.0.1(alpha) Windows x64
    const QString content = QApplication::applicationName() + " " + QApplication::applicationVersion() + " " + System::GetOSName();
    //template: MinGW-W64 13.1.0 / Qt6.7.1
    const QString buildinfo = System::GetCompilerName() + " / Qt" + QT_VERSION_STR;
    ui->label_about_content->setText(content);
    ui->label_about_buildinfo->setText(buildinfo);
    ui->label_about_buildinfo->setToolTip("Build-Info " + buildinfo);

    //buttons
    ui->btn_github->set_bgRadius(8);
    ui->btn_website->set_bgRadius(8);
    ui->btn_github->set_bgHoverColor({0,170,255,64});
    ui->btn_website->set_bgHoverColor({0,170,255,64});
    connect(ui->btn_github, &QPushButton::clicked, []
        { QDesktopServices::openUrl(QUrl("https://github.com/fischldesu/Whisperm"));});
    connect(ui->btn_website, &QPushButton::clicked, []
        { QDesktopServices::openUrl(QUrl("https://fischldesu.com"));});

    this->InitialzieComponents();
}

PageSettings::~PageSettings() {
    delete ui;
}

void PageSettings::InitialzieComponents()
{

}

void PageSettings::AddCompoents(SettingsComponent* component)
{
    const auto index = ui->contentLayout->indexOf(ui->vspacer);
    // ui->contentLayout->insertWidget(index, component);
}

