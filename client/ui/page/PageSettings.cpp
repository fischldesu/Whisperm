#include "ui/PageSettings.h"
#include "ui_PageSettings.h"

#include <QDesktopServices>
#include <QCheckBox>
#include "utils/Multiplatform.hpp"
#include "uicomponent/Panel.h"

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
    ui->btn_github->set_bgColor(Button::Hover, {0,170,255,64});
    ui->btn_website->set_bgColor(Button::Hover, {0,170,255,64});
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
    const auto panel1 = new Panel(this);
    const auto checker1 = new QCheckBox(panel1);
    checker1->setText(" ");
    panel1->set_Controler(checker1);
    panel1->set_Content({"在系统后台运行程序", "窗口关闭时不退出程序，并在系统托盘显示图标"});
    this->AddComponent(panel1);

    const auto panel2 = new Panel(this);
    const auto checker2 = new QCheckBox(panel2);
    checker2->setText(" ");
    panel2->set_Controler(checker2);
    panel2->set_Content({"不使用背景样式", "禁用窗口背景样式以提高性能"});
    this->AddComponent(panel2);
}

void PageSettings::AddComponent(Panel* component)
{
    const auto index = ui->contentLayout->indexOf(ui->vspacer);
    ui->contentLayout->insertWidget(index, component);
}
