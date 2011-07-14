/*
  Copyright (C) 2010 Laurent Cozic. All right reserved.
  Use of this source code is governed by a GNU/GPL license that can be
  found in the LICENSE file.
*/

#include "LauGettext.h"


#if defined(_WINDOWS) || defined(WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

#include <fstream>
#include <iostream> 

LauGettext* LauGettext::instance_ = NULL;


LauGettext* LauGettext::instance() {
  if (instance_) return instance_;
  instance_ = new LauGettext();
  return instance_;
}


void LauGettext::destroyInstance() {
  if (instance_) delete instance_;
  instance_ = NULL;
}


LauGettext::LauGettext() {
  catalogueName_ = "locale";
  languageCode_ = "en";
  countryCode_ = "US";
  locale_ = "en_US";
}


LauGettext::~LauGettext() {

}


QString LauGettext::moFilePath() const {
  QString filePath = catalogueLocation();
  filePath.append(PATH_SEPARATOR);
  filePath.append(locale());
  filePath.append(PATH_SEPARATOR);
  filePath.append(catalogueName());
  filePath.append(".mo");

  std::ifstream fileTest(filePath.toStdString().c_str(), std::ios::in | std::ios::binary);
  if (fileTest) return filePath;

  filePath = catalogueLocation();
  filePath.append(PATH_SEPARATOR);
  filePath.append(languageCode());
  filePath.append(PATH_SEPARATOR);
  filePath.append(catalogueName());
  filePath.append(".mo");
  std::ifstream fileTest2(filePath.toStdString().c_str(), std::ios::in | std::ios::binary);
  if (fileTest2) return filePath;

  return "";
}


GettextMessage* LauGettext::getTranslation(const char* originalString, int originalLength) const {
  if (!moParser_.ready()) {
    QString p = moFilePath();
    if (p.isEmpty()) return NULL; // File doesn't exist or cannot be opened
    bool success = moParser_.parseFile(moFilePath().toStdString().c_str());
    if (!success) {
      // The file could not be parsed
      return NULL;
    }
  }

  return moParser_.getTranslation(originalString, originalLength);
}


void LauGettext::setCatalogueName(QString name) {
  catalogueName_ = name;
}


void LauGettext::setCatalogueLocation(QString location) {
  catalogueLocation_ = location;
}


void LauGettext::setLocale(QString localeCode) {
  size_t splitIndex = localeCode.indexOf("_");
  if (splitIndex != localeCode.toStdString().npos) {
    languageCode_ = localeCode.mid(0, splitIndex);
    countryCode_ = localeCode.mid(splitIndex + 1, 10);
  } else {
    languageCode_ = localeCode;
    countryCode_ = "";
  }

  if (countryCode_.isEmpty()) {
    locale_ = languageCode_;
  } else {
    locale_ = languageCode_;
    locale_.append("_");
    locale_.append(countryCode_);
  }

  moParser_.clearData();
}


#undef PATH_SEPARATOR
