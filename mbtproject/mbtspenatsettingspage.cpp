#include "sutsettingspage.hpp"
#include "sutsettings.hpp"
#include "distestprojectconstants.hpp"
#include "globalrendersettings.hpp"

#include <coreplugin/icore.h>

#include <mobata/model/ts/ts.hpp>
#include <mobatawidgets/ts/comrendersut.hpp>

#include <QColorDialog>

#include "helpers.hpp"

namespace distestproject {
namespace Internal {

class DisTestSutSettingsWidget::Private
{
    friend class DisTestSutSettingsWidget;

public:
  Private()
    : _tsModel(0),
      _sutScene(0),
      _currentSutSettings(0),
      _currentCompOptions(0),
      _currentPortOptions(0),
      _currentLinkOptions(0)
  {}

  ~Private()
  {}

private:
    model::ts::TestSuite*               _tsModel;
    view::ts::SutScene*                 _sutScene;
    distestproject::DisTestSutSettings* _currentSutSettings;
    view::graph::NodeOptions*           _currentCompOptions;
    view::graph::NodeOptions*           _currentPortOptions;
    view::graph::ArcOptions*            _currentLinkOptions;
};

DisTestSutSettingsWidget::DisTestSutSettingsWidget(QWidget *parent)
    : QWidget(parent), _d(new Private())
{
    m_ui.setupUi(this);

    buildTestModel();

    this->_d->_currentSutSettings = new distestproject::DisTestSutSettings(GlobalRenderSettings::globalInstance()->sutSettings());
    this->_d->_currentCompOptions = new view::graph::NodeOptions(this->_d->_currentSutSettings->compOptionsFromSettings());
    this->_d->_currentPortOptions = new view::graph::NodeOptions(this->_d->_currentSutSettings->portOptionsFromSettings());
    this->_d->_currentLinkOptions = new view::graph::ArcOptions(this->_d->_currentSutSettings->linkOptionsFromSettings());

    this->_d->_sutScene = new view::ts::SutScene(this);
    this->_d->_sutScene->setSutBoxBrush(QBrush(QColor(this->_d->_currentSutSettings->sutBoxColorName())));
    this->m_ui._sutRenderView->setScene(this->_d->_sutScene);

    connect(m_ui._compTextFontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(changeCompTextFontName(QFont)));
    connect(m_ui._compTextFontSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeCompTextFontSize(int)));
    connect(m_ui._compTextFontBoldCheckBox, SIGNAL(clicked(bool)), this, SLOT(changeCompTextFontBold(bool)));
    connect(m_ui._compTextFontItalicCheckBox, SIGNAL(clicked(bool)), this, SLOT(changeCompTextFontItalic(bool)));
    connect(m_ui._compTextSelectedColor, SIGNAL(clicked()), this, SLOT(changeCompTextColor()));
    connect(m_ui._compBoxWSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeCompBoxWidth(int)));
    connect(m_ui._compBoxHSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeCompBoxHeight(int)));
    connect(m_ui._compBoxSelectedColor, SIGNAL(clicked()), this, SLOT(changeCompBoxColor()));
    connect(m_ui._compBorderWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeCompBorderWidth(int)));
    connect(m_ui._compBorderSelectedColor, SIGNAL(clicked()), this, SLOT(changeCompBorderColor()));
    connect(m_ui._portBoxWSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changePortBoxWidth(int)));
    connect(m_ui._portBoxHSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changePortBoxHeight(int)));
    connect(m_ui._portBoxSelectedColor, SIGNAL(clicked()), this, SLOT(changePortBoxColor()));
    connect(m_ui._portBorderWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changePortBorderWidth(int)));
    connect(m_ui._portBorderSelectedColor, SIGNAL(clicked()), this, SLOT(changePortBorderColor()));
    connect(m_ui._linkLineWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeLinkLineWidth(int)));
    connect(m_ui._linkLineSelectedColor, SIGNAL(clicked()), this, SLOT(changeLinkLineColor()));
    connect(m_ui._sutBoxSelectedColor, SIGNAL(clicked()), this, SLOT(changeSutBoxColor()));
    connect(m_ui._sutBoxCompMarginSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeSutBoxCompMargin(int)));
    connect(m_ui._sutBoxMarginSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeSutBoxCompMargin(int)));
    connect(m_ui._sutBoxRadiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeSutBoxRadius(int)));

  connect(m_ui._sutRenderView, &view::ts::SutView::resized,
          this, &DisTestSutSettingsWidget::renderSut);

  connect(m_ui._sutRenderView, &view::ts::SutView::shown,
          this, &DisTestSutSettingsWidget::renderSut);
}

