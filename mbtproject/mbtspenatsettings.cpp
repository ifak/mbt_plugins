#include "mbtspenatsettings.hpp"

#include <mobatawidgets/ts/comrendersut.hpp>

//static const QLatin1String kSpenatPlaceTextFontName("MbtProjectConfiguration.SpenatPlace.TextFontName");
//static const QLatin1String kSpenatPlaceTextFontSize("MbtProjectConfiguration.SpenatPlace.TextFontSize");
//static const QLatin1String kSpenatPlaceTextFontBold("MbtProjectConfiguration.SpenatPlace.TextFontBold");
//static const QLatin1String kSpenatPlaceTextFontItalic("MbtProjectConfiguration.SpenatPlace.TextFontItalic");
//static const QLatin1String kSpenatPlaceTextColorName("MbtProjectConfiguration.SpenatPlace.TextColorName");
//static const QLatin1String kSpenatPlaceWidth("MbtProjectConfiguration.SpenatPlace.Width");
//static const QLatin1String kSpenatPlaceHeight("MbtProjectConfiguration.SpenatPlace.Height");
//static const QLatin1String kSpenatPlaceColorName("MbtProjectConfiguration.SpenatPlace.ColorName");
//static const QLatin1String kSpenatPlaceBorderWidth("MbtProjectConfiguration.SpenatPlace.BorderWidth");
//static const QLatin1String kSpenatPlaceBorderColorName("MbtProjectConfiguration.SpenatPlace.BorderColorName");

//static const QLatin1String kSpenatTransitionTextFontName("MbtProjectConfiguration.SpenatTransition.TextFontName");
//static const QLatin1String kSpenatTransitionTextFontSize("MbtProjectConfiguration.SpenatTransition.TextFontSize");
//static const QLatin1String kSpenatTransitionTextFontBold("MbtProjectConfiguration.SpenatTransition.TextFontBold");
//static const QLatin1String kSpenatTransitionTextFontItalic("MbtProjectConfiguration.SpenatTransition.TextFontItalic");
//static const QLatin1String kSpenatTransitionTextColorName("MbtProjectConfiguration.SpenatTransition.TextColorName");
//static const QLatin1String kSpenatTransitionWidth("MbtProjectConfiguration.SpenatTransition.Width");
//static const QLatin1String kSpenatTransitionHeight("MbtProjectConfiguration.SpenatTransition.Height");
//static const QLatin1String kSpenatTransitionColorName("MbtProjectConfiguration.SpenatTransition.ColorName");
//static const QLatin1String kSpenatTransitionBorderWidth("MbtProjectConfiguration.SpenatTransition.BorderWidth");
//static const QLatin1String kSpenatTransitionBorderColorName("MbtProjectConfiguration.SpenatTransition.BorderColorName");

//static const QLatin1String kSpenatArcLineWidth("MbtProjectConfiguration.SpenatArc.LineWidth");
//static const QLatin1String kSpenatArcLineColorName("MbtProjectConfiguration.SpenatArc.LineColorName");
//static const QLatin1String kSpenatArcTextFontName("MbtProjectConfiguration.SpenatArc.TextFontName");
//static const QLatin1String kSpenatArcTextFontSize("MbtProjectConfiguration.SpenatArc.TextFontSize");
//static const QLatin1String kSpenatArcTextFontBold("MbtProjectConfiguration.SpenatArc.TextFontBold");
//static const QLatin1String kSpenatArcTextFontItalic("MbtProjectConfiguration.SpenatArc.TextFontItalic");
//static const QLatin1String kSpenatArcTextColorName("MbtProjectConfiguration.SpenatArc.TextColorName");

