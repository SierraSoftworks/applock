/*
  Copyright (C) 2010 Laurent Cozic. All right reserved.
  Use of this source code is governed by a GNU/GPL license that can be
  found in the LICENSE file.
*/

#ifndef Gettext_LauGettext_H
#define Gettext_LauGettext_H

#include "MoParser.h"
#include <QtCore>

class LauGettext {

public:

  static LauGettext* instance();
  static void destroyInstance();

  LauGettext();
  ~LauGettext();
  void setCatalogueLocation(QString location);
  void setCatalogueName(QString name);
  void setLocale(QString localeCode);
  QString moFilePath() const;
  inline QString locale() const { return locale_; }
  inline QString languageCode() const { return languageCode_; }
  inline QString countryCode() const { return countryCode_; }
  inline QString catalogueName() const { return catalogueName_; }
  inline QString catalogueLocation() const { return catalogueLocation_; }

  GettextMessage* getTranslation(const char* originalString, int originalLength) const; 

protected:

  mutable GettextMoParser moParser_;

private:

  QString catalogueLocation_;
  QString languageCode_;
  QString countryCode_;
  QString locale_;
  QString catalogueName_;

  static LauGettext* instance_;

};

#endif // Gettext_LauGettext_H