DisTestSutSettingsWidget::~DisTestSutSettingsWidget()
{
    delete _d->_sutScene;
    delete _d->_currentLinkOptions;
    delete _d->_currentPortOptions;
    delete _d->_currentCompOptions;
    delete _d->_currentSutSettings;
    delete _d->_tsModel;

    delete _d;
}

void DisTestSutSettingsWidget::buildTestModel()
{
    this->_d->_tsModel = new model::ts::TestSuite();

    model::ts::SutItem* sutItem = this->_d->_tsModel->sutItem();
    QString errorString;

    model::base::ComponentItem* sutComp1 = sutItem->addComponent(QStringLiteral("sutComp1"), &errorString);
    model::base::PortItem* sutComp1Port1 = sutComp1->addPort(QStringLiteral("sutPort1"), &errorString);

    model::base::ComponentItem* sutComp2 = sutItem->addComponent(QStringLiteral("sutComp2"), &errorString);
    model::base::PortItem* sutComp2Port1 = sutComp2->addPort(QStringLiteral("sutPort1"), &errorString);

    sutItem->addLink(sutComp1Port1, sutComp2Port1, &errorString);
}

void DisTestSutSettingsWidget::changeCompTextFontName(const QFont &font)
{
    this->_d->_currentSutSettings->setCompTextFontSeries(font.family());

    QFont newCompTextFont(this->_d->_currentCompOptions->textFont());
    newCompTextFont.setFamily(font.family());

    unsigned int fontSettingFlags = distestproject::Helpers::availableFontSettingsState(newCompTextFont);

    if (!m_ui._compTextFontBoldCheckBox->isChecked())
    {
        if (fontSettingFlags & 0x10)
        {
            this->_d->_currentSutSettings->setCompTextFontBold(true);
            newCompTextFont.setBold(true);
            m_ui._compTextFontBoldCheckBox->setChecked(true);
        }
    }
    else
    {
        if (!(fontSettingFlags & 0x10))
        {
            this->_d->_currentSutSettings->setCompTextFontBold(false);
            newCompTextFont.setBold(false);
            m_ui._compTextFontBoldCheckBox->setChecked(false);
        }
    }

    if (!m_ui._compTextFontItalicCheckBox->isChecked())
    {
        if (fontSettingFlags & 0x20)
        {
            this->_d->_currentSutSettings->setCompTextFontItalic(true);
            newCompTextFont.setItalic(true);
            m_ui._compTextFontItalicCheckBox->setChecked(true);
        }
    }
    else
    {
        if (!(fontSettingFlags & 0x20))
        {
            this->_d->_currentSutSettings->setCompTextFontItalic(false);
            newCompTextFont.setItalic(false);
            m_ui._compTextFontItalicCheckBox->setChecked(false);
        }
    }

    m_ui._compTextFontBoldCheckBox->setEnabled(true);
    if (!(fontSettingFlags & 0x01))
    {
        m_ui._compTextFontBoldCheckBox->setEnabled(false);
    }

    m_ui._compTextFontItalicCheckBox->setEnabled(true);
    if (!(fontSettingFlags & 0x02))
    {
        m_ui._compTextFontItalicCheckBox->setEnabled(false);
    }

    this->_d->_currentCompOptions->setTextFont(newCompTextFont);

    renderSut();
}

void DisTestSutSettingsWidget::changeCompTextFontSize(int size)
{
    this->_d->_currentSutSettings->setCompTextFontSize(size);

    QFont newCompTextFont(this->_d->_currentCompOptions->textFont());
    newCompTextFont.setPointSize(size);
    this->_d->_currentCompOptions->setTextFont(newCompTextFont);

    renderSut();
}