//static const char compTextFontNameKey[] = "CompTextFontSeries";
//static const char compTextFontSizeKey[] = "CompTextFontSize";
//static const char compTextFontBoldKey[] = "CompTextFontBold";
//static const char compTextFontItalicKey[] = "CompTextFontItalic";
//static const char compTextColorKey[] = "CompTextFontColorName";
//static const char compBoxWidthKey[] = "CompBoxWidth";
//static const char compBoxHeightKey[] = "CompBoxHeight";
//static const char compBoxColorKey[] = "CompBoxColorName";
//static const char compBorderWidthKey[] = "CompBorderWidth";
//static const char compBorderColorKey[] = "CompBorderColorName";
//static const char portBoxWidthKey[] = "PortBoxWidth";
//static const char portBoxHeightKey[] = "PortBoxHeight";
//static const char portBoxColorKey[] = "PortBoxColorName";
//static const char portBorderWidthKey[] = "PortBorderWidth";
//static const char portBorderColorKey[] = "PortBorderColorName";
//static const char linkLineWidthKey[] = "LinkLineWidth";
//static const char linkLineColorKey[] = "LinkLineColorName";
//static const char sutBoxColorKey[] = "SutBoxColorName";
//static const char sutBoxCompMarginKey[] = "SutBoxCompMargin";
//static const char sutBoxMarginKey[] = "SutBoxMargin";
//static const char sutBoxRadiusKey[] = "SutBoxRadius";
//static const char groupPostfix[] = "SutSettings";

//namespace distestproject {

//DisTestSutSettings::DisTestSutSettings()
//    : _compTextFontSeries(view::ts::ComRenderSut::defaultComponentOptions().textFont().family()),
//      _compTextFontSize(view::ts::ComRenderSut::defaultComponentOptions().textFont().pointSize()),
//      _compTextFontBold(view::ts::ComRenderSut::defaultComponentOptions().textFont().bold()),
//      _compTextFontItalic(view::ts::ComRenderSut::defaultComponentOptions().textFont().italic()),
//      _compTextColorName(view::ts::ComRenderSut::defaultComponentOptions().textColor().name()),
//      _compBoxWidth(view::ts::ComRenderSut::defaultComponentOptions().size().width()),
//      _compBoxHeight(view::ts::ComRenderSut::defaultComponentOptions().size().height()),
//      _compBoxColorName(view::ts::ComRenderSut::defaultComponentOptions().getColor().name()),
//      _compBorderWidth(view::ts::ComRenderSut::defaultComponentOptions().pen().width()),
//      _compBorderColorName(view::ts::ComRenderSut::defaultComponentOptions().pen().color().name()),
//      _portBoxWidth(view::ts::ComRenderSut::defaultPortOptions().size().width()),
//      _portBoxHeight(view::ts::ComRenderSut::defaultPortOptions().size().height()),
//      _portBoxColorName(view::ts::ComRenderSut::defaultPortOptions().getColor().name()),
//      _portBorderWidth(view::ts::ComRenderSut::defaultPortOptions().pen().width()),
//      _portBorderColorName(view::ts::ComRenderSut::defaultPortOptions().pen().color().name()),
//      _linkLineWidth(view::ts::ComRenderSut::defaultLinkOptions().pen().width()),
//      _linkLineColorName(view::ts::ComRenderSut::defaultLinkOptions().pen().color().name()),
//      _sutBoxColorName(view::ts::ComRenderSut::defaultSutBoxBrushOption().color().name()),
//      _sutBoxCompMargin(view::ts::ComRenderSut::defaultCompMarginSpace()),
//      _sutBoxMargin(view::ts::ComRenderSut::defaultSutBoxMarginSpace()),
//      _sutBoxRadius(view::ts::ComRenderSut::defaultSutBoxRadius())
//{
//    // Do nothing
//}

//DisTestSutSettings::DisTestSutSettings(const DisTestSutSettings &source)
//    : _compTextFontSeries(source._compTextFontSeries),
//      _compTextFontSize(source._compTextFontSize),
//      _compTextFontBold(source._compTextFontBold),
//      _compTextFontItalic(source._compTextFontItalic),
//      _compTextColorName(source._compTextColorName),
//      _compBoxWidth(source._compBoxWidth),
//      _compBoxHeight(source._compBoxHeight),
//      _compBoxColorName(source._compBoxColorName),
//      _compBorderWidth(source._compBorderWidth),
//      _compBorderColorName(source._compBorderColorName),
//      _portBoxWidth(source._portBoxWidth),
//      _portBoxHeight(source._portBoxHeight),
//      _portBoxColorName(source._portBoxColorName),
//      _portBorderWidth(source._portBorderWidth),
//      _portBorderColorName(source._portBorderColorName),
//      _linkLineWidth(source._linkLineWidth),
//      _linkLineColorName(source._linkLineColorName),
//      _sutBoxColorName(source._sutBoxColorName),
//      _sutBoxCompMargin(source._sutBoxCompMargin),
//      _sutBoxMargin(source._sutBoxMargin),
//      _sutBoxRadius(source._sutBoxRadius)
//{
//    // Do nothing
//}

