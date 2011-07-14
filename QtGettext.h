/*
  Copyright (C) 2010 Laurent Cozic. All right reserved.
  Use of this source code is governed by a GNU/GPL license that can be
  found in the LICENSE file.
*/


#ifndef Gettext_QtGettext_H
#define Gettext_QtGettext_H

#include <QtCore>
#include "LauGettext.h"

#undef _
#define _(x) QtGettext::instance()->getTranslation(x)


class QtGettext : public LauGettext {

public:

  static QtGettext* instance();
  static void destroyInstance();

  QtGettext();
  ~QtGettext();
  void setCatalogueLocation(const QString& location);
  void setCatalogueName(const QString& name);
  void setLocale(const QString& localeCode);
  QString moFilePath() const;
  inline QString locale() const { return LauGettext::locale(); }
  inline QString languageCode() const { return LauGettext::languageCode(); }
  inline QString countryCode() const { return LauGettext::countryCode(); }
  inline QString catalogueName() const { return LauGettext::catalogueName(); }
  inline QString catalogueLocation() const { return LauGettext::catalogueLocation(); }

  QString getTranslation(const QString& originalString) const; 
  QStringList availableLocales() const;

  QString charset() const;

private:

  static QtGettext* instance_;
  mutable bool gotAvailableLocales_;
  mutable QStringList availableLocales_;

};

#endif // Gettext_QtGettext_H