void DisTestSutSettingsWidget::changeCompTextFontBold(bool checked)
{
    this->_d->_currentSutSettings->setCompTextFontBold(checked);

    QFont newCompTextFont(this->_d->_currentCompOptions->textFont());
    newCompTextFont.setBold(checked);

    unsigned int fontSettingFlags = distestproject::Helpers::availableFontSettingsState(newCompTextFont);

    if (!m_ui._compTextFontBoldCheckBox->isChecked())
    {
        if (fontSettingFlags & 0x10)
        {
            this->_d->_currentSutSettings->setCompTextFontBold(true);
            newCompTextFont.setBold(true);
            m_ui._compTextFontBoldCheckBox->setChecked(true);
        }
    }
    else
    {
        if (!(fontSettingFlags & 0x10))
        {
            this->_d->_currentSutSettings->setCompTextFontBold(false);
            newCompTextFont.setBold(false);
            m_ui._compTextFontBoldCheckBox->setChecked(false);
        }
    }

    if (!m_ui._compTextFontItalicCheckBox->isChecked())
    {
        if (fontSettingFlags & 0x20)
        {
            this->_d->_currentSutSettings->setCompTextFontItalic(true);
            newCompTextFont.setItalic(true);
            m_ui._compTextFontItalicCheckBox->setChecked(true);
        }
    }
    else
    {
        if (!(fontSettingFlags & 0x20))
        {
            this->_d->_currentSutSettings->setCompTextFontItalic(false);
            newCompTextFont.setItalic(false);
            m_ui._compTextFontItalicCheckBox->setChecked(false);
        }
    }

    m_ui._compTextFontBoldCheckBox->setEnabled(true);
    if (!(fontSettingFlags & 0x01))
    {
        m_ui._compTextFontBoldCheckBox->setEnabled(false);
    }

    m_ui._compTextFontItalicCheckBox->setEnabled(true);
    if (!(fontSettingFlags & 0x02))
    {
        m_ui._compTextFontItalicCheckBox->setEnabled(false);
    }

    this->_d->_currentCompOptions->setTextFont(newCompTextFont);

    renderSut();
}

void DisTestSutSettingsWidget::changeCompTextFontItalic(bool checked)
{
    this->_d->_currentSutSettings->setCompTextFontItalic(checked);

    QFont newCompTextFont(this->_d->_currentCompOptions->textFont());
    newCompTextFont.setItalic(checked);

    unsigned int fontSettingFlags = distestproject::Helpers::availableFontSettingsState(newCompTextFont);

  if (!m_ui._compTextFontBoldCheckBox->isChecked())
  {
    if (fontSettingFlags & 0x10)
    {
      this->_d->_currentSutSettings->setCompTextFontBold(true);
      newCompTextFont.setBold(true);
      m_ui._compTextFontBoldCheckBox->setChecked(true);
    }
  }
  else
  {
    if (!(fontSettingFlags & 0x10))
    {
      this->_d->_currentSutSettings->setCompTextFontBold(false);
      newCompTextFont.setBold(false);
      m_ui._compTextFontBoldCheckBox->setChecked(false);
    }
  }

  if (!m_ui._compTextFontItalicCheckBox->isChecked())
  {
    if (fontSettingFlags & 0x20)
    {
      this->_d->_currentSutSettings->setCompTextFontItalic(true);
      newCompTextFont.setItalic(true);
      m_ui._compTextFontItalicCheckBox->setChecked(true);
    }
  }
  else
  {
    if (!(fontSettingFlags & 0x20))
    {
      this->_d->_currentSutSettings->setCompTextFontItalic(false);
      newCompTextFont.setItalic(false);
      m_ui._compTextFontItalicCheckBox->setChecked(false);
    }
  }

  m_ui._compTextFontBoldCheckBox->setEnabled(true);
  if (!(fontSettingFlags & 0x01))
  {
    m_ui._compTextFontBoldCheckBox->setEnabled(false);
  }

  m_ui._compTextFontItalicCheckBox->setEnabled(true);
  if (!(fontSettingFlags & 0x02))
  {
    m_ui._compTextFontItalicCheckBox->setEnabled(false);
  }

  this->_d->_currentCompOptions->setTextFont(newCompTextFont);

  renderSut();
}

void DisTestSutSettingsWidget::changeCompTextColor()
{
  QColor testColor = QColorDialog::getColor(QColor(this->_d->_currentSutSettings->compTextColorName()));
  if (testColor.isValid())
  {
    this->_d->_currentSutSettings->setCompTextColorName(testColor.name());

        distestproject::Helpers::changeLabelColor(m_ui._compTextSelectedColor, testColor);
        this->_d->_currentCompOptions->setTextColor(QColor(testColor.name()));

    renderSut();
  }
}