//void DisTestSutSettings::toSettings(const QString &category, QSettings *settings) const
//{
//    QString group(QString::fromLatin1(groupPostfix));
//    if (!category.isEmpty())
//    {
//        group.prepend(category);
//    }

//    settings->beginGroup(group);
//    settings->setValue(QString::fromLatin1(compTextFontNameKey), _compTextFontSeries);
//    settings->setValue(QString::fromLatin1(compTextFontSizeKey), _compTextFontSize);
//    settings->setValue(QString::fromLatin1(compTextFontBoldKey), _compTextFontBold);
//    settings->setValue(QString::fromLatin1(compTextFontItalicKey), _compTextFontItalic);
//    settings->setValue(QString::fromLatin1(compTextColorKey), _compTextColorName);
//    settings->setValue(QString::fromLatin1(compBoxWidthKey), _compBoxWidth);
//    settings->setValue(QString::fromLatin1(compBoxHeightKey), _compBoxHeight);
//    settings->setValue(QString::fromLatin1(compBoxColorKey), _compBoxColorName);
//    settings->setValue(QString::fromLatin1(compBorderWidthKey), _compBorderWidth);
//    settings->setValue(QString::fromLatin1(compBorderColorKey), _compBorderColorName);
//    settings->setValue(QString::fromLatin1(portBoxWidthKey), _portBoxWidth);
//    settings->setValue(QString::fromLatin1(portBoxHeightKey), _portBoxHeight);
//    settings->setValue(QString::fromLatin1(portBoxColorKey), _portBoxColorName);
//    settings->setValue(QString::fromLatin1(portBorderWidthKey), _portBorderWidth);
//    settings->setValue(QString::fromLatin1(portBorderColorKey), _portBorderColorName);
//    settings->setValue(QString::fromLatin1(linkLineWidthKey), _linkLineWidth);
//    settings->setValue(QString::fromLatin1(linkLineColorKey), _linkLineColorName);
//    settings->setValue(QString::fromLatin1(sutBoxColorKey), _sutBoxColorName);
//    settings->setValue(QString::fromLatin1(sutBoxCompMarginKey), _sutBoxCompMargin);
//    settings->setValue(QString::fromLatin1(sutBoxMarginKey), _sutBoxMargin);
//    settings->setValue(QString::fromLatin1(sutBoxRadiusKey), _sutBoxRadius);
//    settings->endGroup();
//}

//void DisTestSutSettings::fromSettings(const QString &category, const QSettings *settings)
//{
//    QString group(QString::fromLatin1(groupPostfix));
//    if (!category.isEmpty())
//    {
//        group.prepend(category);
//    }

//    if (!settings->childGroups().contains(group))
//    {
//        return;
//    }

