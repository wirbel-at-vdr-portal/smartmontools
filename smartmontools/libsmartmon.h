/*******************************************************************************
 * This is the smartmontool C++ library interface.
 *
 * It exposes basically the functionality of smartctl to external tools.
 ******************************************************************************/
#pragma once
#include <string>
#include <vector>

/*******************************************************************************
 * forward declarations.
 ******************************************************************************/
class smart_interface;
typedef smart_interface* SmartInterface;


/*******************************************************************************
 * Returns a handle to the current SMART interface.
 * This interface gives access to any of the functions below.
 ******************************************************************************/
SmartInterface SM_GetInterface(void);

/*******************************************************************************
 * Returns the smartmontools version.
 * On error, an empty string may be returned.
 ******************************************************************************/
std::string SM_GetVersion(SmartInterface Smart);

/*******************************************************************************
 * Returns the smartmontools copyright.
 * On error, an empty string may be returned.
 ******************************************************************************/
std::string SM_GetCopyright(SmartInterface Smart);

/*******************************************************************************
 * Returns the smartmontools license.
 * On error, an empty string may be returned.
 ******************************************************************************/
std::string SM_GetLicense(SmartInterface Smart);

/*******************************************************************************
 * Returns the smartmontools build info.
 * On error, an empty string may be returned.
 ******************************************************************************/
std::string SM_GetBuildInfo(SmartInterface Smart);

/*******************************************************************************
 * Returns identity information for a device. See 'man smartctl', option
 * '-i, --info'.
 * On error, an empty vector may be returned.
 ******************************************************************************/
std::vector<std::string> SM_GetDeviceIdentity(SmartInterface Smart,
                                              std::string DeviceName);

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
                                           int Choice);











/*

  --identify[=[w][nvb]]


  -g NAME, --get=NAME
        Get device setting: all, aam, apm, dsn, lookahead, security,
        wcache, rcache, wcreorder, wcache-sct

  -a, --all
         Show all SMART information for device

  -x, --xall
         Show all information for device

  --scan
         Scan for devices

  --scan-open
         Scan for devices and try to open each device

================================== SMARTCTL RUN-TIME BEHAVIOR OPTIONS =====

  -j, --json[=cgiosuvy]
         Print output in JSON or YAML format

  -q TYPE, --quietmode=TYPE                                           (ATA)
         Set smartctl quiet mode to one of: errorsonly, silent, noserial

  -d TYPE, --device=TYPE
         Specify device type to one of:
         ata, scsi[+TYPE], nvme[,NSID], sat[,auto][,N][+TYPE], usbasm1352r,N, usbcypress[,X], usbjmicron[,p][,x][,N], usbprolific, usbsunplus, sntasmedia, sntjmicron[,NSID], sntrealtek, jmb39x[-q],N[,sLBA][,force][+TYPE], jms56x,N[,sLBA][,force][+TYPE], areca,N/E, 3ware,N, hpt,L/M/N, megaraid,N, aacraid,H,L,ID, sssraid,E,S, cciss,N, auto, test

  -T TYPE, --tolerance=TYPE                                           (ATA)
         Tolerance: normal, conservative, permissive, verypermissive

  -b TYPE, --badsum=TYPE                                              (ATA)
         Set action on bad checksum to one of: warn, exit, ignore

  -r TYPE, --report=TYPE
         Report transactions (see man page)

  -n MODE[,STATUS[,STATUS2]], --nocheck=MODE[,STATUS[,STATUS2]] (ATA, SCSI)
         No check if: never, sleep, standby, idle (see man page)

============================== DEVICE FEATURE ENABLE/DISABLE COMMANDS =====

  -s VALUE, --smart=VALUE
        Enable/disable SMART on device (on/off)

  -o VALUE, --offlineauto=VALUE                                       (ATA)
        Enable/disable automatic offline testing on device (on/off)

  -S VALUE, --saveauto=VALUE                                          (ATA)
        Enable/disable Attribute autosave on device (on/off)

  -s NAME[,VALUE], --set=NAME[,VALUE]
        Enable/disable/change device setting: aam,[N|off], apm,[N|off],
        dsn,[on|off], lookahead,[on|off], security-freeze,
        standby,[N|off|now], wcache,[on|off], rcache,[on|off],
        wcreorder,[on|off[,p]], wcache-sct,[ata|on|off[,p]]

======================================= READ AND DISPLAY DATA OPTIONS =====

  -H, --health
        Show device SMART health status

  -c, --capabilities                                            (ATA, NVMe)
        Show device SMART capabilities

  -A, --attributes
        Show device SMART vendor-specific Attributes and values

  -f FORMAT, --format=FORMAT                                          (ATA)
        Set output format for attributes: old, brief, hex[,id|val]

  -l TYPE, --log=TYPE
        Show device log. TYPE: error, selftest, selective, directory[,g|s],
        xerror[,N][,error], xselftest[,N][,selftest], background,
        sasphy[,reset], sataphy[,reset], scttemp[sts,hist],
        scttempint,N[,p], scterc[,N,M][,p|reset], devstat[,N], defects[,N],
        ssd, gplog,N[,RANGE], smartlog,N[,RANGE], nvmelog,N,SIZE
        tapedevstat, zdevstat, envrep

  -v N,OPTION , --vendorattribute=N,OPTION                            (ATA)
        Set display OPTION for vendor Attribute N (see man page)

  -F TYPE, --firmwarebug=TYPE                                         (ATA)
        Use firmware bug workaround:
        none, nologdir, samsung, samsung2, samsung3, xerrorlba, swapid

  -P TYPE, --presets=TYPE                                             (ATA)
        Drive-specific presets: use, ignore, show, showall

  -B [+]FILE, --drivedb=[+]FILE                                       (ATA)
        Read and replace [add] drive database from FILE
        [default is +/etc/smart_drivedb.h
         and then    /usr/share/smartmontools/drivedb.h]

============================================ DEVICE SELF-TEST OPTIONS =====

  -t TEST, --test=TEST
        Run test. TEST: offline, short, long, conveyance, force, vendor,N,
                        select,M-N, pending,N, afterselect,[on|off]

  -C, --captive
        Do test in captive mode (along with -t)

  -X, --abort
        Abort any non-captive test on device

=================================================== SMARTCTL EXAMPLES =====

  smartctl --all /dev/sda                    (Prints all SMART information)

  smartctl --smart=on --offlineauto=on --saveauto=on /dev/sda
                                              (Enables SMART on first disk)

  smartctl --test=long /dev/sda          (Executes extended disk self-test)

  smartctl --attributes --log=selftest --quietmode=errorsonly /dev/sda
                                      (Prints Self-Test & Attribute errors)
  smartctl --all --device=3ware,2 /dev/sda
  smartctl --all --device=3ware,2 /dev/twe0
  smartctl --all --device=3ware,2 /dev/twa0
  smartctl --all --device=3ware,2 /dev/twl0
          (Prints all SMART info for 3rd ATA disk on 3ware RAID controller)
  smartctl --all --device=hpt,1/1/3 /dev/sda
          (Prints all SMART info for the SATA disk attached to the 3rd PMPort
           of the 1st channel on the 1st HighPoint RAID controller)
  smartctl --all --device=areca,3/1 /dev/sg2
          (Prints all SMART info for 3rd ATA disk of the 1st enclosure
           on Areca RAID controller)

*/