void DisTestSutSettingsWidget::changeCompBoxWidth(int width)
{
  this->_d->_currentSutSettings->setCompBoxWidth(width);
  this->_d->_currentCompOptions->setSize(width, this->_d->_currentCompOptions->size().height());
  renderSut();
}

void DisTestSutSettingsWidget::changeCompBoxHeight(int height)
{
  this->_d->_currentSutSettings->setCompBoxHeight(height);
  this->_d->_currentCompOptions->setSize(this->_d->_currentCompOptions->size().width(), height);
  renderSut();
}

void DisTestSutSettingsWidget::changeCompBoxColor()
{
  QColor testColor = QColorDialog::getColor(QColor(this->_d->_currentSutSettings->compBoxColorName()));
  if (testColor.isValid())
  {
    this->_d->_currentSutSettings->setCompBoxColorName(testColor.name());

        distestproject::Helpers::changeLabelColor(m_ui._compBoxSelectedColor, testColor);
        this->_d->_currentCompOptions->setColor(QColor(testColor.name()));

    renderSut();
  }
}

void DisTestSutSettingsWidget::changeCompBorderWidth(int width)
{
  this->_d->_currentSutSettings->setCompBorderWidth(width);
  QPen newBorderPen(this->_d->_currentCompOptions->pen());
  newBorderPen.setWidth(width);
  this->_d->_currentCompOptions->setPen(newBorderPen);

  renderSut();
}

void DisTestSutSettingsWidget::changeCompBorderColor()
{
  QColor testColor = QColorDialog::getColor(QColor(this->_d->_currentSutSettings->compBorderColorName()));
  if (testColor.isValid())
  {
    this->_d->_currentSutSettings->setCompBorderColorName(testColor.name());

        distestproject::Helpers::changeLabelColor(m_ui._compBorderSelectedColor, testColor);

    QPen newBorderPen(this->_d->_currentCompOptions->pen());
    newBorderPen.setColor(testColor.name());
    this->_d->_currentCompOptions->setPen(newBorderPen);

    renderSut();
  }
}

void DisTestSutSettingsWidget::changePortBoxWidth(int width)
{
  this->_d->_currentSutSettings->setPortBoxWidth(width);
  this->_d->_currentPortOptions->setSize(width, this->_d->_currentPortOptions->size().height());
  renderSut();
}

void DisTestSutSettingsWidget::changePortBoxHeight(int height)
{
  this->_d->_currentSutSettings->setPortBoxHeight(height);
  this->_d->_currentPortOptions->setSize(this->_d->_currentPortOptions->size().width(), height);
  renderSut();
}

void DisTestSutSettingsWidget::changePortBoxColor()
{
  QColor testColor = QColorDialog::getColor(QColor(this->_d->_currentSutSettings->portBoxColorName()));
  if (testColor.isValid())
  {
    this->_d->_currentSutSettings->setPortBoxColorName(testColor.name());

        distestproject::Helpers::changeLabelColor(m_ui._portBoxSelectedColor, testColor);
        this->_d->_currentPortOptions->setColor(QColor(testColor.name()));

    renderSut();
  }
}

void DisTestSutSettingsWidget::changePortBorderWidth(int width)
{
  this->_d->_currentSutSettings->setPortBorderWidth(width);
  QPen newBorderPen(this->_d->_currentPortOptions->pen());
  newBorderPen.setWidth(width);
  this->_d->_currentPortOptions->setPen(newBorderPen);
  renderSut();
}

void DisTestSutSettingsWidget::changePortBorderColor()
{
  QColor testColor = QColorDialog::getColor(QColor(this->_d->_currentSutSettings->portBorderColorName()));
  if (testColor.isValid())
  {
    this->_d->_currentSutSettings->setPortBorderColorName(testColor.name());

        distestproject::Helpers::changeLabelColor(m_ui._portBorderSelectedColor, testColor);

        QPen newBorderPen(this->_d->_currentPortOptions->pen());
        newBorderPen.setColor(testColor.name());
        this->_d->_currentPortOptions->setPen(newBorderPen);

        renderSut();
    }
}

void DisTestSutSettingsWidget::changeLinkLineWidth(int width)
{
    this->_d->_currentSutSettings->setLinkLineWidth(width);
    QPen newLinkLinePen(this->_d->_currentLinkOptions->pen());
    newLinkLinePen.setWidth(width);
    this->_d->_currentLinkOptions->setPen(newLinkLinePen);
    renderSut();
}

