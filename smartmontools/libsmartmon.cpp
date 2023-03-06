/*******************************************************************************
 * This is the smartmontool C++ library implementation.
 *
 * It exposes basically the functionality of smartctl to external tools.
 ******************************************************************************/
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <repfunc.h>
#include "libsmartmon.h"

// internal interfaces, do not expose in header.
#include "json.h"
#include "dev_interface.h"
#include "atacmds.h"
#include "ataprint.h"
#include "scsiprint.h"
#include "nvmeprint.h"

/*******************************************************************************
 * types
 ******************************************************************************/
enum failure_type { OPTIONAL_CMD, MANDATORY_CMD, };

/*******************************************************************************
 * global vars
 ******************************************************************************/
std::stringstream ss;
json jglb;
bool printing_is_switchable = false;
bool printing_is_off = false;
unsigned char failuretest_permissive = 0;
bool failuretest_conservative = false;








/*


#include "knowndrives.h"
#include "scsicmds.h"

#include "smartctl.h"
#include "utility.h"
#include "svnversion.h"
*/


/*******************************************************************************
 * forward decls
 ******************************************************************************/
extern void check_config();
extern std::string format_version_info(const char* prog_name, bool full);
extern bool init_drive_database(bool use_default_db);




/*******************************************************************************
 * defines
 ******************************************************************************/
#define IsInterface(smi) ( dynamic_cast<smart_interface*>(smi) != nullptr )








/*******************************************************************************
 * Returns a handle to the current SMART interface.
 ******************************************************************************/
SmartInterface SM_GetInterface(void) {
  static bool initialized = false;
  if (not initialized) {
     check_config();
     smart_interface::init();
     initialized = true;
     }
  return smi();
}

/*******************************************************************************
 * Returns the smartmontools version.
 * On error, an empty string may be returned.
 ******************************************************************************/
std::string SM_GetVersion(SmartInterface Smart) {
  if (not IsInterface(Smart))
     return "";

  auto lines = SplitStr(format_version_info("smartmontools", true), '\n');
  if (lines.empty())
     return "";

  return lines.front();
}

/*******************************************************************************
 * Returns the smartmontools copyright.
 * On error, an empty string may be returned.
 ******************************************************************************/
std::string SM_GetCopyright(SmartInterface Smart) {
  if (not IsInterface(Smart))
     return "";

  auto lines = SplitStr(format_version_info("smartmontools", true), '\n');
  for(auto line:lines) {
     if (line.find("Copyright ") == 0)
        return line;
     }

  return "";
}

/*******************************************************************************
 * Returns the smartmontools license.
 * On error, an empty string may be returned.
 ******************************************************************************/
std::string SM_GetLicense(SmartInterface Smart) {
  if (not IsInterface(Smart))
     return "";

  std::string result;
  bool on = false;
  auto lines = SplitStr(format_version_info("smartmontools", true), '\n');
  for(auto line:lines) {
     if (line.find(" comes ") != std::string::npos)
        on = true;
     if (line.empty()) {
        on = false;
        if (not result.empty())
           result.pop_back();
        }
     if (on)
        result += line + "\n";
     }

  return result;
}

/*******************************************************************************
 * Returns the smartmontools build info.
 * On error, an empty string may be returned.
 ******************************************************************************/
std::string SM_GetBuildInfo(SmartInterface Smart) {
  if (not IsInterface(Smart))
     return "";

  std::string result;
  bool on = false;
  auto lines = SplitStr(format_version_info("x", true), '\n');
  for(auto line:lines) {
     if (line.find("smartmontools") == 0)
        on = true;
     if (on)
        result += line + "\n";
     }

  if (not result.empty())
     result.pop_back();

  return result;
}

/*******************************************************************************
 * Returns identity information for a device.
 * On error, an empty vector may be returned.
 ******************************************************************************/
