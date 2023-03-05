#include <iostream>
#include <libsmartmon.h>
#include <repfunc.h>

/* returns -1, if a pass failed. */
int main(int argc, const char** args) {
  (void) argc;
  (void) args;

  auto smi = SM_GetInterface();

  { // test SM_GetVersion()
  auto Version(SM_GetVersion(smi));
  if (Version.empty()) {
     std::cerr << "FAIL: SM_GetVersion() returned empty string." << std::endl;
     return -1;
     }
  std::cout << Version << "\nPASS: SM_GetVersion()\n" << std::endl;
  }


  { // test SM_GetCopyright()
  auto Copyright(SM_GetCopyright(smi));
  if (Copyright.empty()) {
     std::cerr << "FAIL: SM_GetCopyright() returned empty string." << std::endl;
     return -1;
     }
  std::cout << Copyright << "\nPASS: SM_GetCopyright()\n" << std::endl;
  }


  { // test SM_GetLicense()
  auto License(SM_GetLicense(smi));
  if (License.empty()) {
     std::cerr << "FAIL: SM_GetLicense() returned empty string." << std::endl;
     return -1;
     }
  std::cout << License << "\nPASS: SM_GetLicense()\n" << std::endl;
  }


  { // test SM_GetBuildInfo()
  auto BuildInfo(SM_GetBuildInfo(smi));
  if (BuildInfo.empty()) {
     std::cerr << "FAIL: SM_GetBuildInfo() returned empty string." << std::endl;
     return -1;
     }
  std::cout << BuildInfo << "\nPASS: SM_GetBuildInfo()\n" << std::endl;
  }

  std::string dev;
  for(char c='a'; c<'{'; c++) {
     dev = "/dev/sd";
     dev += c;
     if (FileExists(dev))
        break;
     dev.clear();
     }

  if (dev.empty()) 
  for(char c='0'; c<':'; c++) {
     dev = "/dev/nvme";
     dev += c;
     if (FileExists(dev))
        break;
     dev.clear();
     }

  { // test SM_GetBuildInfo()
  if (dev.empty())
     std::cout << "SKIP: SM_GetDeviceIdentity() - no device" << std::endl;
  else {
     std::cout << "device: " << dev << std::endl;
     auto ID = SM_GetDeviceIdentity(smi, dev);
     if (ID.empty()) {
        std::cerr << "FAIL: SM_GetDeviceIdentity() returned empty vector." << std::endl;
        return -1;
        }
     for(auto s:ID)
        std::cout << s << '\n';
     std::cout << "PASS: SM_GetDeviceIdentity()\n" << std::endl;
     }
  }


  std::cout << "all tests passed." << std::endl;
  return 0;
}
