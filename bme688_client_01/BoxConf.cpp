#include "BoxConf.h"

int BoxConf::bsecConfigLength = 1974;

String BoxConf::nextProfileName(String profileName) {
  
  int matchCount = 0;
  int matchIndex = -1;

  File dir = LittleFS.open("/");
  String _profileName;
  // count and index
  while ((_profileName = dir.getNextFileName()) != "") {
    if (_profileName.indexOf(".prf") > 1) {
      _profileName = _profileName.substring(1, _profileName.length() - 4); // strip leading slash and file format
      if (_profileName == profileName) {
        matchIndex = matchCount;
      }
      matchCount = matchCount + 1;
    }
  }
  dir.rewindDirectory();

  if (matchCount > 0) {

    if (matchIndex == -1 || matchIndex >= matchCount - 1) { // first call or rollover
      matchIndex = 0;
    } else {
      matchIndex = matchIndex + 1; // should be safe through the count check - 1 above
    }
    matchCount = 0;

    while ((_profileName = dir.getNextFileName()) != "") {
      if (_profileName.indexOf(".prf") > 1) {
        _profileName = _profileName.substring(1, _profileName.length() - 4); // strip leading slash and file format
        if (matchCount == matchIndex) {
          return _profileName;
        } else {
          matchCount = matchCount + 1;
        }
      }
    }    

  } else {
    return profileName;
  }



  return profileName;

}

void BoxConf::saveProfile(String name, JsonObject& profileJo) {
  String fileName = BoxConf::getFileName(name, "prf");
  File file = LittleFS.open(fileName, "w");
  profileJo.printTo(file);
  file.close();
}

JsonObject& BoxConf::loadProfile(String name) {
  StaticJsonBuffer<512> jsonBuffer;
  String fileName = BoxConf::getFileName(name, "prf");
  File file = LittleFS.open(fileName, "r");
  JsonObject& profileJo = jsonBuffer.parseObject(file);
  file.close();
  return profileJo;
}

void BoxConf::saveBsecConfig(String name, byte* data) {
  String fileName = BoxConf::getFileName(name, "dat");
  File file = LittleFS.open(fileName, "wb");  // binary write mode
  file.write(data, BoxConf::bsecConfigLength);
  file.close();
}

void BoxConf::loadBsecConfig(String name, uint8_t* data) {
  String fileName = BoxConf::getFileName(name, "dat");
  File file = LittleFS.open(fileName, "rb");  // binary read mode
  char fileData[BoxConf::bsecConfigLength];
  file.readBytes(fileData, BoxConf::bsecConfigLength);
  file.close();
  for (int i = 0; i < BoxConf::bsecConfigLength; i++) {
    data[i] = (uint8_t)fileData[i];
  }
}

String BoxConf::getFileName(String name, String type) {
  char fileNameBuf[32];
  sprintf(fileNameBuf, "/%s.%s", name, type);
  return fileNameBuf;
}