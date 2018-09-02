# Dumber
Z: drive repackager for Symbian platform, compatible for all Symbian platform.

## Instructions:
* Please jailbreak your Symbian phone first. 
* After that, uses a file manager to gain access to *C:\System\Apps* or *C:\Sys\Bin* and runs **Dumber.exe**
* Leave your phone there, if when you come back there is an error message or app exits, that means that there is not enough space or there is a error from the app. Else, a success message will display. Press any key to exit.
* Your repackage file should be in *E:* drive.

## Specification:
* EKA2L1 uses the RPKG format to package all *Z:* drive contents. Z: drive should have at least ROM (on previous platform), or is mounted with both *ROFS + ROM*. All contents are repackage to a single file without any compression. All entries should be sort by UID.
* Header
   - Magic: char[4] (RPKG)
   - Version: 
      + Major: char
      + Minor: char
      + Build: short
   - File count: uint32_t
* Entry sections: *File count* * entries
   - Entry:
      + Attribute: Uint64_t
      + Time: TTime
      + FullPathLength: Uint64_t
      + PathStringRawData: UCS-2 string with length declared above
      + DataSize: Uint64_t