//    group.append(QChar::fromLatin1('/'));
//    _compTextFontSeries = settings->value(group + QString::fromLatin1(compTextFontNameKey), _compTextFontSeries).toString();
//    _compTextFontSize = settings->value(group + QString::fromLatin1(compTextFontSizeKey), _compTextFontSize).toInt();
//    _compTextFontBold = settings->value(group + QString::fromLatin1(compTextFontBoldKey), _compTextFontBold).toBool();
//    _compTextFontItalic = settings->value(group + QString::fromLatin1(compTextFontItalicKey), _compTextFontItalic).toBool();
//    _compTextColorName = settings->value(group + QString::fromLatin1(compTextColorKey), _compTextColorName).toString();
//    _compBoxWidth = settings->value(group + QString::fromLatin1(compBoxWidthKey), _compBoxWidth).toDouble();
//    _compBoxHeight = settings->value(group + QString::fromLatin1(compBoxHeightKey), _compBoxHeight).toDouble();
//    _compBoxColorName = settings->value(group + QString::fromLatin1(compBoxColorKey), _compBoxColorName).toString();
//    _compBorderWidth = settings->value(group + QString::fromLatin1(compBorderWidthKey), _compBorderWidth).toInt();
//    _compBorderColorName = settings->value(group + QString::fromLatin1(compBorderColorKey), _compBorderColorName).toString();
//    _portBoxWidth = settings->value(group + QString::fromLatin1(portBoxWidthKey), _portBoxWidth).toDouble();
//    _portBoxHeight = settings->value(group + QString::fromLatin1(portBoxHeightKey), _portBoxHeight).toDouble();
//    _portBoxColorName = settings->value(group + QString::fromLatin1(portBoxColorKey), _portBoxColorName).toString();
//    _portBorderWidth = settings->value(group + QString::fromLatin1(portBorderWidthKey), _portBorderWidth).toInt();
//    _portBorderColorName = settings->value(group + QString::fromLatin1(portBorderColorKey), _portBorderColorName).toString();
//    _linkLineWidth = settings->value(group + QString::fromLatin1(linkLineWidthKey), _linkLineWidth).toInt();
//    _linkLineColorName = settings->value(group + QString::fromLatin1(linkLineColorKey), _linkLineColorName).toString();
//    _sutBoxColorName = settings->value(group + QString::fromLatin1(sutBoxColorKey), _sutBoxColorName).toString();
//    _sutBoxCompMargin = settings->value(group + QString::fromLatin1(sutBoxCompMarginKey), _sutBoxCompMargin).toDouble();
//    _sutBoxMargin = settings->value(group + QString::fromLatin1(sutBoxMarginKey), _sutBoxMargin).toDouble();
//    _sutBoxRadius = settings->value(group + QString::fromLatin1(sutBoxRadiusKey), _sutBoxRadius).toDouble();
//}

//view::graph::NodeOptions DisTestSutSettings::compOptionsFromSettings() const
//{
//    QFont compTextFont(_compTextFontSeries, _compTextFontSize);
//    compTextFont.setBold(_compTextFontBold);
//    compTextFont.setItalic(_compTextFontItalic);

//    view::graph::NodeOptions compOptions;
//    compOptions.setTextFont(compTextFont);
//    compOptions.setTextColor(QColor(_compTextColorName));
//    compOptions.setSize(_compBoxWidth, _compBoxHeight);
//    compOptions.setColor(QColor(_compBoxColorName));
//    compOptions.setPen(QPen(QColor(_compBorderColorName), _compBorderWidth));

//    return compOptions;
//}

//view::graph::NodeOptions DisTestSutSettings::portOptionsFromSettings() const
//{
//    view::graph::NodeOptions portOptions;
//    portOptions.setSize(_portBoxWidth, _portBoxHeight);
//    portOptions.setColor(QColor(_portBoxColorName));
//    portOptions.setPen(QPen(QColor(_portBorderColorName), _portBorderWidth));

//    return portOptions;
//}

//view::graph::ArcOptions DisTestSutSettings::linkOptionsFromSettings() const
//{
//    view::graph::ArcOptions linkOptions;
//    linkOptions.setPen(QPen(QColor(_linkLineColorName), _linkLineWidth));

//    return linkOptions;
//}

//const QString &DisTestSutSettings::compTextFontSeries() const
//{
//    return _compTextFontSeries;
//}

//void DisTestSutSettings::setCompTextFontSeries(const QString &fontName)
//{
//    _compTextFontSeries = fontName;
//}

//const int &DisTestSutSettings::compTextFontSize() const
//{
//    return _compTextFontSize;
//}

//void DisTestSutSettings::setCompTextFontSize(const int fontSize)
//{
//    _compTextFontSize = fontSize;
//}

//const bool &DisTestSutSettings::compTextFontBold() const
//{
//    return _compTextFontBold;
//}

//void DisTestSutSettings::setCompTextFontBold(const bool isBold)
//{
//    _compTextFontBold = isBold;
//}

//const bool &DisTestSutSettings::compTextFontItalic() const
//{
//    return _compTextFontItalic;
//}

//void DisTestSutSettings::setCompTextFontItalic(const bool isItalic)
//{
//    _compTextFontItalic = isItalic;
//}