void DisTestSutSettingsWidget::changeLinkLineColor()
{
    QColor testColor = QColorDialog::getColor(QColor(this->_d->_currentSutSettings->linkLineColorName()));
    if (testColor.isValid())
    {
        this->_d->_currentSutSettings->setLinkLineColorName(testColor.name());

        distestproject::Helpers::changeLabelColor(m_ui._linkLineSelectedColor, testColor);

        QPen newLinkLinePen(this->_d->_currentLinkOptions->pen());
        newLinkLinePen.setColor(testColor.name());
        this->_d->_currentLinkOptions->setPen(newLinkLinePen);

        renderSut();
    }
}

void DisTestSutSettingsWidget::changeSutBoxColor()
{
    QColor testColor = QColorDialog::getColor(QColor(this->_d->_currentSutSettings->sutBoxColorName()));
    if (testColor.isValid())
    {
        this->_d->_currentSutSettings->setSutBoxColorName(testColor.name());

        distestproject::Helpers::changeLabelColor(m_ui._sutBoxSelectedColor, testColor);

        renderSut();
    }
}

void DisTestSutSettingsWidget::changeSutBoxCompMargin(int margin)
{
    this->_d->_currentSutSettings->setSutBoxCompMargin(margin);
    renderSut();
}

void DisTestSutSettingsWidget::changeSutBoxMargin(int margin)
{
    this->_d->_currentSutSettings->setSutBoxMargin(margin);
    renderSut();
}

void DisTestSutSettingsWidget::changeSutBoxRadius(int radius)
{
    this->_d->_currentSutSettings->setSutBoxRadius(radius);
    renderSut();
}

void DisTestSutSettingsWidget::renderSut()
{
    QBrush sutBoxColor(QColor(this->_d->_currentSutSettings->sutBoxColorName()));
    qreal sutBoxCompMargin = this->_d->_currentSutSettings->sutBoxCompMargin();
    qreal sutBoxMargin = this->_d->_currentSutSettings->sutBoxMargin();
    qreal sutBoxRadius = this->_d->_currentSutSettings->sutBoxRadius();
    view::ts::ComRenderSut renderCommand(this->_d->_sutScene,
                                         this->_d->_tsModel->sutItem(),
                                         *(this->_d->_currentCompOptions),
                                         *(this->_d->_currentPortOptions),
                                         *(this->_d->_currentLinkOptions),
                                         sutBoxColor,
                                         sutBoxCompMargin,
                                         sutBoxMargin,
                                         sutBoxRadius);
    QString errorString = QStringLiteral("");
    bool result = renderCommand.execute(&errorString);
    Q_ASSERT(result);
    Q_ASSERT(errorString.isEmpty());

  this->m_ui._sutRenderView->fitInView(this->m_ui._sutRenderView->sceneRect(),
                                       Qt::KeepAspectRatio);
  return;
}

void DisTestSutSettingsWidget::initializeConfigView()
{
    m_ui._compTextFontComboBox->setCurrentFont(QFont(this->_d->_currentSutSettings->compTextFontSeries()));
    m_ui._compTextFontSizeSpinBox->setValue(this->_d->_currentSutSettings->compTextFontSize());
    m_ui._compTextFontBoldCheckBox->setChecked(this->_d->_currentSutSettings->compTextFontBold());
    m_ui._compTextFontItalicCheckBox->setChecked(this->_d->_currentSutSettings->compTextFontItalic());
    distestproject::Helpers::changeLabelColor(m_ui._compTextSelectedColor, QColor(this->_d->_currentSutSettings->compTextColorName()));
    m_ui._compBoxWSpinBox->setValue(this->_d->_currentSutSettings->compBoxWidth());
    m_ui._compBoxHSpinBox->setValue(this->_d->_currentSutSettings->compBoxHeight());
    distestproject::Helpers::changeLabelColor(m_ui._compBoxSelectedColor, QColor(this->_d->_currentSutSettings->compBoxColorName()));
    m_ui._compBorderWidthSpinBox->setValue(this->_d->_currentSutSettings->compBorderWidth());
    distestproject::Helpers::changeLabelColor(m_ui._compBorderSelectedColor, QColor(this->_d->_currentSutSettings->compBorderColorName()));
    m_ui._portBoxWSpinBox->setValue(this->_d->_currentSutSettings->portBoxWidth());
    m_ui._portBoxHSpinBox->setValue(this->_d->_currentSutSettings->portBoxHeight());
    distestproject::Helpers::changeLabelColor(m_ui._portBoxSelectedColor, QColor(this->_d->_currentSutSettings->portBoxColorName()));
    m_ui._portBorderWidthSpinBox->setValue(this->_d->_currentSutSettings->portBorderWidth());
    distestproject::Helpers::changeLabelColor(m_ui._portBorderSelectedColor, QColor(this->_d->_currentSutSettings->portBorderColorName()));
    m_ui._linkLineWidthSpinBox->setValue(this->_d->_currentSutSettings->linkLineWidth());
    distestproject::Helpers::changeLabelColor(m_ui._linkLineSelectedColor, QColor(this->_d->_currentSutSettings->linkLineColorName()));
    distestproject::Helpers::changeLabelColor(m_ui._sutBoxSelectedColor, QColor(this->_d->_currentSutSettings->sutBoxColorName()));
    m_ui._sutBoxCompMarginSpinBox->setValue(this->_d->_currentSutSettings->sutBoxCompMargin());
    m_ui._sutBoxMarginSpinBox->setValue(this->_d->_currentSutSettings->sutBoxMargin());
    m_ui._sutBoxRadiusSpinBox->setValue(this->_d->_currentSutSettings->sutBoxRadius());
}

