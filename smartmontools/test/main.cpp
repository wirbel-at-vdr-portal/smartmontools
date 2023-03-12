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

  { // test SM_GetDeviceIdentity()
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

  { // test SM_IdentifyDevice()
  if (dev.empty())
     std::cout << "SKIP: SM_IdentifyDevice() - no device" << std::endl;
  else {
     std::cout << "device: " << dev << std::endl;
     for(int Choice=0; Choice<4; Choice++) {
        auto ID = SM_IdentifyDevice(smi, dev, Choice);
        if (ID.empty()) {
           std::cerr << "FAIL: SM_IdentifyDevice() returned empty vector." << std::endl;
           return -1;
           }
        for(auto s:ID)
           std::cout << s << '\n';
        }
     std::cout << "PASS: SM_IdentifyDevice()\n" << std::endl;
     }
  }

  { // test SM_DeviceSettings()
  if (dev.empty())
     std::cout << "SKIP: SM_DeviceSettings() - no device" << std::endl;
  else {
     std::cout << "device: " << dev << std::endl;

     std::string v[] = {"all","aam","apm","dsn","lookahead","security","wcache",
                        "rcache","wcreorder","wcache-sct"};

     for(int Choice=0; Choice<9; Choice++) {
        if (Choice == 7) continue; // disable rcache test.

        auto g = SM_DeviceSettings(smi, dev, Choice);
        std::string fname("SM_DeviceSettings");
        fname += "(" + v[Choice] + ")";
        if (g.empty()) {
           std::cerr << "FAIL: " << fname << " returned empty vector." << std::endl;
           return -1;
           }
        std::cout << fname << "\n";
        for(auto s:g)
           std::cout << "\t" << s << '\n';
        }
     std::cout << "PASS: SM_DeviceSettings()\n" << std::endl;
     }
  }

  { // test SM_SmartInfo()
  if (dev.empty())
     std::cout << "SKIP: SM_SmartInfo() - no device" << std::endl;
  else {
     std::cout << "device: " << dev << std::endl;
     auto lines = SM_SmartInfo(smi, dev);
     if (lines.empty()) {
        std::cerr << "FAIL: SM_SmartInfo() returned empty vector." << std::endl;
        return -1;
        }
     for(auto s:lines)
        std::cout << s << '\n';
     std::cout << "PASS: SM_SmartInfo()\n" << std::endl;
     }
  }

  { // test SM_GetInfo()
  if (dev.empty())
     std::cout << "SKIP: SM_GetInfo() - no device" << std::endl;
  else {
     std::cout << "device: " << dev << std::endl;
     auto lines = SM_GetInfo(smi, dev);
     if (lines.empty()) {
        std::cerr << "FAIL: SM_GetInfo() returned empty vector." << std::endl;
        return -1;
        }
     for(auto s:lines)
        std::cout << s << '\n';
     std::cout << "PASS: SM_GetInfo()\n" << std::endl;
     }
  }

  { // test SM_ScanDevices()
  auto lines = SM_ScanDevices(smi);
  if (lines.empty()) {
     std::cerr << "FAIL: SM_ScanDevices() returned empty vector." << std::endl;
     return -1;
     }
  for(auto s:lines)
     std::cout << s << '\n';
  std::cout << "PASS: SM_ScanDevices()\n" << std::endl;
  }

  { // test SM_ScanDevicesOpen()
  auto lines = SM_ScanDevicesOpen(smi);
  if (lines.empty()) {
     std::cerr << "FAIL: SM_ScanDevicesOpen() returned empty vector." << std::endl;
     return -1;
     }
  for(auto s:lines)
     std::cout << s << '\n';
  std::cout << "PASS: SM_ScanDevicesOpen()\n" << std::endl;
  }

  { // test SM_DeviceHealth()
  if (dev.empty())
     std::cout << "SKIP: SM_DeviceHealth() - no device" << std::endl;
  else {
     std::cout << "device: " << dev << std::endl;
     auto lines = SM_DeviceHealth(smi, dev);
     if (lines.empty()) {
        std::cerr << "FAIL: SM_DeviceHealth() returned empty vector." << std::endl;
        return -1;
        }
     for(auto s:lines)
        std::cout << s << '\n';
     std::cout << "PASS: SM_DeviceHealth()\n" << std::endl;
     }
  }








  std::cout << "all tests passed." << std::endl;
  return 0;
}