//const QString &DisTestSutSettings::compTextColorName() const
//{
//    return _compTextColorName;
//}

//void DisTestSutSettings::setCompTextColorName(const QString &colorName)
//{
//    _compTextColorName = colorName;
//}

//const qreal &DisTestSutSettings::compBoxWidth() const
//{
//    return _compBoxWidth;
//}

//void DisTestSutSettings::setCompBoxWidth(const qreal width)
//{
//    _compBoxWidth = width;
//}

//const qreal &DisTestSutSettings::compBoxHeight() const
//{
//    return _compBoxHeight;
//}

//void DisTestSutSettings::setCompBoxHeight(const qreal height)
//{
//    _compBoxHeight = height;
//}

//const QString &DisTestSutSettings::compBoxColorName() const
//{
//    return _compBoxColorName;
//}

//void DisTestSutSettings::setCompBoxColorName(const QString &colorName)
//{
//    _compBoxColorName = colorName;
//}

//const int &DisTestSutSettings::compBorderWidth() const
//{
//    return _compBorderWidth;
//}

//void DisTestSutSettings::setCompBorderWidth(const int width)
//{
//    _compBorderWidth = width;
//}

//const QString &DisTestSutSettings::compBorderColorName() const
//{
//    return _compBorderColorName;
//}

//void DisTestSutSettings::setCompBorderColorName(const QString &colorName)
//{
//    _compBorderColorName = colorName;
//}

//const qreal &DisTestSutSettings::portBoxWidth() const
//{
//    return _portBoxWidth;
//}

//void DisTestSutSettings::setPortBoxWidth(const qreal width)
//{
//    _portBoxWidth = width;
//}

//const qreal &DisTestSutSettings::portBoxHeight() const
//{
//    return _portBoxHeight;
//}

//void DisTestSutSettings::setPortBoxHeight(const qreal height)
//{
//    _portBoxHeight = height;
//}

//const QString &DisTestSutSettings::portBoxColorName() const
//{
//    return _portBoxColorName;
//}

//void DisTestSutSettings::setPortBoxColorName(const QString &colorName)
//{
//    _portBoxColorName = colorName;
//}

//const int &DisTestSutSettings::portBorderWidth() const
//{
//    return _portBorderWidth;
//}

//void DisTestSutSettings::setPortBorderWidth(const int width)
//{
//    _portBorderWidth = width;
//}

//const QString &DisTestSutSettings::portBorderColorName() const
//{
//    return _portBorderColorName;
//}

//void DisTestSutSettings::setPortBorderColorName(const QString &colorName)
//{
//    _portBorderColorName = colorName;
//}

//const int &DisTestSutSettings::linkLineWidth() const
//{
//    return _linkLineWidth;
//}

//void DisTestSutSettings::setLinkLineWidth(const int width)
//{
//    _linkLineWidth = width;
//}

//const QString &DisTestSutSettings::linkLineColorName() const
//{
//    return _linkLineColorName;
//}

//void DisTestSutSettings::setLinkLineColorName(const QString &colorName)
//{
//    _linkLineColorName = colorName;
//}

//const QString &DisTestSutSettings::sutBoxColorName() const
//{
//    return _sutBoxColorName;
//}

//void DisTestSutSettings::setSutBoxColorName(const QString &colorName)
//{
//    _sutBoxColorName = colorName;
//}

//const qreal &DisTestSutSettings::sutBoxCompMargin() const
//{
//    return _sutBoxCompMargin;
//}

//void DisTestSutSettings::setSutBoxCompMargin(const qreal margin)
//{
//    _sutBoxCompMargin = margin;
//}

//const qreal &DisTestSutSettings::sutBoxMargin() const
//{
//    return _sutBoxMargin;
//}

//void DisTestSutSettings::setSutBoxMargin(const qreal margin)
//{
//    _sutBoxMargin = margin;
//}

//const qreal &DisTestSutSettings::sutBoxRadius() const
//{
//    return _sutBoxRadius;
//}

//void DisTestSutSettings::setSutBoxRadius(const qreal radius)
//{
//    _sutBoxRadius = radius;
//}

//}