void DisTestSutSettingsWidget::saveSutSettings() const
{
    this->_d->_currentSutSettings->toSettings(QString::fromLatin1(distestproject::Constants::DISTEST_SETTINGS_PREFIX), Core::ICore::settings());
}

distestproject::DisTestSutSettings *DisTestSutSettingsWidget::sutSettings() const
{
    return this->_d->_currentSutSettings;
}

const QString &DisTestSutSettingsWidget::searchKeywords() const
{
    if (m_searchKeywords.isEmpty()) {
        QLatin1Char sep(' ');
        m_searchKeywords = m_ui._compGroup->title()
                + sep + m_ui._compTextFontGroup->title()
                + sep + m_ui._compBoxGroup->title()
                + sep + m_ui._compBorderGroup->title()
                + sep + m_ui._portGroup->title()
                + sep + m_ui._portBoxGroup->title()
                + sep + m_ui._portBorderGroup->title()
                + sep + m_ui._linkGroup->title()
                + sep + m_ui._linkLineWidthLabel->text()
                + sep + m_ui._linkLineColorLabel->text()
                + sep + m_ui._sutBoxGroup->title()
                + sep + m_ui._sutBoxColorLabel->text()
                + sep + m_ui._sutBoxCompMarginLabel->text()
                + sep + m_ui._sutBoxMarginLabel->text()
                + sep + m_ui._sutBoxRadiusLabel->text()
                ;
        m_searchKeywords.remove(QLatin1Char('&'));
    }

    return m_searchKeywords;
}

// ------------------ DisTestSutSettingsPage
DisTestSutSettingsPage::DisTestSutSettingsPage()
{
    setId(Constants::DISTEST_SUT_SETTINGS_ID);
    setDisplayName(tr("System Under Test"));
    setCategory(Constants::DISTEST_SETTINGS_CATEGORY);
    setDisplayCategory(QCoreApplication::translate("DisTest", Constants::DISTEST_SETTINGS_TR_CATEGORY));
    setCategoryIcon(QLatin1String(Constants::DISTEST_SETTINGS_CATEGORY_ICON));
}

DisTestSutSettingsPage::~DisTestSutSettingsPage()
{
    // Do nothing
}

QWidget *DisTestSutSettingsPage::createPage(QWidget *parent)
{
    m_widget = new DisTestSutSettingsWidget(parent);
    m_widget->initializeConfigView();

    if (m_searchKeywords.isEmpty())
    {
        m_searchKeywords = m_widget->searchKeywords();
    }

    return m_widget;
}

void DisTestSutSettingsPage::apply()
{
    if (m_widget)
    {
        m_widget->saveSutSettings();
        GlobalRenderSettings::globalInstance()->setSutSettings(*(m_widget->sutSettings()));
    }
}

void DisTestSutSettingsPage::finish()
{
    // Do nothing
}

bool DisTestSutSettingsPage::matches(const QString &s) const
{
    return m_searchKeywords.contains(s, Qt::CaseInsensitive);
}

}
}


