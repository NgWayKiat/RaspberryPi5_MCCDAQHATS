/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  This is the Raspberry Pi5 program for MCC 118 DAQ HATS.
//   DATE           Version     Modify By       Description
//   15-OCT-2025    1.0.0       Way Kiat        First Creation
//                                              (cpp file: Global, Logger, Main, MCC_DAQHATS, Misc, ServerSocket, Versions)
//                                              (config file: sys.config)
//   06-NOV-2025    1.1.0       Way Kiat        Add the auto truncate log file is more than 7 days
//                                              (cpp file: Global, Logger, Main)
//                                              (config file: sys.config)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Versions.h"

const string sVersion = "1.1.0";
const string sProgramName = "MCC_DAQ_HATS";
const string sCreationDate = "15-OCT-2025";
const string sModifyDate = "06-NOV-2025";