std::vector<std::string> SM_GetDeviceIdentity(SmartInterface Smart, std::string DeviceName) {
  std::vector<std::string> result;
  std::string s;

  if (not IsInterface(Smart) or DeviceName.empty())
     return result;

  smart_device_auto_ptr dev;
  const char* type = 0;

  dev = smi()->get_smart_device(DeviceName.c_str(), type);

  if (!dev)
     return result;

  init_drive_database(true);

  dev.replace(dev->autodetect_open());
  if (not dev->is_open())
     return result;

  if (dev->is_ata()) {
     ata_print_options opts;
     opts.drive_info = true;
     opts.ignore_presets = false;
     ataPrintMain(dev->to_ata(), opts);
     }
  else if (dev->is_scsi()) {
     scsi_print_options opts;
     opts.drive_info = true;
     scsiPrintMain(dev->to_scsi(), opts);
     }
  else if (dev->is_nvme()) {
     nvme_print_options opts;
     opts.drive_info = true;
     nvmePrintMain(dev->to_nvme(), opts);
     }

  dev->close();

  while(std::getline(ss, s, '\n'))
     if (not s.empty()) result.push_back(s);
  ss.clear();
  ss.str("");

  return result;
}

/*******************************************************************************
 * Returns words and bits from IDENTIFY DEVICE data. See 'man smartctl', option
 * '--identify[=[w][nvb]]'.
 * On error, an empty vector may be returned. Valid only for ATA drives.
 * Params
 *   int Choice
 *      0: enables printing of all 256 words
 *      1: suppresses printing of bits
 *      2: enables printing of all bits from valid words
 *      3: enables printing of all bits
 ******************************************************************************/
std::vector<std::string> SM_IdentifyDevice(SmartInterface Smart,
                                           std::string DeviceName,
                                           int Choice) {
  std::vector<std::string> result;
  std::string s;

  if (not IsInterface(Smart) or DeviceName.empty() or
      Choice < 0 or Choice > 3)
     return result;

  smart_device_auto_ptr dev;
  const char* type = 0;

  dev = smi()->get_smart_device(DeviceName.c_str(), type);

  if (!dev)
     return result;

  init_drive_database(true);

  dev.replace(dev->autodetect_open());
  if (not dev->is_open())
     return result;

  if (dev->is_ata()) {
     ata_print_options opts;
     opts.identify_word_level = 0;
     opts.identify_bit_level = 0;

     switch(Choice) {
        case 0:
           opts.identify_word_level = 1;
           break;
        case 1:
           opts.identify_bit_level = -1;
           break;
        case 2:
           opts.identify_bit_level = 1;
           break;
        case 3:
           opts.identify_bit_level = 2;
           break;
        default:
           return result;
        }
     ataPrintMain(dev->to_ata(), opts);
     }

  dev->close();

  while(std::getline(ss, s, '\n'))
     if (not s.empty()) result.push_back(s);
  ss.clear();
  ss.str("");

  return result;
}






/*******************************************************************************
 * The following functions are stubs to get the original sources to kick in.
 ******************************************************************************/

void pout(const char* fmt, ...) {
  char buf[4096];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  ss << (const char*) buf;
}

void jout(const char* fmt, ...) {
  char buf[4096];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  ss << buf;
}

void jinf(const char* fmt, ...) {
  char buf[4096];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  ss << buf;
}

void jwrn(const char* fmt, ...) {
  char buf[4096];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  ss << buf;
}

void jerr(const char* fmt, ...) {
  char buf[4096];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  ss << buf;
}

void jout_startup_datetime(const char* prefix) {
  (void) prefix;
}

void checksumwarning(const char* string) {
  pout("Invalid checksum: %s\n", string);
}

bool failuretest(failure_type type, int returnvalue) {
  (void) returnvalue;
  switch(type) {
     case 0:
        if (!failuretest_conservative)
           return true;
        pout("An optional SMART command failed\n");
        return false;
     default:
        if (failuretest_permissive--)
           return true;
        pout("ERROR: A mandatory SMART command failed.\n");
        return false;
     }
}
