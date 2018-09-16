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
#include "config.h"
#include "internal_method_names.h"
#include "show_method_names.h"
#include "solver.h"
#include <QMessageBox>
#include <QtCore/qmath.h>
#include <QtDataVisualization/q3dcamera.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dtheme.h>
#include <QtDataVisualization/qscatter3dseries.h>
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/qvalue3daxis.h>
#include <QtWidgets/QComboBox>

using namespace QtDataVisualization;

//#define RANDOM_SCATTER // Uncomment this to switch to random scatter

const int numberOfItems = 3600;
const float curveDivider = 3.0f;
const int lowerNumberOfItems = 900;
const float lowerCurveDivider = 0.75f;

ScatterDataModifier::ScatterDataModifier(Q3DScatter *scatter, const std::vector<QComboBox *> &boxes,
                                         const std::vector<QRadioButton *> &methodButtons,
                                         QChartView *depXt, QChartView *depYt, QChartView *depZt)
    : m_graph(scatter), m_fontSize(40.0f), m_style(QAbstract3DSeries::MeshSphere), m_smooth(true),
      m_itemCount(0), m_curveDivider(lowerCurveDivider), equetionParametres(boxes),
      methodButtons(methodButtons), depXt(depXt), depYt(depYt), depZt(depZt)
{
    //! [0]
    m_graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    //! [0]

    //! [2]
    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(
        QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    m_graph->addSeries(series);
    //! [2]

    //! [3]
    //    addData();
    //! [3]
}

ScatterDataModifier::~ScatterDataModifier() { delete m_graph; }

template <typename T>
static inline std::pair<T, T> _find_min_max(const std::vector<T> &v)
{
    T mi = std::numeric_limits<T>::max();
    T ma = std::numeric_limits<T>::min();
    for (const auto &x : v)
    {
        if (x > ma)
            ma = x;
        if (x < mi)
            mi = x;
    }
    return {mi, ma};
}

static inline void rebuildChartWithNewData(QChart *chart, const std::vector<double> &x,
                                           const std::vector<double> &y)
{
    chart->removeAllSeries();
    std::pair<double, double> min_max_x = _find_min_max(x);
    std::pair<double, double> min_max_y = _find_min_max(y);
    chart->axisX()->setRange(min_max_x.first, min_max_x.second);
    chart->axisY()->setRange(min_max_y.first, min_max_y.second);
    QSplineSeries *series = new QSplineSeries();

    size_t minSz = std::min(x.size(), y.size());

    for (size_t i = 0; i != minSz; ++i)
    {
        series->append(x[i], y[i]);
    }

    chart->addSeries(series);
}

void ScatterDataModifier::addData(const Answer &ans)
{
    // Configure the axes according to the data
    //! [4]
    m_graph->axisX()->setTitle("X");
    m_graph->axisY()->setTitle("Y");
    m_graph->axisZ()->setTitle("Z");
    //! [4]

    //! [5]
    QScatterDataArray *dataArray = new QScatterDataArray;

    m_itemCount = std::min(std::min(ans.x.size(), ans.y.size()), ans.z.size());

    dataArray->clear();
    dataArray->resize(static_cast<int>(m_itemCount));
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    //! [5]

    for (int i = 0; i < m_itemCount; i++, ptrToDataArray++)
    {
        ptrToDataArray->setPosition(QVector3D(ans.x[i], ans.y[i], ans.z[i]));
    }

    //! [7]
    m_graph->seriesList().at(0)->dataProxy()->resetArray(dataArray);
    //! [7]

    rebuildChartWithNewData(depXt->chart(), ans.t, ans.x);
    rebuildChartWithNewData(depYt->chart(), ans.t, ans.y);
    rebuildChartWithNewData(depZt->chart(), ans.t, ans.z);
}

//! [8]
void ScatterDataModifier::changeStyle(int style)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox)
    {
        m_style = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        if (m_graph->seriesList().size())
            m_graph->seriesList().at(0)->setMesh(m_style);
    }
}

void ScatterDataModifier::setSmoothDots(int smooth)
{
    m_smooth = bool(smooth);
    QScatter3DSeries *series = m_graph->seriesList().at(0);
    series->setMeshSmooth(m_smooth);
}

