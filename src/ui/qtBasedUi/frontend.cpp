/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "scatterdatamodifier.h"
#include "../frontend.h"

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QtGui/QFontDatabase>
#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets>

static inline QComboBox *createComboBox()
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

QT_CHARTS_USE_NAMESPACE

static inline QChartView *createChartView(const QSize &sz, const QString &title, const QString &yAxisName)
{
    QSplineSeries *series = new QSplineSeries();
//    QPen pen(QColor(Qt::green));
//    pen.setWidth(5);
//    series->setPen(pen);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle(title);
    chart->createDefaultAxes();
    chart->axisX()->setTitleText("t");
    chart->axisY()->setTitleText(yAxisName);
    QSize screenSize = QSize(sz.width() / 2, sz.height() / 2);
    chart->setMinimumSize(QSize(screenSize.width() / 3, screenSize.height() / 3));
    chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

int showUi(int argc, char **argv)
{
    //! [0]
    QApplication app(argc, argv);
    Q3DScatter *graph = new Q3DScatter();
    QWidget *container = QWidget::createWindowContainer(graph);
    //! [0]

    if (!graph->hasContext())
    {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return -1;
    }

    QGridLayout *chartsLayout = new QGridLayout;
    chartsLayout->addWidget(container, 0, 0);
    QChartView *qchartXt = createChartView(graph->screen()->size(), "x(t)", "x");
    chartsLayout->addWidget(qchartXt, 0, 1);
    QChartView *qchartYt = createChartView(graph->screen()->size(), "y(t)", "y");
    chartsLayout->addWidget(qchartYt, 1, 0);
    QChartView *qchartZt = createChartView(graph->screen()->size(), "z(t)", "z");
    chartsLayout->addWidget(qchartZt, 1, 1);

    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 3, screenSize.height() / 3));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    //! [1]
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addLayout(chartsLayout, 1);
    hLayout->addLayout(vLayout);
    //! [1]

    QGroupBox *configGroupBox = new QGroupBox(QStringLiteral("Lorenz equation configs"));
    QVBoxLayout *configGroupBoxLayout = new QVBoxLayout;
    std::vector<QComboBox *> lineEdits(8, nullptr);

    {

        // Warning! Do not change places of labels and boxes at arrays

        QLabel *labels[8];
        labels[0] = new QLabel("x0");
        labels[1] = new QLabel("y0");
        labels[2] = new QLabel("z0");
        labels[3] = new QLabel("sigma");
        labels[4] = new QLabel("r ( > 0)");
        labels[5] = new QLabel("b");
        labels[6] = new QLabel("dt");
        labels[7] = new QLabel("t_max");

        for (size_t i = 0; i != lineEdits.size(); ++i)
            lineEdits[i] = createComboBox();

        lineEdits[3]->setCurrentText("10");
        lineEdits[3]->lineEdit()->setReadOnly(true);

        lineEdits[5]->setCurrentText("2.666666666");
        lineEdits[5]->lineEdit()->setReadOnly(true);

        QGridLayout *gridLayout = new QGridLayout;

        for (int i = 0; i != 8; ++i)
        {
            gridLayout->addWidget(labels[i], i, 0);
            gridLayout->addWidget(lineEdits[static_cast<size_t>(i)], i, 1);
        }
        configGroupBoxLayout->addLayout(gridLayout);
    }

    std::vector<QRadioButton *> radioButtons(4, nullptr);
    {
        QGroupBox *selectionGroupBox = new QGroupBox(QStringLiteral("Choose Method"));

        radioButtons[0] = new QRadioButton(widget);
        radioButtons[0]->setText("Explicit Euler");
        radioButtons[0]->setChecked(false);

        radioButtons[1] = new QRadioButton(widget);
        radioButtons[1]->setText("Implicit Euler");
        radioButtons[1]->setChecked(false);

        radioButtons[2] = new QRadioButton(widget);
        radioButtons[2]->setText("Runge Kutta");
        radioButtons[2]->setChecked(false);

        radioButtons[3] = new QRadioButton(widget);
        radioButtons[3]->setText("Explicit Adams");
        radioButtons[3]->setChecked(false);

        QVBoxLayout *selectionBox = new QVBoxLayout;
        for (int i = 0; i != 4; ++i)
            selectionBox->addWidget(radioButtons[i]);
        selectionGroupBox->setLayout(selectionBox);
        configGroupBoxLayout->addWidget(selectionGroupBox);
    }

    QPushButton *replotButton = new QPushButton(widget);
    {
        QPalette pal = replotButton->palette();
        pal.setColor(QPalette::Button, QColor(Qt::green));
        replotButton->setText(QStringLiteral("Integrate"));
        replotButton->setAutoFillBackground(true);
        replotButton->setPalette(pal);

        QSize sz = replotButton->size();
        replotButton->setMinimumSize(sz.width() + 20, sz.height() + 20);

        configGroupBoxLayout->addWidget(replotButton);
    }

    configGroupBox->setLayout(configGroupBoxLayout);
    vLayout->addWidget(configGroupBox);

    widget->setWindowTitle(QStringLiteral("Lorenz Attractor"));

    QGroupBox *styleGroupBox = new QGroupBox(QStringLiteral("3D Chart Stle"));
    QVBoxLayout *styleBoxLayout = new QVBoxLayout;

    //! [4]
    QComboBox *themeList = new QComboBox(widget);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));
    themeList->setCurrentIndex(6);

    QPushButton *labelButton = new QPushButton(widget);
    labelButton->setText(QStringLiteral("Change label style"));

    QCheckBox *smoothCheckBox = new QCheckBox(widget);
    smoothCheckBox->setText(QStringLiteral("Smooth dots"));
    smoothCheckBox->setChecked(true);

    QComboBox *itemStyleList = new QComboBox(widget);
    itemStyleList->addItem(QStringLiteral("Sphere"), int(QAbstract3DSeries::MeshSphere));
    itemStyleList->addItem(QStringLiteral("Cube"), int(QAbstract3DSeries::MeshCube));
    itemStyleList->addItem(QStringLiteral("Minimal"), int(QAbstract3DSeries::MeshMinimal));
    itemStyleList->addItem(QStringLiteral("Point"), int(QAbstract3DSeries::MeshPoint));
    itemStyleList->setCurrentIndex(0);

    QPushButton *cameraButton = new QPushButton(widget);
    cameraButton->setText(QStringLiteral("Change camera preset"));

    QCheckBox *backgroundCheckBox = new QCheckBox(widget);
    backgroundCheckBox->setText(QStringLiteral("Show background"));
    backgroundCheckBox->setChecked(true);

    QCheckBox *gridCheckBox = new QCheckBox(widget);
    gridCheckBox->setText(QStringLiteral("Show grid"));
    gridCheckBox->setChecked(true);

    QComboBox *shadowQuality = new QComboBox(widget);
    shadowQuality->addItem(QStringLiteral("None"));
    shadowQuality->addItem(QStringLiteral("Low"));
    shadowQuality->addItem(QStringLiteral("Medium"));
    shadowQuality->addItem(QStringLiteral("High"));
    shadowQuality->addItem(QStringLiteral("Low Soft"));
    shadowQuality->addItem(QStringLiteral("Medium Soft"));
    shadowQuality->addItem(QStringLiteral("High Soft"));
    shadowQuality->setCurrentIndex(4);

    QFontComboBox *fontList = new QFontComboBox(widget);
    fontList->setCurrentFont(QFont("Arial"));
    //! [4]

    //! [5]
    styleBoxLayout->addWidget(labelButton, 0, Qt::AlignTop);
    styleBoxLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    styleBoxLayout->addWidget(backgroundCheckBox);
    styleBoxLayout->addWidget(gridCheckBox);
    styleBoxLayout->addWidget(smoothCheckBox, 0, Qt::AlignTop);
    styleBoxLayout->addWidget(new QLabel(QStringLiteral("Change dot style")));
    styleBoxLayout->addWidget(itemStyleList);
    styleBoxLayout->addWidget(new QLabel(QStringLiteral("Change theme")));
    styleBoxLayout->addWidget(themeList);
    styleBoxLayout->addWidget(new QLabel(QStringLiteral("Adjust shadow quality")));
    styleBoxLayout->addWidget(shadowQuality);
    styleBoxLayout->addWidget(new QLabel(QStringLiteral("Change font")));
    styleBoxLayout->addWidget(fontList, 1, Qt::AlignTop);
    styleGroupBox->setLayout(styleBoxLayout);
    vLayout->addWidget(styleGroupBox);
    //! [5]

    //! [2]
    ScatterDataModifier *modifier = new ScatterDataModifier(graph, lineEdits, radioButtons, qchartXt, qchartYt, qchartZt);
    //! [2]

    //! [6]
    QObject::connect(cameraButton, &QPushButton::clicked, modifier,
                     &ScatterDataModifier::changePresetCamera);
    QObject::connect(labelButton, &QPushButton::clicked, modifier,
                     &ScatterDataModifier::changeLabelStyle);

    QObject::connect(backgroundCheckBox, &QCheckBox::stateChanged, modifier,
                     &ScatterDataModifier::setBackgroundEnabled);
    QObject::connect(gridCheckBox, &QCheckBox::stateChanged, modifier,
                     &ScatterDataModifier::setGridEnabled);
    QObject::connect(smoothCheckBox, &QCheckBox::stateChanged, modifier,
                     &ScatterDataModifier::setSmoothDots);

    QObject::connect(modifier, &ScatterDataModifier::backgroundEnabledChanged, backgroundCheckBox,
                     &QCheckBox::setChecked);
    QObject::connect(modifier, &ScatterDataModifier::gridEnabledChanged, gridCheckBox,
                     &QCheckBox::setChecked);
    QObject::connect(itemStyleList, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeStyle(int)));

    QObject::connect(themeList, SIGNAL(currentIndexChanged(int)), modifier, SLOT(changeTheme(int)));

    QObject::connect(shadowQuality, SIGNAL(currentIndexChanged(int)), modifier,
                     SLOT(changeShadowQuality(int)));

    QObject::connect(modifier, &ScatterDataModifier::shadowQualityChanged, shadowQuality,
                     &QComboBox::setCurrentIndex);
    QObject::connect(graph, &Q3DScatter::shadowQualityChanged, modifier,
                     &ScatterDataModifier::shadowQualityUpdatedByVisual);

    QObject::connect(fontList, &QFontComboBox::currentFontChanged, modifier,
                     &ScatterDataModifier::changeFont);

    QObject::connect(modifier, &ScatterDataModifier::fontChanged, fontList,
                     &QFontComboBox::setCurrentFont);

    QObject::connect(replotButton, &QPushButton::clicked, modifier, &ScatterDataModifier::replot);

    //! [6]

    const QRect screenGeometry = QApplication::desktop()->screenGeometry();
    widget->resize(screenGeometry.width(), screenGeometry.height());

    //! [3]
    widget->show();
    return app.exec();
    //! [3]
}