void ScatterDataModifier::changeTheme(int theme)
{
    Q3DTheme *currentTheme = m_graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
}

void ScatterDataModifier::changePresetCamera()
{
    static int preset = Q3DCamera::CameraPresetFrontLow;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
}

void ScatterDataModifier::changeLabelStyle()
{
    m_graph->activeTheme()->setLabelBackgroundEnabled(
        !m_graph->activeTheme()->isLabelBackgroundEnabled());
}

void ScatterDataModifier::changeFont(const QFont &font)
{
    QFont newFont = font;
    newFont.setPointSizeF(m_fontSize);
    m_graph->activeTheme()->setFont(newFont);
}

void ScatterDataModifier::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
{
    int quality = int(sq);
    emit shadowQualityChanged(quality); // connected to a checkbox in main.cpp
}

void ScatterDataModifier::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    m_graph->setShadowQuality(sq);
}

void ScatterDataModifier::setBackgroundEnabled(int enabled)
{
    m_graph->activeTheme()->setBackgroundEnabled((bool)enabled);
}

void ScatterDataModifier::setGridEnabled(int enabled)
{
    m_graph->activeTheme()->setGridEnabled((bool)enabled);
}
//! [8]

const char *ScatterDataModifier::getMethodName(const QString &str) const
{
    if (str == show_method_names::euler_explicit)
        return internal_method_names::euler_explicit;
    if (str == show_method_names::euler_implicit)
        return internal_method_names::euler_implicit;
    if (str == show_method_names::runge_kutta)
        return internal_method_names::runge_kutta;
    if (str == show_method_names::adams_explicit)
        return internal_method_names::adams_explicit;
    return nullptr;
}

bool ScatterDataModifier::validateParametersSetting(QString *error_msg) const
{
    for (const QComboBox *paramBox : equetionParametres)
    {
        bool is_ok;
        paramBox->currentText().toDouble(&is_ok);
        if (!is_ok)
        {
            *error_msg = "Not all configuration fields are set";
            return false;
        }
    }

    bool okMethodSet = false;
    for (const QRadioButton *methodB : methodButtons)
    {
        if (methodB->isChecked())
        {
            okMethodSet = true;
        }
    }

    if (!okMethodSet)
    {
        *error_msg = "Set any available method";
        return false;
    }

    return true;
}

const ConfigSingleton *ScatterDataModifier::loadLorenzConfig() const
{
    double x0 = equetionParametres[0]->currentText().toDouble();
    double y0 = equetionParametres[1]->currentText().toDouble();
    double z0 = equetionParametres[2]->currentText().toDouble();
    double sigma = equetionParametres[3]->currentText().toDouble();
    double r = equetionParametres[4]->currentText().toDouble();
    double b = 8. / 3.; /*equetionParametres[5]->currentText().toDouble();*/
    double dt = equetionParametres[6]->currentText().toDouble();
    double t_max = equetionParametres[7]->currentText().toDouble();

    const char *method_name;
    for (const QRadioButton *methodB : methodButtons)
    {
        if (methodB->isChecked())
        {
            method_name = getMethodName(methodB->text());
            break;
        }
    }

    return ConfigSingleton::createConfigs(x0, y0, z0, sigma, r, b, dt, t_max, method_name);
}

static inline void showMessage(const QString &msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

void ScatterDataModifier::replot()
{
    ConfigSingleton::releaseConfigs();
    QString error_msg;
    if (!validateParametersSetting(&error_msg))
    {
        showMessage(error_msg);
        return;
    }
    loadLorenzConfig();
    Answer ans = solve();
    //    ans.trimNansAndInfs();
    if (!ans.solvedCorrect())
    {
        showMessage(QString::fromUtf8(ans.error_msg.c_str()));
        return;
    }

    addData(ans);
    buildChartGnuPlot(ans);
}

QVector3D ScatterDataModifier::randVector()
{
    return QVector3D((float)(rand() % 100) / 2.0f - (float)(rand() % 100) / 2.0f,
                     (float)(rand() % 100) / 100.0f - (float)(rand() % 100) / 100.0f,
                     (float)(rand() % 100) / 2.0f - (float)(rand() % 100) / 2.0f);
}
